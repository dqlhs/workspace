#specify the minimum version of cmake
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8 FATAL_ERROR)

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/src/)

install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/config/wifi.conf DESTINATION /etc/dbus-1/system.d/)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/config/Wifi.service DESTINATION /etc/systemd/system/multi-user.target.wants/)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/test_5G.conf DESTINATION /etc/)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/test_24G.conf DESTINATION /etc/)

install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/entropy.bin DESTINATION /etc/)

install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/libsigc-2.0.so DESTINATION /usr/lib)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/hostapd PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE DESTINATION /usr/bin)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/hostapd_cli PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE DESTINATION /usr/bin)
#update wifi bin

install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/Wi-Fi/drivers/cfg80211.ko DESTINATION /lib/modules/)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/Wi-Fi/drivers/cfg80211.ko DESTINATION /)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/Wi-Fi/drivers/compat.ko DESTINATION /lib/modules/)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/Wi-Fi/drivers/compat.ko DESTINATION /)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/Wi-Fi/drivers/wlan.ko DESTINATION /lib/modules/)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/Wi-Fi/drivers/wlan.ko DESTINATION /)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/Wi-Fi/firmwares/lib/firmware/bdwlan30.bin DESTINATION /lib/firmware/)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/Wi-Fi/firmwares/lib/firmware/otp30.bin DESTINATION /lib/firmware/)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/Wi-Fi/firmwares/lib/firmware/qwlan30.bin DESTINATION /lib/firmware/)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/Wi-Fi/firmwares/lib/firmware/utf30.bin DESTINATION /lib/firmware/)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/Wi-Fi/firmwares/lib/firmware/utfbd30.bin DESTINATION /lib/firmware/)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/Wi-Fi/firmwares/lib/firmware/wlan/cfg.dat DESTINATION /lib/firmware/wlan)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/Wi-Fi/firmwares/lib/firmware/wlan/qcom_cfg.ini DESTINATION /lib/firmware/wlan)



