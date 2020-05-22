#pragma once

#include <map>
#include <chrono>
#include <mutex>
#include <set>

#include "plugin_base.h"

namespace hsae {
namespace oxygen {

class plugin_manager {
public:
        static std::shared_ptr<plugin_manager> get();

        plugin_manager();

        ~plugin_manager();

        void load_plugins();

        std::shared_ptr<plugin> get_plugin(std::string _name);

        std::shared_ptr<plugin> load_plugin(const std::string _library, const uint32_t _version);

        bool unload_plugin(std::string _name);

private:
        void add_plugin(const std::shared_ptr<plugin> &_plugin, const std::string _name);

        void * load_library(const std::string &_path);
        void * load_symbol(void * _handle, const std::string &_symbol);

        bool plugins_loaded_;
        std::mutex loader_mutex_;

        std::map<std::string, std::shared_ptr<plugin> >  plugins_;
        std::map<std::string, void*>  handles_;

        std::mutex plugins_mutex_;

        static std::shared_ptr<plugin_manager> the_plugin_manager__;
};

} // namespace oxygen
} // namespace hsae
