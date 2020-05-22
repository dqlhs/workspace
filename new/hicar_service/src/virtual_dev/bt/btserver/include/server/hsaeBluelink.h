/*!
 *************************************************************************************
 * 2012 (c) HSAE Technology Co., Ltd.  All rights reserved.
 *
 * \file bluelink_api.h
 *
 * \brief
 *    BTlink middleware APIs declaration
 *    Please reference to "HSAE BTlink Programming Guide" for function description
 *
 *
 *************************************************************************************
 */

#ifndef HSAEBLUELINK_H
#define HSAEBLUELINK_H

#ifdef __cplusplus
 extern "C" {
#else
 #include <stdio.h>
 #define true 1
 #define false 0
#endif // __cplusplus

#ifndef bool
// typedef int bool;
#endif

#include <limits.h>

#define MILLI_SEC               1000    // MILLI_SEC * 1000 = 1 second;
#define BT_ADDR_LEN             6
#define BT_MAC_LEN              17
#define BT_LOCAL_NAME_MAX       250
#define PINCODE_MAX_LEN         16
#define PHONE_NUM_MAX_LEN       20
#define PLAYER_ATTRIBUTE        5       // 0~4, but 0 is not used, add it only for coding convenience
#define MAX_MAP_MSG_HANDLE_LEN  20
#define PATH_MAX_LEN            1024

#define DUT_FREQ_MIN    2402
#define DUT_FREQ_MAX    2480
#define DUT_FREQ_MID    2440

#define DUT_STOP_TEST   0x00
#define DUT_RX_MODE     0x01
#define DUT_TX_MODE 0x02
#define DUT_TX_MOD_MODE 0x03

#define DUT_POWER_MIN   0
#define DUT_POWER_MAX   7


// Return code for HSAE command function
typedef enum HSAE_CMD_RET_e
{
    HSAE_CMD_SUCCESS_FIRST = 0,
    HSAE_CMD_SUCCESS,
    HSAE_CMD_SUCCESS_IGNORE,
    HSAE_CMD_SUCCESS_ALREADY,
    HSAE_CMD_SUCCESS_LAST,

    HSAE_CMD_FAIL_FIRST,
    HSAE_CMD_FAIL,
    HSAE_CMD_FAIL_INVALID_HANDLE,
    HSAE_CMD_FAIL_INVALID_CALLBACK,
    HSAE_CMD_FAIL_NOT_SUPPORT,
    HSAE_CMD_FAIL_NOT_READY,
    HSAE_CMD_FAIL_CANCEL,
    HSAE_CMD_FAIL_MEMORY_NOT_ENOUGH,
    HSAE_CMD_FAIL_DEPENDENCY,
    HSAE_CMD_FAIL_STATE,
    HSAE_CMD_FAIL_REPEAT,
    HSAE_CMD_FAIL_IO,
    HSAE_CMD_FAIL_CUS,
    HSAE_CMD_FAIL_PARAMETER,
    HSAE_CMD_FAIL_RETRY,
    HSAE_CMD_FAIL_REMOTE,
    HSAE_CMD_FAIL_REGISTER_ERROR,
    HSAE_CMD_FAIL_LAST,

    HSAE_CMD_STATUS_INITIAL
} HSAE_CMD_RET;

// ID of Bluetooth profiles
typedef enum HSAE_PROFILE_INDEX_e
{
    NPINDEX_HEADSET,
    NPINDEX_HANDFREE,
    NPINDEX_AUDIOSINK,
    NPINDEX_AVRCP,
    NPINDEX_PBAP,
    NPINDEX_MAP,
    NPINDEX_SPP,
    NPINDEX_SYNCML,
    NPINDEX_SPP_IAP,
    NPINDEX_HID,
    NPINDEX_PAN,
    NPINDEX_OPP,
    NPINDEX_RFCOMM_1,
    NPINDEX_RFCOMM_2,
    NPINDEX_RFCOMM_3,
    NPINDEX_RFCOMM_4,
    NPINDEX_RFCOMM_5,
    NPINDEX_RFCOMM_6,
    NPINDEX_RFCOMM_7,
    NPINDEX_RFCOMM_8,
    NPINDEX_RFCOMM_9,
    NPINDEX_ALL_RFCOMM,
    NPINDEX_MAX,
    NPINDEX_ALL_PROFILE = 0xffffffff
} HSAE_PROFILEINDEX;

typedef enum HSAE_PAIRED_SOFT_MODE_e
{
    HSAE_PAIREDSOFT_LAST_PAIRED_FIRST = 0,
    HSAE_PAIREDSOFT_LAST_CONNECTED_FIRST
}HSAE_PAIRED_SOFT_MODE;

// ID of Bluetooth profiles
typedef enum HSAE_PROFILEID_e
{
    NPI_ALL_AUDIO   = 0x0000,
    NPI_HEADSET     = 1<<NPINDEX_HEADSET,
    NPI_HANDFREE    = 1<<NPINDEX_HANDFREE,
    NPI_AUDIOSINK   = 1<<NPINDEX_AUDIOSINK,
    NPI_AVRCP       = 1<<NPINDEX_AVRCP,
    NPI_PBAP        = 1<<NPINDEX_PBAP,
    NPI_MAP         = 1<<NPINDEX_MAP,
    NPI_SPP         = 1<<NPINDEX_SPP,
    NPI_SYNCML      = 1<<NPINDEX_SYNCML,
    NPI_SPP_IAP     = 1<<NPINDEX_SPP_IAP,
    NPI_HID         = 1<<NPINDEX_HID,
    NPI_PAN         = 1<<NPINDEX_PAN,
    NPI_OPP         = 1<<NPINDEX_OPP,
    NPI_RFCOMM_1    = 1<<NPINDEX_RFCOMM_1,
    NPI_RFCOMM_2    = 1<<NPINDEX_RFCOMM_2,
    NPI_RFCOMM_3    = 1<<NPINDEX_RFCOMM_3,
    NPI_RFCOMM_4    = 1<<NPINDEX_RFCOMM_4,
    NPI_RFCOMM_5    = 1<<NPINDEX_RFCOMM_5,
    NPI_RFCOMM_6    = 1<<NPINDEX_RFCOMM_6,
    NPI_RFCOMM_7    = 1<<NPINDEX_RFCOMM_7,
    NPI_RFCOMM_8    = 1<<NPINDEX_RFCOMM_8,
    NPI_RFCOMM_9    = 1<<NPINDEX_RFCOMM_9,
    NPI_ALL_RFCOMM  = 1<<NPINDEX_ALL_RFCOMM,
    NPI_MAX         = 1<<NPINDEX_MAX,
    NPI_ALL_PROFILE = 0xffffffff
} HSAE_PROFILEID;

/* Profile version supported by remote device */
typedef enum
{
    VER_00, // means unknown
    VER_10, // means version 1.0
    VER_11,
    VER_12,
    VER_13,
    VER_14,
    VER_15,
    VER_16,
    VER_17,
    VER_18,
    VER_19,
    VER_20,
    VER_21,
    VER_22,
    VER_23,
    VER_24,
    VER_25
}HSAE_SUPPORT_VERSION;

// Category ID of device
typedef enum HSAE_MAJORDEVICEID_e
{
    NMJRDI_Miscellaneous        = 0, // 0 0 0 0 0
    NMJRDI_Computer             = 1, // 0 0 0 0 1
    NMJRDI_Phone                = 2, // 0 0 0 1 0
    NMJRDI_LAN                  = 3, // 0 0 0 1 1
    NMJRDI_Audio_Video          = 4, // 0 0 1 0 0
    NMJRDI_Peripheral           = 5, // 0 0 1 0 1
    NMJRDI_Imaging              = 6, // 0 0 1 1 0
    NMJRDI_Wearable             = 7, // 0 0 1 1 1
    NMJRDI_Toy                  = 8, // 0 1 0 0 0
    NMJRDI_Health               = 9, // 0 1 0 0 1
    NMJRDI_Uncategorized        = 31 // 1 1 1 1 1
} HSAE_MAJORDEVICEID;

// Return code for HSAE callback function
typedef enum HSAE_CBK_RET_e
{
    HSAE_CBK_SUCCESS_FIRST = 0,
    HSAE_CBK_SUCCESS,
    HSAE_CBK_SUCCESS_LAST,

    HSAE_CBK_FAIL_FIRST,
    HSAE_CBK_FAIL,
    HSAE_CBK_FAIL_LAST,

    HSAE_CBK_STATUS_INITIAL
} HSAE_CBK_RET;

// Return code for HSAE_HANDLE
typedef enum HSAE_HANDLE_TYPE_e
{
    HSAE_HANDLE_TYPE_FAIL = 0,
    HSAE_HANDLE_TYPE_SYS,
    HSAE_HANDLE_TYPE_APP,
} HSAE_HANDLE_TYPE;

typedef void* HSAE_HANDLE;

/* BD Address */
typedef struct {
    unsigned char b[BT_ADDR_LEN];
} __attribute__((packed)) BT_ADDR;

// Version of this software
typedef struct HSAE_VER_s
{
    unsigned char major;
    unsigned char minor;
    unsigned short build;
} HSAE_VER;

// Power
typedef enum HSAE_POWER_e
{
    HSAE_POWEROFF,
    HSAE_POWERON,
    HSAE_POWERSUSPEND,
    HSAE_POWERUNKNOWN,
} HSAE_POWER;

typedef struct HSAE_DEV_INFO_e
{
    char        *szName;
    BT_ADDR     btAddr;
}HSAE_PAIRED, HSAE_DEVICE;

typedef enum HSAE_ANSWERPAIR_STATE_e
{
    HSAE_ANSWERPAIR_REQUEST,
    HSAE_ANSWERPAIR_CONFIRM,
    HSAE_ANSWERPAIR_RESULT
} HSAE_ANSWERPAIR_STATE;

typedef enum HSAE_CONNECTION_STATE_e
{
    HSAE_CON_CONNECTION = 0,
    HSAE_CON_DISCONNECTION,
    HSAE_CON_NOT_SUPPORT,
    HSAE_CON_CONNECTING
} HSAE_CONNECTION_STATE;

typedef enum HSAE_AUTO_CONNECTION_STATE_e
{
    HSAE_AUTO_CON_END = 0,
    HSAE_AUTO_CON_START
} HSAE_AUTO_CONNECTION_STATE;

/*  Bluetooth disconnect reason */
typedef enum HSAE_DISCONNECT_REASON_e
{
    HSAE_DISCONNECT_REASON_NOT_FIND  = 0x04,
    HSAE_DISCONNECT_REASON_LINK_LOSS  = 0x08,
    HSAE_DISCONNECT_REASON_REMOTE_USER  = 0x13,
    HSAE_DISCONNECT_REASON_REMOTE_POWER_OFF  = 0x15, 
}HSAE_DISCONNECT_REASON;

/*Bluetooth ACL connection state*/
typedef enum HSAE_ACL_CONNECTION_STATE_e
{
	HSAE_ACL_CONNECTION_CONNECTED	= 0,	
	HSAE_ACL_CONNECTION_DISCONNECTED = 1,	
}HSAE_ACL_CONNECTION_STATE;

/*  Diagnostic responese */
/*implement to bt Diagnostic event*/
typedef enum HSAE_DIAG_CODE_e
{
    HSAE_NOERR                       = 0x0,
    HSAE_DBG_SYS_A2DP_AUDIO_ERROR,
    HSAE_DBG_SYS_A2DP_AUDIO_ACTIVE,
    HSAE_DBG_SYS_A2DP_AUDIO_RELEASE,
    HSAE_DBG_SYS_RING_AUDIO_ERROR,
    HSAE_DBG_SYS_RING_AUDIO_ACTIVE,
    HSAE_DBG_SYS_RING_AUDIO_RELEASE,
    HSAE_DBG_SYS_SCO_AUDIO_ERROR,
    HSAE_DBG_SYS_SCO_AUDIO_ACTIVE,
    HSAE_DBG_SYS_SCO_AUDIO_RELEASE,
    HSAE_DBG_SYS_SCO_MIC_ERROR,
    HSAE_DBG_SYS_SCO_MIC_ACTIVE,
    HSAE_DBG_SYS_SCO_MIC_RELEASE,
    HSAE_ERR_BT_HW_EXCEPTION,
    HSAE_ERR_SYS_AUDIO_DRIVER_ERROR,
}HSAE_DIAG_CODE;

/********** States of profiles  **********/

//AVRCP
typedef enum HSAE_AVRCP_STATE_e
{
    HSAE_AVRCP_EVENT_DISCONNECTED,
    HSAE_AVRCP_EVENT_READY,
    HSAE_AVRCP_EVENT_CONNECTING,
    HSAE_AVRCP_EVENT_CONNECTED,
    HSAE_AVRCP_EVENT_RESERVED
} HSAE_AVRCP_STATE;

typedef enum HSAE_AVRCP_PLAY_STATE_e
{
    HSAE_PLAY_STATE_STOP     = 0x00,
    HSAE_PLAY_STATE_PLAYING  = 0x01,
    HSAE_PLAY_STATE_PAUSED   = 0x02,
    HSAE_PLAY_STATE_FWD      = 0x03,
    HSAE_PLAY_STATE_REV      = 0x04,
    HSAE_PLAY_STATE_ERROR    = 0xFF
} HSAE_AVRCP_PLAY_STATE;

typedef enum HSAE_AVRCP_PLAY_OPERATION_e
{
    HSAE_PLAY_OP_STOP,
    HSAE_PLAY_OP_PLAY,
    HSAE_PLAY_OP_PAUSE,
    HSAE_PLAY_OP_FWD,
    HSAE_PLAY_OP_BWD,
    /* NOTE:
     * If one of START operation is sent, then send corresponding STOP before sending any START.
     * Operations above can be sent between START and STOP, but this is not recommended.
     */
    HSAE_PLAY_OP_START_FAST_FWD,
    HSAE_PLAY_OP_STOP_FAST_FWD,
    HSAE_PLAY_OP_START_REV,
    HSAE_PLAY_OP_STOP_REV
} HSAE_AVRCP_PLAY_OPERATION;

typedef enum HSAE_AVRCP_PDUID_e
{
    GET_CAPS_PDU_ID                     /*= 16,*/   =(0x10),
    LIST_APP_ATTRIBUTES_PDU_ID,
    LIST_APP_VALUE_PDU_ID,
    GET_APP_VALUE_PDU_ID,
    SET_APP_VALUE_PDU_ID,
    GET_APP_ATTRIBUTE_TEXT_PDU_ID,
    GET_APP_VALUE_TEXT_PDU_ID,
    INFORM_CHARACTER_SET_PDU_ID,
    INFORM_BATTERY_STATUS_PDU_ID,
    /* 0x19 - 0x1F Reserved */
    GET_ELEMENT_ATTRIBUTES_PDU_ID       /*=32,*/    =(0x20),
    /* 0x21 - 0x2F Reserved */
    GET_PLAY_STATUS_PDU_ID              /*=48,*/    =(0x30),
    REGISTER_NOTIFICATION_PDU_ID,
    /* 0x32 - 0x3F Reserved */
    REQUEST_CONTINUING_RESPONSE_PDU_ID  /*=64,*/    =(0x40),
    ABORT_CONTINUING_RESPONSE_PDU_ID,
    /* 0x42 - 0x4F Reserved */
    SET_ABSOLUTE_VOLUME_PDU_ID          /*=80,*/    =(0x50),
    /* 0x51 - 0x5F Reserved */
    SET_ADDRESSED_PLAYER_PDU_ID         /*=96,*/    =(0x60),
    /* 0x61 - 0x73 Reserved */
    PLAY_ITEM_PDU_ID                    /*=116,*/   =(0x74),
    /* 0x75 - 0x8F Reserved */
    ADD_TO_NOW_PLAYING_PDU_ID           /*=144,*/   =(0x90)
} HSAE_AVRCP_PDUID;

// For AVRCP 1.4 browsing
typedef enum HSAE_AVRCP_BROWSE_PDUID_e
{
    SET_BROWSED_PLAYER_PDU_ID           /*=112,*/   =(0x70),
    GET_FOLDER_ITEM_PDU_ID              /*=113,*/   =(0x71),
    CHANGE_PATH_PDU_ID                  /*=114,*/   =(0x72),
    GET_ITEM_ATTRIBUTE_PDU_ID           /*=115,*/   =(0x73),
    SEARCH_PDU_ID                       /*=128,*/   =(0x80)
} HSAE_AVRCP_BROWSE_PDUID;

typedef enum HSAE_AVRCP_SUPPORTED_EVENTS_e {
    /* AVRCP 1.3 */
    EVENT_PLAYBACK_STATUS_CHANGED        = 0x01,
    EVENT_TRACK_CHANGED,                /* 0x02 */
    EVENT_TRACK_REACHED_END,            /* 0x03 */
    EVENT_TRACK_REACHED_START,          /* 0x04 */
    EVENT_PLAYBACK_POS_CHANGED,         /* 0x05 */
    EVENT_BATT_STATUS_CHANGED,          /* 0x06 */
    EVENT_SYSTEM_STATUS_CHANGED,        /* 0x07 */
    EVENT_PLAYER_APP_SETTING_CHANGED,   /* 0x08 */
    /* AVRCP 1.4 */
    EVENT_NOW_PLAYING_CONTENT_CHANGED,  /* 0x09 */
    EVENT_AVAILABLE_PLAYER_CHANGED,     /* 0x0A */
    EVENT_ADDRESSED_PLAYER_CHANGED,     /* 0x0B */
    EVENT_UIDS_CHANGED,                 /* 0x0C */
    EVENT_VOLUME_CHANGED,               /* 0x0D */
    EVENT_RESERVED                      /* 0x0E */
}HSAE_AVRCP_SUPPORTED_EVENTS;

/* Player Application Setting Attribute Value*/
typedef enum
{
    ATTRIBUTE_ILLEGAL = 0x00,
    ATTRIBUTE_EQUALIZER,
    ATTRIBUTE_REPEAT_MODE,
    ATTRIBUTE_SHUFFLE,
    ATTRIBUTE_SCAN,
    ATTRIBUTE_RESERVED
}HSAE_AVRCP_PLAYER_ATTRIBUTE;

typedef enum
{
    EQUALIZER_OFF = 0x01,
    EQUALIZER_ON,
    EQUALIZER_RESERVED
}HSAE_AVRCP_PLAYER_EQUALIZER;

typedef enum
{
    REPEAT_OFF = 0x01,
    REPEAT_SINGLE_TRACK,
    REPEAT_ALL_TRACK,
    REPEAT_GROUP,
    REPEAT_RESERVED
}HSAE_AVRCP_PLAYER_REPEAT;

typedef enum
{
    SHUFFLE_OFF = 0x01,
    SHUFFLE_ALL_TRACK,
    SHUFFLE_GROUP,
    SHUFFLE_RESERVED
}HSAE_AVRCP_PLAYER_SHUFFLE;

typedef enum
{
    SCAN_OFF = 0x01,
    SCAN_ALL_TRACK,
    SCAN_GROUP,
    SCAN_RESERVED
}HSAE_AVRCP_PLAYER_SCAN;

/* EventTrackChanged value*/
#define ELEMENT_ID_PLAYING          0x00                // returned when player got no track selected and not playing
/* other : correspond to a media element in the Now Playing folder */
// returned when player got no track selected
#if defined ULONG_LONG_MAX
# define ELEMENT_ID_NO_SELECTED     ULONG_LONG_MAX
#else
# if defined ULLONG_MAX
#  define ELEMENT_ID_NO_SELECTED        ULLONG_MAX
# else
#  define ELEMENT_ID_NO_SELECTED        0xFFFFFFFFFFFFFFFF
# endif
#endif

/* EventPlaybackPosChanged value*/
#define PLAYBACK_POS_NO_SELECTED    0xFFFFFFFF // returned when player got no track selected

/* GetElementAttributes Attribute ID*/
typedef enum
{
    ELEMENT_ATTRIBUTE_ILLEGAL       = 0x00,
    ELEMENT_ATTRIBUTE_TITLE,
    ELEMENT_ATTRIBUTE_ARTIST,
    ELEMENT_ATTRIBUTE_ALBUM,
    ELEMENT_ATTRIBUTE_TRACK_NUMBER,
    ELEMENT_ATTRIBUTE_TRACKS,
    ELEMENT_ATTRIBUTE_GENRE,
    ELEMENT_ATTRIBUTE_PLAYING_TIME,
    ELEMENT_ATTRIBUTE_RESERVED
}HSAE_AVRCP_ELEMENT_ATTRIBUTE;

typedef struct{
    unsigned char attribute_id;     /**< Player Attribute ID */
    unsigned char value_id;         /**< Attribute Value ID */
}HSAE_PLAYER_SETTING_ID;

/* Battery Status ID*/
typedef enum
{
    BATT_STATUS_NORMAL      = 0x00,
    BATT_STATUS_WARNING,
    BATT_STATUS_CRITICAL,
    BATT_STATUS_EXTERNAL,
    BATT_STATUS_FULL_CHARGE
}HSAE_AVRCP_BATT_STATUS;

/* System Power Status ID*/
typedef enum
{
    AVRCP_POWER_ON          = 0x00,
    AVRCP_POWER_OFF,
    AVRCP_UNPLUGGED,
}HSAE_AVRCP_SYS_POWER_STATUS;

/** Attribute of now playing element */
typedef struct{
    const char              *media_attribute;   /**< Element Attribute */
    unsigned short int  charset_id;         /**< Character Set ID */
    char                    *string;            /**< string area (NULL terminated) */
}HSAE_NOW_PLAYING_ATTRIBUTE;

/** Attribute of browsed player */
typedef struct{
    unsigned short int      uid_counter;        /**< UID counter */
    unsigned int            item_count;         /**< Item count in current directory */
    unsigned short int      charset_id;         /**< Character set ID */
    unsigned char           folder_depth;       /**< Current directory depth */
    char                    **folder;           /**< string area (NULL terminated) */
}HSAE_BROWSED_PLAYER_ATTRIBUTE;

/* Item Entry ID*/
typedef enum
{
    ITEM_ENTRY_HAS_NEXT     = 0x01,
    ITEM_ENTRY_LASY,
}HSAE_AVRCP_ITEM_ENTRY;

/* Player Item Type ID*/
typedef enum
{
    PLAYER_ITEM_TYPE_AUDIO              = 0x01,
    PLAYER_ITEM_TYPE_VIDEO              = 0x02,
    PLAYER_ITEM_TYPE_BROADCASTING_AUDIO = 0x04,
    PLAYER_ITEM_TYPE_BROADCASTING_VIDEO = 0x08,
}HSAE_PLAYER_ITEM_TYPE;

/* Player Item Sub Type ID*/
typedef enum
{
    PLAYER_ITEM_SUBTYPE_AUDIO_BOOK      = 0x01,
    PLAYER_ITEM_SUBTYPE_PODCAST,
}HSAE_PLAYER_ITEM_SUBTYPE;

#define  PLAYER_ITEM_FERTURE_BIT_MASK   16
/** Attribute of player item */
typedef struct{
    HSAE_AVRCP_ITEM_ENTRY    last_entry;                                     /**< Notice that is the last entry or not */
    unsigned short int      uid_counter;                                    /**< UID counter */
    unsigned short int      player_id;                                      /**< Player ID */
    HSAE_PLAYER_ITEM_TYPE    type_flags;                                     /**< Type */
    HSAE_PLAYER_ITEM_SUBTYPE subtype_flags;                                  /**< Sub type */
    HSAE_AVRCP_PLAY_STATE    play_status_id;                                 /**< Play status ID */
    unsigned char           feature_bit_mask[PLAYER_ITEM_FERTURE_BIT_MASK]; /**< Player features, fixed in 16 bytes */
    unsigned short int      charset_id;                                     /**< Character set ID */
    char                    *name;                                          /**< Player name */
}HSAE_PLAYER_ITEM;

/* Folder Item Type ID*/
typedef enum
{
    FOLDER_ITEM_TYPE_MIXED              = 0x00,
    FOLDER_ITEM_TYPE_TITLES,
    FOLDER_ITEM_TYPE_ALBUMS,
    FOLDER_ITEM_TYPE_ARTISTS,
    FOLDER_ITEM_TYPE_GENRES,
    FOLDER_ITEM_TYPE_PLAYLISTS,
    FOLDER_ITEM_TYPE_YEARS,
}HSAE_FOLDER_ITEM_TYPE;


/* Folder Item Playable*/
typedef enum
{
    FOLDER_ITEM_NOT_PLAYABLE            = 0x01,
    FOLDER_ITEM_PLAYABLE,
}HSAE_FOLDER_ITEM_PLAYABLE;

/** Attribute of folder item */
typedef struct{
    HSAE_AVRCP_ITEM_ENTRY        last_entry;         /**< Notice that is the last entry or not */
    unsigned short int          uid_counter;        /**< UID counter */
    unsigned long long int      uid;                /**< UID */
    HSAE_FOLDER_ITEM_TYPE        folder_type;        /**< Type */
    HSAE_FOLDER_ITEM_PLAYABLE    is_playable;        /**< Is folder playable */
    unsigned short int          charset_id;         /**< Character set ID */
    char                        *folder;            /**< Directory name */
}HSAE_FOLDER_ITEM;

/* Media Item Type ID*/
typedef enum
{
    MEDIA_ITEM_TYPE_AUDIO               = 0x01,
    MEDIA_ITEM_TYPE_VIDEO               = 0x02,
}HSAE_MEDIA_ITEM_TYPE;

/** Attribute of media item */
typedef struct{
    HSAE_AVRCP_ITEM_ENTRY        last_entry;         /**< Notice that is the last entry or not */
    unsigned short int          uid_counter;        /**< UID counter */
    unsigned long long int      uid;                /**< UID */
    HSAE_MEDIA_ITEM_TYPE         media_type;         /**< Type */
    unsigned short int          charset_id;         /**< Character set ID */
    char                        *name;              /**< Media name */
}HSAE_MEDIA_ITEM;

/** Direction of change folder */
typedef enum
{
    CHANGE_FOLDER_DIRECTION_UP          = 0x00,
    CHANGE_FOLDER_DIRECTION_DOWN,
}HSAE_AVRCP_CHANGE_FOLDER_DIRECTION;

/** Scope of item that can be get */
typedef enum
{
    SCOPE_ID_PLAYER_LIST                    = 0x00,
    SCOPE_ID_PLAYER_VIRTUAL_FILE_SYSTEM,
    /** IDs below is currently not supported by Bluelink */
    SCOPE_ID_SEARCH,
    SCOPE_ID_NOW_PLAYING,
}HSAE_FOLDER_ITEM_SCOPE_ID;

/** AVRCP version supported by remote device */
typedef enum
{
    AVRCP_VER_00    = 0x0000, // means no support
    AVRCP_VER_10    = 0x0100,
    AVRCP_VER_13    = 0x0103,
    AVRCP_VER_14    = 0x0104,
    AVRCP_VER_15    = 0x0105
}HSAE_AVRCP_SUPPORT_VERSION;

//A2DP
typedef enum HSAE_A2DP_STATE_e
{
    HSAE_A2DP_STATE_DISCONNECTED,
    HSAE_A2DP_STATE_INITIALISING,
    HSAE_A2DP_STATE_READY,
    HSAE_A2DP_STATE_CONNECTING,
    HSAE_A2DP_STATE_SIGNALLING,
    HSAE_A2DP_STATE_OPEN,
    HSAE_A2DP_STATE_STREAMING,
    HSAE_A2DP_STATE_A2DP_PAUSED,
} HSAE_A2DP_STATE;

// HFP
typedef struct HSAE_HF_MPTY_INFO_e
{
    char            *pwzNumber;
    unsigned char   idx;
    unsigned char   mpty_state;
}HSAE_HF_MPTY_INFO;

typedef enum HSAE_HANDSFREE_STATE_e
{
    HSAE_HF_STATE_DISCONNECTED,
    HSAE_HF_STATE_INITIALISING,
    HSAE_HF_STATE_READY,
    HSAE_HF_STATE_SLCCONNECTING,
    HSAE_HF_STATE_SLCCONNECTED,
    HSAE_HF_STATE_OUTGOINGCALLESTABLISH,
    HSAE_HF_STATE_INCOMINGCALLESTABLISH,
    HSAE_HF_STATE_ACTIVECALL,
    HSAE_HS_STATE_INITIALISING,
    HSAE_HS_STATE_READY,
    HSAE_HS_STATE_ACTIVECALL,
    HSAE_HANDSFREE_STATE_MAX
} HSAE_HANDSFREE_STATE;

typedef enum HSAE_HANDSFREE_AUTO_REACTION_e
{
    HSAE_HF_AUTO_NO_ACTION,
    HSAE_HF_AUTO_ANSWER,
    HSAE_HF_AUTO_REJECT
} HSAE_HANDSFREE_AUTO_REACTION;

/* Battery Status*/
typedef enum
{
    BATT_STATUS_0       = 0x00,
    BATT_STATUS_1,
    BATT_STATUS_2,
    BATT_STATUS_3,
    BATT_STATUS_4,
    BATT_STATUS_5
}HSAE_HANDSFREE_BATT_STATUS;

/* Signal Strength*/
typedef enum
{
    SIGNASL_STRENGTH_LEVEL_0    = 0x00,
    SIGNASL_STRENGTH_LEVEL_1,

    SIGNASL_STRENGTH_LEVEL_2,
    SIGNASL_STRENGTH_LEVEL_3,
    SIGNASL_STRENGTH_LEVEL_4,
    SIGNASL_STRENGTH_LEVEL_5
}HSAE_HANDSFREEP_SIGNASL_STRENGTH;

/* Call Held Option*/
typedef enum
{
    CALL_HELD_RELEASE_HELD  = 0x00,     // 0.Releases all held calls or sets User Determined User Busy (UDUB) for a waiting call
    CALL_HELD_RELEASE_ACTS,             // 1.Releases all active calls (if any exist) and accepts the other (held or waiting)
    CALL_HELD_HOLD_CALLS,               // 2.Places all active calls (if any exist) on hold and accepts the other (held or waiting) call
    CALL_HELD_ADD_HELD_CALL,            // 3.Adds a held call to the conversation
    CALL_HELD_RELEASE_SPEC_ACT,         // 1x.Releases specified active call only (<idx>)
    CALL_HELD_REQ_PRIVATE_SPEC_CALL,    // 2x.Request private consultation mode with specified call (<idx>). (Place all calls on hold EXCEPT the call indicated by <idx>.)
}HSAE_HANDSFREE_CALLHELD_OPTION;

/* AT command*/
typedef enum
{
    HSAE_HF_ANSWER,              // answer call
    HSAE_HF_DIAL,                // dial call
    HSAE_HF_DTMF,                // DTMF
    HSAE_HF_REDIAL,              // re-dial call
    HSAE_HF_HANGUP,              // hang up call
    HSAE_HF_HOLD,                // hold call
    HSAE_HF_SPK_VOL,             // speaker volume
    HSAE_HF_MIC_VOL,             // microphone volume
    HSAE_HF_VR,                  // Voice Recognition
    HSAE_HF_XAPL,                // Apple custom AT commands
    HSAE_HF_APLEFM,              // Apple Siri status
    HSAE_HF_CGMI,                // request device manufacturer identification
    HSAE_HF_CGMM,                // request device manufacturer model identification
    HSAE_HS_ANSWER,              // answer call using HF
    HSAE_HS_HANGUP,              // hang up call using HF
    HSAE_HF_GET_CURRECT_CALL_REJECT = 16
}HSAE_HANDSFREEP_AT_COMMAND;


/*  Bluetooth hfp voice dial actions */
typedef enum
{
    HFP_VR_STOP = 0,
    HFP_VR_START
}HFP_VOICE_RECOGNITION_ACTION;


//PBAP
typedef enum HSAE_DOWNLOAD_TYPE_e
{
    HSAE_DLT_MAIN = -1,
    HSAE_DLT_PBAP = 0,
    HSAE_DLT_SYNCML,
    HSAE_DLT_AT,
    HSAE_DLT_MAP,
} HSAE_DOWNLOAD_TYPE;

//TODO: rename to HSAE_OBEX_TRANSFER_STATE
typedef enum HSAE_DOWNLOAD_STATE_e
{
    HSAE_LS_END = 0,
    HSAE_LS_START,
    HSAE_DLS_ING,
    HSAE_LS_STOP,
    HSAE_ULS_ING,
} HSAE_DOWNLOAD_STATE;

typedef enum HSAE_PHONEBOOK_LOCATION_e
{
    HSAE_PB_LOC_PHONE,
    HSAE_PB_LOC_SIM,
    HSAE_PB_LOC_SIM2,
} HSAE_PHONEBOOK_LOCATION;

typedef enum HSAE_PHONE_TYPE_e
{
    HSAE_PHONE_PREF = 0,
    HSAE_PHONE_WORK,
    HSAE_PHONE_HOME,
    HSAE_PHONE_VOICE,
    HSAE_PHONE_FAX,
    HSAE_PHONE_MSG,
    HSAE_PHONE_CELL,
    HSAE_PHONE_PAGER,
    HSAE_PHONE_BBS,
    HSAE_PHONE_MODEM,
    HSAE_PHONE_CAR,
    HSAE_PHONE_ISDN,
    HSAE_PHONE_VIDEO,
    HSAE_PHONE_NONE
} HSAE_PHONE_TYPE;

typedef struct HSAE_PHONE_e
{
    char            *phone_number;
    HSAE_PHONE_TYPE  *phone_type;
    unsigned int    phone_type_count;
}HSAE_PHONE;

typedef enum HSAE_PHONEBOOK_TYPE_e
{
    HSAE_PB_TYPE_PB,
    HSAE_PB_TYPE_MISSED,
    HSAE_PB_TYPE_OUTGOING,
    HSAE_PB_TYPE_INCOMING,
    HSAE_PB_TYPE_COMBINED,
} HSAE_PHONEBOOK_TYPE;

typedef struct HSAE_PHONEBOOK_DATA_e
{
    HSAE_PHONEBOOK_LOCATION  location;
    HSAE_PHONEBOOK_TYPE      type;
    unsigned int            max_count;
}HSAE_PHONEBOOK_DATA;

typedef enum HSAE_PHONE_DIRECTION_e
{
    HSAE_PHONE_MISSED,   // missed calls
    HSAE_PHONE_RECEIVED, // incoming calls
    HSAE_PHONE_DIALED    // outgoing calls
}HSAE_PHONE_DIRECTION;

typedef struct HSAE_CALL_e
{
    char *dir;  // missed, incoming, outgoing
    char *date; // yyyyymmdd
    char *time; // hhmmss
}HSAE_CALL;

typedef struct HSAE_CONTACT_INFO_e
{
    char            *name;
    char            *address;
    HSAE_PHONE       *phone;
    unsigned int    phone_count;
    char            *photo_data;
    HSAE_CALL        call_info;
    char            *fullname;
} HSAE_CONTACT_INFO;

// MAP

/* MAP folder structure */
typedef enum
{
    MAP_FOLDER_STRUCTURE_INBOX = 0,
    MAP_FOLDER_STRUCTURE_SENT,
    MAP_FOLDER_STRUCTURE_OUTBOX
}MAP_FOLDER_STRUCT;

typedef struct HSAE_MESSAGE_INFO_e
{
    char                        *Content;
    char                        *Subject;
    char                        *Timestamp;
    char                        *Sender;
    char                        *SenderAddress;
    char                        *ReplyTo;
    char                        *Recipient;
    char                        *RecipientAddress;
    char                        *Type;
    unsigned long long int      Size;
    char                        *Status;
    int                         Priority;
    int                         Read;
    int                         Sent;
    int                         Protected;
    char                        *handle;
    char                        *folder;
} HSAE_MESSAGE_INFO;

typedef enum HSAE_MESSAGE_LOCATION_e
{
    HSAE_MAP_ALL_MESSAGE,
    HSAE_MAP_NEW_MESSAGE,
    HSAE_MAP_SPECIFIC_MESSAGE
} HSAE_MESSAGE_TYPE;

typedef enum HSAE_MESSAGE_SETTING_e
{
    HSAE_MAP_SETTING_READ,
    HSAE_MAP_SETTING_DELETE
} HSAE_MESSAGE_SETTING;

typedef enum HSAE_MESSAGE_EVENT_TYPE_e
{
    HSAE_MESSAGE_EVENT_NEW_MESSAGE,      // NewMessage
    HSAE_MESSAGE_EVENT_DELIVERY_SUCCESS, // Delivery Success
                                        // Message has been successfully delivered to its intended recipient.
    HSAE_MESSAGE_EVENT_SENDING_SUCCESS,  // Sending Success
                                        // Message has been successfully sent to a remote network.
    HSAE_MESSAGE_EVENT_DELIVERY_FAILURE, // DeliveryFailure
                                        // The delivery of a message to its intended recipient failed.
                                        // This event type shall not be applied for emails.
    HSAE_MESSAGE_EVENT_SENDING_FAILURE,  // SendingFailure
                                        // Sending to a remote network failed.
    HSAE_MESSAGE_EVENT_MEMORY_FULL,      // MemoryFull
    HSAE_MESSAGE_EVENT_MEMORY_AVAILABLE, // MemoryAvailable
                                        // Shall receive only if there was a " MemoryFull" event before.
    HSAE_MESSAGE_EVENT_MESSAGE_DELETED,  // MessageDeleted
    HSAE_MESSAGE_EVENT_MESSAGE_SHIFT,    // MessageShift
                                        // Shifted on the MSE from one folder to another folder.
    HSAE_MESSAGE_EVENT_CONNECTION_LOST,  // Connection Lost
    HSAE_MESSAGE_EVENT_NOTIFY_SUCCESS,   // Notify register success
    HSAE_MESSAGE_EVENT_SENDING_REJECT,   // Sending Reject
    HSAE_MESSAGE_EVENT_SENDING_ACCEPT,   // Sending Accept
    HSAE_MESSAGE_EVENT_DELETE_REJECT,    // Delete Reject
    HSAE_MESSAGE_EVENT_DELETE_ACCEPT,    // Delete Accept
    HSAE_MESSAGE_EVENT_READSTATUS_REJECT,// Set read status Reject
    HSAE_MESSAGE_EVENT_READSTATUS_ACCEPT,// Set read status Accept

    HSAE_MESSAGE_EVENT_INVALID
}HSAE_MESSAGE_EVENT_TYPE;

// SPP
typedef enum HSAE_SPP_STATE_e
{
    HSAE_SPP_STATE_DISCONNECTED,
    HSAE_SPP_STATE_CONNECTED
} HSAE_SPP_STATE;

// HID
typedef enum HSAE_HID_STATE_e
{
    HSAE_HID_STATE_DISCONNECTED,
    HSAE_HID_STATE_CONNECTED
} HSAE_HID_STATE;

// PAN
typedef enum HSAE_PAN_STATE_e
{
    HSAE_PAN_STATE_DISCONNECTED,
    HSAE_PAN_STATE_BUSY,
    HSAE_PAN_STATE_CONNECTED
} HSAE_PAN_STATE;

//GATT Client
typedef enum HSAE_GATT_CLIENT_SCAN_DATA_TYPE_e
{
	HSAE_GATT_CLIENT_SCAN_DATA_ADV = 0,
	HSAE_GATT_CLIENT_SCAN_DATA_SCAN_RESPONSE = 1,		
}HSAE_GATT_CLIENT_SCAN_DATA_TYPE;

typedef enum HSAE_GATT_CLIENT_SCAN_ACTION_e
{
	HSAE_GATT_CLIENT_SCAN_STOP = 0,
	HSAE_GATT_CLIENT_SCAN_START	= 1,
}HSAE_GATT_CLIENT_SCAN_ACTION;

typedef enum HSAE_GATT_CLIENT_SEARCHSERVICE_FLAG_e
{
	HSAE_GATT_CLIENT_SEARCHUUID = 0,
	HSAE_GATT_CLIENT_SEARCHALL = 1,		
}SAE_GATT_CLIENT_SEARCHSERVICE_FLAG;

typedef enum HSAE_GATT_STATUS_e
{
	HSAE_GATT_SUCCESS = 0,
	HSAE_GATT_READ_NOT_PERMITTED = 0x02,
	HSAE_GATT_WRITE_NOT_PERMITTED = 0x03,
	HSAE_GATT_INSUFFICIENT_AUTHENTICATION = 0x05,
	HSAE_GATT_REQUEST_NOT_SUPPORTED = 0x06,
	HSAE_GATT_INVALID_OFFSET = 0x7,
	HSAE_GATT_INVALID_ATTRIBUTE_LENGTH = 0xd,
	HSAE_GATT_INSUFFICIENT_ENCRYPTION = 0xf,
	HSAE_GATT_CONNECTION_CONGESTED = 0x8f,
	HSAE_GATT_FAILURE = 0x101,	
}HSAE_GATT_STATUS;

typedef enum HSAE_GATT_SERVICE_TYPE_e
{
	HSAE_GATT_SERVICE_TYPE_PRIMARY = 0,
	HSAE_GATT_SERVICE_TYPE_SECONDARY = 1	
}HSAE_GATT_SERVICE_TYPE;

typedef enum HSAE_GATT_WRITE_TYPE_e
{
	HSAE_GATT_WRITE_TYPE_NO_RESPONSE = 0x01,
	HSAE_GATT_WRITE_TYPE_DEFAULT = 0x02,
	HSAE_GATT_WRITE_TYPE_SIGNED = 0x04
}HSAE_GATT_WRITE_TYPE;

typedef enum HSAE_GATT_PERMISSION_e
{
	HSAE_GATT_PERMISSION_READ = 0x01,
	HSAE_GATT_PERMISSION_READ_ENCRYPTED = 0x02,
	HSAE_GATT_PERMISSION_READ_ENCRYPTED_MITM = 0x04,
	HSAE_GATT_PERMISSION_WRITE = 0x10,
	HSAE_GATT_PERMISSION_WRITE_ENCRYPTED = 0x20,
	HSAE_GATT_PERMISSION_WRITE_ENCRYPTED_MITM = 0x40,
	HSAE_GATT_PERMISSION_WRITE_SIGNED = 0x80,
	HSAE_GATT_PERMISSION_WRITE_SIGNED_MITM = 0x100
}HSAE_GATT_PERMISSION;

typedef enum HSAE_GATT_CHARACTERISTIC_PROPERTY_e
{
	HSAE_GATT_PROPERTY_BROADCAST = 0x01,
	HSAE_GATT_PROPERTY_READ = 0x02,
	HSAE_GATT_PROPERTY_WRITE_NO_RESPONSE = 0x04,
	HSAE_GATT_PROPERTY_WRITE = 0x08,
	HSAE_GATT_PROPERTY_NOTIFY = 0x10,
	HSAE_GATT_PROPERTY_INDICATE = 0x20,
	HSAE_GATT_PROPERTY_SIGNED_WRITE = 0x40,
	HSAE_GATT_PROPERTY_EXTENDED_PROPS = 0x80		
}HSAE_GATT_CHARACTERISTIC_PROPERTY;

typedef enum HSAE_GATT_SATE_e
{
	HSAE_GATT_STATE_DISCONNECTED = 0x0,
	HSAE_GATT_STATE_CONNECTING = 0x01,
	HSAE_GATT_STATE_CONNECTED = 0x02,
	HSAE_GATT_STATE_DSICONNECTING = 0x03,	
}HSAE_GATT_SATE;

typedef enum HSAE_DEVICE_MANUFACTURER_e{
	HSAE_BT_MANUFACTURER_CODE_UNKNOWN,
	HSAE_BT_MANUFACTURER_CODE_APPLE,
	HSAE_BT_MANUFACTURER_CODE_OTHER,
} HSAE_DEVICE_MANUFACTURER;

/*const unsigned char HSAE_GATT_DESCRIPTOR_DISABLE_NOTIFICATION_VALUE[] = {0x00, 0x00};
const unsigned char HSAE_GATT_DESCRIPTOR_ENABLE_INDICATION_VALUE[] = {0x02, 0x00};
const unsigned char HSAE_GATT_DESCRIPTOR_ENABLE_NOTIFICATION_VALUE[] = {0x01, 0x00};*/

typedef struct
{
	 unsigned char uuid[16];
}HSAE_UUID_t;

typedef struct {
	BT_ADDR device;
	unsigned char service_type;
	HSAE_UUID_t  uuid;	
}HSAE_GATT_Service_t;

typedef struct {
	BT_ADDR device;
	HSAE_UUID_t service;
	HSAE_UUID_t characteristic;
	unsigned char property;
	unsigned short int permission;
	unsigned short int value_len;
	unsigned char *value;
}HSAE_GATT_Characteristic_t;

typedef struct {
	BT_ADDR device;
	HSAE_UUID_t service;
	HSAE_UUID_t characteristic;
	HSAE_UUID_t descriptor;
	unsigned short int permission;
	unsigned short int value_len;
	unsigned char *value;
}HSAE_GATT_Descriptor_t;

typedef struct _HSAE_GATT_SCAN_RESULT{
	BT_ADDR device;
	signed char rssi;
	unsigned char name[64];	
	unsigned char adv_data[31];
	unsigned char scan_response[31];
}HSAE_GATT_SCAN_RESULT;

typedef struct 
{
	HSAE_UUID_t server_uuid;	
}HSAE_GATT_Server;

typedef struct
{
	unsigned char includeName;
	unsigned char includeTxPower;
	unsigned short int minSlaveConnectInterval;
	unsigned short int maxSlaveConnectInterval;
	unsigned short int appearance;
	unsigned short int manufacturerDataLength;
	unsigned char manfacturerData[29];
	unsigned short int serviceDataLength;
	unsigned char serviceData[29];
	unsigned short int serviceUUIDLength;
	unsigned char serviceUUID[29];
}HSAE_GATT_Advertise_Info_t;

typedef struct {
	unsigned char includeName;
	unsigned char includeTxPower;
	unsigned short int minSlaveConnectInterval;
	unsigned short int maxSlaveConnectInterval;
	unsigned short int appearance;
	unsigned short int manufacturerDataLength;
	unsigned char manfacturerData[29];
	unsigned short int serviceDataLength;
	unsigned char serviceData[29];
	unsigned short int serviceUUIDLength;
	unsigned char serviceUUID[29];
}HSAE_GATT_ScanResponse_Info_t;

typedef struct {
	unsigned char len;
	unsigned char type;
	unsigned char data[1];
} HSAE_GATT_Usr_def_Advertise_Info_t;

//GATT Server
typedef HSAE_CBK_RET(*HSAE_CBK_OnConnectionStateChange)(void *hsaeCbkParam, const BT_ADDR ba, HSAE_GATT_Server server, int status, unsigned char newState);
typedef HSAE_CBK_RET(*HSAE_CBK_OnCharacteristicReadRequest)(void *hsaeCbkParam, const BT_ADDR ba, int requestId, int offset,  HSAE_GATT_Server server,  HSAE_GATT_Characteristic_t characteristic);
typedef HSAE_CBK_RET(*HSAE_CBK_OnCharacteristicWriteRequest)(void *hsaeCbkParam, const BT_ADDR ba, int requestId, HSAE_GATT_Server server, HSAE_GATT_Characteristic_t characteristic, bool preparedWrite, bool responseNeeded, int offset, unsigned short int value_len, unsigned char* value);

typedef HSAE_CBK_RET(*HSAE_CBK_OnDescriptorReadRequest)(void *hsaeCbkParam, const BT_ADDR ba, int requestId, int offset, HSAE_GATT_Server server, HSAE_GATT_Descriptor_t descriptor);
typedef HSAE_CBK_RET(*HSAE_CBK_OnDescriptorWriteRequest)(void *hsaeCbkParam, const BT_ADDR ba, int requestId, HSAE_GATT_Server server, HSAE_GATT_Descriptor_t descriptor, bool preparedWrite, bool responseNeeded, int offset, unsigned short int value_len, unsigned char* value);

typedef HSAE_CBK_RET(*HSAE_CBK_OnExecuteWrite)(void *hsaeCbkParam, const BT_ADDR ba, int requestId, HSAE_GATT_Server server, bool execute);
typedef HSAE_CBK_RET(*HSAE_CBK_OnMtuChanged)(void *hsaeCbkParam, const BT_ADDR ba, HSAE_GATT_Server server, int mtu);
typedef HSAE_CBK_RET(*HSAE_CBK_OnNotificationSent)(void *hsaeCbkParam, const BT_ADDR ba, HSAE_GATT_Server server, int status);
typedef HSAE_CBK_RET(*HSAE_CBK_OnServiceStarted)(void *hsaeCbkParam, int status, HSAE_GATT_Server server, HSAE_GATT_Service_t service);
typedef HSAE_CBK_RET(*HSAE_CBK_OnServiceStopped)(void *hsaeCbkParam, int status, HSAE_GATT_Server server, HSAE_GATT_Service_t service);



typedef struct {
	HSAE_CBK_OnCharacteristicReadRequest 	OnCharacteristicReadRequestCbk;
	HSAE_CBK_OnCharacteristicWriteRequest 	OnCharacteristicWriteRequestCbk;
	HSAE_CBK_OnConnectionStateChange 	OnConnectionStateChangeCbk;
	HSAE_CBK_OnDescriptorReadRequest 	OnDescriptorReadRequestCbk;
	HSAE_CBK_OnDescriptorWriteRequest 	OnDescriptorWriteRequestCbk;
	HSAE_CBK_OnExecuteWrite				OnExecuteWriteCbk;
	HSAE_CBK_OnMtuChanged				OnMtuChangedCbk;
	HSAE_CBK_OnNotificationSent			OnNotificationSentCbk;
	HSAE_CBK_OnServiceStarted			OnServiceStartedCbk;
	HSAE_CBK_OnServiceStopped			OnServiceStoppedCbk;
}HSAE_GATT_SERVER_CALLBACK_t;


////////////////////////////////////////////////////////////////////////////////////////
//6. Callback Event Function
//
////////////////////////////////////////////////////////////////////////////////////////

/* 6.1.1 HSAE_CBK_BLUEZSTATUS
 *
 * Association command: hsaeCmdStartup
 *
 * When middleware status change, HSAE_CBK_BLUEZSTATUS will trigger with following parameter:
 *
 *      bool on             - middleware (initial/terminate)
 *      HSAE_CMD_RET cmd_ret - internal execute result
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_BLUEZSTATUS)(void *hsaeCbkParam, const bool on, HSAE_CMD_RET cmd_ret);

/* 6.1.2 HSAE_CBK_POWERSTATUS
 *
 * Association command: hsaeCmdSetPower
 *
 * When middleware modify device power setting,
 * HSAE_CBK_POWERSTATUS will trigger with following parameter:
 *
 *      HSAE_POWER hsaePower  - device power setting
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_POWERSTATUS)(void *hsaeCbkParam, const HSAE_POWER hsaePower);

/* 6.2.1 HSAE_CBK_INQUIRYSTATUS
 *
 * Association command: hsaeCmdInquiryStart, hsaeCmdInquiryStop
 *
 * HSAE_CBK_INQUIRYSTATUS return scan status.
 * If found device, also return device information:
 *
 *      bool scanning       - scanning or not
 *
 *      HSAE_DEVICE device   - device information
 *          char *szName    - device name
 *          BT_ADDR btAddr  - device mac address by hex(big endian)
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_INQUIRYSTATUS)(void *hsaeCbkParam, const bool scanning, const HSAE_DEVICE device);

/* 6.2.2 HSAE_CBK_GETPAIREDLIST
 *
 * Association command: hsaeCmdGetPairedList
 *
 * HSAE_CBK_GETPAIREDLIST return paired list.
 *
 *      HSAE_PAIRED *pPairedList - device information array
 *          char *szName    - device name
 *          BT_ADDR btAddr  - device mac address by hex(big endian)
 *
 *      int len - pPairedList length
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_GETPAIREDLIST)(void *hsaeCbkParam, const HSAE_PAIRED *pPairedList, const int len);

/* 6.2.3 HSAE_CBK_REMOTEPAIRREQUEST
 *
 * Association command: hsaeCmdPairRequest, hsaeCmdAnswerPair, hsaeCmdRefusePair
 *
 * HSAE_CBK_REMOTEPAIRREQUEST return pair state and pair result:
 *
 *      int pincode                 - indicate remote device pin code when state is HSAE_ANSWERPAIR_CONFIRM
 *      HSAE_ANSWERPAIR_STATE state  - pair state
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      bool success                - indicate pair result when state is HSAE_ANSWERPAIR_RESULT
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_REMOTEPAIRREQUEST)(void *hsaeCbkParam, const int pincode, const HSAE_ANSWERPAIR_STATE state, const BT_ADDR ba, const bool success);

/* 6.2.4 HSAE_CBK_LOCALSSPNUMBER
 *
 * This callback has been deprecated.
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_LOCALSSPNUMBER)(void *hsaeCbkParam, const int pincode, const BT_ADDR ba);


/* 6.2.5 HSAE_CBK_DIAGOSTICMESSAGE
 *
 * Association command:
 *
 * HSAE_CBK_DIAGNOSTICMESSAGE return audio thread status
 *
 *      code - the return code for audio thread status include A2DP, SCO and Ring
 */
typedef HSAE_CBK_RET (*HSAE_CBK_DIAGNOSTICMESSAGE)(void *hsaeCbkParam, HSAE_DIAG_CODE code);

/* 6.3.1 HSAE_CBK_CONNECTIONSTATUS
 *
 * Association command: hsaeCmdConnectRemoteDevice, hsaeCmdDisconnectRemoteDevice
 *
 * When profile connection state change
 * HSAE_CBK_CONNECTIONSTATUS will trigger with following parameter:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      HSAE_PROFILEID ProfileID     - profile unique ID
 *      HSAE_CONNECTION_STATE state  - connection state
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_CONNECTIONSTATUS)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_PROFILEID ProfileID, const HSAE_CONNECTION_STATE state);

/* 6.3.2 HSAE_CBK_AUTOCONNECTIONSTATUS
 *
 * Association command: hsaeCmdSetRuntimeAutoConnectionEnable
 *
 * When bluelink doing auto connection after power on or linklost and auto connection state change
 * HSAE_CBK_AUTOCONNECTIONSTATUS will trigger with following parameter:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      HSAE_AUTO_CONNECTION_STATE state - auto connection state
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AUTOCONNECTIONSTATUS)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_AUTO_CONNECTION_STATE state);

/*
 * Association command: hsaeCmdConnectRemoteDevice, hsaeCmdDisconnectRemoteDevice
 *
 * When the link is disconnected, HSAE_CBK_DISCONNECTREASON will be triggered with following parameter:
 *
 *      BT_ADDR ba                      -remote device mac address by hex(big endian)
 *      HSAE_DISCONNECT_REASON reason    -disconnect reason
 */
typedef HSAE_CBK_RET (*HSAE_CBK_DISCONNECTREASON)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_DISCONNECT_REASON reason);


