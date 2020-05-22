#include "net_helper.h"

#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdexcept>

#include "log/oxygen_log.h"
#include "exception/throw.h"

namespace hsae {

std::string interface_to_ip(const std::string & _dev)
{
    struct ifreq ifc;

    int sockfd = 0;
    bool is_successful = !_dev.empty()
                         && ((void)strcpy(ifc.ifr_name, _dev.c_str()), true)
                         && ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0)
                         && (ioctl(sockfd, SIOCGIFADDR, &ifc) >= 0);

    std::string interface_ip;

    if (is_successful) {
        interface_ip = inet_ntoa(((struct sockaddr_in * )(&(ifc.ifr_addr)))->sin_addr);
    } else {
        THROW(std::runtime_error("!!! Failed to get target interface IP"));
    }

    if (sockfd >= 0) {
        close(sockfd);
    }

    return interface_ip;
}

std::array<uint8_t, 6> interface_to_mac(const std::string & _dev)
{
    struct ifreq ifc;

    int sockfd = 0;
    bool is_successful = !_dev.empty()
                         && ((void)strcpy(ifc.ifr_name, _dev.c_str()), true)
                         && ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0)
                         && (ioctl(sockfd, SIOCGIFHWADDR, &ifc) >= 0);

    std::array<uint8_t, 6> interface_mac;

    if (is_successful) {
        memcpy(interface_mac.data(), ifc.ifr_ifru.ifru_hwaddr.sa_data, 6);
    } else {
        THROW(std::runtime_error("interface_to_mac"));
    }

    if (sockfd >= 0) {
        close(sockfd);
    }

    return interface_mac;
}

} // namespace hase
