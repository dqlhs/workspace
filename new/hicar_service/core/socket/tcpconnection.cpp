#include "tcpconnection.h"

#include "log/oxygen_log.h"

#define TCP_SERVER_BUFFER_SIZE 2048

namespace hsae {

tcpconnection::tcpconnection(tcp::socket && _socket)
    : socket_(std::move(_socket))
{
    INFO("socket connect ip : %s, port : %d",
         socket_.remote_endpoint().address().to_string().c_str(),
         int(socket_.remote_endpoint().port()));

    set_recv();
}

tcpconnection::~tcpconnection()
{
    close();
}

void tcpconnection::close()
{
    if (socket_.is_open()) {
        INFO("socket disconnect ip : %s, port : %d",
             socket_.remote_endpoint().address().to_string().c_str(),
             int(socket_.remote_endpoint().port()));
        socket_.cancel();
        socket_.close();
    }
}

void tcpconnection::send(const std::shared_ptr<std::vector<uint8_t> > & _buffer)
{
    if (!socket_.is_open()) {
        ERROR("socket is not open");
        return;
    }

    socket_.get_executor().post([_buffer, this] () {
        if (send_queue_.empty()) {
            send_queue_.push(_buffer);
            socket_.async_write_some(asio::buffer(send_queue_.front()->data(),
                                                  send_queue_.front()->size()),
                                     std::bind(&tcpconnection::on_write_handler, this,
                                               std::placeholders::_1,
                                               std::placeholders::_2,
                                               send_queue_.front()->size()));
        } else {
            send_queue_.push(_buffer);
        }
    }, std::allocator<void>());
}

void tcpconnection::set_recv_handler(recv_handler && _func)
{
    recv_handler_ = std::move(_func);
}

void tcpconnection::set_error_handler(std::function<void (const error_code &)> && _handler)
{
    error_handler_ = std::move(_handler);
}

void tcpconnection::on_recv(error_code _ec, std::size_t _size)
{
    if (_ec) {
        on_error(_ec);
        return;
    }

    if (_size == 0) {
        ERROR("_size == 0");
    } else {
        // handle recv buffer
        handle_recv(buf_, _size);
    }

    // recv again
    set_recv();
}

void tcpconnection::on_error(const error_code & _ec)
{
    if (error_handler_) {
        error_handler_(_ec);
    }
}

void tcpconnection::on_write_handler(asio::error_code _ec, std::size_t _size, std::size_t _remained)
{
    if (_ec) {
        WARN("error : %s", _ec.message().c_str());
        on_error(_ec);
        return;
    }

    if (_remained == _size) {
        if (send_queue_.empty()) {
            ERROR("logic error remained == size, but send_queue_ is empty");
            abort();
        }
        send_queue_.pop();

        if (send_queue_.empty()) {
            TRACE("send_queue_ is empty");
            return;
        } else {
            socket_.async_write_some(asio::buffer(send_queue_.front()->data(),
                                                  send_queue_.front()->size()),
                                     std::bind(&tcpconnection::on_write_handler, this,
                                               std::placeholders::_1,
                                               std::placeholders::_2,
                                               send_queue_.front()->size()));
        }


    } else if (_size > _remained) {
        ABORT("logic error size > remained");
    } else {
        _remained = _remained - _size;
        socket_.async_write_some(asio::buffer(send_queue_.front()->data()
                                              + (send_queue_.front()->size() - _remained),
                                              _remained),
                                 std::bind(&tcpconnection::on_write_handler, this,
                                           std::placeholders::_1,
                                           std::placeholders::_2,
                                           _remained));
    }

}

void tcpconnection::handle_recv(const shared_buffer & _buffer, std::size_t _size)
{
    if (recv_handler_) {
        recv_handler_(_buffer, _size);
    }
}

void tcpconnection::set_recv()
{
    buf_ = std::make_shared<std::vector<uint8_t>>(TCP_SERVER_BUFFER_SIZE);
    socket_.async_receive(asio::buffer(buf_->data(), buf_->size()),
                          std::bind(&tcpconnection::on_recv, this,
                                    std::placeholders::_1,
                                    std::placeholders::_2));
}

} // namespace hsae