typedef HSAE_CBK_RET (*HSAE_CBK_ACLCONNECTIONSTATECHANGE)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_ACL_CONNECTION_STATE newState, unsigned char status);


/* 6.4.1 HSAE_CBK_HANDSFREESTATUS
 *
 * This callback has been deprecated.
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREESTATUS)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_HANDSFREE_STATE state);

/* 6.4.2 HSAE_CBK_HANDSFREECURRENTCALL
 *
 * Association command: Remote device trigger
 *
 * When HANDFREE profile receive a incoming call signal from Bluez
 * HSAE_CBK_HANDSFREECURRENTCALL will trigger with following parameter:
 *
 *      BT_ADDR ba          - remote device mac address by hex(big endian)
 *      char *pwzNumber     - incoming call number
 *      unsigned char idx   - line number
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREECURRENTCALL)(void *hsaeCbkParam, const BT_ADDR ba, const char *pwzNumber, unsigned char idx);

/* 6.4.3 HSAE_CBK_HANDSFREERING
 *
 * Association command: Remote device trigger
 *
 * When HEADSET profile receive ring signal from Bluez
 * HSAE_CBK_HANDSFREERING will trigger with following parameter:
 *
 *      BT_ADDR ba  - remote device mac address by hex(big endian)
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREERING)(void *hsaeCbkParam, const BT_ADDR ba);

/* 6.4.4 HSAE_CBK_HANDSFREECALLSTARTED
 *
 * Association command: hsaeCmdDialCall, hsaeCmdAnswerCall, hsaeCmdHoldCall
 *
 * This callback has been deprecated.
 * HSAE_CBK_HANDSFREECALLACTIVE is recommend.
 *
 * When HANDFREE profile start a call
 * HSAE_CBK_HANDSFREECALLSTARTED will trigger with following parameter:
 *
 *      BT_ADDR ba          - remote device mac address by hex(big endian)
 *      char *pwzNumber     - incoming call number
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREECALLSTARTED)(void *hsaeCbkParam, const BT_ADDR ba, const char *pwzNumber);

/* 6.4.5 HSAE_CBK_HANDSFREECALLREJECTED
 *
 * This callback has been deprecated.
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREECALLREJECTED)(void *hsaeCbkParam, const BT_ADDR ba);

/* 6.4.6 HSAE_CBK_HANDSFREECALLTERMINATED
 *
 * Association command: hsaeCmdRejectCall, hsaeCmdTerminateCall, hsaeCmdHoldCall
 *
 * When HANDFREE profile terminate a call
 * HSAE_CBK_HANDSFREECALLTERMINATED will trigger with following parameter:
 *
 *      BT_ADDR ba          - remote device mac address by hex(big endian)
 *      char *pwzNumber     - incoming call number
 *      unsigned char idx   - line number
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREECALLTERMINATED)(void *hsaeCbkParam, const BT_ADDR ba, const char *pwzNumber, unsigned char idx);

/* 6.4.7 HSAE_CBK_HANDSFREECALLHOLD
 *
 * Association command: hsaeCmdHoldCall
 *
 * When HANDFREE profile hold a call
 * HSAE_CBK_HANDSFREECALLHOLD will trigger with following parameter:
 *
 *      BT_ADDR ba          - remote device mac address by hex(big endian)
 *      char *pwzNumber     - incoming call number
 *      unsigned char idx   - line number
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREECALLHOLD)(void *hsaeCbkParam, const BT_ADDR ba, const char *pwzNumber, unsigned char idx);

/* 6.4.8 HSAE_CBK_HANDSFREECALLACTIVE
 *
 * Association command: hsaeCmdDialCall, hsaeCmdAnswerCall, hsaeCmdHoldCall
 *
 * When HANDFREE profile active a call
 * HSAE_CBK_HANDSFREECALLACTIVE will trigger with following parameter:
 *
 *      BT_ADDR ba          - remote device mac address by hex(big endian)
 *      char *pwzNumber     - incoming call number
 *      unsigned char idx   - line number
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREECALLACTIVE)(void *hsaeCbkParam, const BT_ADDR ba, const char *pwzNumber, unsigned char idx);

/* 6.4.9 HSAE_CBK_HANDSFREEMPTYCALLHOLD
 *
 * Association command: hsaeCmdHoldCall
 *
 * When HANDFREE profile hold a con-call
 * HSAE_CBK_HANDSFREEMPTYCALLHOLD will trigger with following parameter:
 *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *
 *      HSAE_HF_MPTY_INFO *pMptyInfo     - con-call members information array
 *          char *pwzNumber             - phone number of member
 *          unsigned char idx           - line number of member
 *          unsigned char mpty_state    - call state, always 0x01(hold)
 *
 *      unsigned char mpty_line_count   - pMptyInfo length
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREEMPTYCALLHOLD)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_HF_MPTY_INFO *pMptyInfo, const unsigned char mpty_line_count);

/* 6.4.10 HSAE_CBK_HANDSFREEMPTYCALLACTIVE
 *
 * Association command: hsaeCmdHoldCall
 *
 * When HANDFREE profile active a con-call
 * HSAE_CBK_HANDSFREEMPTYCALLACTIVE will trigger with following parameter:
 *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *
 *      HSAE_HF_MPTY_INFO *pMptyInfo     - con-call members information array
 *          char *pwzNumber             - phone number of member
 *          unsigned char idx           - line number of member
 *          unsigned char mpty_state    - call state, always 0x01(hold)
 *
 *      unsigned char mpty_line_count   - pMptyInfo length
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREEMPTYCALLACTIVE)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_HF_MPTY_INFO *pMptyInfo, const unsigned char mpty_line_count);

/* 6.4.11 HSAE_CBK_HANDSFREEOUTGOINGCALL
 *
 * Association command: hsaeCmdDialCall
 *
 * When HANDFREE profile receive a outgoing call signal from Bluez
 * HSAE_CBK_HANDSFREEOUTGOINGCALL will trigger with following parameter:
 *
 *      BT_ADDR ba          - remote device mac address by hex(big endian)
 *      char *pwzNumber     - incoming call number
 *      unsigned char idx   - line number
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREEOUTGOINGCALL)(void *hsaeCbkParam, const BT_ADDR ba, const char *pwzNumber, unsigned char idx);

/* 6.4.12 HSAE_CBK_HANDSFREESCOSTATUS
 *
 * Association command: hsaeCmdDialCall, hsaeCmdAnswerCall
 *
 * When HANDFREE profile modify SCO status
 * HSAE_CBK_HANDSFREESCOSTATUS will trigger with following parameter:
 *
 *      BT_ADDR ba  - remote device mac address by hex(big endian)
 *      bool on     - SCO (connected/disconnected)
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREESCOSTATUS)(void *hsaeCbkParam, const BT_ADDR ba, const bool on);

/* 6.4.13 HSAE_CBK_HANDSFREEBATTSTATUSCHANGED
 *
 * Association command: Remote device trigger
 *
 * When HANDFREE profile receive a battery status changed signal from Bluez
 * HSAE_CBK_HANDSFREEBATTSTATUSCHANGED will trigger with following parameter:
 *
 *      BT_ADDR ba                          - remote device mac address by hex(big endian)
 *      HSAE_HANDSFREE_BATT_STATUS status    - battery status, 0x00 for almost discharged, 0x05 for full charged
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREEBATTSTATUSCHANGED)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_HANDSFREE_BATT_STATUS status);

/* 6.4.14 HSAE_CBK_HANDSFREESIGNALSTRENGTH
 *
 * Association command: Remote device trigger
 *
 * When HANDFREE profile receive a signal strength level changed signal from Bluez
 * HSAE_CBK_HANDSFREESIGNALSTRENGTH will trigger with following parameter:
 *
 *      BT_ADDR ba                                  - remote device mac address by hex(big endian)
 *      HSAE_HANDSFREEP_SIGNASL_STRENGTH strength    - signal strength
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREESIGNALSTRENGTH)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_HANDSFREEP_SIGNASL_STRENGTH strength);

/* 6.4.15 HSAE_CBK_HANDSFREESPKVOL
 *
 * Association command: hsaeCmdSetSpeakerVolume
 *
 * When remote device has been change speaker volume
 * HSAE_CBK_HANDSFREESPKVOL will trigger with following parameter:
 *
 *      BT_ADDR ba              - remote device mac address by hex(big endian)
 *      unsigned char volume    - speaker volume
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREESPKVOL)(void *hsaeCbkParam, const BT_ADDR ba, const unsigned char volume);

/* 6.4.16 HSAE_CBK_HANDSFREEMICVOL
 *
 * Association command: hsaeCmdSetMicrophoneVolume
 *
 * When remote device has been change microphone volume
 * HSAE_CBK_HANDSFREEMICVOL will trigger with following parameter:
 *
 *      BT_ADDR ba              - remote device mac address by hex(big endian)
 *      unsigned char volume    - microphone volume
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREEMICVOL)(void *hsaeCbkParam, const BT_ADDR ba, const unsigned char volume);

/* 6.4.17 HSAE_CBK_HANDSFREECOMMANDRESPONSE
 *
 * Association command: most of HANDFREE/HEADSET profile command
 *
 * This callback type implement two callback: HandsfreeCommandOK, HandsfreeCommandFail
 *
 * HSAE_CBK_HANDSFREECOMMANDRESPONSE return HANDFREE/HEADSET profile command result:
 *
 *      BT_ADDR ba                          - remote device mac address by hex(big endian)
 *      HSAE_HANDSFREEP_AT_COMMAND command   - command op_code
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREECOMMANDRESPONSE)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_HANDSFREEP_AT_COMMAND command);

/* 6.4.18 HSAE_CBK_HANDSFREEMANUFACTURERID
 *
 * Association command: hsaeCmdGetDevManufacturerID
 *
 * When HANDFREE profile request the manufacturer ID of device and receive the manufacturer ID from connected AG.
 * HSAE_CBK_HANDSFREEMANUFACTURERID will trigger with following parameter:
 *
 *      BT_ADDR ba          - remote device mac address by hex(big endian)
 *      char *manufacturerID- manufacturer identification
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREEMANUFACTURERID)(void *hsaeCbkParam, const BT_ADDR ba, const char *manufacturerID);

/* 6.4.19 HSAE_CBK_HANDSFREEMANUFACTURERMODELID
 *
 * Association command: hsaeCmdGetDevManufacturerModelID
 *
 * When HANDFREE profile request the manufacturer model ID of device and receive the manufacturer model ID from connected AG.
 * HSAE_CBK_HANDSFREEMANUFACTURERMODELID will trigger with following parameter:
 *
 *      BT_ADDR ba          - remote device mac address by hex(big endian)
 *      char *manufacturerModelID- manufacturer model identification
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREEMANUFACTURERMODELID)(void *hsaeCbkParam, const BT_ADDR ba, const char *manufacturerModelID);

/* 6.4.20 HSAE_CBK_HANDSFREEMPTYCALLTERMINATED
 *
 * Association command: hsaeCmdRejectCall, hsaeCmdTerminateCall, hsaeCmdHoldCall
 *
 * When HANDFREE profile terminate the multi-party
 * *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREEMPTYCALLTERMINATED)(void *hsaeCbkParam, const BT_ADDR ba);

/* 6.4.21 HSAE_CBK_HANDSFREESERVICESTATE
*/
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREESERVICESTATE)(void *hsaeCbkParam, const BT_ADDR ba, const unsigned char status);

