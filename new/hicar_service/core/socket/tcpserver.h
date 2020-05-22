#pragma once

#include "netcore.h"
#include "tcpconnection.h"

namespace hsae {

class tcpserver {
public:
    tcpserver(io_context & _io, uint16_t _port);
    virtual ~tcpserver();

    virtual void on_connection(const std::shared_ptr<hsae::tcpconnection> & _connection,
                               const tcp::endpoint & _remote);
    virtual void on_error(const asio::error_code & _ec);

private:
    void accept_handler(asio::error_code _ec);

    void add_connection();

private:
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    tcp::endpoint remote_;
};

} // namespace hsae
