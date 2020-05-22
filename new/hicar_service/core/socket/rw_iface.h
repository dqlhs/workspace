#pragma once

#include <memory>
#include <functional>

namespace hsae {

template <typename BufType, typename ErrorType>
class rw_iface {
public:
    virtual ErrorType write (const std::shared_ptr<BufType> & _buffer);
    virtual void set_read_handler(std::function<void (const std::shared_ptr<BufType> &)> && _handler);
    virtual void set_error_handler(std::function<void(const ErrorType &)> && _handler_error);
};

} // namespace hsae