/* 6.4.22 HSAE_CBK_HANDSFREESSUBSCRIBERINFO
*/
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREESUBSCRIBERINFO)(void *hsaeCbkParam, const BT_ADDR ba, const char *pwzNumber);

/* 6.4.23 HSAE_CBK_HANDSFREEVOICE_RECOGNITION_STATE 
*/
typedef HSAE_CBK_RET (*HSAE_CBK_HANDSFREEVOICE_RECOGNITION_STATE)(void *hsaeCbkParam, const BT_ADDR ba, HFP_VOICE_RECOGNITION_ACTION vr_action);

/* 6.5.1 HSAE_CBK_AVRCPSTATUS
 *
 * This callback has been deprecated.
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPSTATUS)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_AVRCP_STATE state);

/* 6.5.2 HSAE_CBK_AVRCPRESPONSE
 *
 * Association command: AVRCP 1.0 command
 *
 * HSAE_CBK_AVRCPRESPONSE return AVRCP 1.0 command result:
 *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *      HSAE_AVRCP_PLAY_OPERATION opCode - command op_code
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPRESPONSE)(void *hsaeCbkParam, const BT_ADDR ba, const char *response, const HSAE_AVRCP_PLAY_OPERATION opCode);

/* 6.5.3 HSAE_CBK_AVRCPGETCAPABILITY
 *
 * Association command: hsaeCmdGetCapability
 *
 * When hsaeCmdGetCapability result arrive,
 * HSAE_CBK_AVRCPGETCAPABILITY will trigger with following parameter:
 *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *      unsigned char capability_count  - amount of capabilities
 *      unsigned char eventID[]         - capability ID
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPGETCAPABILITY)(void *hsaeCbkParam, const BT_ADDR ba, const unsigned char capability_count, const unsigned char eventID[]);

/* 6.5.4 HSAE_CBK_AVRCPPLAYERSETTINGATTR
 *
 * Association command: hsaeCmdGetPlayerSettingAttributes
 *
 * When hsaeCmdGetPlayerSettingAttributes result arrive,
 * HSAE_CBK_AVRCPPLAYERSETTINGATTR will trigger with following parameter:
 *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *      unsigned char attribute_count   - amount of attributes
 *      unsigned char attributeID[]     - attribute ID
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPPLAYERSETTINGATTR)(void *hsaeCbkParam, const BT_ADDR ba, const unsigned char attribute_count, const unsigned char attributeID[]);

/* 6.5.5 HSAE_CBK_AVRCPPLAYERSETTINGVALUE
 *
 * Association command: hsaeCmdGetPlayerSettingValues
 *
 * When hsaeCmdGetPlayerSettingValues result arrive,
 * HSAE_CBK_AVRCPPLAYERSETTINGVALUE will trigger with following parameter:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      unsigned char value_count   - amount of values
 *      unsigned char valueID[]     - value ID
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPPLAYERSETTINGVALUE)(void *hsaeCbkParam, const BT_ADDR ba, const unsigned char value_count, const unsigned char valueID[]);

/* 6.5.6 HSAE_CBK_AVRCPGETCURRENTSETTING
 *
 * Association command: hsaeCmdGetCurrentPlayerAttributeValues
 *
 * When hsaeCmdGetCurrentPlayerAttributeValues result arrive,
 * HSAE_CBK_AVRCPGETCURRENTSETTING will trigger with following parameter:
 *
 *      BT_ADDR ba                              - remote device mac address by hex(big endian)
 *      unsigned char setting_count             - amount of attributes
 *      HSAE_PLAYER_SETTING_ID player_setting[]  - attribute information
 *          unsigned char attribute_id          - attribute ID
 *          unsigned char value_id              - current value ID
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPGETCURRENTSETTING)(void *hsaeCbkParam, const BT_ADDR ba, const unsigned char setting_count, const HSAE_PLAYER_SETTING_ID player_setting[]);

/* 6.5.7 HSAE_CBK_AVRCPSETSUCCESS
 *
 * Association command: hsaeCmdSetCurrentPlayerAttributeValues
 *
 * When hsaeCmdSetCurrentPlayerAttributeValues fail,
 * HSAE_CBK_AVRCPCOMMANDERROR(6.5.20) will trigger.
 *
 * When hsaeCmdSetCurrentPlayerAttributeValues success,
 * HSAE_CBK_AVRCPSETSUCCESS will trigger with following parameter:
 *
 *      BT_ADDR ba  - remote device mac address by hex(big endian)
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPSETSUCCESS)(void *hsaeCbkParam, const BT_ADDR ba);

/* 6.5.8 HSAE_CBK_AVRCPNOWPLAYING
 *
 * Association command: hsaeCmdGetNowPlayingAttributes
 *
 * When hsaeCmdGetNowPlayingAttributes result arrive,
 * HSAE_CBK_AVRCPNOWPLAYING will trigger with following parameter:
 *
 *      BT_ADDR ba                              - remote device mac address by hex(big endian)
 *      unsigned char attribute_count           - amount of metadata attributes
 *      HSAE_NOW_PLAYING_ATTRIBUTE attributes[]  - attribute information
 *          char *media_attribute               - attribute name
 *          unsigned short int charset_id       - character set ID
 *          char *string                        - attribute context
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPNOWPLAYING)(void *hsaeCbkParam, const BT_ADDR ba, const unsigned char attribute_count, const HSAE_NOW_PLAYING_ATTRIBUTE attributes[]);

/* 6.5.9 HSAE_CBK_AVRCPPLAYSTATUS
 *
 * Association command: hsaeCmdGetPlayStatus
 *
 * When hsaeCmdGetPlayStatus result arrive,
 * HSAE_CBK_AVRCPPLAYSTATUS will trigger with following parameter:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      char *song_length           - current media duration
 *      char *song_position         - current media position
 *      HSAE_AVRCP_PLAY_STATE state  - current media state
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPPLAYSTATUS)(void *hsaeCbkParam, const BT_ADDR ba, const char *song_length, const char *song_position, const HSAE_AVRCP_PLAY_STATE state);

/* 6.5.10 HSAE_CBK_AVRCPREGISTERFAILED
 *
 * Association command: hsaeCmdRegisterEventWatcher
 *
 * If AVRCP register command fail,
 * HSAE_CBK_AVRCPREGISTERFAILED will trigger with following parameter:
 *
 *      BT_ADDR ba                          - remote device mac address by hex(big endian)
 *      HSAE_AVRCP_SUPPORTED_EVENTS PduId    - event ID witch register fail
 *      char *reason                        - error message
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPREGISTERFAILED)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_AVRCP_SUPPORTED_EVENTS PduId, const char *reason);

/* 6.5.11 HSAE_CBK_EVENT_PLAYBACKSTATUSCHANGED
 *
 * Association command: hsaeCmdRegisterEventWatcher, hsaeCmdUnregisterEventWatcher
 *
 * If remote device playback status changed and EVENT_PLAYBACK_STATUS_CHANGED(0x01) registered,
 * HSAE_CBK_EVENT_PLAYBACKSTATUSCHANGED will trigger with following parameter:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      HSAE_AVRCP_PLAY_STATE state  - playback status ID
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_EVENT_PLAYBACKSTATUSCHANGED)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_AVRCP_PLAY_STATE state);

/* 6.5.11 HSAE_CBK_EVENT_PLAYSTATUSPREFER
 *
 * Association command: hsaeCmdRegisterEventWatcher, hsaeCmdUnregisterEventWatcher
 *
 * If remote device playback status changed and EVENT_PLAYBACK_STATUS_CHANGED(0x01) registered,
 * HSAE_CBK_EVENT_PLAYSTATUSPREFER will trigger with following parameter:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      HSAE_AVRCP_PLAY_STATE state  - playback status ID
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_EVENT_PLAYSTATUSPREFER)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_AVRCP_PLAY_STATE state);

/* 6.5.12 HSAE_CBK_EVENT_TRACKCHANGED
 *
 * Association command: hsaeCmdRegisterEventWatcher, hsaeCmdUnregisterEventWatcher
 *
 * If remote device has been changed player track and EVENT_TRACK_CHANGED(0x02) registered,
 * HSAE_CBK_EVENT_TRACKCHANGED will trigger with following parameter:
 *
 *      BT_ADDR ba                          - remote device mac address by hex(big endian)
 *      unsigned long long int element_id   - track ID
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_EVENT_TRACKCHANGED)(void *hsaeCbkParam, const BT_ADDR ba, const unsigned long long int element_id);

/* 6.5.13 HSAE_CBK_EVENT_TRACKREACHEDEND
 *
 * Association command: hsaeCmdRegisterEventWatcher, hsaeCmdUnregisterEventWatcher
 *
 * If remote device reach player track end and EVENT_TRACK_REACHED_END(0x03) registered,
 * HSAE_CBK_EVENT_TRACKREACHEDEND will trigger with following parameter:
 *
 *      BT_ADDR ba      - remote device mac address by hex(big endian)
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_EVENT_TRACKREACHEDEND)(void *hsaeCbkParam, const BT_ADDR ba);

/* 6.5.14 HSAE_CBK_EVENT_TRACKREACHEDSTART
 *
 * Association command: hsaeCmdRegisterEventWatcher, hsaeCmdUnregisterEventWatcher
 *
 * If remote device reach player track start and EVENT_TRACK_REACHED_START(0x04) registered,
 * HSAE_CBK_EVENT_TRACKREACHEDSTART will trigger with following parameter:
 *
 *      BT_ADDR ba      - remote device mac address by hex(big endian)
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_EVENT_TRACKREACHEDSTART)(void *hsaeCbkParam, const BT_ADDR ba);

/* 6.5.15 HSAE_CBK_EVENT_PLAYBACKPOSCHANGED
 *
 * Association command: hsaeCmdRegisterEventWatcher, hsaeCmdUnregisterEventWatcher
 *
 * If EVENT_PLAYBACK_POS_CHANGED(0x05) registered and
 * remote device player position shift a interval witch setting by hsaeCmdRegisterEventWatcher,
 * HSAE_CBK_EVENT_PLAYBACKPOSCHANGED will trigger with following parameter:
 *
 *      BT_ADDR ba          - remote device mac address by hex(big endian)
 *      char *current_pos   - currect position
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_EVENT_PLAYBACKPOSCHANGED)(void *hsaeCbkParam, const BT_ADDR ba, const char *current_pos);

/* 6.5.16 HSAE_CBK_EVENT_BATTSTATUSCHANGED
 *
 * Association command: hsaeCmdRegisterEventWatcher, hsaeCmdUnregisterEventWatcher
 *
 * If remote device change the battery status and EVENT_BATT_STATUS_CHANGED(0x06) registered,
 * HSAE_CBK_EVENT_BATTSTATUSCHANGED will trigger with following parameter:
 *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *      HSAE_AVRCP_BATT_STATUS status    - battery status
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_EVENT_BATTSTATUSCHANGED)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_AVRCP_BATT_STATUS status);

/* 6.5.17 HSAE_CBK_EVENT_SYSTEMSTATUSCHANGED
 *
 * Association command: hsaeCmdRegisterEventWatcher, hsaeCmdUnregisterEventWatcher
 *
 * If remote device change the system power status and EVENT_SYSTEM_STATUS_CHANGED(0x07) registered,
 * HSAE_CBK_EVENT_SYSTEMSTATUSCHANGED will trigger with following parameter:
 *
 *      BT_ADDR ba                          - remote device mac address by hex(big endian)
 *      HSAE_AVRCP_SYS_POWER_STATUS status   - system power status
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_EVENT_SYSTEMSTATUSCHANGED)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_AVRCP_SYS_POWER_STATUS status);

/* 6.5.18 HSAE_CBK_EVENT_PLAYERSETTINGCHANGED
 *
 * Association command: hsaeCmdRegisterEventWatcher, hsaeCmdUnregisterEventWatcher
 *
 * If remote device change the player setting and EVENT_PLAYER_APP_SETTING_CHANGED(0x08) registered,
 * HSAE_CBK_EVENT_PLAYERSETTINGCHANGED will trigger with following parameter:
 *
 *      BT_ADDR ba                              - remote device mac address by hex(big endian)
 *      unsigned char setting_count             - amount of setting
 *      HSAE_PLAYER_SETTING_ID player_setting[]  - player setting array
 *          unsigned char attribute_id          - attribute ID
 *          unsigned char value_id              - value ID
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_EVENT_PLAYERSETTINGCHANGED)(void *hsaeCbkParam, const BT_ADDR ba, const unsigned char setting_count, const HSAE_PLAYER_SETTING_ID player_setting[]);

/* 6.5.19 HSAE_CBK_EVENT_UIDSCHANGED
 *
 * Association command: hsaeCmdRegisterEventWatcher, hsaeCmdUnregisterEventWatcher
 *
 * If remote device change the UIDs and EVENT_UIDS_CHANGED(0x0C) registered,
 * HSAE_CBK_EVENT_UIDSCHANGED will trigger with following parameter:
 *
 *      BT_ADDR ba                          - remote device mac address by hex(big endian)
 *      unsigned short int uid_counter      - UID counter
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_EVENT_UIDSCHANGED)(void *hsaeCbkParam, const BT_ADDR ba, const unsigned short int uid_counter);

/* 6.5.20 HSAE_CBK_AVRCPCOMMANDERROR
 *
 * Association command: AVRCP 1.3, 1.4 command
 *
 * HSAE_CBK_AVRCPCOMMANDERROR will trigger when AVRCP 1.3, 1.4 command fail:
 *
 *      BT_ADDR ba              - remote device mac address by hex(big endian)
 *      HSAE_AVRCP_PDUID PduId   - command PDU ID
 *      char *reason            - error message
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPCOMMANDERROR)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_AVRCP_PDUID PduId, const char* reason);

/* 6.5.21 HSAE_CBK_AVRCPABORTEDRECEIVING
 *
 * Association command: AVRCP 1.3, 1.4 command, hsaeCmdAbortReceivingResponse
 *
 * When Bluez abort to receive AVRCP 1.3, 1.4 command result,
 * HSAE_CBK_AVRCPABORTEDRECEIVING will trigger with following parameter:
 *
 *      BT_ADDR ba              - remote device mac address by hex(big endian)
 *      HSAE_AVRCP_PDUID PduId   - command PDU ID
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPABORTEDRECEIVING)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_AVRCP_PDUID PduId);

/* 6.5.22 HSAE_CBK_AVRCPPLAYITEMSUCCESS
 *
 * Association command: hsaeCmdPlayItem
 *
 * When hsaeCmdPlayItem success,
 * HSAE_CBK_AVRCPPLAYITEMSUCCESS will trigger with following parameter:
 *
 *      BT_ADDR ba      - remote device mac address by hex(big endian)
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPPLAYITEMSUCCESS)(void *hsaeCbkParam, const BT_ADDR ba);

/* 6.5.23 HSAE_CBK_AVRCPADDNOWPLAYINGSUCCESS
 *
 * Association command: hsaeCmdAddToNowPlaying
 *
 * When hsaeCmdAddToNowPlaying success,
 * HSAE_CBK_AVRCPADDNOWPLAYINGSUCCESS will trigger with following parameter:
 *
 *      BT_ADDR ba      - remote device mac address by hex(big endian)
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPADDNOWPLAYINGSUCCESS)(void *hsaeCbkParam, const BT_ADDR ba);

/* 6.5.24 HSAE_CBK_AVRCPBROWSEDPLAYER
 *
 * Association command: hsaeCmdAVRCPSetBrowsedPlayer
 *
 * When hsaeCmdAVRCPSetBrowsedPlayer result arrive,
 * HSAE_CBK_AVRCPBROWSEDPLAYER will trigger with following parameter:
 *
 *      BT_ADDR ba                                  - remote device mac address by hex(big endian)
 *      HSAE_BROWSED_PLAYER_ATTRIBUTE browsed_player - browse information
 *          unsigned short int uid_counter          - UID counter
 *          unsigned int item_count                 - amount of item
 *          unsigned short int charset_id           - character set ID
 *          unsigned char folder_depth              - folder depth
 *          char **folder                           - folder name array
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPBROWSEDPLAYER)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_BROWSED_PLAYER_ATTRIBUTE browsed_player);

/* 6.5.25 HSAE_CBK_AVRCPMEDIAPLAYERITEM
 *
 * Association command: hsaeCmdAVRCPGetFolderItems, hsaeCmdAVRCPGetFolderItemsSectional
 *
 * If target folder has player item,
 * each player item will trigger HSAE_CBK_AVRCPMEDIAPLAYERITEM with following parameter:
 *
 *      BT_ADDR ba                                  - remote device mac address by hex(big endian)
 *      HSAE_PLAYER_ITEM player_item                 - item information
 *          HSAE_AVRCP_ITEM_ENTRY last_entry         - indicate this item is last item or not
 *          unsigned short int uid_counter          - UID counter
 *          unsigned short int player_id            - player ID
 *          HSAE_PLAYER_ITEM_TYPE type_flags         - item type
 *          HSAE_PLAYER_ITEM_SUBTYPE subtype_flags   - item sub-type
 *          HSAE_AVRCP_PLAY_STATE play_status_id     - play status ID
 *          unsigned char feature_bit_mask[]        - feature mask(little endian)
 *          unsigned short int charset_id           - character set ID
 *          char *name                              - player name
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPMEDIAPLAYERITEM)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_PLAYER_ITEM player_item);

/* 6.5.26 HSAE_CBK_AVRCPFOLDERITEM
 *
 * Association command: hsaeCmdAVRCPGetFolderItems, hsaeCmdAVRCPGetFolderItemsSectional
 *
 * If target folder has folder item,
 * each folder item will trigger HSAE_CBK_AVRCPFOLDERITEM with following parameter:
 *
 *      BT_ADDR ba                                  - remote device mac address by hex(big endian)
 *      HSAE_FOLDER_ITEM folder_item                 - item information
 *          HSAE_AVRCP_ITEM_ENTRY last_entry         - indicate this item is last item or not
 *          unsigned short int uid_counter          - UID counter
 *          unsigned long long int uid              - UID
 *          HSAE_FOLDER_ITEM_TYPE folder_type        - folder type
 *          HSAE_FOLDER_ITEM_PLAYABLE is_playable    - indicate this folder is playable or not
 *          unsigned short int charset_id           - character set ID
 *          char *folder                            - folder name
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPFOLDERITEM)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_FOLDER_ITEM folder_item);

/* 6.5.27 HSAE_CBK_AVRCPMEDIAITEM
 *
 * Association command: hsaeCmdAVRCPGetFolderItems, hsaeCmdAVRCPGetFolderItemsSectional
 *
 * If target folder has media item,
 * each media item will trigger HSAE_CBK_AVRCPMEDIAITEM with following parameter:
 *
 *      BT_ADDR ba                              - remote device mac address by hex(big endian)
 *
 *      HSAE_MEDIA_ITEM media_item               - item information
 *          HSAE_AVRCP_ITEM_ENTRY last_entry     - indicate this item is last item or not
 *          unsigned short int uid_counter      - UID counter
 *          unsigned long long int uid          - UID
 *          HSAE_MEDIA_ITEM_TYPE media_type      - meida type
 *          unsigned short int charset_id       - character set ID
 *          char *name                          - media name
 *
 *      unsigned char attribute_count           - amount of attribute
 *
 *      HSAE_NOW_PLAYING_ATTRIBUTE attribute[]   - attribute information
 *          const char *media_attribute         - attribute name
 *          unsigned short int charset_id       - character set ID
 *          char *string                        - attribute context
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPMEDIAITEM)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_MEDIA_ITEM media_item, const unsigned char attribute_count, HSAE_NOW_PLAYING_ATTRIBUTE attribute[]);

/* 6.5.28 HSAE_CBK_AVRCPGETFOLDERITEMSSUCCESS
 *
 * Association command: hsaeCmdAVRCPGetFolderItems, hsaeCmdAVRCPGetFolderItemsSectional
 *
 * If get AVRCP get folder command success,
 * HSAE_CBK_AVRCPGETFOLDERITEMSSUCCESS will trigger with following parameter:
 *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *      unsigned short int uid_counter  - UID counter
 *      unsigned int total_item_count   - amount of total item
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPGETFOLDERITEMSSUCCESS)(void *hsaeCbkParam, const BT_ADDR ba, const unsigned short int uid_counter, const unsigned int total_item_count);

/* 6.5.29 HSAE_CBK_AVRCPCHANGEPATHSUCCESS
 *
 * Association command: hsaeCmdAVRCPChangePath
 *
 * When hsaeCmdAVRCPChangePath success,
 * HSAE_CBK_AVRCPCHANGEPATHSUCCESS will trigger with following parameter:
 *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *      unsigned short int item_count   - amount of item in folder
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPCHANGEPATHSUCCESS)(void *hsaeCbkParam, const BT_ADDR ba, const unsigned short int item_count);

/* 6.5.30 HSAE_CBK_AVRCPBROWSINGERROR
 *
 * Association command: AVRCP 1.4 command
 *
 * If AVRCP 1.4 command fail,
 * HSAE_CBK_AVRCPBROWSINGERROR will trigger with following parameter:
 *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *      HSAE_AVRCP_BROWSE_PDUID PduId    - command PDU ID
 *      char *reason                    - error message
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_AVRCPBROWSINGERROR)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_AVRCP_BROWSE_PDUID PduId, const char* reason);

/* 6.5.31 HSAE_CBK_A2DPSTATUS
 *
 * This callback has been deprecated.
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_A2DPSTATUS)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_A2DP_STATE state);

/* 6.5.32 HSAE_CBK_A2DPSTREAMSTATUS
 *
 * Association command: Remote device trigger
 *
 * When BlueZ A2DP stream status change,
 * HSAE_CBK_A2DPSTREAMSTATUS will trigger with following parameter:
 *
 *      BT_ADDR ba      - remote device mac address by hex(big endian)
 *      bool on         - A2DP stream (on/off)
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_A2DPSTREAMSTATUS)(void *hsaeCbkParam, const BT_ADDR ba, const bool on);

/* 6.6.1 HSAE_CBK_PHONEBOOKDOWNLOADSTATUS
 *
 * Association command: most of PBAP profile command
 *
 * HSAE_CBK_PHONEBOOKDOWNLOADSTATUS return PBAP download status:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      HSAE_DOWNLOAD_STATE state    - download state
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_PHONEBOOKDOWNLOADSTATUS)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_DOWNLOAD_STATE state);

/* 6.6.2 HSAE_CBK_DOWNLOADCONTACT
 *
 * Association command: hsaeCmdDownload(NoPhoto)PhonebookStart, hsaeCmdDownload(NoPhoto)ContactsStart
 *
 * HSAE_CBK_DOWNLOADCONTACT return PBAP phone book context:
 *
 *      BT_ADDR ba                              - remote device mac address by hex(big endian)
 *
 *      HSAE_CONTACT_INFO contact[]              - phone book element array
 *          char *name                          - name
 *          char *address                       - address
 *          HSAE_PHONE *phone                    - phone information array
 *              char *phone_number              - phone number
 *              HSAE_PHONE_TYPE *phone_type      - phone types array
 *              unsigned int phone_type_count   - phone types array length
 *          unsigned int phone_count            - phone information array length
 *          char *photo_data                    - photo data(path)
 *          HSAE_CALL call_info                  - call information
 *              char *dir                       - call direction
 *              char *date                      - record data(yyyyymmdd)
 *              char *time                      - record time(hhmmss)
 *          char *fullname                      - full name
 *
 *      size_t contact_len                      - phone book element array length
 *
 *      HSAE_PHONEBOOK_DATA data                 - phone book information
 *          HSAE_PHONEBOOK_LOCATION location     - phone book location
 *          HSAE_PHONEBOOK_TYPE type             - phone book type
 *
 *      HSAE_CMD_RET cmd_ret                     - download result
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_DOWNLOADCONTACT)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_CONTACT_INFO contact[], const size_t contact_len, const HSAE_PHONEBOOK_DATA data, HSAE_CMD_RET cmd_ret);

/* 6.6.3 HSAE_CBK_CONTACTCOUNT
 *
 * Association command: hsaeCmdDownload(NoPhoto)PhonebookStart, hsaeCmdDownload(NoPhoto)ContactsStart
 *
 * HSAE_CBK_CONTACTCOUNT return amount of content before download:
 *
 *      BT_ADDR ba                          - remote device mac address by hex(big endian)
 *      unsigned short int contact_size     - amount of content in phone book
 *      HSAE_PHONEBOOK_DATA data             - phone book information
 *          HSAE_PHONEBOOK_LOCATION location - phone book location
 *          HSAE_PHONEBOOK_TYPE type         - phone book type
 *      HSAE_CMD_RET cmd_ret                 - result of blueZ get size method
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_CONTACTCOUNT)(void *hsaeCbkParam, const BT_ADDR ba, const unsigned short int contact_size, const HSAE_PHONEBOOK_DATA data, HSAE_CMD_RET cmd_ret);

/* 6.7.1 HSAE_CBK_MESSAGEDOWNLOADSTATUS
 *
 * Association command: most of MAP profile command
 *
 * HSAE_CBK_MESSAGEDOWNLOADSTATUS return MAP download status:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      HSAE_DOWNLOAD_STATE state    - download state
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_MESSAGEDOWNLOADSTATUS)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_DOWNLOAD_STATE state);

/* 6.7.2 HSAE_CBK_DOWNLOADMESSAGE
 *
 * Association command: hsaeCmdDownloadMessageStart, hsaeCmdDownloadSentStart, hsaeCmdMessageStartNotify, hsaeCmdDownloadSpecificMsgStart
 *
 * HSAE_CBK_DOWNLOADMESSAGE return remote device message:
 *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *      bool is_new_msg                 - indicate this message is new or not
 *
 *      HSAE_MESSAGE_INFO message[]      - message element array
 *          char *Content               - message content
 *          char *Subject               - message subject
 *          char *Timestamp             - receive timestamp
 *          char *Sender                - message sender
 *          char *SenderAddress         - sender number
 *          char *ReplyTo               - reply target
 *          char *Recipient             - message receiver
 *          char *RecipientAddress      - receiver number
 *          char *Type                  - message type
 *          unsigned long long int Size - message content size
 *          char *Status                - message status
 *          bool Priority               - message priority
 *          bool Read                   - this message has been read
 *          bool Sent                   - this message has been sent
 *          bool Protected              - this message has been protected
 *          char *handle                - message handle ID for hsaeCmdSetMessageProperty, hsaeCmdDownloadSpecificMsgStart
 *          char *folder                - indicate the folder that the message is in (folder: inbox/sent)
 *
 *      size_t message_len              - message element array length
 *      HSAE_CMD_RET cmd_ret             - download result
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_DOWNLOADMESSAGE)(void *hsaeCbkParam, const BT_ADDR ba, const bool is_new_msg, const HSAE_MESSAGE_INFO message[], const size_t message_len, HSAE_CMD_RET cmd_ret);

/* 6.7.3 HSAE_CBK_MESSAGENOTIFY
 *
 * Association command: hsaeCmdMessageStartNotify, hsaeCmdMessageStopNotify
 *
 * If remote device send/receive message and MAP has been registered
 * HSAE_CBK_MESSAGENOTIFY will trigger with following parameter:
 *
 *      BT_ADDR ba                          - remote device mac address by hex(big endian)
 *      HSAE_MESSAGE_EVENT_TYPE event_type   - message notify event type
 *      char *handle                        - message handle ID for hsaeCmdSetMessageProperty, hsaeCmdDownloadSpecificMsgStart
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_MESSAGENOTIFY)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_MESSAGE_EVENT_TYPE event_type, char *handle);

/* 6.8.1 HSAE_CBK_SPPINPUT
 *
 * Association command: Remote device trigger
 *
 * When SPP or SPP_IAP profile receive input,
 * HSAE_CBK_SPPSTREAMINPUT will trigger with following parameter:
 *
 *      BT_ADDR ba      - remote device mac address by hex(big endian)
 *      char input      - SPP input by character
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_SPPINPUT)(void *hsaeCbkParam, const BT_ADDR ba, const char input, const HSAE_PROFILEID ProfileID);

/* 6.8.2 HSAE_CBK_SPPSTREAMINPUT
 *
 * Association command: Remote device trigger
 *
 * When SPP or SPP_IAP profile receive input,
 * HSAE_CBK_SPPSTREAMINPUT will trigger with following parameter:
 *
 *      BT_ADDR ba      - remote device mac address by hex(big endian)
 *      char *input     - SPP input by string
 *      size_t len      - string length
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_SPPSTREAMINPUT)(void *hsaeCbkParam, const BT_ADDR ba, const char *input, size_t len, const HSAE_PROFILEID ProfileID);


typedef HSAE_CBK_RET(*HSAE_CBK_RFCOMMREGSERVER)(void *hsaeCbkParam, HSAE_PROFILEID ProfileID, int status, int channel);

/* 6.9.1 HSAE_CBK_HIDCALIBRATION
 *
 * This callback has been deprecated.
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_HIDCALIBRATION)(void *hsaeCbkParam, const BT_ADDR ba, const bool success);

/* 6.10.1 HSAE_CBK_OBJECTDOWNLOADSTATUS
 *
 * Association command: most of OPP profile command
 *
 * HSAE_CBK_OBJECTDOWNLOADSTATUS return OPP download status:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      HSAE_DOWNLOAD_STATE state    - download state
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_OBJECTDOWNLOADSTATUS)(void *hsaeCbkParam, const BT_ADDR ba, const HSAE_DOWNLOAD_STATE state);

/* 6.10.2 HSAE_CBK_OBJECTAUTHORIZE
 *
 * Association command: Remote device trigger
 *
 * If OPP profile receive a authorize from remote device
 * HSAE_CBK_OBJECTAUTHORIZE will trigger with following parameter:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      char *filename              - file name
 *      char *filetype              - file type
 *      long long int size          - file size
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_OBJECTAUTHORIZE)(void *hsaeCbkParam, const BT_ADDR ba, const char *filename, const char *filetype, long long int size);

/* 6.11.1 HSAE_CBK_OBEXTRANSFERRESULT
 *
 * Association command: Remote device trigger
 *
 * If Obex profiles receive a transfer complete signal from remote device
 * HSAE_CBK_OBEXTRANSFERRESULT will trigger with following parameter:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      HSAE_CMD_RET                 - transfer result
 *
 */
