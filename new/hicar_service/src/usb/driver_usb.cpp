#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "log/oxygen_log.h"
#include "log/oxygen_assert.h"
#include "utility/format.h"
#include "utility/hex_conv.h"
#include "unique_config.h"

#include "net_helper.h"

#include "HiCarDefine.h"
#include "hicar_usb_protocol.h"
#include "hicar_usb_manager.h"

#include "driver_adapt.h"


using namespace hsae;
using namespace oxygen;

typedef ChannelCallback hicar_usb_channel_callback_t;

class hicar_usb_driver {
public:
    // init
    static bool is_init_;

    // hicar global value
    static const hicar_usb_channel_callback_t * hicar_usb_channel_callback;
    static std::string usb_ncm_iface_ip;
    static uint16_t business_port;

    // phone br addr
    static std::string phone_br_addr;

    // thread
    static std::thread main_loop;

    // helper
    template<typename SendFunc>
    static int channel_send_helper(const std::string & _tag, SendFunc & _func, const uint8_t * _data, std::size_t _size) {
        if (_func) {
            INFO("%s <<< %d : %s", _tag.c_str(), (int)_size, hex(_data, _size).c_str());
            _func(_data, _size);
            return RESULT_RET_OK;
        } else {
            WARN("!!! timing problem, tag : %s", _tag.c_str());
            return RESULT_RET_ERROR;
        }
    }

    // usb driver interface
    static int hicar_usb_driver_init (const void *context, const char *ifName);
    static int hicar_usb_driver_deinit(const void * context);
    static int hicar_usb_register_channel_callback(const ChannelCallback *callback);

    static void hicar_usb_plug_status_change_notify(usb_plug_status status);
};

// class static member varialbe init feild

bool hicar_usb_driver::is_init_ = false;

const hicar_usb_channel_callback_t *
hicar_usb_driver::hicar_usb_channel_callback = nullptr;

std::string hicar_usb_driver::usb_ncm_iface_ip;
uint16_t hicar_usb_driver::business_port = 0;

std::string hicar_usb_driver::phone_br_addr;

std::thread hicar_usb_driver::main_loop;

void hicar_usb_driver::hicar_usb_plug_status_change_notify(usb_plug_status _status)
{
    INFO("usb plugin status change [%s]", _status == usb_plug_status::plugin ? "plugin" : "unplug");

    EventType event_type = static_cast<EventType>(0);

    switch (_status) {
        case usb_plug_status::plugin:
            event_type = EVENT_CONNECT;
        break;

        case usb_plug_status::unplug:
            event_type = EVENT_DISCONNECT;
        break;
    }

    if (event_type != 0) {
        if (hicar_usb_channel_callback) {
            INFO("notify nearby event type [%d]", (int)event_type);
            hicar_usb_channel_callback->ChannelStateChange(CHANNEL_USB, event_type, nullptr, 0);
            INFO("!!! notify nearby event finish");
        }
    }
}

// hicar_usb_driver unique
static std::unique_ptr<hicar_usb_manager> hicar_usb;

int hicar_usb_driver::hicar_usb_driver_init(const void * context, const char * ifName)
{
    (void)context;
    (void)ifName;
    INFO("init");

    if (is_init_) {
        WARN("!!! function reentrancy");
        return RESULT_RET_ERROR;
    }

    // prevent reentrancy
    is_init_ = true;

    // get init para
    const UsbParam* usb_init_data = reinterpret_cast<const UsbParam *>(context);

    OXYGEN_ASSERT(usb_init_data && usb_init_data->localIp[0] != 0);

    usb_ncm_iface_ip = usb_init_data->localIp;
    business_port = static_cast<uint16_t>(usb_init_data->port);

    TRACE("nearby set ip : %s, port : %d", usb_ncm_iface_ip.c_str(), (int)business_port);

    // Parameter verification
    OXYGEN_ASSERT(!usb_ncm_iface_ip.empty());
    OXYGEN_ASSERT(business_port != 0);

    // init usb plugin state mechine
    unique_config & config = unique_config::instance();
    std::string remove_char = config.local_br_addr_mac();
    remove_char.erase(std::remove(remove_char.begin(), remove_char.end(), ':'), remove_char.end());

    hicar_usb = std::make_unique<hicar_usb_manager>(remove_char, config.modelid(), config.submodelid(),VENDOR_ID_HUAWEI,
                                                    usb_ncm_iface_ip, business_port);
    hicar_usb->set_usb_plugin_status_change_callback(std::bind(&hicar_usb_driver::hicar_usb_plug_status_change_notify, std::placeholders::_1));

    // start main loop be used to state mechine and tcp server
    main_loop = std::thread([&] () {
        hicar_usb->event_loop();
    });

    return RESULT_RET_OK;
}

int hicar_usb_driver::hicar_usb_driver_deinit(const void * context)
{
    (void)context;
    INFO("deinit, usb driver not use");
    return RESULT_RET_OK;
}

int hicar_usb_driver::hicar_usb_register_channel_callback(const ChannelCallback * callback)
{
    INFO("callback ptr -> %p", (void *)callback);

    OXYGEN_ASSERT(callback != nullptr);

    hicar_usb_channel_callback = callback;
    return RESULT_RET_OK;
}

struct FcDriverAdaptOps g_fcDriverUsbOps = {
    .name                    = "usb",
    .desc                    = "usb driver",
    .Init                    = hicar_usb_driver::hicar_usb_driver_init,
    .Deinit                  = hicar_usb_driver::hicar_usb_driver_deinit,
    .SetParam                = nullptr,
    .GetParam                = nullptr,
    .StartAdv                = nullptr,
    .StopAdv                 = nullptr,
    .SetAdvData              = nullptr,
    .StartScan               = nullptr,
    .StopScan                = nullptr,
    .SendData                = nullptr,
    .NotifyData              = nullptr,
    .DisplayPin              = nullptr,
    .PushMsgBox              = nullptr,
    .CreateWifiChannel       = nullptr,
    .Connect                 = nullptr,
    .Disconnect              = nullptr,
    .RegisterChannelCallback = hicar_usb_driver::hicar_usb_register_channel_callback,
};
