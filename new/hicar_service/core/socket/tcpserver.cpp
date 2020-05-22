#include "tcpserver.h"

#include <functional>

#include "log/oxygen_log.h"

namespace hsae {

tcpserver::tcpserver(io_context & _io, uint16_t _port)
    : acceptor_(_io, tcp::endpoint(tcp::v4(), _port)),
      socket_(_io)
{
    acceptor_.async_accept(socket_, remote_, std::bind(&tcpserver::accept_handler, this, std::placeholders::_1));
}

tcpserver::~tcpserver()
{

}

void tcpserver::on_connection(const std::shared_ptr<tcpconnection> & _connection, const tcp::endpoint & _remote)
{
    (void)_connection;
    INFO("remote connection ip : %s, port : %d", _remote.address().to_string().c_str(), int(_remote.port()));
}

void tcpserver::on_error(const error_code & _ec)
{
    INFO("!!! error : %s", _ec.message().c_str());
}

void tcpserver::accept_handler(error_code _ec)
{
    if (_ec) {
        ERROR("accept error : %s", _ec.message().c_str());
        on_error(_ec);
        return;
    }

    add_connection();

    acceptor_.async_accept(socket_, std::bind(&tcpserver::accept_handler, this, std::placeholders::_1));
}

void tcpserver::add_connection()
{
    auto connection = std::make_shared<hsae::tcpconnection>(std::move(socket_));
    on_connection(std::move(connection), remote_);
}

} // namespace hsae