typedef HSAE_CBK_RET (*HSAE_CBK_OBEXTRANSFERRESULT)(void *hsaeCbkParam, const BT_ADDR ba, HSAE_CMD_RET result);

typedef HSAE_CBK_RET (*HSAE_CBK_GATTSCAN)(void *hsaeCbkParam, const BT_ADDR ba, signed char rssi, unsigned short int advdata_len, unsigned char* adv_data, unsigned char data_type);
typedef HSAE_CBK_RET(*HSAE_CBK_GATTSCANRESULT)(void *hsaeCbkParam, HSAE_GATT_SCAN_RESULT* result, unsigned short int num);
typedef HSAE_CBK_RET (*HSAE_CBK_GATTCONNECTIONSTATECHANGE)(void *hsaeCbkParam, const BT_ADDR ba, int status, unsigned char newState);
typedef HSAE_CBK_RET (*HSAE_CBK_GATTGETSERVICE)(void *hsaeCbkParam, const BT_ADDR ba, HSAE_GATT_Service_t *service, unsigned char service_num, int status);
typedef HSAE_CBK_RET (*HSAE_CBK_GATTGETCHARACTERISTIC)(void *hsaeCbkParam, HSAE_GATT_Service_t service, HSAE_GATT_Characteristic_t *characteristic, unsigned char characteristic_num, int status);
typedef HSAE_CBK_RET (*HSAE_CBK_GATTGETDESCRIPTOR)(void *hsaeCbkParam, HSAE_GATT_Characteristic_t characteristic, HSAE_GATT_Descriptor_t* descriptor, unsigned char descriptor_num, int status);
typedef HSAE_CBK_RET (*HSAE_CBK_GATTREADCHARACTERISTIC)(void *hsaeCbkParam,  HSAE_GATT_Characteristic_t characteristic, int status);
typedef HSAE_CBK_RET (*HSAE_CBK_GATTWRITECHARACTERISTIC)(void *hsaeCbkParam,  HSAE_GATT_Characteristic_t characteristic, int status);
typedef HSAE_CBK_RET (*HSAE_CBK_GATTREADDESCRIPTOR)(void *hsaeCbkParam,  HSAE_GATT_Descriptor_t descriptor, int status);
typedef HSAE_CBK_RET (*HSAE_CBK_GATTWRITEDESCRIPTOR)(void *hsaeCbkParam,  HSAE_GATT_Descriptor_t descriptor, int status);
typedef HSAE_CBK_RET (*HSAE_CBK_GATTCHARACTERISTICCHANGED)(void *hsaeCbkParam, HSAE_GATT_Characteristic_t characteristic);
typedef HSAE_CBK_RET (*HSAE_CBK_GATTMTUCHANGED)(void *hsaeCbkParam,  const BT_ADDR ba, int mtu, int status);
typedef HSAE_CBK_RET (*HSAE_CBK_GATTRELIABLEWRITECOMPLETED)(void *hsaeCbkParam,  const BT_ADDR ba, int status);
typedef HSAE_CBK_RET (*HSAE_CBK_GATTREADREMOTERSSI)(void *hsaeCbkParam,  const BT_ADDR ba, int rssi, int status);


