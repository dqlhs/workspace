/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: Driver interface definition
 * Author: min.li@huawei.com
 * Create: 2019-05-17
 */

#ifndef DRIVER_ADAPT_H
#define DRIVER_ADAPT_H

#ifdef __cplusplus
extern "C" {
#endif
#define MAC_ADDR_LEN 6
#define BLE_ADV_MAX_LEN 31
#define STR_MAC_ADDR_LEN 17
#define STR_IP_LEN 15
#define DEV_MODEL_ID_LEN 3
#define BT_SERVICE_UUID_LEN 64
#define WIFI_SSID_LEN 24
#define WIFI_KEY_LEN 24
#define WIFI_ABILITY_ABAND 1
#define WIFI_ABILITY_GBAND 2
#define WIFI_ABILITY_UNKNOWN 3
#define BLE_DRIVER_NAME "ble"
#define P2P_DRIVER_NAME "p2p"
#define BR_DRIVER_NAME "br"
#define WIFI_DRIVER_NAME "wifi"
#define USB_DRIVER_NAME "usb"
#define BLE_ADV_INTERVAL_DEFAULT 0x0040
#define BLE_ADV_CHANNEL_37 1
#define BLE_ADV_CHANNEL_38 2
#define BLE_ADV_CHANNEL_39 4
#define BLE_ADV_CHANNEL_ALL (BLE_ADV_CHANNEL_37 | BLE_ADV_CHANNEL_38 | BLE_ADV_CHANNEL_39)
#define RESULT_RET_OK 0
#define RESULT_RET_ERROR (-1)

#define NFC_DATA_MAX_LEN 256
#define NFC_DRIVER_NAME "nfc"
#define MAC_STRING_LEN 17
#define CHANNEL_MAX_NUM 100
#define COUNTRY_CODE_LEN 16

// Get the equipment related param type.
typedef enum {
    GET_DEV_MAC,      // Get device mac str.
    GET_MODEL_ID,     // Get device model id.
    GET_DEV_NAME,     // Get device name.
    GET_MAP_PORT,     // Get service uuid mapped port.
    GET_WIFI_ABILITY, // Get wifi ability，
} GetParamsType;

// Set the equipment related param type.
typedef enum {
    SET_BLE_PARAMS,    // Set Ble param.
    SET_BR_UUID,       // Set Br service uuid.
    SET_WIFI_LOCAL_IP, // Set local static ip.
} SetParamsType;

// Ble GATT params.
typedef struct {
    // Ble gatt server.
    const char *serviceUuid; // 使用Gatts进行Nearby数据通信时，需要的service uuid.
    const char *readUuid;    // Gatt service对应的read characteristic uuid.
    const char *writeUuid;   // Gatt service对应的write characteristic uuid.
    int serviceEnable;       // Gatts 是否启动service，0:不启动；1:启动.
    int attrPerm;            // Attribute read / write permission in GATT protocol standard.
    int charProp;            // GATT协议标准中特征(characteristic)对应的性质(properties).

    // Ble gatt client.
    int minInterval;     // Gatt client minimum time interval for adv.
    int maxInterval;     // Gatt client maximum time interval for adv.
    int timeout;         // Gatt client timeout for adv, 0 means adv all the time.
    char *nearbyAdvData; // Gatt client adv data.
    int advDataLen;      // Gatt client the length of adv data.
} BleParams;

// Device model Id struct.
typedef struct {
    /*
     * The device model id is the unique identification applied by HuaWei
     * for querying the device information.
     */
    unsigned char modelId[DEV_MODEL_ID_LEN];

    /*
     * Equipment sub model id, used to distinguish the same equipment
     * with different appearance features.
     */
    unsigned char subModelId;

    /*
     * brief Identify whether the device has a sub model id.
     * 1 indicates a submodel ID, 0 indicates no submodel id.
     */
    unsigned char subModelFlag;
} DeviceModelId;

// Get service uuid mapped connection port.
typedef struct {
    char serviceUuid[BT_SERVICE_UUID_LEN + 1]; // Bt service uuid.
    int port;                                  // Bt service uuid mapped port.
} ServiceUuidMapPort;

// The param of GetDevMac.
typedef struct {
    char *mac;
    int len;
} MacParam;

// Set the mapping relationship between serviceuuid and channelid.
typedef struct {
    char serviceUuid[BT_SERVICE_UUID_LEN + 1]; // Bt service uuid.
    int sessionId;                             // One Bt service uuid corresponde to a session id.
} ServiceUuidRelatedSessionId;

// Br channel data.
typedef struct {
    int sessionId;                        // Session id.
    char remoteMac[STR_MAC_ADDR_LEN + 1]; // Remote mac address (xx:xx:xx:xx:xx:xx).
} BrChannelData;

// Wifi params.
typedef struct {
    char ssid[WIFI_SSID_LEN + 1];   // Wifi ssid.
    char key[WIFI_KEY_LEN + 1];     // Wifi key.
    char mac[STR_MAC_ADDR_LEN + 1]; // Wifi mac.
    int frequency;                  // Wifi frequency.
} WifiParams;

// Usb param.
typedef struct {
    char localIp[STR_IP_LEN + 1]; // Local IP (xx.xx.xx.xx).
    int port;                     // Connecton on this port.
} UsbParam;

// Channel event type.
typedef enum {
    EVENT_CONNECT = 1,         // Channel connect.
    EVENT_CONNECT_FAIL,        // Channel connect fail.
    EVENT_DISCONNECT,          // Channel disconnect.
    EVENT_SCAN_RESULT,         // Scan result.
    EVENT_CREATE_WIFI_SUCCESS, // Create Wifi success.
    EVENT_CONFIG_IP_SUCCESS,   // Config local ip success.
    EVENT_GROUP_STARTED,       // Start group
    EVENT_GROUP_REMOVED,       // remove group
} EventType;

typedef enum {
    CHANNEL_UNSET = 0, // Unset channel.
    CHANNEL_BLE = 1,   // Ble channel.
    CHANNEL_BREDR = 2, // Br channel.
    CHANNEL_P2P = 3,   // Wifi p2p channel.
    CHANNEL_USB = 6,   // Usb channel.
} ChannelType;

typedef struct {
    unsigned short minInterval;
    unsigned short maxInterval;
    unsigned char advChannel;
} BleAdvParams;

typedef struct {
    unsigned char advDataLen;
    unsigned char advData[BLE_ADV_MAX_LEN];
    unsigned char rspDataLen;
    unsigned char rspData[BLE_ADV_MAX_LEN];
} BleAdvData;

typedef enum {
    ADV_IND = 0x00,         // Connectable and scannable undirected advertising.
    ADV_DIRECT_IND = 0x01,  // Connectable directed advertising.
    ADV_SCAN_IND = 0x02,    // Scannable undirected advertising.
    ADV_NONCONN_IND = 0x03, // Non connectable undirected advertising.
    SCAN_RSP = 0x04,        // Scan Response.
} BleScanEventType;

/* *
 * @ingroup AllConnectSDK
 * @brief scan result struct
 * @brief for more details please check <<Bluetooth Core Specification>> 7.7.65.2 LE Advertising Report Event
 */
typedef struct {
    unsigned char evtType;               // LE Advertising Report event ADV_IND ADV_SCAN_IND.
    unsigned char addrType;              // Device Address type Public or random.
    unsigned char addr[MAC_ADDR_LEN];    // Device Address.
    unsigned char len;                   // Length of the data field max 31.
    unsigned char data[BLE_ADV_MAX_LEN]; // Advertising or scan response data.
    char rssi;                           // Range: -127 ≤ N ≤ +20 Units: dBm.
} BleScanResult;

typedef struct {
    unsigned char addrType;           // Device Address type Public or random.
    unsigned char addr[MAC_ADDR_LEN]; // Device Address.
} BleConnectParam;

typedef struct {
    char ipAddr[STR_IP_LEN + 1];
    char mask[STR_IP_LEN + 1];
    char gateway[STR_IP_LEN + 1];
} WifiSetIpParam;

typedef struct {
    int channelNum;
    int channelList[CHANNEL_MAX_NUM];
} Channel;

typedef struct {
    int isSupportGO;
    int isSupportGC;
    int isSupportGband;
    int isSupportAband;
    int isSupportRSDB;
    int currStaFreq;
    char selfGcMac[STR_MAC_ADDR_LEN + 1];
    char selfBssid[STR_MAC_ADDR_LEN + 1];
    Channel channelData;
    char countryCode[COUNTRY_CODE_LEN];
} WifiAbilityParam;

typedef struct {
    /*
     * @function   : ChannelStateChange.
     * @brief      : Call back the method when channel state change.
     * @param[IN]  : channelType, Refer to ChannelType.
     * @param[IN]  : eventType, Refer to EventType.
     * @param[IN]  : data, Channel information refer to BrChannelData.
     * @param[IN]  : dataLen, the length of data.(not used, structure data is fixed length).
     * @param[OUT] : N/A.
     * @return     : N/A.
     */
    void (*ChannelStateChange)(ChannelType channelType, EventType eventType, const void *data, int dataLen);

    /*
     * @function   : ScanCallback.
     * @brief      : Call back the method when the channel successfully scaned the device.
     * @param[IN]  : channelType, Refer to ChannelType
     * @param[IN]  : result, Refer to BleScanResult.
     * @param[OUT] : N/A.
     * @return     : N/A.
     */
    void (*ScanCallback)(ChannelType channelType, const BleScanResult *result);

    /*
     * @function   : ReceiveData.
     * @brief      : Call back the method when the channel received data.
     * @param[IN]  : channelType, Refer to ChannelType
     * @param[IN]  : data, The received data.
     * @param[IN]  : dataLen, The length of received data.
     * @param[OUT] : N/A.
     * @return     : N/A.
     */
    void (*ReceiveData)(ChannelType channelType, const void *channelParam, const unsigned char *data, int dataLen);
} ChannelCallback;

/*
 * struct FcDriverOps - Driver interface API definition.
 * Define different types of devices (Ble, Br, WiFi, etc.) to abstract and unify API interfaces,
 * and each device needs to be adapted.
 */
struct FcDriverAdaptOps {
    // Device interface name.
    const char *name;

    // Device interface description, such as mtk ble.
    const char *desc;

    /*
     * @function   : Init.
     * @brief      : Init the driver.
     * @param[IN]  : context, Not used temporarily
     * @param[IN]  : ifName, pass in driver interface name.
     * @param[OUT] : N/A.
     * @return     : 0-success, other value-failed.
     */
    int (*Init)(const void *context, const char *ifName);

    /*
     * @function   : Deinit.
     * @brief      : Deinit the driver, release the resource.
     * @param[IN]  : context, Not used temporarily.
     * @param[OUT] : N/A.
     * @return     : 0-success, other value-failed.
     */
    int (*Deinit)(const void *context);

    /*
     * @function   : SetParam.
     * @brief      : Set the driver param .
     * @param[IN]  : paramType, Refer to SetParamsType.
     * @param[IN]  : paramValue, The param value.
     * @param[OUT] : N/A.
     * @return     : 0-success, other value-failed.
     */
    int (*SetParam)(SetParamsType paramType, const void *paramValue);

    /*
     * @function   : GetParam.
     * @brief      : Get the driver param value .
     * @param[IN]  : paramType, Refer to GetParamsType.
     * @param[OUT] : paramValue, The param value.
     * @return     : 0-success, other value-failed.
     */
    int (*GetParam)(GetParamsType paramType, void *paramValue);

    /*
     * @function   : StartAdv.
     * @brief      : Start driver adv.
     * @param[IN]  : context, Hicar not used tempority, pass in NULL value.
     * @return     : 0-success, other value-failed.
     */
    int (*StartAdv)(const void *context);

    /*
     * @function   : StopAdv.
     * @brief      : Stop driver adv.
     * @param[IN]  : context, Hicar not used tempority, pass in NULL value.
     * @return     : 0-success, other value-failed.
     */
    int (*StopAdv)(const void *priv);

    /*
     * 配置广播内容
     * 参数: *priv 数据的结构体指针，采用void *通配不同设备数据结构体差异
     * 返回值: 0 on success, others on failure
     */
    int (*SetAdvData)(const void *priv);

    /*
     * 开启广播扫描
     * 参数: *priv 数据的结构体指针，采用void *通配不同设备数据结构体差异
     * 返回值: 0 on success, others on failure
     */
    int (*StartScan)(const void *priv);

    /*
     * 关闭广播扫描
     * 参数: *priv 数据的结构体指针，采用void *通配不同设备数据结构体差异
     * 返回值: 0 on success, others on failure
     */
    int (*StopScan)(const void *priv);

    /*
     * @function   : SendData.
     * @brief      : Send data to peer.
     * @param[IN]  : data, The data to send.
     * @param[IN]  : dataLen, the length of data.
     * @param[IN]  : channelInfo, The channel information refer to BrChannelData.
     * @param[OUT] : N/A.
     * @return     : 0-success, other value-failed.
     */
    int (*SendData)(const unsigned char *data, int dataLen, const void *channelInfo);

    /* *
     * 用于服务器向客户端发送数据
     * 参数: data是待发送的数据，dataLen是待发送数据的长度
     * *priv 发送数据的结构体指针，采用void *通配不同设备发送数据结构体差异
     * 返回值: 0 on success, others on failure
     */
    int (*NotifyData)(const unsigned char *data, int dataLen, void *priv);

    /*
     * @function   : DisplayPin.
     * @brief      : Display pin code.
     * @param[IN]  : context, The data which is required for the operation of displaying pin.
     * @param[IN]  : pinCode, The pin code to display.
     * @param[OUT] : N/A.
     * @return     : 0-success,other value-failed.
     */
    int (*DisplayPin)(const void *priv, const unsigned char *pinCode);

    /*
     * @function   : PushMsgBox.
     * @brief      : Provide message prompt box interface.
     * @param[IN]  : context, The data which is required for the operation of pushing message box.
     * @param[OUT] : N/A.
     * @return     : 0-success, other value-failed.
     */
    int (*PushMsgBox)(const void *priv);

    /*
     * @function   : CreateWifiChannel.
     * @brief      : Create wifi channel(create wifi AP or Go server).
     * @param[IN]  : context, Not used temporarily, NULL value.
     * @param[IN]  : wifiData, int* frequency.
     * @param[OUT] : N/A.
     * @return     : 0-success, other value-failed.
     */
    int (*CreateWifiChannel)(const void *context, const void *wifiData);

    /*
     * @function   : Connect.
     * @brief      : open channel connection.
     * @param[IN]  : channelInfo, BR driver refer to BrChannelData, Wi-Fi driver refer to WifiParams.
     * @param[OUT] : N/A.
     * @return     : 0-success, other value-failed.
     */
    int (*Connect)(const void *channelInfo);

    /*
     * @function   : Disconnect.
     * @brief      : close channel connection.
     * @param[IN]  : channelInfo, BR driver refer to BrChannelData, Wi-Fi driver refer to WifiParams.
     * @param[OUT] : N/A.
     * @return     : 0-success, other value-failed.
     */
    int (*Disconnect)(const void *channelInfo);

    /*
     * @function   : RegisterChannelCallback.
     * @brief      : Register channel callback.
     * @param[IN]  : channelCallback, Refer to ChannelCallback.
     * @param[OUT] : N/A.
     * @return     : 0-success, other value-failed.
     */
    int (*RegisterChannelCallback)(const ChannelCallback *channelCallback);
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DRIVER_ADAPT_H */