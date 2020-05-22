#pragma once

#include <memory>

#define OXYGEN_PLUGIN_INIT_SYMBOL "oxygen_plugin_init"

namespace hsae {
namespace oxygen {

class plugin;

typedef std::shared_ptr<plugin> (*create_plugin_func)();
typedef create_plugin_func (*plugin_init_func)();

class plugin {
public:
    virtual ~plugin();

    virtual uint32_t get_plugin_version() const = 0;
    virtual const std::string &get_plugin_name() const = 0;
};

template<class Plugin_>
class plugin_impl : public plugin {
public:
    static std::shared_ptr<plugin> get_plugin() {
        return std::make_shared<Plugin_>();
    }

    plugin_impl(const std::string &_name, uint32_t _version) {
        name_ = _name;
        version_ = _version;
    }

    const std::string &get_plugin_name() const {
        return name_;
    }

    uint32_t get_plugin_version() const {
        return version_;
    }

private:
    uint32_t version_;
    std::string name_;
};

#define OXYGEN_PLUGIN(class_name) \
    extern "C" { \
        hsae::oxygen::create_plugin_func oxygen_plugin_init() { \
            return class_name::get_plugin; \
        } \
    }

} // namespace oxygen
} // namespace hsae