// Registration of callback functions
typedef struct
{
    HSAE_CBK_BLUEZSTATUS                 hsae_cb_BlueZStatus;
    HSAE_CBK_POWERSTATUS                 hsae_cb_PowerStatus;
    HSAE_CBK_INQUIRYSTATUS               hsae_cb_InquiryStatus;
    HSAE_CBK_GETPAIREDLIST               hsae_cb_GetPairedList;
    HSAE_CBK_REMOTEPAIRREQUEST           hsae_cb_RemotePairRequest;
    HSAE_CBK_LOCALSSPNUMBER              hsae_cb_LocalSSPNumber;      // This function has been deprecated.
    HSAE_CBK_DIAGNOSTICMESSAGE           hsae_cb_DiagnosticMessage;
    HSAE_CBK_CONNECTIONSTATUS            hsae_cb_ConnectionStatus;
    HSAE_CBK_AUTOCONNECTIONSTATUS        hsae_cb_AutoConnectionStatus;
    HSAE_CBK_DISCONNECTREASON            hsae_cb_DisconnectReason;

    /* HFP */
    HSAE_CBK_HANDSFREESTATUS             hsae_cb_HandsfreeStatus;     // This function has been deprecated.
    HSAE_CBK_HANDSFREECURRENTCALL        hsae_cb_HandsfreeCurrentCall;
    HSAE_CBK_HANDSFREERING               hsae_cb_HandsfreeRing;
    HSAE_CBK_HANDSFREECALLSTARTED        hsae_cb_HandsfreeCallStarted;    // This function has been deprecated.
    HSAE_CBK_HANDSFREECALLREJECTED       hsae_cb_HandsfreeCallRejected;   // This function has been deprecated.
    HSAE_CBK_HANDSFREECALLTERMINATED     hsae_cb_HandsfreeCallTerminated;
    HSAE_CBK_HANDSFREECALLHOLD           hsae_cb_HandsfreeCallHold;
    HSAE_CBK_HANDSFREECALLACTIVE         hsae_cb_HandsfreeCallActive;
    HSAE_CBK_HANDSFREEMPTYCALLHOLD       hsae_cb_HandsfreeMptyCallHold;
    HSAE_CBK_HANDSFREEMPTYCALLACTIVE     hsae_cb_HandsfreeMptyCallActive;
    HSAE_CBK_HANDSFREEMPTYCALLTERMINATED hsae_cb_HandsfreeMptyCallTerminated;
    HSAE_CBK_HANDSFREEOUTGOINGCALL       hsae_cb_HandsfreeOutgoingCall;
    HSAE_CBK_HANDSFREESCOSTATUS          hsae_cb_HandsfreeScoStatus;
    HSAE_CBK_HANDSFREEBATTSTATUSCHANGED  hsae_cb_HandsfreeBattStatusChanged;
    HSAE_CBK_HANDSFREESIGNALSTRENGTH     hsae_cb_HandsfreeSignalStrength;
    HSAE_CBK_HANDSFREESPKVOL             hsae_cb_HandsfreeSpeakerVolume;
    HSAE_CBK_HANDSFREEMICVOL             hsae_cb_HandsfreeMicrophoneVolume;
    HSAE_CBK_HANDSFREECOMMANDRESPONSE    hsae_cb_HandsfreeCommandOK;
    HSAE_CBK_HANDSFREECOMMANDRESPONSE    hsae_cb_HandsfreeCommandFail;
    HSAE_CBK_HANDSFREEMANUFACTURERID     hsae_cb_HandsfreeManufacturerID;
    HSAE_CBK_HANDSFREEMANUFACTURERMODELID    hsae_cb_HandsfreeManufacturerModelID;
    HSAE_CBK_HANDSFREESERVICESTATE       hsae_cb_HandsfreeServiceState;
    HSAE_CBK_HANDSFREESUBSCRIBERINFO     hsae_cb_HandsfreeSubscriberInfo;
	HSAE_CBK_HANDSFREEVOICE_RECOGNITION_STATE       hsae_cb_HandsfreeVoice_Recognition_State;
    
    /* AVRCP */
    HSAE_CBK_AVRCPSTATUS                 hsae_cb_AVRCPStatus;     // This function has been deprecated.
    HSAE_CBK_AVRCPRESPONSE               hsae_cb_AVRCPResponse;
    HSAE_CBK_AVRCPGETCAPABILITY          hsae_cb_AVRCPGetCapability;
    HSAE_CBK_AVRCPPLAYERSETTINGATTR      hsae_cb_AVRCPPlayerSettingAttribute;
    HSAE_CBK_AVRCPPLAYERSETTINGVALUE     hsae_cb_AVRCPPlayerSettingValue;
    HSAE_CBK_AVRCPGETCURRENTSETTING      hsae_cb_AVRCPGetCurrentSetting;
    HSAE_CBK_AVRCPSETSUCCESS             hsae_cb_AVRCPSetSuccess;
    HSAE_CBK_AVRCPNOWPLAYING             hsae_cb_AVRCPNowPlaying;
    HSAE_CBK_AVRCPPLAYSTATUS             hsae_cb_AVRCPPlayStatus;
    HSAE_CBK_AVRCPREGISTERFAILED         hsae_cb_AVRCPRegisterFailed;
    HSAE_CBK_EVENT_PLAYBACKSTATUSCHANGED hsae_cb_Event_PlaybackStatusChanged;
    HSAE_CBK_EVENT_PLAYSTATUSPREFER      hsae_cb_Event_PlayStatusPrefer;
    HSAE_CBK_EVENT_TRACKCHANGED          hsae_cb_Event_TrackChanged;
    HSAE_CBK_EVENT_TRACKREACHEDEND       hsae_cb_Event_TrackReachedEnd;
    HSAE_CBK_EVENT_TRACKREACHEDSTART     hsae_cb_Event_TrackReachedStart;
    HSAE_CBK_EVENT_PLAYBACKPOSCHANGED    hsae_cb_Event_PlaybackPosChanged;
    HSAE_CBK_EVENT_BATTSTATUSCHANGED     hsae_cb_Event_BattStatusChanged;
    HSAE_CBK_EVENT_SYSTEMSTATUSCHANGED   hsae_cb_Event_SystemStatusChanged;
    HSAE_CBK_EVENT_PLAYERSETTINGCHANGED  hsae_cb_Event_PlayerSettingChanged;
    HSAE_CBK_EVENT_UIDSCHANGED           hsae_cb_Event_UIDsChanged;
    HSAE_CBK_AVRCPCOMMANDERROR           hsae_cb_AVRCPCommandError;
    HSAE_CBK_AVRCPABORTEDRECEIVING       hsae_cb_AVRCPAbortedReceiving;
    HSAE_CBK_AVRCPPLAYITEMSUCCESS        hsae_cb_AVRCPPlayItemSuccess;
    HSAE_CBK_AVRCPADDNOWPLAYINGSUCCESS   hsae_cb_AVRCPAddNowPlayingSuccess;
    HSAE_CBK_AVRCPBROWSEDPLAYER          hsae_cb_AVRCPBrowsedPlayer;
    HSAE_CBK_AVRCPMEDIAPLAYERITEM        hsae_cb_AVRCPMediaPlayerItem;
    HSAE_CBK_AVRCPFOLDERITEM             hsae_cb_AVRCPFolderItem;
    HSAE_CBK_AVRCPMEDIAITEM              hsae_cb_AVRCPMediaItem;
    HSAE_CBK_AVRCPGETFOLDERITEMSSUCCESS  hsae_cb_AVRCPGetFolderItemsSuccess;
    HSAE_CBK_AVRCPCHANGEPATHSUCCESS      hsae_cb_AVRCPChangePathSuccess;
    HSAE_CBK_AVRCPBROWSINGERROR          hsae_cb_AVRCPBrowsingError;

    /* A2DP */
    HSAE_CBK_A2DPSTATUS                  hsae_cb_A2DPStatus;      // This function has been deprecated.
    HSAE_CBK_A2DPSTREAMSTATUS            hsae_cb_A2DPStreamStatus;
    /* PBAP */
    HSAE_CBK_PHONEBOOKDOWNLOADSTATUS     hsae_cb_PhonebookDownloadStatus;
    HSAE_CBK_DOWNLOADCONTACT             hsae_cb_DownloadContact;
    HSAE_CBK_CONTACTCOUNT                hsae_cb_ContactCount;
    /* MAP */
    HSAE_CBK_MESSAGEDOWNLOADSTATUS       hsae_cb_MessageDownloadStatus;
    HSAE_CBK_DOWNLOADMESSAGE             hsae_cb_DownloadMessage;
    HSAE_CBK_MESSAGENOTIFY               hsae_cb_MessageNotify;
    /* SPP */
    HSAE_CBK_SPPINPUT                    hsae_cb_SPPInput;
    HSAE_CBK_SPPSTREAMINPUT              hsae_cb_SPPStreamInput;
    HSAE_CBK_RFCOMMREGSERVER			hsae_cb_RFCOMMRegServer;
    /* HID */
    HSAE_CBK_HIDCALIBRATION              hsae_cb_HIDCalibration;
    /* OPP */
    HSAE_CBK_OBJECTDOWNLOADSTATUS        hsae_cb_ObjectDownloadStatus;
    HSAE_CBK_OBJECTAUTHORIZE             hsae_cb_ObjectAuthorize;
    HSAE_CBK_OBEXTRANSFERRESULT          hsae_cb_ObexTransferResult;

   /*GATT Client*/
   HSAE_CBK_GATTSCAN						hsae_cb_GATTScan;
   HSAE_CBK_GATTSCANRESULT 					hsae_cb_GATTScanResult;
   HSAE_CBK_GATTCONNECTIONSTATECHANGE       hsae_cb_GATTConnectionStateChange;
   HSAE_CBK_GATTGETSERVICE					hsae_cb_GATTGetService;
   HSAE_CBK_GATTGETCHARACTERISTIC			hsae_cb_GATTGetCharacteristic;
   HSAE_CBK_GATTGETDESCRIPTOR				hsae_cb_GATTGetDescriptor;
   HSAE_CBK_GATTREADCHARACTERISTIC			hsae_cb_GATTReadCharacteristic;
   HSAE_CBK_GATTWRITECHARACTERISTIC		    hsae_cb_GATTWriteCharacteristic;
   HSAE_CBK_GATTREADDESCRIPTOR				hsae_cb_GATTReadDescriptor;
   HSAE_CBK_GATTWRITEDESCRIPTOR			    hsae_cb_GATTWriteDescriptor;
   HSAE_CBK_GATTCHARACTERISTICCHANGED		hsae_cb_GATTCharacteristicChanged;
   HSAE_CBK_GATTMTUCHANGED					hsae_cb_GATTMtuChanged;
   HSAE_CBK_GATTRELIABLEWRITECOMPLETED		hsae_cb_GATTReliableWriteComplete;
   HSAE_CBK_GATTREADREMOTERSSI				hsae_cb_GATTReadRemoteRSSI;

   /*extension*/
   HSAE_CBK_ACLCONNECTIONSTATECHANGE		hsae_cb_ACLConnectionStateChange;
} HSAE_CALLBACK;

