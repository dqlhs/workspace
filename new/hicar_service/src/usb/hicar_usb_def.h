#pragma once

namespace hsae {

enum class usb_sm_status {
    wait_huawei_usb_plugin,
    wait_usb_ncm_switch,
    wait_ip_addr_setting,
    wait_mdns_start,
    usb_discover_finish,
    process_error
};

enum class usb_plug_status {
    unplug,
    plugin
};

} // namespace hsae
