#include "hicar_usb_manager.h"

#include <string.h>
#include <unistd.h>

#include "log/oxygen_log.h"
#include "log/oxygen_assert.h"

#include "utility/format.h"
#include "utility/hex_conv.h"

#include "HiCarDefine.h"
#include "net_helper.h"

#define HICAR_STATUS_UPDATE_DURATION 100
#define HICAR_DEVICE_AILAS_NAME "HICAR_VEHICLE"

namespace hsae {

hicar_usb_manager::hicar_usb_manager(const std::string & _bt_mac_addr,
                                     uint32_t _modelid, uint8_t _submodelid,
                                     int _vendor_id,
                                     const std::string & _ip, uint16_t _port)
    : bt_mac_addr_(_bt_mac_addr),
      model_id_(_modelid),
      submodel_id_(_submodelid),
      io_(),
      work_(io_),
      status_update_timer_(io_),
      timeout_(io_),
      status_update_duration_(HICAR_STATUS_UPDATE_DURATION),
      fsm_(*this),
      monitor_(_vendor_id, LIBUSB_HOTPLUG_MATCH_ANY),
      handle_(nullptr),
      status_(usb_sm_status::wait_huawei_usb_plugin),
      previous_status_(usb_sm_status::usb_discover_finish),
      usb_ncm_addr_path_(std::string("/sys/class/net/") + NCM_IF_NAME + "/address"),
      bonjourHandle_(nullptr),
      ip_(_ip), port_(_port)
{
    // print params
    TRACE("bt mac : %s", bt_mac_addr_.c_str());
    // mac not include ":"
    OXYGEN_ASSERT(bt_mac_addr_.length() == 12);

    fsm_.setDebugFlag(true);

    monitor_.set_plugin_change_callback(std::bind(&hicar_usb_manager::usb_plugin_change_handler, this,
                                                  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    monitor_handleer_ = std::thread([this] () {
        for(;;) {
            monitor_.usb_monitor_loop();
        }
    });

    status_update_timer_.expires_after(std::chrono::milliseconds(status_update_duration_));
    status_update_timer_.async_wait(std::bind(&hicar_usb_manager::status_update_handler, this,
                                              std::placeholders::_1));
}

hicar_usb_manager::~hicar_usb_manager()
{

}

void hicar_usb_manager::event_loop()
{
    try {
        io_.run();
    } catch (std::exception _e) {
        ERROR("%s", _e.what());
    }
}

void hicar_usb_manager::set_status_change_callback(std::function<void (usb_sm_status)> && _cb)
{
    sm_status_change_cb_ = std::move(_cb);
}

void hicar_usb_manager::set_usb_plugin_status_change_callback(std::function<void (usb_plug_status)> && _cb)
{
    plug_status_change_cb_ = std::move(_cb);
}

void hicar_usb_manager::set_status_update_duration(uint32_t _ms)
{

}

usb_sm_status hicar_usb_manager::current_status() const
{
    return status_;
}

void hicar_usb_manager::set_current_status(usb_sm_status _status)
{
    status_ = _status;
    if (sm_status_change_cb_) {
        sm_status_change_cb_(_status);
    }
}

usb_sm_status hicar_usb_manager::previous_status() const
{
    return previous_status_;
}

void hicar_usb_manager::set_previous_status(usb_sm_status _status)
{
    previous_status_ = _status;
}

void hicar_usb_manager::set_iface_ip()
{
    char cmd[256] = {0};
    //use static ip
    snprintf(cmd, sizeof (cmd), "ifconfig %s %s netmask 255.255.255.0 up", NCM_IF_NAME, ip_.c_str());
    int ret = system(cmd);
    if (ret) {
        ERROR("cmd : %s, failed: %d", cmd, ret);
    }
}

void hicar_usb_manager::start_mdns_service()
{
    uint8_t txtBuf[256] = {0};
    TXTRecordRef txtRec;
    TXTRecordCreate( &txtRec, (uint16_t) sizeof( txtBuf ), txtBuf );

    std::string iface_ip;
    try {
        iface_ip = interface_to_ip(NCM_IF_NAME);
    } catch (std::exception _ec) {
        ERROR("interface_to_ip : %s", _ec.what());
        post_event(std::bind(&usb_status_smcContext::mdns_activation_failed, std::ref(fsm_)));
        return;
    }

    DNSServiceErrorType err = TXTRecordSetValue( &txtRec,
                                                 HICAR_MDNS_NCMIP_TAG,
                                                 (uint8_t) iface_ip.size(), iface_ip.data() );
    if (err != kDNSServiceErr_NoError) {
        ERROR("TXTRecordSetValue failed: %d",int(err));
        goto end;
    }

    if (bonjourHandle_ != nullptr) {
        DNSServiceRefDeallocate(bonjourHandle_);
    }

    bonjourHandle_ = nullptr;

    err = DNSServiceRegister((DNSServiceRef*)&bonjourHandle_, 0,
                             if_nametoindex(NCM_IF_NAME), HICAR_MDNS_NAME, HICAR_MDNS_REG_TYPE, HICAR_MDNS_DOMAIN,
                             nullptr, htons(port_),
                             TXTRecordGetLength( &txtRec ), TXTRecordGetBytesPtr( &txtRec ),
                             &hicar_usb_manager::dns_service_register_reply, this);
    if (err != kDNSServiceErr_NoError) {
        ERROR("DNSServiceRegister failed: %d",int(err));
        post_event(std::bind(&usb_status_smcContext::mdns_activation_failed, std::ref(fsm_)));
    } else {
        INFO("DNSServiceRegister: %s : %d",HICAR_MDNS_REG_TYPE, (int)port_);
        post_event(std::bind(&usb_status_smcContext::mdns_already_activated, std::ref(fsm_)));
    }

end:

    TXTRecordDeallocate( &txtRec );
}

std::vector<uint8_t> hicar_usb_manager::create_ncm_switch_message() const
{
    int totalLen = 0;

    uint8_t buf[256] = {0};
    //fill nearby head
    HiCarNearbyHead *pHead = reinterpret_cast<HiCarNearbyHead *>(buf + totalLen);
    totalLen += sizeof(HiCarNearbyHead);
    memcpy(pHead->magic_string, HICAR_NEARBY_STRING, 8);
    pHead->type = Nearby_Type_Car;
    pHead->version = Nearby_Version;
    pHead->subtype = Nearby_Subtype_Car;

    //fill Extend Data, format of TLV
    //Trans Channel
    HiCarUSBExtendData *pTLV = (HiCarUSBExtendData*)(buf + totalLen);
    pTLV->tag = TAG_TransferType;
    pTLV->len = 1;
    pTLV->value[0] = VALUE_TransferType_NCM;
    totalLen += TLV_TOATL_LEN(pTLV);

    //ModelID
    pTLV = (HiCarUSBExtendData*)(buf + totalLen);
    pTLV->tag = TAG_ModelID;
    pTLV->len = 3;
    uint32_t modelID = model_id_;
    pTLV->value[0] = (modelID >> 16) & 0x000000ff;
    pTLV->value[1] = (modelID >> 8) & 0x000000ff;
    pTLV->value[2] = (modelID) & 0x000000ff;
    totalLen += TLV_TOATL_LEN(pTLV);

    //SubModelID
    pTLV = (HiCarUSBExtendData*)(buf + totalLen);
    pTLV->tag = TAG_SubModelID;
    pTLV->len = 1;
    pTLV->value[0] = submodel_id_;
    totalLen += TLV_TOATL_LEN(pTLV);

    //DeviceID
    pTLV = (HiCarUSBExtendData*)(buf + totalLen);
    pTLV->tag = TAG_DeviceID;
    pTLV->len = HICAR_NEARBY_DEVICEID_LEN;

    auto device_id_array = device_id();
    memcpy(pTLV->value, device_id_array.data(), device_id_array.size());
    totalLen += TLV_TOATL_LEN(pTLV);

    //BluetoothMac
    pTLV = (HiCarUSBExtendData*)(buf + totalLen);
    pTLV->tag = TAG_BluetoothMac;
    pTLV->len = 6;

    for(size_t i= 0; i < 6; i++) {
        pTLV->value[i] = (uint8_t)stoi(bt_mac_addr_.substr(i * 2, 2), nullptr, 16);
    }

    totalLen += TLV_TOATL_LEN(pTLV);

    // device ailas name
    pTLV = (HiCarUSBExtendData*)(buf + totalLen);
    pTLV->tag = TAG_DeviceAilasName;
    pTLV->len = strlen(HICAR_DEVICE_AILAS_NAME);
    memcpy(pTLV->value, HICAR_DEVICE_AILAS_NAME, pTLV->len);
    totalLen += TLV_TOATL_LEN(pTLV);

    // local ip addr
    pTLV = (HiCarUSBExtendData*)(buf + totalLen);
    pTLV->tag = TAG_LocalIP;
    pTLV->len = static_cast<uint8_t>(ip_.length());
    memcpy(pTLV->value, ip_.c_str(), pTLV->len);
    totalLen += TLV_TOATL_LEN(pTLV);

    // local port
    pTLV = (HiCarUSBExtendData*)(buf + totalLen);
    pTLV->tag = TAG_LocalPort;
    uint32_t port = port_;
    pTLV->len = sizeof(port);
    for (size_t i = 0; i < sizeof (port); ++i) {
        *(pTLV->value + i) = (port >> 8 * i) & 0xFF;
    }
    totalLen += TLV_TOATL_LEN(pTLV);

    //fill total len
    pHead->total_len = static_cast<uint16_t>(totalLen);

    return std::vector<uint8_t>(buf, buf + totalLen);
}

bool hicar_usb_manager::ncm_dev_is_up()
{
    return (0 == access(usb_ncm_addr_path_.c_str(), F_OK));
}

std::array<uint8_t, SHA256_DIGEST_LENGTH> hicar_usb_manager::device_id() const
{
    std::string device_id_raw;
    // model id just have 3 byte, so need get sub str;
    std::string model_id_string = oxygen::value_to_hex(model_id_);
    device_id_raw.append(model_id_string.substr(2));
    device_id_raw.append(oxygen::value_to_hex(submodel_id_));
    device_id_raw.append(bt_mac_addr_);

    INFO("device id raw : %s", device_id_raw.c_str());
    auto device_id_sha256 = sha256(device_id_raw);
    auto device_id_hex = oxygen::hex(device_id_sha256.data(), device_id_sha256.size());
    INFO("device id sha256 : %s", device_id_hex.c_str());

    return device_id_sha256;
}

void hicar_usb_manager::iface_ip_check()
{
    try {
        if (interface_to_ip(NCM_IF_NAME) == ip_) {
            post_event(std::bind(&usb_status_smcContext::iface_ip_is_ok, std::ref(fsm_)));
        }
    } catch (std::exception & _e) {
        WARN("interface_to_ip : %s", _e.what());
        return;
    }
}

void hicar_usb_manager::ncm_dev_check()
{
    if (ncm_dev_is_up()) {
        post_event(std::bind(&usb_status_smcContext::ncm_dev_up, std::ref(fsm_)));
    }
}

void hicar_usb_manager::add_status_check(const std::string & _status_name, std::function<void ()> && _cb)
{
    if (event_handler_.find(_status_name) == event_handler_.end()) {
        INFO("add status check : %s", _status_name.c_str());
        event_handler_[_status_name] = std::move(_cb);
    }
}

void hicar_usb_manager::clear_status_check()
{
    event_handler_.clear();
}

void hicar_usb_manager::notify_usb_plug_status(usb_plug_status _status)
{  
    static usb_plug_status last_plugin_status = usb_plug_status::unplug;

    if (!plug_status_change_cb_) {
        WARN("!!! plug_status_change_cb_ is not registered");
        return;
    }

    if (_status == last_plugin_status) {
        WARN("!!! WTF : last plugin status == current %s", _status == usb_plug_status::plugin ? "plugin" : "unplug");
        if (last_plugin_status == usb_plug_status::plugin) {
            WARN("!!! WTF: Report the same incident [plugin] twice in a row and need to reissue an [unplug] event");
            plug_status_change_cb_(usb_plug_status::unplug);
        } else {
            return;
        }
    }

    plug_status_change_cb_(_status);
    last_plugin_status = _status;
}

bool hicar_usb_manager::netwrok_dev_is_up()
{
    std::string cmd = std::string("ifconfig -a | grep ") + NCM_IF_NAME;
    return system(cmd.c_str()) == 0;
}

std::string hicar_usb_manager::usb_dev_address(libusb_device * _dev)
{
    char tmp[256] = {0};
    uint8_t bus = libusb_get_bus_number(_dev);
    uint8_t address = libusb_get_device_address(_dev);

    int ret = snprintf(tmp, sizeof (tmp), "/dev/bus/usb/0%02d/0%02d", bus, address);
    return std::string(tmp, tmp + ret);
}

void hicar_usb_manager::status_update_handler(asio::error_code _ec)
{
    if (_ec) {
        INFO("error code : %s", _ec.message().c_str());
        return;
    }

    for (auto & handler : event_handler_) {
        handler.second();
    }

    status_update_timer_.expires_after(std::chrono::milliseconds(status_update_duration_));
#if 0
    status_update_timer_.async_wait(std::bind(&hicar_usb_manager::status_update_handler, this,
                                              std::placeholders::_1));
#else
    status_update_timer_.async_wait([this] (asio::error_code _ec) {
        status_update_handler(_ec);
    });
#endif
}

void hicar_usb_manager::usb_plugin_change_handler(libusb_context * _ctxt,
                                                  libusb_device * _dev,
                                                  libusb_hotplug_event _event)
{
    (void)_ctxt;

    int ret = 0;
    if (LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED == _event) {
        INFO("device plugin");

        // open usb
        ret = libusb_open(_dev, &handle_);
        if (LIBUSB_SUCCESS != ret) {
            ERROR("Could not open USB device");
            return;
        }

        // event handle
        if (usb_device_addr_.empty()) {
            INFO("usb_device_addr_ is empty, need send ncm switch");
            auto usb_control_message = create_ncm_switch_message();
            auto hex_view = hsae::oxygen::hex(usb_control_message.data(), usb_control_message.size());
            INFO("usb ncm switch protocol : %s", hex_view.c_str());

            if (!send_usb_control_message(handle_, usb_control_message)) {
                ERROR("send usb control message error ");
                post_event(std::bind(&usb_status_smcContext::active_num_swich_failure, std::ref(fsm_)));
            } else {
                INFO("send successful");
                usb_device_addr_ = usb_dev_address(_dev);

                INFO("Huawei device USB Path: %s",usb_device_addr_.c_str());
                post_event(std::bind(&usb_status_smcContext::active_ncm_switch_successful, std::ref(fsm_)));
            }
        } else {
            WARN("!!! The hicar device is already connected and does not need to send NCM switching instructions");
        }

        // close usb
        if (handle_) {
            libusb_close(handle_);
            handle_ = nullptr;
        }

    } else if (LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT == _event) {
        INFO("device left");
        auto usb_dev_addr = usb_dev_address(_dev);
        if (usb_dev_addr == usb_device_addr_) {
            INFO("usb device addr is same, send unplug event");
            dispatch_event(std::bind(&usb_status_smcContext::unplug, std::ref(fsm_)));
            usb_device_addr_.clear();
        } else {
            WARN("usb dev address is %s != %s, not need send unplug event",
                 usb_dev_addr.c_str(), usb_device_addr_.c_str());
        }

    } else {
        ERROR("Unhandled event %d", _event);
    }
}

void hicar_usb_manager::post_event(std::function<void (void)> && _event)
{
    io_.post(std::move(_event));
}

void hicar_usb_manager::dispatch_event(std::function<void ()> && _event)
{
    io_.dispatch(std::move(_event));
}

bool hicar_usb_manager::send_usb_control_message(libusb_device_handle * _handle,
                                                 const std::vector<uint8_t> & _message)
{
    int ret = 0;

    if (ret != LIBUSB_SUCCESS) {
        ERROR("libusb_open failed: %d", int(ret));
        return false;
    }

    //send control transfer
    INFO("send data to usb device");
    unsigned char data[256] = {0};
    memcpy(data, _message.data(), _message.size());
    uint16_t dataLen = static_cast<uint16_t>(_message.size());
    libusb_claim_interface(_handle, 0);
    ret = libusb_control_transfer(_handle,
                                  HICAR_USB_REQUEST_TYPE, HICAR_USB_REQUEST, 0, HICAR_USB_INDEX,
                                  data, dataLen, 0);

    if (ret != (int)dataLen) {
        INFO("libusb_control_transfer failed: %d",ret);
    } else {
        ret = 0;
    }

    return ret == 0;
}

void hicar_usb_manager::dns_service_register_reply(DNSServiceRef sdRef,
                                                   DNSServiceFlags flags,
                                                   DNSServiceErrorType errorCode,
                                                   const char * name,
                                                   const char * regtype,
                                                   const char * domain,
                                                   void * context)
{
    (void) domain;
    (void) regtype;
    (void) name;
    (void) flags;
    (void) sdRef;
    auto mamager = reinterpret_cast<hicar_usb_manager *>(context);
    if (errorCode == kDNSServiceErr_NoError) {
        INFO("Start mDNS Success!!!");
        mamager->post_event(std::bind(&usb_status_smcContext::mdns_already_activated, std::ref(mamager->fsm_)));
    } else {
        ERROR("Start mDNS Failed:: %d", errorCode);
        mamager->post_event(std::bind(&usb_status_smcContext::mdns_activation_failed, std::ref(mamager->fsm_)));
    }
}

} // namespace hsae
