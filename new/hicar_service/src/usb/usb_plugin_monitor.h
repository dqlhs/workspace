#pragma once

#include <functional>

#include <libusb-1.0/libusb.h>

namespace hsae {

class usb_plugin_monitor final {
public:
    usb_plugin_monitor(int _vendor_id, int _product_id);

    virtual ~usb_plugin_monitor();

    void set_plugin_change_callback(std::function<void(struct libusb_context *_ctx, struct libusb_device *_dev,
                                                       libusb_hotplug_event _event)> && _cb);

    void usb_monitor_loop();

private:
    static int hotplug_callback(struct libusb_context *_ctx, struct libusb_device *_dev,
                                libusb_hotplug_event _event, void *_user_data);

    int hotplug_handler(struct libusb_context *_ctx, struct libusb_device *_dev,
                        libusb_hotplug_event _event);

private:
    int vendor_id_;
    int product_id_;

    bool flag_;

    libusb_context * ctx_;
    libusb_hotplug_callback_handle handle_;

    std::function<void(struct libusb_context *_ctx, struct libusb_device *_dev,
                       libusb_hotplug_event _event)> cb_;
};

} // namespace hsae
