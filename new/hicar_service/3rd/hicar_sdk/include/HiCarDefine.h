#ifndef HICARDEFINE_H
#define HICARDEFINE_H


#include <stdint.h>

#define NCM_IF_NAME		"wwan0"

#define VENDOR_ID_HUAWEI 0x12d1
#define HICAR_MDNS_NAME "com.huawei.nearby.usb.nsd"
#define HICAR_MDNS_REG_TYPE "_nearby._tcp"
#define HICAR_MDNS_NCMIP_TAG "ncm.ip"
#define HICAR_MDNS_DOMAIN "local"

#define HICAR_USB_REQUEST_TYPE 0x40 //USB_DIR_OUT | USB_TYPE_VENDOR
#define HICAR_USB_REQUEST 0x34
#define HICAR_USB_INDEX   0xa0

#define HICAR_NEARBY_DEVICEID_LEN 32



typedef struct _st_nearby_head_ {
    char magic_string[8];
    uint16_t total_len;
    uint16_t type;
    uint16_t version;
    uint16_t subtype;
} HiCarNearbyHead;

typedef struct _st_usb_extend_data_ {
    uint8_t tag;
    uint8_t len;
    uint8_t value[256];
} HiCarUSBExtendData;

#define TLV_TOATL_LEN(pTLV) (pTLV->len + 2)

#define HICAR_NEARBY_STRING "hwnearby"

enum {
    Nearby_Type_Car = 0x1,
    Nearby_Version = 0x0100,
    Nearby_Subtype_Car = 0x1,
};

enum {
    TAG_TransferType = 0x0,
    TAG_ModelID = 0x01,
    TAG_SubModelID = 0x02,
    TAG_DeviceID = 0x03,
    TAG_BluetoothMac = 0x04,
    TAG_DeviceAilasName = 0x05,
    TAG_LocalIP = 0x06,
    TAG_LocalPort= 0x07
};

enum {
    VALUE_TransferType_NCM = 0x01,
    VALUE_TransferType_AOA = 0x02,
    VALUE_TransferType_NCMAOA = 0x03,
};

enum {
    MODELID_DEFAULT		= 0x0003E8,
    MODELID_AUDI		= 0x0003E9,
    MODELID_BENZ		= 0x0003EA,
    MODELID_BMW			= 0x0003EB,
    MODELID_PORSCHE		= 0x0003EC,
    MODELID_VOLVO       = 0x0003ED,
    MODELID_VOLKSWAGEN	= 0x0003EE,
    MODELID_TOYOTA		= 0x0003EF,
    MODELID_HONDA		= 0x0003F0,
    MODELID_NISSAN		= 0x0003F1,
    MODELID_CHEVROLET	= 0x0003F2,
    MODELID_HYUNDRI		= 0x0003F3,
    MODELID_BUICK		= 0x0003F4,
    MODELID_FORD		= 0x0003F5,
    MODELID_MAZDA		= 0x0003F6,
};
enum {
    SUB_MODELID_DEFAULT		= 0x00,
};
#endif // HICARDEFINE_H
