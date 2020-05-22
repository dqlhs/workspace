#include "unique_config.h"

#include <fstream>

#include <exception>

#include "nlohmann/json.hpp"

#include "utility/hex_conv.h"

#define CAR_DEVICE_ID_STUB "BJ-530-2019-ABCDEFG"

#define UNIQUE_CONFIG_ENV_VALUE "UNIQUE_CONFIG_ENV_VALUE"

#define DEFAULT_UNIQUE_CONFIG_PATH "/etc/unique_config.json"

namespace hsae {

unique_config & unique_config::instance()
{
    static unique_config obj;
    return obj;
}

std::string unique_config::local_br_addr_mac() const
{
    static std::string mac;
    if (mac.empty()) {
        char buffer[256] = {0};
        snprintf(buffer, sizeof (buffer),
                 "/bin/grep \"bd_addr=\" %s | /usr/bin/awk -F= '{print $2}'", unique_config_file_.c_str());
        FILE * fp = popen(buffer, "r");
        if (fp && fgets(buffer, sizeof (buffer), fp)) {
            size_t len = strlen(buffer);
            if (len >= 17) {
                mac.append(buffer, 17);
            }
        } else {
            throw std::runtime_error("!!! popen error");
        }
    }
    return mac;
}

uint32_t unique_config::modelid() const
{
    uint32_t value = 0;
    if (modelid_.length() == 6) {
        value = oxygen::hex_6byte_to_uint32(modelid_);
    } else {
        throw std::runtime_error("!!! device_id_.length() == 6");
    }

    return value;
}

uint8_t unique_config::submodelid() const
{
    uint8_t value = 0;
    if (submodelid_.length() == 2) {
        value = oxygen::hex_2byte_to_uint8(submodelid_);
    } else {
        throw std::runtime_error("!!! submodelid_.length() == 2");
    }

    return value;
}

CarConfig unique_config::hicarconfig() const
{
    return hicarconfig_;
}

const std::string & unique_config::gps_interface() const
{
    return gps_interface_;
}

unique_config::unique_config()
{
    char * env_value = getenv(UNIQUE_CONFIG_ENV_VALUE);
    std::string config_file_path = env_value ? env_value : DEFAULT_UNIQUE_CONFIG_PATH;
    std::ifstream file_handler(config_file_path);
    if (!file_handler.is_open()) {
        throw (std::runtime_error("!!! open unique config file fail"));
    }

    nlohmann::json json;
    file_handler >> json;


    unique_config_file_ = json.at("unique_config_file").get<std::string>();

    modelid_ = json.at("modelid").get<std::string>();
    submodelid_ = json.at("submodelid").get<std::string>();

    gps_interface_ = json.at("gps_interface").get<std::string>();

    initHicarconfig(json);
}

unique_config::~unique_config()
{
    if (hicarconfig_.connectInfo.carDeviceId != NULL) {
           free(hicarconfig_.connectInfo.carDeviceId);
           hicarconfig_.connectInfo.carDeviceId = NULL;
       }
    if (hicarconfig_.featureInfo.carModel.value != NULL) {
           free(hicarconfig_.featureInfo.carModel.value);
           hicarconfig_.featureInfo.carModel.value = NULL;
       }
    if (hicarconfig_.featureInfo.manufacturer.value != NULL) {
           free(hicarconfig_.featureInfo.manufacturer.value);
           hicarconfig_.featureInfo.manufacturer.value = NULL;
       }
    if (hicarconfig_.featureInfo.model.value != NULL) {
           free(hicarconfig_.featureInfo.model.value);
           hicarconfig_.featureInfo.model.value = NULL;
       }
    if (hicarconfig_.featureInfo.os.value != NULL) {
           free(hicarconfig_.featureInfo.os.value);
           hicarconfig_.featureInfo.os.value = NULL;
       }
    if (hicarconfig_.featureInfo.cpu.value != NULL) {
           free(hicarconfig_.featureInfo.cpu.value);
           hicarconfig_.featureInfo.cpu.value = NULL;
       }
    if (hicarconfig_.featureInfo.ram.value != NULL) {
           free(hicarconfig_.featureInfo.ram.value);
           hicarconfig_.featureInfo.ram.value = NULL;
       }
    if (hicarconfig_.featureInfo.rom.value != NULL) {
           free(hicarconfig_.featureInfo.rom.value);
           hicarconfig_.featureInfo.rom.value = NULL;
       }
    if (hicarconfig_.featureInfo.screenSize.value != NULL) {
           free(hicarconfig_.featureInfo.screenSize.value);
           hicarconfig_.featureInfo.screenSize.value = NULL;
       }
    if (hicarconfig_.featureInfo.screenMetrics.value != NULL) {
           free(hicarconfig_.featureInfo.screenMetrics.value);
           hicarconfig_.featureInfo.screenMetrics.value = NULL;
       }
    if (hicarconfig_.featureInfo.linuxKernel.value != NULL) {
           free(hicarconfig_.featureInfo.linuxKernel.value);
           hicarconfig_.featureInfo.linuxKernel.value = NULL;
       }
    if (hicarconfig_.featureInfo.btChip.value != NULL) {
           free(hicarconfig_.featureInfo.btChip.value);
           hicarconfig_.featureInfo.btChip.value = NULL;
       }
    if (hicarconfig_.featureInfo.wifiChip.value != NULL) {
           free(hicarconfig_.featureInfo.wifiChip.value);
           hicarconfig_.featureInfo.wifiChip.value = NULL;
    }
}

void unique_config::initHicarconfig(nlohmann::json json)
{
    INFO("initHIcarconfig");
    try{
        std::string t_modelId = json.at("init").at("modelId").get<std::string>();
        memset(hicarconfig_.connectInfo.modelId, 0, MODEL_ID_MAX_LEN);
        memcpy(hicarconfig_.connectInfo.modelId, t_modelId.data() , MODEL_ID_MAX_LEN);
        memset(hicarconfig_.connectInfo.brMac, 0, BR_MAC_MAX_LEN);
        memcpy(hicarconfig_.connectInfo.brMac, local_br_addr_mac().data(), 17);

        hicarconfig_.connectInfo.connectAllowType = json.at("init").at("connectAllowType").get<uint32_t>();

        std::string t_carDeviceId = json.at("init").at("carDeviceId").get<std::string>();
        hicarconfig_.connectInfo.carDeviceId = (char*)malloc(strlen(t_carDeviceId.data()) + 1);
        memcpy(hicarconfig_.connectInfo.carDeviceId, t_carDeviceId.data(), t_carDeviceId.length());
        hicarconfig_.connectInfo.idLen = strlen(hicarconfig_.connectInfo.carDeviceId);

        std::string t_carBrand = json.at("init").at("carBrand").get<std::string>();
        memset(hicarconfig_.deviceInfo.carBrand, 0, CAR_BRAND_MAX_LEN);
        memcpy(hicarconfig_.deviceInfo.carBrand,t_carBrand.data(), t_carBrand.length());
        hicarconfig_.deviceInfo.carBrandLen = t_carBrand.length();

        std::string t_dayNightInfo = json.at("init").at("dayNightInfo").get<std::string>();
        memset(hicarconfig_.deviceInfo.dayNightInfo, 0, CAR_DAY_NIGHT_MAX_LEN);
        memcpy(hicarconfig_.deviceInfo.dayNightInfo, t_dayNightInfo.data(), t_dayNightInfo.length());
        hicarconfig_.deviceInfo.carDayNightLen = t_dayNightInfo.length();

        hicarconfig_.deviceInfo.steeringWheelPos = json.at("init").at("steeringWheelPos").get<uint32_t>();

        hicarconfig_.displayCapability.codecs = json.at("init").at("codecs").get<uint32_t>();
        hicarconfig_.displayCapability.fps = json.at("init").at("fps").get<uint32_t>();
        hicarconfig_.displayCapability.gop = json.at("init").at("gop").get<uint32_t>();
        hicarconfig_.displayCapability.bitrate = json.at("init").at("bitrate").get<uint32_t>();
        hicarconfig_.displayCapability.minBitrate = json.at("init").at("minBitrate").get<uint32_t>();
        hicarconfig_.displayCapability.maxBitrate = json.at("init").at("maxBitrate").get<uint32_t>();
        hicarconfig_.displayCapability.dpi = json.at("init").at("dpi").get<uint32_t>();
        hicarconfig_.displayCapability.profile = json.at("init").at("profile").get<uint32_t>();
        hicarconfig_.displayCapability.level = json.at("init").at("level").get<uint32_t>();
        hicarconfig_.displayCapability.screenWidth = json.at("init").at("screenWidth").get<uint32_t>();
        hicarconfig_.displayCapability.screenHeight = json.at("init").at("screenHeight").get<uint32_t>();
        hicarconfig_.displayCapability.width = json.at("init").at("width").get<uint32_t>();
        hicarconfig_.displayCapability.height = json.at("init").at("height").get<uint32_t>();

        int len = 0;
        std::string t_carModel = json.at("init").at("carModel").get<std::string>();
        len = t_carModel.length();
        hicarconfig_.featureInfo.carModel.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.carModel.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.carModel.value, t_carModel.data() , len);
        hicarconfig_.featureInfo.carModel.length = len;

        std::string t_manufacturer = json.at("init").at("manufacturer").get<std::string>();
        len = t_manufacturer.length();
        hicarconfig_.featureInfo.manufacturer.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.manufacturer.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.manufacturer.value, t_manufacturer.data() , len);
        hicarconfig_.featureInfo.manufacturer.length = len;

