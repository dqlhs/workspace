#include "plugin_manager.h"

#include <dlfcn.h>

#include <sstream>
#include <vector>
#include <stdlib.h>
#include <iostream>

#include "oxygen_internal.h"
#include "log/oxygen_log.h"


namespace hsae {
namespace oxygen {

std::shared_ptr<plugin_manager> plugin_manager::the_plugin_manager__ =
        std::make_shared<plugin_manager>();

std::shared_ptr<plugin_manager> plugin_manager::get() {
    return the_plugin_manager__;
}

plugin_manager::plugin_manager() :
    plugins_loaded_(false) {
}

plugin_manager::~plugin_manager() {
    handles_.clear();
    plugins_.clear();
}

void plugin_manager::load_plugins() {
    {
        std::lock_guard<std::mutex> its_lock_start_stop(loader_mutex_);
        if (plugins_loaded_) {
            return;
        }
        plugins_loaded_ = true;
    }

    // Get plug-ins libraries from environment
    std::vector<std::string> plugins;
    const char *its_plugins = getenv(OXYGEN_ENV_LOAD_PLUGINS);
    if (nullptr != its_plugins) {
        std::string token;
        std::stringstream ss(its_plugins);
        while(std::getline(ss, token, ',')) {
            plugins.push_back(token);
        }
    }

    std::lock_guard<std::mutex> its_lock_start_stop(plugins_mutex_);
    // Load plug-in info from libraries parsed before
    for (auto plugin_name : plugins) {
        void* handle = load_library(plugin_name);
        plugin_init_func its_init_func =  reinterpret_cast<plugin_init_func>(
                                              load_symbol(handle, OXYGEN_PLUGIN_INIT_SYMBOL));
        if (its_init_func) {
            create_plugin_func its_create_func = (*its_init_func)();
            if (its_create_func) {
                auto its_plugin = (*its_create_func)();
                if (its_plugin) {
                    handles_[plugin_name] = handle;
                    INFO("load library name : %s, version : %d",
                         plugin_name.c_str(), (int)its_plugin->get_plugin_version());
                    add_plugin(its_plugin, plugin_name);
                }
            }
        }
    }
}

std::shared_ptr<plugin> plugin_manager::get_plugin(std::string _name) {
    std::lock_guard<std::mutex> its_lock_start_stop(plugins_mutex_);
    auto its_name = plugins_.find(_name);
    if (its_name != plugins_.end()) {
        return its_name->second;
    }
    return load_plugin(_name, 1);
}

std::shared_ptr<plugin> plugin_manager::load_plugin(const std::string _library, uint32_t _version) {
    void* handle = load_library(_library);
    plugin_init_func its_init_func = reinterpret_cast<plugin_init_func>(
                                         load_symbol(handle, OXYGEN_PLUGIN_INIT_SYMBOL));
    if (its_init_func) {
        create_plugin_func its_create_func = (*its_init_func)();
        if (its_create_func) {
            handles_[_library] = handle;
            auto its_plugin = (*its_create_func)();
            if (its_plugin) {
                if (its_plugin->get_plugin_version() == _version) {
                    add_plugin(its_plugin, _library);
                    return its_plugin;
                } else {
                    ERROR("Plugin version mismatch. Ignoring plugin %s",
                          its_plugin->get_plugin_name().c_str());
                }
            }
        }
    }
    return nullptr;
}

bool plugin_manager::unload_plugin(std::string _name) {
    std::lock_guard<std::mutex> its_lock_start_stop(plugins_mutex_);
    const auto found_name = handles_.find(_name);
    if (found_name != handles_.end()) {
#ifdef _WIN32
        FreeLibrary((HMODULE)its_name.second);
#else
        if (dlclose(found_name->second)) {
            ERROR("Unloading failed: (%s)", dlerror());
        }
#endif
    } else {
        ERROR("plugin_manager::unload_plugin didn't find plugin name: %s", _name.c_str());
        return false;
    }
    return plugins_.erase(_name);
}

void plugin_manager::add_plugin(const std::shared_ptr<plugin> &_plugin, const std::string _name) {
    plugins_[_name] = _plugin;
}

void * plugin_manager::load_library(const std::string &_path) {
#ifdef _WIN32
    return LoadLibrary(_path.c_str());
#else
    return dlopen(_path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
#endif
}

void * plugin_manager::load_symbol(void * _handle,
                                   const std::string &_symbol) {
    void * its_symbol = nullptr;
#ifdef _WIN32
    HINSTANCE hDLL = (HINSTANCE)_handle;
    if (hDLL != NULL) {

        typedef UINT(CALLBACK* LPFNDLLFUNC1)(DWORD, UINT);

        LPFNDLLFUNC1 lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL, _symbol.c_str());
        if (!lpfnDllFunc1) {
            FreeLibrary(hDLL);
            std::cerr << "Loading symbol \"" << _symbol << "\" failed (" << GetLastError() << ")" << std::endl;
        }
        else {
            its_symbol = lpfnDllFunc1;
        }
    }
#else
    if (nullptr != _handle) {
        its_symbol = dlsym(_handle, _symbol.c_str());
        const char *dlsym_error = dlerror();
        if (dlsym_error) {
            INFO("Cannot load symbol : %s  %s", _symbol.c_str(), dlsym_error);
            dlclose(_handle);
        }
    } else {
        ERROR("Loading failed: (%s)", dlerror());
    }
#endif
    return (its_symbol);
}

} // namespace oxygen
} // namespace hsae
