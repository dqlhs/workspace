#pragma once

#include <string>
#include <memory>
#include "nlohmann/json.hpp"
#include "hicar.h"

namespace hsae {
class unique_config {
public:
    unique_config(const unique_config &) = delete;
    unique_config(unique_config &&) = delete;

    unique_config & operator = (const unique_config &) = delete;
    unique_config & operator = (unique_config &&) = delete;

    static unique_config & instance();

    /**
     * @brief get local br mac addr, format : xx:xx:xx:xx:xx:xx;
     * @return mac addr
     */
    std::string local_br_addr_mac() const;


    uint32_t modelid() const;
    uint8_t submodelid() const;
    CarConfig hicarconfig() const;

    const std::string & gps_interface() const;

private:
    unique_config();
    ~unique_config();

    void initHicarconfig(nlohmann::json json);

    std::string unique_config_file_;
    std::string modelid_;
    std::string submodelid_;

    std::string gps_interface_;
    CarConfig hicarconfig_;
};

} // namespace hsae
