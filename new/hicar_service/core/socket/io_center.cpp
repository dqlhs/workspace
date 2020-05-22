#include "io_center.h"

#include <functional>

#include "log/oxygen_log.h"

namespace hsae {


std::shared_ptr<io_center> & io_center::default_center()
{
    static std::shared_ptr<io_center> center = std::make_shared<io_center>();
    return center;
}

asio::io_context & io_center::io()
{
    return io_;
}

io_center::io_center()
    : io_(), work_(io_)
{
    INFO("io context create");
    thread_ = std::thread(std::bind(&io_center::io_run, this));
}

io_center::~io_center()
{
    io_.stop();
    thread_.join();
}

void io_center::io_run()
{
    try {
        io_.run();
    } catch (std::exception _ec) {
        WARN("io error : %s", _ec.what());
    }
}

} // namespace hsae
