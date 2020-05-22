#pragma once

#include <queue>
#include <functional>

#include "netcore.h"

namespace hsae {

typedef std::shared_ptr<std::vector<uint8_t>> shared_buffer;

typedef std::function<void(const shared_buffer & , std::size_t size)> recv_handler;
typedef std::function<void(const asio::error_code & _ec)> error_handler;

class tcpconnection {
public:
    tcpconnection(tcp::socket && _socket);
    ~tcpconnection();

    void close();

    void send (const std::shared_ptr<std::vector<uint8_t>> & _buffer);

    void set_recv_handler(recv_handler && _func);
    void set_error_handler(std::function<void(const asio::error_code &)> && _handler);

private:
    void on_recv(asio::error_code _ec, std::size_t _size);
    void on_error(const asio::error_code & _ec);
    void on_write_handler(asio::error_code _ec, std::size_t _size, std::size_t _remained);

    void handle_recv(const shared_buffer & _buffer, std::size_t _size);

    void set_recv();

private:
    tcp::socket socket_;
    recv_handler recv_handler_;
    error_handler error_handler_;

    std::queue<std::shared_ptr<std::vector<uint8_t>>> send_queue_;
    std::shared_ptr<std::vector<uint8_t>> buf_;
};

} // namespace hsae