////////////////////////////////////////////////////////////////////////////////////////
//5 Callback Registration Function
//
////////////////////////////////////////////////////////////////////////////////////////

#ifdef __GNUC__
  #define BLUELINK_PUBLIC_API __attribute__ ((visibility ("default")))
#else
  #define BLUELINK_PUBLIC_API 
#endif

//5.1.1 hsaeCmdSetCbk_BlueZStatus
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_BlueZStatus(HSAE_HANDLE hsaeHandle, HSAE_CBK_BLUEZSTATUS pfn);

//5.1.2 hsaeCmdSetCbk_PowerStatus
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_PowerStatus(HSAE_HANDLE hsaeHandle, HSAE_CBK_POWERSTATUS pfn);

//5.2.1 hsaeCmdSetCbk_InquiryStatus
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_InquiryStatus(HSAE_HANDLE hsaeHandle, HSAE_CBK_INQUIRYSTATUS pfn);

//5.2.2 hsaeCmdSetCbk_GetPairedList
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_GetPairedList(HSAE_HANDLE hsaeHandle, HSAE_CBK_GETPAIREDLIST pfn);

//5.2.3 hsaeCmdSetCbk_RemotePairRequest
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_RemotePairRequest (HSAE_HANDLE hsaeHandle, HSAE_CBK_REMOTEPAIRREQUEST pfn);

//5.2.4 hsaeCmdSetCbk_LocalSSPNumber
//This function has been deprecated.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_LocalSSPNumber (HSAE_HANDLE hsaeHandle, HSAE_CBK_LOCALSSPNUMBER pfn);

//5.2.5 hsaeCmdSetCbk_RemotePairRequest
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_DiagnosticMessage (HSAE_HANDLE hsaeHandle, HSAE_CBK_DIAGNOSTICMESSAGE pfn);

//5.3.1 hsaeCmdSetCbk_ConnectionStatus
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_ConnectionStatus(HSAE_HANDLE hsaeHandle, HSAE_CBK_CONNECTIONSTATUS pfn);

//5.3.2 hsaeCmdSetCbk_AutoConnectionStatus
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AutoConnectionStatus(HSAE_HANDLE hsaeHandle, HSAE_CBK_AUTOCONNECTIONSTATUS pfn);

//5.3.3 hsaeCmdSetCbk_DisconnectReason
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_DisconnectReason(HSAE_HANDLE hsaeHandle, HSAE_CBK_DISCONNECTREASON pfn);

//5.3.4 hsaeCmdSetCbk_AclConnStateChanged
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_ACLConnectionStateChanged(HSAE_HANDLE hsaeHandle, HSAE_CBK_ACLCONNECTIONSTATECHANGE pfn);

//5.4.1 hsaeCmdSetCbk_HandsfreeStatus
//This function has been deprecated.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeStatus(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREESTATUS pfn);

//5.4.2 hsaeCmdSetCbk_HandsfreeCurrentCall
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeCurrentCall(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREECURRENTCALL pfn);

//5.4.3 hsaeCmdSetCbk_HandsfreeRing
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeRing(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREERING pfn);

//5.4.4 hsaeCmdSetCbk_HandsfreeCallStarted
//This function has been deprecated.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeCallStarted(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREECALLSTARTED pfn);

//5.4.5 hsaeCmdSetCbk_HandsfreeCallRejected
//This function has been deprecated.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeCallRejected(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREECALLREJECTED pfn);

//5.4.6 hsaeCmdSetCbk_HandsfreeCallTerminated
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeCallTerminated(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREECALLTERMINATED pfn);

//5.4.7 hsaeCmdSetCbk_HandsfreeCallHold
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeCallHold(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREECALLHOLD pfn);

//5.4.8 hsaeCmdSetCbk_HandsfreeCallActive
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeCallActive(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREECALLACTIVE pfn);

//5.4.9 hsaeCmdSetCbk_HandsfreeMptyCallHold
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeMptyCallHold(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREEMPTYCALLHOLD pfn);

//5.4.10 hsaeCmdSetCbk_HandsfreeMptyCallActive
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeMptyCallActive(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREEMPTYCALLACTIVE pfn);

//5.4.11 hsaeCmdSetCbk_HandsfreeOutgoingCall
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeOutgoingCall(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREEOUTGOINGCALL pfn);

//5.4.12 hsaeCmdSetCbk_HandsfreeScoStatus
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeScoStatus(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREESCOSTATUS pfn);

//5.4.13 hsaeCmdSetCbk_HandsfreeBattStatusChanged
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeBattStatusChanged(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREEBATTSTATUSCHANGED pfn);

//5.4.14 hsaeCmdSetCbk_HandsfreeSignalStrength
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeSignalStrength(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREESIGNALSTRENGTH pfn);

//5.4.15 hsaeCmdSetCbk_HandsfreeSpeakerVolume
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeSpeakerVolume(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREESPKVOL pfn);

//5.4.16 hsaeCmdSetCbk_HandsfreeMicrophoneVolume
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeMicrophoneVolume(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREEMICVOL pfn);

//5.4.17 hsaeCmdSetCbk_HandsfreeCommandOK
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeCommandOK(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREECOMMANDRESPONSE pfn);

//5.4.18 hsaeCmdSetCbk_HandsfreeCommandFail
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeCommandFail(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREECOMMANDRESPONSE pfn);

//5.4.19 hsaeCmdSetCbk_HandsfreeManufacturerID
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeManufacturerID(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREEMANUFACTURERID pfn);

//5.4.20 hsaeCmdSetCbk_HandsfreeManufacturerModelID
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeManufacturerModelID(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREEMANUFACTURERMODELID pfn);

//5.4.21 hsaeCmdSetCbk_HandsfreeMptyCallTerminated
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeMptyCallTerminated(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREEMPTYCALLTERMINATED pfn);

//5.4.22 hsaeCmdSetCbk_HandsfreeServiceState
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeServiceState(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREESERVICESTATE pfn);

//5.4.23 hsaeCmdSetCbk_HandsfreeSubscriberInfo
BLUELINK_PUBLIC_API HSAE_CMD_RET	hsaeCmdSetCbk_HandsfreeSubscriberInfo(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREESUBSCRIBERINFO pfn);

//5.4.24 hsaeCmdSetCbk_HandsfreeVoice_Recognition_States
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HandsfreeVoice_Recognition_States(HSAE_HANDLE hsaeHandle, HSAE_CBK_HANDSFREEVOICE_RECOGNITION_STATE pfn);

//5.5.1 hsaeCmdSetCbk_AVRCPStatus
//This function has been deprecated.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPStatus(HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPSTATUS pfn);

//5.5.2 hsaeCmdSetCbk_AVRCPResponse
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPResponse(HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPRESPONSE pfn);

//5.5.3 hsaeCmdSetCbk_AVRCPGetCapability
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPGetCapability(HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPGETCAPABILITY pfn);

//5.5.4 hsaeCmdSetCbk_AVRCPPlayerSettingAttribute
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPPlayerSettingAttribute(HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPPLAYERSETTINGATTR pfn);

//5.5.5 hsaeCmdSetCbk_AVRCPPlayerSettingValue
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPPlayerSettingValue(HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPPLAYERSETTINGVALUE pfn);

//5.5.6 hsaeCmdSetCbk_AVRCPGetCurrentSetting
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPGetCurrentSetting(HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPGETCURRENTSETTING pfn);

//5.5.7 hsaeCmdSetCbk_AVRCPSetSuccess
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPSetSuccess(HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPSETSUCCESS pfn);

//5.5.8 hsaeCmdSetCbk_AVRCPNowPlaying
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPNowPlaying(HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPNOWPLAYING pfn);

//5.5.9 hsaeCmdSetCbk_AVRCPPlayStatus
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPPlayStatus (HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPPLAYSTATUS pfn);

//5.5.10 hsaeCmdSetCbk_AVRCPRegisterFailed
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPRegisterFailed(HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPREGISTERFAILED pfn);

//5.5.11 hsaeCmdSetCbk_Event_PlaybackStatusChanged
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_Event_PlaybackStatusChanged(HSAE_HANDLE hsaeHandle, HSAE_CBK_EVENT_PLAYBACKSTATUSCHANGED pfn);

//5.5.11 hsaeCmdSetCbk_Event_PlayStatusPrefer
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_Event_PlayStatusPrefer(HSAE_HANDLE hsaeHandle, HSAE_CBK_EVENT_PLAYSTATUSPREFER pfn);

//5.5.12 hsaeCmdSetCbk_Event_TrackChanged
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_Event_TrackChanged(HSAE_HANDLE hsaeHandle, HSAE_CBK_EVENT_TRACKCHANGED pfn);

//5.5.13 hsaeCmdSetCbk_Event_TrackReachedEnd
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_Event_TrackReachedEnd(HSAE_HANDLE hsaeHandle, HSAE_CBK_EVENT_TRACKREACHEDEND pfn);

//5.5.14 hsaeCmdSetCbk_Event_TrackReachedStart
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_Event_TrackReachedStart(HSAE_HANDLE hsaeHandle, HSAE_CBK_EVENT_TRACKREACHEDSTART pfn);

//5.5.15 hsaeCmdSetCbk_Event_PlaybackPosChanged
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_Event_PlaybackPosChanged(HSAE_HANDLE hsaeHandle, HSAE_CBK_EVENT_PLAYBACKPOSCHANGED pfn);

//5.5.16 hsaeCmdSetCbk_Event_BattStatusChanged
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_Event_BattStatusChanged(HSAE_HANDLE hsaeHandle, HSAE_CBK_EVENT_BATTSTATUSCHANGED pfn);

//5.5.17 hsaeCmdSetCbk_Event_SystemStatusChanged
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_Event_SystemStatusChanged(HSAE_HANDLE hsaeHandle, HSAE_CBK_EVENT_SYSTEMSTATUSCHANGED pfn);

//5.5.18 hsaeCmdSetCbk_Event_PlayerSettingChanged
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_Event_PlayerSettingChanged(HSAE_HANDLE hsaeHandle, HSAE_CBK_EVENT_PLAYERSETTINGCHANGED pfn);

//5.5.19 hsaeCmdSetCbk_Event_UIDsChanged
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_Event_UIDsChanged(HSAE_HANDLE hsaeHandle, HSAE_CBK_EVENT_UIDSCHANGED pfn);

//5.5.20 hsaeCmdSetCbk_AVRCPCommandError
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPCommandError(HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPCOMMANDERROR pfn);

//5.5.21 hsaeCmdSetCbk_AVRCPAbortedReceiving
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPAbortedReceiving(HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPABORTEDRECEIVING pfn);

