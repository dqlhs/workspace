#pragma once

#include <cstdint>
#include "tcpserver.h"

namespace hsae {

enum class tcpserver_status {
    server_error = 0x00,
    channel_error = 0x01,
    new_connection = 0x02,
};

typedef std::function<void (const uint8_t * _data, std::size_t _size)> buf_handler;
typedef std::function<void (const tcpserver_status & status)> status_handler;

class tcpserver_impl : public tcpserver {
public:
    tcpserver_impl(uint16_t _port);

    virtual ~tcpserver_impl() override;

    virtual void on_connection(const std::shared_ptr<hsae::tcpconnection> & _connection,
                               const tcp::endpoint & _remote) override;

    virtual void on_error(const asio::error_code &_ec) override;

    void send_data(const uint8_t * _data, std::size_t _size);

    void set_recv_handler(buf_handler && _cb);
    void set_status_handler(status_handler && _cb);

    std::string peer_properties_ip() const;
    uint16_t peer_properties_port() const;

private:
    void connection_recv_handler(const shared_buffer & _data, std::size_t _size);
    void connection_error_handler(std::shared_ptr<tcpconnection> _connection,
                                  const asio::error_code & _ec);

    void notify_status(const tcpserver_status & _status);

private:
    std::shared_ptr<hsae::tcpconnection> last_connection_;
    buf_handler recv_handler_;
    status_handler status_handler_;
    tcp::endpoint remote_;
};

} // namespace hsae