        std::string t_model = json.at("init").at("model").get<std::string>();
        len = t_model.length();
        hicarconfig_.featureInfo.model.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.model.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.model.value, t_model.data() , len);
        hicarconfig_.featureInfo.model.length = len;

        std::string t_os = json.at("init").at("os").get<std::string>();
        len = t_os.length();
        hicarconfig_.featureInfo.os.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.os.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.os.value, t_os.data() , len);
        hicarconfig_.featureInfo.os.length = len;

        std::string t_cpu = json.at("init").at("cpu").get<std::string>();
        len = t_cpu.length();
        hicarconfig_.featureInfo.cpu.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.cpu.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.cpu.value, t_cpu.data() , len);
        hicarconfig_.featureInfo.cpu.length = len;

        std::string t_ram = json.at("init").at("ram").get<std::string>();
        len = t_ram.length();
        hicarconfig_.featureInfo.ram.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.ram.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.ram.value, t_ram.data() , len);
        hicarconfig_.featureInfo.ram.length = len;

        std::string t_rom = json.at("init").at("rom").get<std::string>();
        len = t_rom.length();
        hicarconfig_.featureInfo.rom.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.rom.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.rom.value, t_rom.data() , len);
        hicarconfig_.featureInfo.rom.length = len;

        std::string t_screenSize = json.at("init").at("screenSize").get<std::string>();
        len = t_screenSize.length();
        hicarconfig_.featureInfo.screenSize.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.screenSize.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.screenSize.value, t_screenSize.data() , len);
        hicarconfig_.featureInfo.screenSize.length = len;

        std::string t_screenMetrics = json.at("init").at("screenMetrics").get<std::string>();
        len = t_screenMetrics.length();
        hicarconfig_.featureInfo.screenMetrics.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.screenMetrics.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.screenMetrics.value, t_screenMetrics.data() , len);
        hicarconfig_.featureInfo.screenMetrics.length = len;

        std::string t_linuxKernel = json.at("init").at("linuxKernel").get<std::string>();
        len = t_linuxKernel.length();
        hicarconfig_.featureInfo.linuxKernel.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.linuxKernel.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.linuxKernel.value, t_linuxKernel.data() , len);
        hicarconfig_.featureInfo.linuxKernel.length = len;

        std::string t_btChip = json.at("init").at("btChip").get<std::string>();
        len = t_btChip.length();
        hicarconfig_.featureInfo.btChip.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.btChip.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.btChip.value, t_btChip.data(), len);
        hicarconfig_.featureInfo.btChip.length = len;

        std::string t_wifiChip = json.at("init").at("wifiChip").get<std::string>();
        len = t_wifiChip.length();
        hicarconfig_.featureInfo.wifiChip.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.wifiChip.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.wifiChip.value, t_wifiChip.data(), len);
        hicarconfig_.featureInfo.wifiChip.length = len;
    }

    catch (std::exception & e) {
        INFO("json error : %s", e.what());
        memset(hicarconfig_.connectInfo.modelId, 0, MODEL_ID_MAX_LEN);
        memcpy(hicarconfig_.connectInfo.modelId, "00041401" , MODEL_ID_MAX_LEN);
        memset(hicarconfig_.connectInfo.brMac, 0, BR_MAC_MAX_LEN);
        memcpy(hicarconfig_.connectInfo.brMac, local_br_addr_mac().data(), 17);

        hicarconfig_.connectInfo.connectAllowType = 0x3;

        hicarconfig_.connectInfo.carDeviceId = (char*)malloc(strlen(CAR_DEVICE_ID_STUB) + 1);
        memcpy(hicarconfig_.connectInfo.carDeviceId, CAR_DEVICE_ID_STUB, strlen(CAR_DEVICE_ID_STUB));
        hicarconfig_.connectInfo.idLen = strlen(hicarconfig_.connectInfo.carDeviceId);

        memset(hicarconfig_.deviceInfo.carBrand, 0, CAR_BRAND_MAX_LEN);
        memcpy(hicarconfig_.deviceInfo.carBrand,"BJ", strlen("BJ"));
        hicarconfig_.deviceInfo.carBrandLen = strlen("BJ");

        memset(hicarconfig_.deviceInfo.dayNightInfo, 0, CAR_DAY_NIGHT_MAX_LEN);
        memcpy(hicarconfig_.deviceInfo.dayNightInfo, "night", strlen("night"));
        hicarconfig_.deviceInfo.carDayNightLen = strlen("night");

        hicarconfig_.deviceInfo.steeringWheelPos = 0;

        hicarconfig_.displayCapability.codecs = 1;
        hicarconfig_.displayCapability.fps = 30;
        hicarconfig_.displayCapability.gop = 30;
        hicarconfig_.displayCapability.bitrate = 2*1000*1000;
        hicarconfig_.displayCapability.minBitrate = 1*1000*1000;
        hicarconfig_.displayCapability.maxBitrate = 10*1000*1000;
        hicarconfig_.displayCapability.dpi = 180;
        hicarconfig_.displayCapability.profile = 0;
        hicarconfig_.displayCapability.level = 0;
        hicarconfig_.displayCapability.screenWidth = 1280;
        hicarconfig_.displayCapability.screenHeight = 720;
        hicarconfig_.displayCapability.width = 1280;
        hicarconfig_.displayCapability.height = 720;

        int len = 0;
        len = strlen("BaoJun530");
        hicarconfig_.featureInfo.carModel.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.carModel.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.carModel.value, "BaoJun530" , len);
        hicarconfig_.featureInfo.carModel.length = len;

        len = strlen("qcom");
        hicarconfig_.featureInfo.manufacturer.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.manufacturer.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.manufacturer.value, "qcom" , len);
        hicarconfig_.featureInfo.manufacturer.length = len;

        len = strlen("msm8937_32go");
        hicarconfig_.featureInfo.model.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.model.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.model.value, "msm8937_32go" , len);
        hicarconfig_.featureInfo.model.length = len;

        len = strlen("8.1.0");
        hicarconfig_.featureInfo.os.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.os.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.os.value, "8.1.0" , len);
        hicarconfig_.featureInfo.os.length = len;

        len = strlen("armeabi-v7a");
        hicarconfig_.featureInfo.cpu.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.cpu.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.cpu.value, "armeabi-v7a" , len);
        hicarconfig_.featureInfo.cpu.length = len;

        len = strlen("1.81GB");
        hicarconfig_.featureInfo.ram.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.ram.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.ram.value, "1.81GB" , len);
        hicarconfig_.featureInfo.ram.length = len;

        len = strlen("9.68GB");
        hicarconfig_.featureInfo.rom.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.rom.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.rom.value, "1.81GB" , len);
        hicarconfig_.featureInfo.rom.length = len;

        len = strlen("12.82");
        hicarconfig_.featureInfo.screenSize.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.screenSize.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.screenSize.value, "12.82" , len);
        hicarconfig_.featureInfo.screenSize.length = len;

        len = strlen("1920 X 720");
        hicarconfig_.featureInfo.screenMetrics.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.screenMetrics.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.screenMetrics.value, "1920 X 720" , len);
        hicarconfig_.featureInfo.screenMetrics.length = len;

        len = strlen("3.18.71-ged28818 (289)");
        hicarconfig_.featureInfo.linuxKernel.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.linuxKernel.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.linuxKernel.value, "3.18.71-ged28818 (289)" , len);
        hicarconfig_.featureInfo.linuxKernel.length = len;

        len = strlen("QCC5120");
        hicarconfig_.featureInfo.btChip.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.btChip.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.btChip.value, "QCC5120" , len);
        hicarconfig_.featureInfo.btChip.length = len;

        len = strlen("BCM6338");
        hicarconfig_.featureInfo.wifiChip.value = (uint8_t*)malloc(len + 1);
        memset(hicarconfig_.featureInfo.wifiChip.value, 0 , len + 1);
        memcpy(hicarconfig_.featureInfo.wifiChip.value, "BCM6338" , len);
        hicarconfig_.featureInfo.wifiChip.length = len;
    }

}

} // namespace hsae
