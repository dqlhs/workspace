#include "tcp_server_channel.h"

#include <functional>

#include "log/oxygen_log.h"

#include "io_center.h"

namespace hsae {

tcpserver_impl::tcpserver_impl(uint16_t _port)
    : tcpserver(io_center::default_center()->io(), _port)
{

}

tcpserver_impl::~tcpserver_impl() {}

void tcpserver_impl::on_connection(const std::shared_ptr<tcpconnection> & _connection, const tcp::endpoint & _remote)
{
    (void) _remote;
    if (last_connection_) {
        last_connection_->close();
    }

    last_connection_ = _connection;
    last_connection_->set_error_handler(std::bind(&tcpserver_impl::connection_error_handler, this,
                                                  _connection,
                                                  std::placeholders::_1));
    last_connection_->set_recv_handler(std::bind(&tcpserver_impl::connection_recv_handler, this,
                                                 std::placeholders::_1, std::placeholders::_2));

    remote_ = _remote;

    INFO("new connection");
    notify_status(tcpserver_status::new_connection);
}

void tcpserver_impl::on_error(const error_code & _ec)
{
    WARN("!!! error : %s", _ec.message().c_str());
    notify_status(tcpserver_status::server_error);
}

void tcpserver_impl::send_data(const uint8_t * _data, std::size_t _size)
{
    if (last_connection_) {
        auto buf = std::make_shared<std::vector<uint8_t>>(_data, _data + _size);
        last_connection_->send(buf);
    }
}

void tcpserver_impl::set_recv_handler(buf_handler && _cb)
{
    recv_handler_ = std::move(_cb);
}

void tcpserver_impl::set_status_handler(status_handler && _cb)
{
    status_handler_ = std::move(_cb);
}

std::string tcpserver_impl::peer_properties_ip() const
{
    return remote_.address().to_string().c_str();
}

uint16_t tcpserver_impl::peer_properties_port() const
{
    return remote_.port();
}

void tcpserver_impl::connection_error_handler(std::shared_ptr<tcpconnection> _connection,
                                              const error_code & _ec)
{
    WARN("!!! connection 0x%" PRIXPTR " error : %s", (uintptr_t)_connection.get(), _ec.message().c_str());
    if (_connection == last_connection_) {
        notify_status(tcpserver_status::channel_error);
    } else {
        TRACE("last_connection 0x%" PRIXPTR " != connection 0x%" PRIXPTR,
              (uintptr_t)last_connection_.get(), (uintptr_t)_connection.get());
    }
}

void tcpserver_impl::notify_status(const tcpserver_status & _status)
{
    if (status_handler_) {
        status_handler_(_status);
    }
}

void tcpserver_impl::connection_recv_handler(const shared_buffer & _data, std::size_t _size)
{
    TRACE("recv data size : %d", (int)_size);
    if (recv_handler_) {
        recv_handler_(_data->data(), _size);
    }
}

} // namespace hsae
