#pragma once

#include <memory>
#include <thread>
#include <asio.hpp>

namespace hsae {

class io_center {
public:
    static std::shared_ptr<io_center> & default_center();
    asio::io_context & io();

    io_center();
    ~io_center();

private:
    void io_run();

private:
    asio::io_context io_;
    asio::io_context::work work_;
    std::thread thread_;
};

} // namespace hsae
