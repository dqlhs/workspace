#pragma once

#include <map>
#include <string>
#include <vector>
#include <functional>

#include <asio.hpp>

#include <dns_sd.h>
#include <libusb-1.0/libusb.h>

#include "usb_plugin_monitor.h"
#include "usb_status_smc_sm.h"
#include "hicar_usb_def.h"
#include "verify.h"

namespace hsae {

class hicar_usb_manager final {

public:
    hicar_usb_manager(const std::string & _bt_mac_addr,
                      uint32_t _modelid, uint8_t _submodelid,
                      int _vendor_id,
                      const std::string & _ip, uint16_t _port);

    ~ hicar_usb_manager();

    void event_loop();

    void set_status_change_callback(std::function<void(usb_sm_status)> && _cb);

    void set_usb_plugin_status_change_callback(std::function<void(usb_plug_status)> && _cb);

    void set_status_update_duration(uint32_t _ms);

    void post_event(std::function<void()> && _event);

    void dispatch_event(std::function<void()> && _event);

public:
    // state machine function
    usb_sm_status current_status() const;
    void set_current_status(usb_sm_status _status);

    usb_sm_status previous_status() const;
    void set_previous_status(usb_sm_status _status);

    void set_iface_ip();
    void start_mdns_service();

    void iface_ip_check();
    void ncm_dev_check();

    void add_status_check(const std::string & _status_name,
                          std::function<void(void)> && _cb);
    void clear_status_check();

    void notify_usb_plug_status(usb_plug_status _status);

    static bool netwrok_dev_is_up();

    static std::string usb_dev_address(libusb_device * _dev);

private:
    std::vector<uint8_t> create_ncm_switch_message() const;

    bool ncm_dev_is_up();

    std::array<uint8_t, SHA256_DIGEST_LENGTH> device_id() const;

    void status_update_handler(asio::error_code _ec);

    void usb_plugin_change_handler(libusb_context * _ctxt,
                                   libusb_device * _dev,
                                   libusb_hotplug_event _event);

    static bool send_usb_control_message(libusb_device_handle * _handle, const std::vector<uint8_t> & _message);
    static void dns_service_register_reply (DNSServiceRef sdRef,
                                            DNSServiceFlags flags,
                                            DNSServiceErrorType errorCode,
                                            const char *name,
                                            const char *regtype,
                                            const char *domain,
                                            void *context);


private:
    const std::string bt_mac_addr_;
    const uint32_t model_id_;
    const uint8_t submodel_id_;

    asio::io_context io_;
    asio::io_context::work work_;
    asio::steady_timer status_update_timer_;
    asio::steady_timer timeout_;

    std::map<std::string, std::function<void(void)>> event_handler_;
    uint32_t status_update_duration_;

    usb_status_smcContext fsm_;

    usb_plugin_monitor monitor_;
    libusb_device_handle *handle_;
    std::thread monitor_handleer_;

    usb_sm_status status_;
    usb_sm_status previous_status_;
    std::size_t loop_interval_;

    DNSServiceRef bonjour_handle_;

    std::string usb_device_addr_;
    std::string usb_ncm_addr_path_;

    DNSServiceRef bonjourHandle_;

    std::function<void(usb_sm_status)> sm_status_change_cb_;
    std::function<void(usb_plug_status)>  plug_status_change_cb_;

    const std::string ip_;
    const uint16_t port_;
};

} // namespace hsae