//5.5.22 hsaeCmdSetCbk_AVRCPPlayItemSuccess
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPPlayItemSuccess(HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPPLAYITEMSUCCESS pfn);

//5.5.23 hsaeCmdSetCbk_AVRCPAddNowPlayingSuccess
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPAddNowPlayingSuccess(HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPADDNOWPLAYINGSUCCESS pfn);

//5.5.24 hsaeCmdSetCbk_AVRCPBrowsedPlayer
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPBrowsedPlayer(HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPBROWSEDPLAYER pfn);

//5.5.25 hsaeCmdSetCbk_AVRCPMediaPlayerItem
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPMediaPlayerItem(HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPMEDIAPLAYERITEM pfn);

//5.5.26 hsaeCmdSetCbk_AVRCPFolderItem
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPFolderItem(HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPFOLDERITEM pfn);

//5.5.27 hsaeCmdSetCbk_AVRCPMediaItem
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPMediaItem(HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPMEDIAITEM pfn);

//5.5.28 hsaeCmdSetCbk_AVRCPGetFolderItemsSuccess
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPGetFolderItemsSuccess(HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPGETFOLDERITEMSSUCCESS pfn);

//5.5.29 hsaeCmdSetCbk_AVRCPChangePathSuccess
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPChangePathSuccess(HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPCHANGEPATHSUCCESS pfn);

//5.5.30 hsaeCmdSetCbk_AVRCPBrowsingError
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_AVRCPBrowsingError(HSAE_HANDLE hsaeHandle, HSAE_CBK_AVRCPBROWSINGERROR pfn);

//5.5.31 hsaeCmdSetCbk_A2DPStatus
//This function has been deprecated.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_A2DPStatus(HSAE_HANDLE hsaeHandle, HSAE_CBK_A2DPSTATUS pfn);

//5.5.32 hsaeCmdSetCbk_A2DPStreamStatus
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_A2DPStreamStatus(HSAE_HANDLE hsaeHandle, HSAE_CBK_A2DPSTREAMSTATUS pfn);

//5.6.1 hsaeCmdSetCbk_PhonebookDownloadStatus
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_PhonebookDownloadStatus(HSAE_HANDLE hsaeHandle, HSAE_CBK_PHONEBOOKDOWNLOADSTATUS pfn);

//5.6.2 hsaeCmdSetCbk_DownloadContact
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_DownloadContact(HSAE_HANDLE hsaeHandle, HSAE_CBK_DOWNLOADCONTACT pfn);

//5.6.3 hsaeCmdSetCbk_ContactCount
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_ContactCount(HSAE_HANDLE hsaeHandle, HSAE_CBK_CONTACTCOUNT pfn);

//5.7.1 hsaeCmdSetCbk_MessageDownloadStatus
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_MessageDownloadStatus(HSAE_HANDLE hsaeHandle, HSAE_CBK_MESSAGEDOWNLOADSTATUS pfn);

//5.7.2 hsaeCmdSetCbk_DownloadMessage
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_DownloadMessage(HSAE_HANDLE hsaeHandle, HSAE_CBK_DOWNLOADMESSAGE pfn);

//5.7.3 hsaeCmdSetCbk_MessageNotify
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_MessageNotify(HSAE_HANDLE hsaeHandle, HSAE_CBK_MESSAGENOTIFY pfn);

//5.8.1 hsaeCmdSetCbk_SPPInput
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_SPPInput(HSAE_HANDLE hsaeHandle, HSAE_CBK_SPPINPUT pfn);

//5.8.2 hsaeCmdSetCbk_SPPStreamInput
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_SPPStreamInput(HSAE_HANDLE hsaeHandle, HSAE_CBK_SPPSTREAMINPUT pfn);

//5.8.3 hsaeCmdSetCbk_RFCOMMRegServer
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_RFCOMMRegServer(HSAE_HANDLE hsaeHandle, HSAE_CBK_RFCOMMREGSERVER pfn);

//5.9.1 hsaeCmdSetCbk_HIDCalibration
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_HIDCalibration(HSAE_HANDLE hsaeHandle, HSAE_CBK_HIDCALIBRATION pfn);

//5.10.1 hsaeCmdSetCbk_ObjectDownloadStatus
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_ObjectDownloadStatus(HSAE_HANDLE hsaeHandle, HSAE_CBK_OBJECTDOWNLOADSTATUS pfn);

//5.10.2 hsaeCmdSetCbk_ObjectAuthorize
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_ObjectAuthorize(HSAE_HANDLE hsaeHandle, HSAE_CBK_OBJECTAUTHORIZE pfn);

//5.11.1 hsaeCmdSetCbk_ObexTransferResult
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_ObexTransferResult(HSAE_HANDLE hsaeHandle, HSAE_CBK_OBEXTRANSFERRESULT pfn);


////////////////////////////////////////////////////////////////////////////////////////
//4.1 Local Information
//Functions deal with MW startup, device information
////////////////////////////////////////////////////////////////////////////////////////
//4.1.1 hsaeCmdStartup
//This is the first middleware function should be called before any other functions.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdStartup(HSAE_HANDLE *phsaeHandle, void *hsaeCbkParam);

//4.1.2 hsaeCmdSetCallback
//Set the callback functions for this middleware.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCallback(HSAE_HANDLE hsaeHandle, HSAE_CALLBACK *hsaeCbk);

//4.1.3 hsaeCmdEnd
//This is the last middleware function should be called before application termination.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdEnd(HSAE_HANDLE *phsaeHandle);

//4.1.4 hsaeCmdSetLogEnable
//Set the log option for this middleware.
//Since log is used in hsaeCmdStartup, this function may be called earlier to make it silent.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetLogEnable(HSAE_HANDLE hsaeHandle, bool enable);

//4.1.5 hsaeCmdGetMwVersion
//Get the middleware version number.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetMwVersion (HSAE_VER *pVer);

//4.1.6 hsaeCmdGetLocalBtAddr
//Get the address of local Bluetooth device.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetLocalBtAddr(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.1.7 hsaeCmdGetLocalDeviceName
//Get the name of local Bluetooth device.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetLocalDeviceName(HSAE_HANDLE hsaeHandle, char *pwzDevName,
        size_t nDevName);

//4.1.8 hsaeCmdSetLocalDeviceName
//Set the name of local Bluetooth device.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetLocalDeviceName(HSAE_HANDLE hsaeHandle, const char *pwzDevName);

//4.1.9 hsaeCmdGetCOD
//Get the class of device settings of local Bluetooth device.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetCOD(HSAE_HANDLE phsaeHandle, unsigned int *pcod);

//4.1.10 hsaeCmdGetPower
//Application uses this function to get the current power status of local Bluetooth device.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetPower(HSAE_HANDLE hsaeHandle, HSAE_POWER *phsaePower);

//4.1.11 hsaeCmdSetPower
//Application should power on the Bluetooth device by calling this function.
//To power on, set hsaePower to HSAE_POWERON.
//To power off, set hsaePower to HSAE_POWEROFF.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetPower(HSAE_HANDLE hsaeHandle, HSAE_POWER hsaePower);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetPowerEx(HSAE_HANDLE hsaeHandle, HSAE_POWER hsaePower, int download_firmware);

//4.1.12 hsaeCmdSetLocalBtAddr
//Set the address of local Bluetooth device.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetLocalBtAddr(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.1.13 hsaeCmdSetAudioDeviceName
//Set Audio output device name. Both A2DP and RING
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetAudioDeviceName(HSAE_HANDLE hsaeHandle, char *AudioDevName);

//4.1.14 hsaeCmdSetA2DPDeviceName
//Set A2DP output device name. Only A2DP
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetA2DPDeviceName(HSAE_HANDLE hsaeHandle, char *A2DPDevName);

//4.1.15 hsaeCmdSetRingDeviceName
//Set RING output device name. Only RING
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetRingDeviceName(HSAE_HANDLE hsaeHandle, char *RingDevName);

//4.1.16 hsaeCmdGetA2DPDeviceName
//Get A2DP output device name. Only A2DP
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetA2DPDeviceName(HSAE_HANDLE hsaeHandle, char *A2DPDevName, int size);

//4.1.17 hsaeCmdGetRingDeviceName
//Get RING output device name. Only RING
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetRingDeviceName(HSAE_HANDLE hsaeHandle, char *RingDevName, int size);

//4.1.18 hsaeCmdSetTestMode
//Set Bluetooth Test Mode.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetTestMode(HSAE_HANDLE hsaeHandle, bool enable);

//4.1.19 hsaeCmdSetBrEdrTestMode
//Set BrEdr Test Mode
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetBrEdrTestMode(HSAE_HANDLE hsaeHandle, unsigned char mode, unsigned short freq, unsigned char power, unsigned char data_type, unsigned char modulation);

////////////////////////////////////////////////////////////////////////////////////////
//4.2 Scanning and Pairing
//Functions deal with scanning devices and pairing
////////////////////////////////////////////////////////////////////////////////////////

//4.2.1 hsaeCmdGetScanEnable
//Get the scan enable settings of local Bluetooth device.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetScanEnable(HSAE_HANDLE hsaeHandle, bool *enable);

//4.2.2 hsaeCmdSetScanEnable
//Set the scan enable settings of local Bluetooth device.
//  Parameter mask is a bitwise value,
//  0x01: inquiry scan enable.
//  0x02: page scan enable.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetScanEnable(HSAE_HANDLE hsaeHandle, bool enable);

//4.2.3 hsaeCmdSetScanEnableTime
//Set the scan enable time settings of local Bluetooth device.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetScanEnableTime(HSAE_HANDLE hsaeHandle, unsigned int enable_time);

//4.2.4 hsaeCmdGetPairedList
//Application uses this function to get the previous paired Bluetooth devices.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetPairedList(HSAE_HANDLE hsaeHandle);

//4.2.5 hsaeCmdPairRequest
//Application uses this function to pair a remote Bluetooth device.
// Parameter:
// nPinLength, the maximum value is 16.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdPairRequest(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        int nPinLength, char *ppin);

//4.2.6 hsaeCmdDeleteDeviceData
//Application uses this function to delete all storage data (phonebook, photo, msg) of the remote Bluetooth device.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdDeleteDeviceData(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.2.7 hsaeCmdDeletePairDevice
//Application uses this function to delete a previous paired remote Bluetooth device.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdDeletePairDevice(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.2.8 hsaeCmdAnswerPair
//uses this function to accept a pairing request from remote Bluetooth device.
//The pairing request is an event notified by callback function defined in pfnNfmCbkRemotePairRequest.
//Parameter:
//      nPinLength, the maximum value is 16.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdAnswerPair(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        int nPinLength, char *ppin);

//4.2.9 hsaeCmdRefusePair
//Application uses this function to refuse a pairing request from remote Bluetooth device.
//The pairing request is an event notified by callback function defined in pfnNfmCbkRemotePairRequest.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdRefusePair(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.2.10 hsaeCmdQueryPairService
//Application uses this function to query the available services supported by remote Bluetooth device.
//HSAE_CMD_RET   hsaeCmdQueryPairService(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.2.10 hsaeCmdInquiryStart
//find other Bluetooth device near its neighborhood.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdInquiryStart(HSAE_HANDLE hsaeHandle);

//4.2.11 hsaeCmdInquiryStop
//cancel the previous inquiry request.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdInquiryStop(HSAE_HANDLE hsaeHandle);

//4.2.12 hsaeCmdGetInquiryInterval
//Get the inquiry interval.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetInquiryInterval(HSAE_HANDLE hsaeHandle, int *interval);

//4.2.13 hsaeCmdIsPaired
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdIsPaired(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, bool *paired);

//4.2.14 hsaeCmdSupportProfilelist
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSupportProfilelist(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, int *Profile_ID);

//4.2.15 hsaeCmdSupportProfileVersion
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSupportProfileVersion(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, HSAE_PROFILEID Profile_ID, HSAE_SUPPORT_VERSION *version);

//4.2.16 hsaeCmdSetSSPMode
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetSSPMode(HSAE_HANDLE hsaeHandle, bool enable);

//4.2.16 hsaeCmdSetPairedDeviceNum
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetPairDeviceNum(HSAE_HANDLE hsaeHandle, int num );

//4.2.17 hsaeCmdGetPairedDeviceNum
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetPairDeviceNum(HSAE_HANDLE hsaeHandle, int *num );

////////////////////////////////////////////////////////////////////////////////////////
//4.3 Connecting
//Functions deal with connecting
////////////////////////////////////////////////////////////////////////////////////////

//4.3.1 hsaeCmdConnectRemoteDevice
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetRemoteDeviceManufacturerID(HSAE_HANDLE hsaeHandle, BT_ADDR * pba, int * manu_id);

//Activate a service supported by remote Bluetooth device.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdConnectRemoteDevice(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        HSAE_PROFILEID ProfileID);

BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdConnectRemoteDeviceEx(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        HSAE_PROFILEID ProfileID, unsigned char channel);

//4.3.2 hsaeCmdDisconnectRemoteDevice
//Deactivate a service that was previously activated
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdDisconnectRemoteDevice(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        HSAE_PROFILEID ProfileID);

//4.3.3 hsaeCmdSetRfcommUUID
//Set UUID before rfcomm connect.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetRfcommUUID(HSAE_HANDLE hsaeHandle, const char* uuid);

//4.3.4 hsaeCmdSetAutoConnectionEnable
//Set auto connection configuration for BT power-on.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetAutoConnectionEnable(HSAE_HANDLE hsaeHandle, int retry_count);

//4.3.5 hsaeCmdGetAutoConnectionEnable
//Get auto connection configuration for BT power-on.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetAutoConnectionEnable(HSAE_HANDLE hsaeHandle, int* retry_count);

//4.3.6 hsaeCmdSetAutoConnectionEnableLinklost
//Set auto connection configuration for linklost.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetAutoConnectionEnableLinklost(HSAE_HANDLE hsaeHandle, int retry_count);

//4.3.7 hsaeCmdGetAutoConnectionEnableLinklost
//Get auto connection configuration for linklost.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetAutoConnectionEnableLinklost(HSAE_HANDLE hsaeHandle, int* retry_count);

//4.3.8 hsaeCmdSetConnectionAllAudioEnable
//Set connection all audio profile configuration
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetConnectionAllAudioEnable(HSAE_HANDLE hsaeHandle, int enable);

//4.3.9 hsaeCmdGetConnectionAllAudioEnable
//Get connection all audio profile configuration
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetConnectionAllAudioEnable(HSAE_HANDLE hsaeHandle, int* enable);

//4.3.10 hsaeCmdSetRemoteDeviceProfile
//Set device enable audio profile
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetRemoteDeviceProfile(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, HSAE_PROFILEID ProfileID);

//4.3.11 hsaeCmdGetRemoteDeviceProfile
//Get device enable audio profile
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetRemoteDeviceProfile(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, int *ProfileID);

//4.3.12 hsaeCmdSetRuntimeAutoConnectionEnable
//Set auto connection configuration for now or BT power-on
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetRuntimeAutoConnectionEnable(HSAE_HANDLE hsaeHandle, int retry_count);

//4.3.13 hsaeCmdGetRuntimeAutoConnectionEnable
//Get auto connection configuration for now or BT power-on.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetRuntimeAutoConnectionEnable(HSAE_HANDLE hsaeHandle, int* retry_count);

//4.3.14 hsaeCmdSetAutoConnectionPeriod
//Set auto connection retry period configuration.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetAutoConnectionPeriod(HSAE_HANDLE hsaeHandle, int retry_period);

//4.3.15 hsaeCmdGetAutoConnectionPeriod
//Get auto connection retry period configuration.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetAutoConnectionPeriod(HSAE_HANDLE hsaeHandle, int* retry_period);

////////////////////////////////////////////////////////////////////////////////////////
//4.4 HFP
//Functions deal with hands-free
////////////////////////////////////////////////////////////////////////////////////////

//4.4.1 hsaeCmdHSAnswerCall
//Application uses this function to answer to current incoming call on connected headset AG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdHSAnswerCall(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.4.2 hsaeCmdHSHangUpCall
//Application uses this function to terminate the ongoing call through the connected headset AG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdHSHangUpCall(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.4.3 hsaeCmdDialCall
//Application uses this function to dial to the given number through the connected AG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdDialCall(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        char *pwzNumber);

//4.4.4 hsaeCmdDialLast
//Application uses this function to dial to the last dialed number through the connected AG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdDialLast(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.4.5 hsaeCmdSendDTMF
//Application uses this function to send DTMF number through the connected AG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSendDTMF(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        char *pwzNumber);

//4.4.6 hsaeCmdAudioTransfer
//iActive, 0: transfer to handsfree unit, 1: transfer to audio gateway.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdAudioTransfer(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        bool iActive);

//4.4.7 hsaeCmdAnswerCall
//Application uses this function to answer to current incoming call on connected AG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdAnswerCall(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.4.8 hsaeCmdRejectCall
//Application uses this function to reject to current incoming call on connected AG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdRejectCall(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.4.9 hsaeCmdTerminateCall
//Application uses this function to terminate the ongoing call through the connected AG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdTerminateCall(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.4.10 hsaeCmdSetCallAutoReaction
//Application uses this function to set auto answer to incoming call on connected AG
//Set seconds to -1 to cancel auto answer
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCallAutoReaction(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        HSAE_HANDSFREE_AUTO_REACTION action);

//4.4.11 hsaeCmdHoldCall
//Application uses this function to send hold call command to connected AG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdHoldCall(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        HSAE_HANDSFREE_CALLHELD_OPTION option, unsigned int specified_call_number);

//4.4.12 hsaeCmdSetSpeakerVolume
//Application uses this function to set the speaker volume through the connected AG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetSpeakerVolume(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        const char *volume);

//4.4.13 hsaeCmdSetMicrophoneVolume
//Application uses this function to set microphone volume through the connected AG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetMicrophoneVolume(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        const char *volume);

//4.4.14 hsaeCmdSetPlayRing
//Application uses this function to set ringtone play or not
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetPlayRing(HSAE_HANDLE hsaeHandle, bool action);

//4.4.15 hsaeCmdGetPlayRing
//Application uses this function to get ringtone play or not
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetPlayRing(HSAE_HANDLE hsaeHandle, bool* action);

//4.4.16 hsaeCmdSetVoiceRecognition
//Application uses this function to set Voice Recognition through the connected AG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetVoiceRecognition(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, int enable);

//4.4.17 hsaeCmdSetXAPL
//Application uses this function to set Apple custom AT commands through the connected AG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetXAPL(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        unsigned int vid, unsigned int pid, unsigned int version, unsigned int feature);

//4.4.18 hsaeCmdSetSiriEyeFree
//Application uses this function to set Siri eye free mode information through the connected AG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetSiriEyeFree(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, int enable);

//4.4.19 hsaeCmdSetCallAutoReactionTime
//Application uses this function to set the waiting time to do auto answer or reject.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCallAutoReactionTime(HSAE_HANDLE hsaeHandle, int Sec);

//4.4.20 hsaeCmdGetCallAutoReaction
//Application uses this function to get the action which will be taken when there is an incoming call.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetCallAutoReaction(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        HSAE_HANDSFREE_AUTO_REACTION *action);
//4.4.21 hsaeCmdGetCallAutoReactionTime
//Application uses this function to get the waiting time to do auto answer or reject.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetCallAutoReactionTime(HSAE_HANDLE hsaeHandle, int *Sec);

//4.4.22 hsaeCmdMicrophoneMute
//Application uses this function to set HFP sco microphone mute/unmute (local device).
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdMicrophoneMute(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, bool opt);

//4.4.23 hsaeCmdGetMicMuteStatus
//Application uses this function to get HFP sco microphone status (mute or unmute) (local device).
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetMicMuteStatus(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, bool *opt);

