#include "usb_plugin_monitor.h"

#include <stdlib.h>

#include "log/oxygen_log.h"
#include "log/oxygen_assert.h"



namespace hsae {

usb_plugin_monitor::usb_plugin_monitor(int _vendor_id, int _product_id)
    : vendor_id_(_vendor_id), product_id_(_product_id), flag_(false)
{

}

usb_plugin_monitor::~usb_plugin_monitor()
{
    if (flag_) {
        libusb_hotplug_deregister_callback(ctx_, handle_);
        libusb_exit(ctx_);
    }
}

void usb_plugin_monitor::set_plugin_change_callback(std::function<void (libusb_context *,
                                                                        libusb_device *,
                                                                        libusb_hotplug_event)> && _cb)
{
    if (cb_) return;
    cb_ = std::move(_cb);

    OXYGEN_ASSERT(LIBUSB_SUCCESS == libusb_init(&ctx_));
    libusb_hotplug_event event = static_cast<libusb_hotplug_event>(LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED
                                                                   | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT);
    libusb_hotplug_flag flag = static_cast<libusb_hotplug_flag>(LIBUSB_HOTPLUG_ENUMERATE);
    auto rc = libusb_hotplug_register_callback(ctx_, event, flag, 0x12d1, LIBUSB_HOTPLUG_MATCH_ANY,
                                          LIBUSB_HOTPLUG_MATCH_ANY, hotplug_callback, this,
                                          &handle_);

    OXYGEN_ASSERT(LIBUSB_SUCCESS == rc);

    flag_ = true;
}

void usb_plugin_monitor::usb_monitor_loop()
{
    timeval timeout = {1, 0};
    libusb_handle_events_timeout(ctx_, &timeout);
}

int usb_plugin_monitor::hotplug_callback(libusb_context * _ctx, libusb_device * _dev,
                                         libusb_hotplug_event _event, void * _user_data)
{
    auto monitor = reinterpret_cast<usb_plugin_monitor * >(_user_data);
    if (monitor != nullptr
            && monitor->ctx_ == _ctx){
        return monitor->hotplug_handler(_ctx, _dev, _event);
    }

    return 0;
}

int usb_plugin_monitor::hotplug_handler(libusb_context * _ctx, libusb_device * _dev, libusb_hotplug_event _event)
{
    if (cb_) {
        cb_(_ctx, _dev, _event);
    }
    return 0;
}

} // namespace hsae