//4.4.24 hsaeCmdGetDevManufacturerID
//Application uses this function to request manufacturer identification to AG which is connected handsfree.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetDevManufacturerID(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.4.25 hsaeCmdGetDevManufacturerModelID
//Application uses this function to request manufacturer model identification to AG which is connected handsfree.
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetDevManufacturerModelID(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.4.26 hsaeCmdSetSpkRender
//Application uses this function to set HFP sco speaker render or not (local device).
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetSpkRender(HSAE_HANDLE hsaeHandle, bool action);

//4.4.27 hsaeCmdGetSpkRender
//Application uses this function to get HFP sco speaker render status (render or not) (local device).
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetSpkRender(HSAE_HANDLE hsaeHandle, bool* action);

//4.4.28 hsaeCmdSetRejectInBandRing
//Application uses this function to reject in-band ring tone from phone ane play internal ring tone from host or not
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetRejectInBandRing(HSAE_HANDLE hsaeHandle, bool action);

////////////////////////////////////////////////////////////////////////////////////////
//4.5 A2DP and AVRCP
//Functions deal with A2DP and AVRCP
////////////////////////////////////////////////////////////////////////////////////////

/**********************************************/
/* Functions below need AVRCP higher than 1.0 */
/**********************************************/

//4.5.1 hsaeCmdAVRCPControlCommand
//Application uses this function to control the player on connected TG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdAVRCPControlCommand(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        HSAE_AVRCP_PLAY_OPERATION opidCode);

/**********************************************/
/* Functions below need AVRCP higher than 1.3 */
/**********************************************/

//4.5.2 hsaeCmdGetCapability
//Application uses this function to get the capability of player on connected TG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetCapability (HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.5.3 hsaeCmdGetPlayerSettingAttributes
//Application uses this function to get the attributes supported by player on connected TG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetPlayerSettingAttributes(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.5.4 hsaeCmdGetPlayerSettingValues
//Application uses this function to get the attribute values supported by player on connected TG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetPlayerSettingValues(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        const unsigned char attr_id);

//4.5.5 hsaeCmdGetCurrentPlayerAttributeValues
//Application uses this function to get the current attribute values of player on connected TG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetCurrentPlayerAttributeValues(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.5.6 hsaeCmdSetCurrentPlayerAttributeValues
//Application uses this function to set the current attribute values of player on connected TG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCurrentPlayerAttributeValues(HSAE_HANDLE hsaeHandle,
        BT_ADDR *pba, unsigned char setting[PLAYER_ATTRIBUTE]);

//4.5.7 hsaeCmdGetNowPlayingAttributes
//Application uses this function to get the attributes of currently playing track on connected TG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetNowPlayingAttributes(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.5.8 hsaeCmdGetPlayStatus
//Application uses this function to get the play status of player on connected TG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetPlayStatus(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.5.9 hsaeCmdRegisterEventWatcher
//Application uses this function to register a event watcher on connected TG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdRegisterEventWatcher(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        HSAE_AVRCP_SUPPORTED_EVENTS event, unsigned int playback_interval_sec);

//4.5.10 hsaeCmdUnregisterEventWatcher
//Application uses this function to unregister a event watcher on connected TG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdUnregisterEventWatcher(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        HSAE_AVRCP_SUPPORTED_EVENTS event);

//4.5.11 hsaeCmdAbortReceivingResponse
//Application uses this function to abort receiving response of last request on connected TG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdAbortReceivingResponse(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

/**********************************************/
/* Functions below need AVRCP higher than 1.4 */
/**********************************************/

//4.5.12 hsaeCmdAVRCPSetBrowsedPlayer
//Application uses this function to set browsed player on connected TG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdAVRCPSetBrowsedPlayer(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        unsigned short int player_id);

//4.5.13 hsaeCmdAVRCPGetFolderItems
//Application uses this function to get folder item on connected TG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdAVRCPGetFolderItems(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        const HSAE_FOLDER_ITEM_SCOPE_ID scope_id);

//4.5.14 hsaeCmdAVRCPGetFolderItemsSectional
//Application uses this function to get folder item sectional on connected TG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdAVRCPGetFolderItemsSectional(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        const HSAE_FOLDER_ITEM_SCOPE_ID scope_id, unsigned int start_item_offset, unsigned int max_count);


//4.5.15 hsaeCmdAVRCPChangePath
//Application uses this function to change path of current navigating folder on connected TG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdAVRCPChangePath(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        const unsigned short int uid_counter,
        const HSAE_AVRCP_CHANGE_FOLDER_DIRECTION direction,
        const unsigned long long int uid);

//4.5.16 hsaeCmdPlayItem
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdPlayItem(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        const unsigned long long int uid, const unsigned short int uid_counter);

//4.5.17 hsaeCmdAddToNowPlaying
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdAddToNowPlaying(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        const unsigned long long int uid, const unsigned short int uid_counter);

//4.5.18 hsaeCmdGetAVRCPSupportVersion
//Application uses this function to get AVRCP version supported by remote TG
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetAVRCPSupportVersion(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        HSAE_AVRCP_SUPPORT_VERSION *version);

//4.5.19 hsaeCmdSetAVRCPVolumeChanged
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetAVRCPVolumeChanged(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        unsigned char volume);

/**********************************************/
/* Functions below a2dp local processing      */
/**********************************************/

//4.5.19 hsaeCmdA2dpMute
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdA2dpMute(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, bool opt);

//4.5.20 hsaeCmdA2dpGetMuteStatus
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdA2dpGetMuteStatus(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, bool *opt);

////////////////////////////////////////////////////////////////////////////////////////
//4.6 PBAP
//Functions deal with Phonebook downloading
////////////////////////////////////////////////////////////////////////////////////////

//4.6.1 hsaeCmdDownloadPhonebookStart
//Application uses this function to download phonebook at given location and type
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdDownloadPhonebookStart(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, HSAE_PHONEBOOK_DATA data);

//4.6.2 hsaeCmdDownloadContactsStart
//Application uses this function to download phonebook in phone memory and SIM card
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdDownloadContactsStart(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, HSAE_PHONEBOOK_DATA data);

//4.6.3 hsaeCmdDownloadPhonebookStop
//Application uses this function to stop download phonebook
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdDownloadPhonebookStop(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.6.4 hsaeCmdPbapTransferCancel
//Application uses this function to cancel download phonebook for BQB test
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdPbapTransferCancel (HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.6.5 hsaeCmdDownloadPhonebookState
//Application uses this function to get phonebook download status
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdDownloadPhonebookState(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        HSAE_DOWNLOAD_STATE *state, HSAE_PHONEBOOK_DATA *data);

//4.6.6 hsaeCmdDownloadNoPhotoPhonebookStart
//Application uses this function to download phonebook without photo at given location and type
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdDownloadNoPhotoPhonebookStart(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, HSAE_PHONEBOOK_DATA data);

//4.6.7 hsaeCmdDownloadNoPhotoContactsStart
//Application uses this function to download phonebook without photo in phone memory and SIM card
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdDownloadNoPhotoContactsStart(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, HSAE_PHONEBOOK_DATA data);

//4.6.8 hsaeCmdDownloadPhonebookState
//Application uses this function to download phonebook without disconnect after download complete
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdDownloadPhonebookAlwaysConnected(HSAE_HANDLE hsaeHandle, bool enable);


////////////////////////////////////////////////////////////////////////////////////////
//4.7 MAP
//Functions deal with Message downloading
////////////////////////////////////////////////////////////////////////////////////////
//4.7.1 hsaeCmdDownloadMessageStart
//Application uses this function to download message in folder inbox
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdDownloadMessageStart(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, unsigned char is_detail);

//4.7.2 hsaeCmdDownloadMessageStop
//Application uses this function to stop download message
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdDownloadMessageStop(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.7.3 hsaeCmdPushMessage
//Application uses this function to push message
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdPushMessage(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        char *pwzNumber, char *message);

//4.7.4 hsaeCmdMessageStartNotify
//Application uses this function to start receive message notification from remote device
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdMessageStartNotify(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.7.5 hsaeCmdMessageStopNotify
//Application uses this function to stop receive message notification from remote device
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdMessageStopNotify(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.7.6 hsaeCmdDownloadMessageState
//Application uses this function to get message download status
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdDownloadMessageState(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        HSAE_DOWNLOAD_STATE *state, HSAE_MESSAGE_TYPE *type);

//4.7.7 hsaeCmdSetMessageProperty
//Application uses this function to set message status
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetMessageProperty(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        HSAE_MESSAGE_SETTING setting, const char* message_handle);

//4.7.8 hsaeCmdDownloadSentStart
//Application uses this function to download message in folder sent
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdDownloadSentStart(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, unsigned char is_detail);

//4.7.9 hsaeCmdDownloadSpecificMsgStart
//Application uses this function to download one specific message in folder inbox/sent by message handle
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdDownloadSpecificMsgStart(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, const char* message_handle, MAP_FOLDER_STRUCT folder);

//4.7.10 hsaeCmdDownloadOutboxStart
//Application uses this function to download message in folder outbox
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdDownloadOutboxStart(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, unsigned char is_detail);

//4.7.11 hsaeCmdSetMAPMaxCount
//Application to set MAP max downloading count
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetMAPMaxCount(HSAE_HANDLE hsaeHandle, unsigned short map_max_listing_count);

//4.7.12 hsaeCmdGetMAPMaxCount
//Application to get MAP max downloading count
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGetMAPMaxCount(HSAE_HANDLE hsaeHandle, unsigned short *map_max_listing_count);

////////////////////////////////////////////////////////////////////////////////////////
//4.8 SPP
//Functions deal with serial port simulation
////////////////////////////////////////////////////////////////////////////////////////

//4.8.1 hsaeCmdSPPOutput
//Application uses this function to send text to remote device
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSPPOutput(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, char *output, const HSAE_PROFILEID ProfileID);

//4.8.2 hsaeCmdSPPStreamOutput
//Application uses this function to send text to remote device
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSPPStreamOutput(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, char *output, const int len, const HSAE_PROFILEID ProfileID);

//4.8.3 hsaeCmdSetRFCOMMProfileUUID
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetRFCOMMUsrDefUUID(HSAE_HANDLE hsaeHandle, HSAE_PROFILEID rfcomm_id, HSAE_UUID_t uuid);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetRFCOMMUsrDefUUIDEx(HSAE_HANDLE hsaeHandle, HSAE_PROFILEID rfcomm_id, HSAE_UUID_t uuid, unsigned char bServer);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdDelRFCOMMUsrDefUUID(HSAE_HANDLE hsaeHandle, HSAE_PROFILEID rfcomm_id);

//4.8.3 hsaeCmdRFCOMMAddServer
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdRFCOMMAddServer(HSAE_HANDLE hsaeHandle, HSAE_PROFILEID rfcomm_id);

//4.8.4 hsaeCmdRFCOMMRemoveServer
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdRFCOMMRemoveServer(HSAE_HANDLE hsaeHandle, HSAE_PROFILEID rfcomm_id);

////////////////////////////////////////////////////////////////////////////////////////
//4.9 HID
//Functions deal with Human Interface Device operation
////////////////////////////////////////////////////////////////////////////////////////

//4.9.1 hsaeCmdStartHID
//Application uses this function to BT HID Service
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdStartHID(HSAE_HANDLE hsaeHandle);

//4.9.2 hsaeCmdHIDSendKey
//Application uses this function to send HID key to remote device
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdHIDSendKey(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, int key);

//4.9.3 hsaeCmdHIDSendAbsoluteXY
//Application uses this function to send HID absolute coordinate (x , y) to remote device
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdHIDSendAbsoluteXY(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, int x, int y, unsigned char button);

//4.9.4 hsaeCmdHIDSendRelativeXY
//Application uses this function to send HID relative coordinate (x , y) to remote device
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdHIDSendRelativeXY(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, int x, int y, unsigned char button);

//4.9.5 hsaeCmdHIDEnterCalibration
//Application uses this function to enter calibration mode
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdHIDEnterCalibration(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

//4.9.6 hsaeCmdHIDCancelCalibration
//Application uses this function to cancel calibration mode
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdHIDCancelCalibration(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

////////////////////////////////////////////////////////////////////////////////////////
//4.9 OPP
//Functions deal with object push operation
////////////////////////////////////////////////////////////////////////////////////////

//4.10.1 hsaeCmdOPPCreateSession
//Application uses this function to send a file to remote device with absolute path
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdOPPCreateSession(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, char *file_path);
//4.10.2 hsaeCmdOPPCancelSession
//Application uses this function to cancel sending transfer
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdOPPCancelSession(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);
//4.10.3 hsaeCmdOPPAuthorizationReply
//Application uses this function to reply authorization to remote device
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdOPPAuthorizationReply(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, bool allow);
//4.10.4 hsaeCmdOPPCancelReceive
//Application uses this function to cancel receiving transfer
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdOPPCancelReceive(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);
//4.10.5 hsaeCmdOPPSetStoragePath
//Application uses this function to set storage directory by absolute path
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdOPPSetStoragePath(HSAE_HANDLE hsaeHandle, char *storage_path);
//4.10.6 hsaeCmdOPPGetStoragePath
//Application uses this function to get storage directory
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdOPPGetStoragePath(HSAE_HANDLE hsaeHandle, char **storage_path);

//4.11.1 hsaeCmdSetSpeakerVolume2DSP
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetSpeakerVolume2DSP(HSAE_HANDLE hsaeHandle, BT_ADDR *pba,
        const char *volume);

BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetPairedDeviceSoft(HSAE_PAIRED_SOFT_MODE mode);

////////////////////////////////////////////////////////////////////////////////////////
//4.10   GATT Client 
//Functions deal with GATT Client operation
////////////////////////////////////////////////////////////////////////////////////////
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTScan(HSAE_HANDLE hsaeHandle, unsigned char opt);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTConnect(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTDisconnect(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTSearchService(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTGetIncludedService(HSAE_HANDLE hsaeHandle, HSAE_GATT_Service_t service);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTGetCharacteristic(HSAE_HANDLE hsaeHandle, HSAE_GATT_Service_t service);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTGetDescriptor(HSAE_HANDLE hsaeHandle, HSAE_GATT_Characteristic_t characteristic);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTReadCharacteristic(HSAE_HANDLE hsaeHandle, HSAE_GATT_Characteristic_t characteristic);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTWriteCharacteristic(HSAE_HANDLE hsaeHandle, HSAE_GATT_Characteristic_t characteristic);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTReadDescriptor(HSAE_HANDLE hsaeHandle, HSAE_GATT_Descriptor_t descriptor);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTWriteDescriptor(HSAE_HANDLE hsaeHandle, HSAE_GATT_Descriptor_t descriptor);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTSetCharacteristicNotification(HSAE_HANDLE hsaeHandle, HSAE_GATT_Characteristic_t characteristic, bool enable);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTSetMtu(HSAE_HANDLE hsaeHandle, BT_ADDR *pba, int mtu);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTExecuteReliableWrite(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTReadRemoteRssi(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTRefresh(HSAE_HANDLE hsaeHandle, BT_ADDR *pba);

BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_GATTScan(HSAE_HANDLE hsaeHandle, HSAE_CBK_GATTSCAN pfn);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_GATTScanResult(HSAE_HANDLE hsaeHandle, HSAE_CBK_GATTSCANRESULT pfn);

BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_GATTConnectionStateChange(HSAE_HANDLE hsaeHandle, HSAE_CBK_GATTCONNECTIONSTATECHANGE pfn);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_GATTGetService(HSAE_HANDLE hsaeHandle, HSAE_CBK_GATTGETSERVICE pfn);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_GATTGetCharacteristic(HSAE_HANDLE hsaeHandle, HSAE_CBK_GATTGETCHARACTERISTIC pfn);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_GATTGetDescriptor(HSAE_HANDLE hsaeHandle, HSAE_CBK_GATTGETDESCRIPTOR pfn);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_GATTReadCharacteristic(HSAE_HANDLE hsaeHandle, HSAE_CBK_GATTREADCHARACTERISTIC pfn);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_GATTWriteCharacteristic(HSAE_HANDLE hsaeHandle, HSAE_CBK_GATTWRITECHARACTERISTIC pfn);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_GATTReadDescriptor(HSAE_HANDLE hsaeHandle, HSAE_CBK_GATTREADDESCRIPTOR pfn);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_GATTWriteDescriptor(HSAE_HANDLE hsaeHandle, HSAE_CBK_GATTWRITEDESCRIPTOR pfn);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_GATTCharacteristicChanged(HSAE_HANDLE hsaeHandle, HSAE_CBK_GATTCHARACTERISTICCHANGED pfn);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_GATTMtuChanged(HSAE_HANDLE hsaeHandle, HSAE_CBK_GATTMTUCHANGED pfn);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_GATTReliableWriteCompleted(HSAE_HANDLE hsaeHandle, HSAE_CBK_GATTRELIABLEWRITECOMPLETED pfn);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdSetCbk_GATTReadRemoteRssi(HSAE_HANDLE hsaeHandle, HSAE_CBK_GATTREADREMOTERSSI pfn);

////////////////////////////////////////////////////////////////////////////////////////
//4.10   GATT Server 
//Functions deal with GATT Server operation
////////////////////////////////////////////////////////////////////////////////////////
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTOpenServer(HSAE_HANDLE hsaeHandle, HSAE_UUID_t	*context, HSAE_GATT_SERVER_CALLBACK_t callback, HSAE_GATT_Server *gatt_server);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTCloseServer(HSAE_HANDLE hsaeHandle, HSAE_GATT_Server *gatt_server);

BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTStartAdvertise(HSAE_HANDLE hsaeHandle, HSAE_UUID_t* context, HSAE_GATT_Advertise_Info_t *adv_info, HSAE_GATT_ScanResponse_Info_t *scanResponse_info);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTStartAdvertiseEx(HSAE_HANDLE hsaeHandle, HSAE_UUID_t* context, HSAE_GATT_Advertise_Info_t *adv_info, HSAE_GATT_ScanResponse_Info_t *scanResponse_info, unsigned short adv_min_interval, unsigned short adv_max_interval);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTStopAdvertise(HSAE_HANDLE hsaeHandle, HSAE_UUID_t* context);

BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTAddAdvertiseUsrdefInfo(HSAE_HANDLE hsaeHandle, HSAE_UUID_t * context, HSAE_GATT_Usr_def_Advertise_Info_t* adv_info);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTStartAdvertiseEx2(HSAE_HANDLE hsaeHandle, HSAE_UUID_t * context, unsigned short adv_min_interval, unsigned short adv_max_interval);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTStopAdvertiseEx(HSAE_HANDLE hsaeHandle, HSAE_UUID_t* context);

BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTSvrConnect(HSAE_HANDLE hsaeHandle, HSAE_GATT_Server *gatt_server, BT_ADDR *pba);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTSvrDisconnect(HSAE_HANDLE hsaeHandle, HSAE_GATT_Server *gatt_server);

BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTServerAddService(HSAE_HANDLE hsaeHandle, HSAE_GATT_Server *gatt_server, HSAE_GATT_Service_t gatt_service);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTServerAddIncludedService(HSAE_HANDLE hsaeHandle, HSAE_GATT_Server *gatt_server, HSAE_GATT_Service_t gatt_service, HSAE_GATT_Service_t included_service);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTServerAddCharacteristic(HSAE_HANDLE hsaeHandle, HSAE_GATT_Server *gatt_server, HSAE_GATT_Service_t gatt_service, HSAE_GATT_Characteristic_t gatt_characteristic);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTServerAddDescriptor(HSAE_HANDLE hsaeHandle, HSAE_GATT_Server *gatt_server,   HSAE_GATT_Characteristic_t gatt_characteristic, HSAE_GATT_Descriptor_t gatt_descriptor);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTServerDeleteService(HSAE_HANDLE hsaeHandle, HSAE_GATT_Server *gatt_server, HSAE_GATT_Service_t gatt_service);

BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTServerStartService(HSAE_HANDLE hsaeHandle, HSAE_GATT_Server *gatt_server, HSAE_GATT_Service_t gatt_service);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTServerStopService(HSAE_HANDLE hsaeHandle, HSAE_GATT_Server *gatt_server, HSAE_GATT_Service_t gatt_service);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTServerNotifyCharacteristicChanged(HSAE_HANDLE hsaeHandle, HSAE_GATT_Server *gatt_server, HSAE_GATT_Characteristic_t gatt_characteristic, bool confirm);
BLUELINK_PUBLIC_API HSAE_CMD_RET hsaeCmdGATTServerSendResponse(HSAE_HANDLE hsaeHandle, HSAE_GATT_Server *gatt_server, int requestId, int status, int offset, unsigned short int value_len, unsigned char* value);




 #ifdef __cplusplus
 } // extern "C"
 #endif // __cplusplus

#endif //NFBLUELINK_H


