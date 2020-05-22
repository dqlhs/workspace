/*!
 *************************************************************************************
 * 2012 (c) nFore Technology Co., Ltd.  All rights reserved.
 *
 * \file bluelink_api.h
 *
 * \brief
 *    Bluelink middleware APIs declaration
 *    Please reference to "nFore Bluelink Programming Guide" for function description
 *
 *
 *************************************************************************************
 */

#ifndef NFBLUELINK_H
#define NFBLUELINK_H

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


// Return code for NFM command function
typedef enum NFM_CMD_RET_e
{
    NFM_CMD_SUCCESS_FIRST = 0,
    NFM_CMD_SUCCESS,
    NFM_CMD_SUCCESS_IGNORE,
    NFM_CMD_SUCCESS_ALREADY,
    NFM_CMD_SUCCESS_LAST,

    NFM_CMD_FAIL_FIRST,
    NFM_CMD_FAIL,
    NFM_CMD_FAIL_INVALID_HANDLE,
    NFM_CMD_FAIL_INVALID_CALLBACK,
    NFM_CMD_FAIL_NOT_SUPPORT,
    NFM_CMD_FAIL_NOT_READY,
    NFM_CMD_FAIL_CANCEL,
    NFM_CMD_FAIL_MEMORY_NOT_ENOUGH,
    NFM_CMD_FAIL_DEPENDENCY,
    NFM_CMD_FAIL_STATE,
    NFM_CMD_FAIL_REPEAT,
    NFM_CMD_FAIL_IO,
    NFM_CMD_FAIL_CUS,
    NFM_CMD_FAIL_PARAMETER,
    NFM_CMD_FAIL_RETRY,
    NFM_CMD_FAIL_REMOTE,
    NFM_CMD_FAIL_REGISTER_ERROR,
    NFM_CMD_FAIL_LAST,

    NFM_CMD_STATUS_INITIAL
} NFM_CMD_RET;

// ID of Bluetooth profiles
typedef enum NFM_PROFILE_INDEX_e
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
} NFM_PROFILEINDEX;

typedef enum NFM_PAIRED_SOFT_MODE_e
{
    NFM_PAIREDSOFT_LAST_PAIRED_FIRST = 0,
    NFM_PAIREDSOFT_LAST_CONNECTED_FIRST
}NFM_PAIRED_SOFT_MODE;

// ID of Bluetooth profiles
typedef enum NFM_PROFILEID_e
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
} NFM_PROFILEID;

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
}NFM_SUPPORT_VERSION;

// Category ID of device
typedef enum NFM_MAJORDEVICEID_e
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
} NFM_MAJORDEVICEID;

// Return code for NFM callback function
typedef enum NFM_CBK_RET_e
{
    NFM_CBK_SUCCESS_FIRST = 0,
    NFM_CBK_SUCCESS,
    NFM_CBK_SUCCESS_LAST,

    NFM_CBK_FAIL_FIRST,
    NFM_CBK_FAIL,
    NFM_CBK_FAIL_LAST,

    NFM_CBK_STATUS_INITIAL
} NFM_CBK_RET;

// Return code for NFM_HANDLE
typedef enum NFM_HANDLE_TYPE_e
{
    NFM_HANDLE_TYPE_FAIL = 0,
    NFM_HANDLE_TYPE_SYS,
    NFM_HANDLE_TYPE_APP,
} NFM_HANDLE_TYPE;

typedef void* NFM_HANDLE;

/* BD Address */
typedef struct {
    unsigned char b[BT_ADDR_LEN];
} __attribute__((packed)) BT_ADDR;

// Version of this software
typedef struct NFM_VER_s
{
    unsigned char major;
    unsigned char minor;
    unsigned short build;
} NFM_VER;

// Power
typedef enum NFM_POWER_e
{
    NFM_POWEROFF,
    NFM_POWERON,
    NFM_POWERSUSPEND,
    NFM_POWERUNKNOWN,
} NFM_POWER;

typedef struct NFM_DEV_INFO_e
{
    char        *szName;
    BT_ADDR     btAddr;
}NFM_PAIRED, NFM_DEVICE;

typedef enum NFM_ANSWERPAIR_STATE_e
{
    NFM_ANSWERPAIR_REQUEST,
    NFM_ANSWERPAIR_CONFIRM,
    NFM_ANSWERPAIR_RESULT
} NFM_ANSWERPAIR_STATE;

typedef enum NFM_CONNECTION_STATE_e
{
    NFM_CON_CONNECTION = 0,
    NFM_CON_DISCONNECTION,
    NFM_CON_NOT_SUPPORT,
    NFM_CON_CONNECTING
} NFM_CONNECTION_STATE;

typedef enum NFM_AUTO_CONNECTION_STATE_e
{
    NFM_AUTO_CON_END = 0,
    NFM_AUTO_CON_START
} NFM_AUTO_CONNECTION_STATE;

/*  Bluetooth disconnect reason */
typedef enum NFM_DISCONNECT_REASON_e
{
    NFM_DISCONNECT_REASON_NOT_FIND  = 0x04,
    NFM_DISCONNECT_REASON_LINK_LOSS  = 0x08,
    NFM_DISCONNECT_REASON_REMOTE_USER  = 0x13,
    NFM_DISCONNECT_REASON_REMOTE_POWER_OFF  = 0x15, 
}NFM_DISCONNECT_REASON;

/*  Diagnostic responese */
/*implement to bt Diagnostic event*/
typedef enum NFM_DIAG_CODE_e
{
    NFM_NOERR                       = 0x0,
    NFM_DBG_SYS_A2DP_AUDIO_ERROR,
    NFM_DBG_SYS_A2DP_AUDIO_ACTIVE,
    NFM_DBG_SYS_A2DP_AUDIO_RELEASE,
    NFM_DBG_SYS_RING_AUDIO_ERROR,
    NFM_DBG_SYS_RING_AUDIO_ACTIVE,
    NFM_DBG_SYS_RING_AUDIO_RELEASE,
    NFM_DBG_SYS_SCO_AUDIO_ERROR,
    NFM_DBG_SYS_SCO_AUDIO_ACTIVE,
    NFM_DBG_SYS_SCO_AUDIO_RELEASE,
    NFM_DBG_SYS_SCO_MIC_ERROR,
    NFM_DBG_SYS_SCO_MIC_ACTIVE,
    NFM_DBG_SYS_SCO_MIC_RELEASE,
    NFM_ERR_BT_HW_EXCEPTION,
    NFM_ERR_SYS_AUDIO_DRIVER_ERROR,
}NFM_DIAG_CODE;

/********** States of profiles  **********/

//AVRCP
typedef enum NFM_AVRCP_STATE_e
{
    NFM_AVRCP_EVENT_DISCONNECTED,
    NFM_AVRCP_EVENT_READY,
    NFM_AVRCP_EVENT_CONNECTING,
    NFM_AVRCP_EVENT_CONNECTED,
    NFM_AVRCP_EVENT_RESERVED
} NFM_AVRCP_STATE;

typedef enum NFM_AVRCP_PLAY_STATE_e
{
    NFM_PLAY_STATE_STOP     = 0x00,
    NFM_PLAY_STATE_PLAYING  = 0x01,
    NFM_PLAY_STATE_PAUSED   = 0x02,
    NFM_PLAY_STATE_FWD      = 0x03,
    NFM_PLAY_STATE_REV      = 0x04,
    NFM_PLAY_STATE_ERROR    = 0xFF
} NFM_AVRCP_PLAY_STATE;

typedef enum NFM_AVRCP_PLAY_OPERATION_e
{
    NFM_PLAY_OP_STOP,
    NFM_PLAY_OP_PLAY,
    NFM_PLAY_OP_PAUSE,
    NFM_PLAY_OP_FWD,
    NFM_PLAY_OP_BWD,
    /* NOTE:
     * If one of START operation is sent, then send corresponding STOP before sending any START.
     * Operations above can be sent between START and STOP, but this is not recommended.
     */
    NFM_PLAY_OP_START_FAST_FWD,
    NFM_PLAY_OP_STOP_FAST_FWD,
    NFM_PLAY_OP_START_REV,
    NFM_PLAY_OP_STOP_REV
} NFM_AVRCP_PLAY_OPERATION;

typedef enum NFM_AVRCP_PDUID_e
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
} NFM_AVRCP_PDUID;

// For AVRCP 1.4 browsing
typedef enum NFM_AVRCP_BROWSE_PDUID_e
{
    SET_BROWSED_PLAYER_PDU_ID           /*=112,*/   =(0x70),
    GET_FOLDER_ITEM_PDU_ID              /*=113,*/   =(0x71),
    CHANGE_PATH_PDU_ID                  /*=114,*/   =(0x72),
    GET_ITEM_ATTRIBUTE_PDU_ID           /*=115,*/   =(0x73),
    SEARCH_PDU_ID                       /*=128,*/   =(0x80)
} NFM_AVRCP_BROWSE_PDUID;

typedef enum NFM_AVRCP_SUPPORTED_EVENTS_e {
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
}NFM_AVRCP_SUPPORTED_EVENTS;

/* Player Application Setting Attribute Value*/
typedef enum
{
    ATTRIBUTE_ILLEGAL = 0x00,
    ATTRIBUTE_EQUALIZER,
    ATTRIBUTE_REPEAT_MODE,
    ATTRIBUTE_SHUFFLE,
    ATTRIBUTE_SCAN,
    ATTRIBUTE_RESERVED
}NFM_AVRCP_PLAYER_ATTRIBUTE;

typedef enum
{
    EQUALIZER_OFF = 0x01,
    EQUALIZER_ON,
    EQUALIZER_RESERVED
}NFM_AVRCP_PLAYER_EQUALIZER;

typedef enum
{
    REPEAT_OFF = 0x01,
    REPEAT_SINGLE_TRACK,
    REPEAT_ALL_TRACK,
    REPEAT_GROUP,
    REPEAT_RESERVED
}NFM_AVRCP_PLAYER_REPEAT;

typedef enum
{
    SHUFFLE_OFF = 0x01,
    SHUFFLE_ALL_TRACK,
    SHUFFLE_GROUP,
    SHUFFLE_RESERVED
}NFM_AVRCP_PLAYER_SHUFFLE;

typedef enum
{
    SCAN_OFF = 0x01,
    SCAN_ALL_TRACK,
    SCAN_GROUP,
    SCAN_RESERVED
}NFM_AVRCP_PLAYER_SCAN;

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
}NFM_AVRCP_ELEMENT_ATTRIBUTE;

typedef struct{
    unsigned char attribute_id;     /**< Player Attribute ID */
    unsigned char value_id;         /**< Attribute Value ID */
}NFM_PLAYER_SETTING_ID;

/* Battery Status ID*/
typedef enum
{
    BATT_STATUS_NORMAL      = 0x00,
    BATT_STATUS_WARNING,
    BATT_STATUS_CRITICAL,
    BATT_STATUS_EXTERNAL,
    BATT_STATUS_FULL_CHARGE
}NFM_AVRCP_BATT_STATUS;

/* System Power Status ID*/
typedef enum
{
    AVRCP_POWER_ON          = 0x00,
    AVRCP_POWER_OFF,
    AVRCP_UNPLUGGED,
}NFM_AVRCP_SYS_POWER_STATUS;

/** Attribute of now playing element */
typedef struct{
    const char              *media_attribute;   /**< Element Attribute */
    unsigned short int  charset_id;         /**< Character Set ID */
    char                    *string;            /**< string area (NULL terminated) */
}NFM_NOW_PLAYING_ATTRIBUTE;

/** Attribute of browsed player */
typedef struct{
    unsigned short int      uid_counter;        /**< UID counter */
    unsigned int            item_count;         /**< Item count in current directory */
    unsigned short int      charset_id;         /**< Character set ID */
    unsigned char           folder_depth;       /**< Current directory depth */
    char                    **folder;           /**< string area (NULL terminated) */
}NFM_BROWSED_PLAYER_ATTRIBUTE;

/* Item Entry ID*/
typedef enum
{
    ITEM_ENTRY_HAS_NEXT     = 0x01,
    ITEM_ENTRY_LASY,
}NFM_AVRCP_ITEM_ENTRY;

/* Player Item Type ID*/
typedef enum
{
    PLAYER_ITEM_TYPE_AUDIO              = 0x01,
    PLAYER_ITEM_TYPE_VIDEO              = 0x02,
    PLAYER_ITEM_TYPE_BROADCASTING_AUDIO = 0x04,
    PLAYER_ITEM_TYPE_BROADCASTING_VIDEO = 0x08,
}NFM_PLAYER_ITEM_TYPE;

/* Player Item Sub Type ID*/
typedef enum
{
    PLAYER_ITEM_SUBTYPE_AUDIO_BOOK      = 0x01,
    PLAYER_ITEM_SUBTYPE_PODCAST,
}NFM_PLAYER_ITEM_SUBTYPE;

#define  PLAYER_ITEM_FERTURE_BIT_MASK   16
/** Attribute of player item */
typedef struct{
    NFM_AVRCP_ITEM_ENTRY    last_entry;                                     /**< Notice that is the last entry or not */
    unsigned short int      uid_counter;                                    /**< UID counter */
    unsigned short int      player_id;                                      /**< Player ID */
    NFM_PLAYER_ITEM_TYPE    type_flags;                                     /**< Type */
    NFM_PLAYER_ITEM_SUBTYPE subtype_flags;                                  /**< Sub type */
    NFM_AVRCP_PLAY_STATE    play_status_id;                                 /**< Play status ID */
    unsigned char           feature_bit_mask[PLAYER_ITEM_FERTURE_BIT_MASK]; /**< Player features, fixed in 16 bytes */
    unsigned short int      charset_id;                                     /**< Character set ID */
    char                    *name;                                          /**< Player name */
}NFM_PLAYER_ITEM;

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
}NFM_FOLDER_ITEM_TYPE;


/* Folder Item Playable*/
typedef enum
{
    FOLDER_ITEM_NOT_PLAYABLE            = 0x01,
    FOLDER_ITEM_PLAYABLE,
}NFM_FOLDER_ITEM_PLAYABLE;

/** Attribute of folder item */
typedef struct{
    NFM_AVRCP_ITEM_ENTRY        last_entry;         /**< Notice that is the last entry or not */
    unsigned short int          uid_counter;        /**< UID counter */
    unsigned long long int      uid;                /**< UID */
    NFM_FOLDER_ITEM_TYPE        folder_type;        /**< Type */
    NFM_FOLDER_ITEM_PLAYABLE    is_playable;        /**< Is folder playable */
    unsigned short int          charset_id;         /**< Character set ID */
    char                        *folder;            /**< Directory name */
}NFM_FOLDER_ITEM;

/* Media Item Type ID*/
typedef enum
{
    MEDIA_ITEM_TYPE_AUDIO               = 0x01,
    MEDIA_ITEM_TYPE_VIDEO               = 0x02,
}NFM_MEDIA_ITEM_TYPE;

/** Attribute of media item */
typedef struct{
    NFM_AVRCP_ITEM_ENTRY        last_entry;         /**< Notice that is the last entry or not */
    unsigned short int          uid_counter;        /**< UID counter */
    unsigned long long int      uid;                /**< UID */
    NFM_MEDIA_ITEM_TYPE         media_type;         /**< Type */
    unsigned short int          charset_id;         /**< Character set ID */
    char                        *name;              /**< Media name */
}NFM_MEDIA_ITEM;

/** Direction of change folder */
typedef enum
{
    CHANGE_FOLDER_DIRECTION_UP          = 0x00,
    CHANGE_FOLDER_DIRECTION_DOWN,
}NFM_AVRCP_CHANGE_FOLDER_DIRECTION;

/** Scope of item that can be get */
typedef enum
{
    SCOPE_ID_PLAYER_LIST                    = 0x00,
    SCOPE_ID_PLAYER_VIRTUAL_FILE_SYSTEM,
    /** IDs below is currently not supported by Bluelink */
    SCOPE_ID_SEARCH,
    SCOPE_ID_NOW_PLAYING,
}NFM_FOLDER_ITEM_SCOPE_ID;

/** AVRCP version supported by remote device */
typedef enum
{
    AVRCP_VER_00    = 0x0000, // means no support
    AVRCP_VER_10    = 0x0100,
    AVRCP_VER_13    = 0x0103,
    AVRCP_VER_14    = 0x0104,
    AVRCP_VER_15    = 0x0105
}NFM_AVRCP_SUPPORT_VERSION;

//A2DP
typedef enum NFM_A2DP_STATE_e
{
    NFM_A2DP_STATE_DISCONNECTED,
    NFM_A2DP_STATE_INITIALISING,
    NFM_A2DP_STATE_READY,
    NFM_A2DP_STATE_CONNECTING,
    NFM_A2DP_STATE_SIGNALLING,
    NFM_A2DP_STATE_OPEN,
    NFM_A2DP_STATE_STREAMING,
    NFM_A2DP_STATE_A2DP_PAUSED,
} NFM_A2DP_STATE;

// HFP
typedef struct NFM_HF_MPTY_INFO_e
{
    char            *pwzNumber;
    unsigned char   idx;
    unsigned char   mpty_state;
}NFM_HF_MPTY_INFO;

typedef enum NFM_HANDSFREE_STATE_e
{
    NFM_HF_STATE_DISCONNECTED,
    NFM_HF_STATE_INITIALISING,
    NFM_HF_STATE_READY,
    NFM_HF_STATE_SLCCONNECTING,
    NFM_HF_STATE_SLCCONNECTED,
    NFM_HF_STATE_OUTGOINGCALLESTABLISH,
    NFM_HF_STATE_INCOMINGCALLESTABLISH,
    NFM_HF_STATE_ACTIVECALL,
    NFM_HS_STATE_INITIALISING,
    NFM_HS_STATE_READY,
    NFM_HS_STATE_ACTIVECALL,
    NFM_HANDSFREE_STATE_MAX
} NFM_HANDSFREE_STATE;

typedef enum NFM_HANDSFREE_AUTO_REACTION_e
{
    NFM_HF_AUTO_NO_ACTION,
    NFM_HF_AUTO_ANSWER,
    NFM_HF_AUTO_REJECT
} NFM_HANDSFREE_AUTO_REACTION;

/* Battery Status*/
typedef enum
{
    BATT_STATUS_0       = 0x00,
    BATT_STATUS_1,
    BATT_STATUS_2,
    BATT_STATUS_3,
    BATT_STATUS_4,
    BATT_STATUS_5
}NFM_HANDSFREE_BATT_STATUS;

/* Signal Strength*/
typedef enum
{
    SIGNASL_STRENGTH_LEVEL_0    = 0x00,
    SIGNASL_STRENGTH_LEVEL_1,

    SIGNASL_STRENGTH_LEVEL_2,
    SIGNASL_STRENGTH_LEVEL_3,
    SIGNASL_STRENGTH_LEVEL_4,
    SIGNASL_STRENGTH_LEVEL_5
}NFM_HANDSFREEP_SIGNASL_STRENGTH;

/* Call Held Option*/
typedef enum
{
    CALL_HELD_RELEASE_HELD  = 0x00,     // 0.Releases all held calls or sets User Determined User Busy (UDUB) for a waiting call
    CALL_HELD_RELEASE_ACTS,             // 1.Releases all active calls (if any exist) and accepts the other (held or waiting)
    CALL_HELD_HOLD_CALLS,               // 2.Places all active calls (if any exist) on hold and accepts the other (held or waiting) call
    CALL_HELD_ADD_HELD_CALL,            // 3.Adds a held call to the conversation
    CALL_HELD_RELEASE_SPEC_ACT,         // 1x.Releases specified active call only (<idx>)
    CALL_HELD_REQ_PRIVATE_SPEC_CALL,    // 2x.Request private consultation mode with specified call (<idx>). (Place all calls on hold EXCEPT the call indicated by <idx>.)
}NFM_HANDSFREE_CALLHELD_OPTION;

/* AT command*/
typedef enum
{
    NFM_HF_ANSWER,              // answer call
    NFM_HF_DIAL,                // dial call
    NFM_HF_DTMF,                // DTMF
    NFM_HF_REDIAL,              // re-dial call
    NFM_HF_HANGUP,              // hang up call
    NFM_HF_HOLD,                // hold call
    NFM_HF_SPK_VOL,             // speaker volume
    NFM_HF_MIC_VOL,             // microphone volume
    NFM_HF_VR,                  // Voice Recognition
    NFM_HF_XAPL,                // Apple custom AT commands
    NFM_HF_APLEFM,              // Apple Siri status
    NFM_HF_CGMI,                // request device manufacturer identification
    NFM_HF_CGMM,                // request device manufacturer model identification
    NFM_HS_ANSWER,              // answer call using HF
    NFM_HS_HANGUP,              // hang up call using HF
    NFM_HF_GET_CURRECT_CALL_REJECT = 16
}NFM_HANDSFREEP_AT_COMMAND;

//PBAP
typedef enum NFM_DOWNLOAD_TYPE_e
{
    NFM_DLT_MAIN = -1,
    NFM_DLT_PBAP = 0,
    NFM_DLT_SYNCML,
    NFM_DLT_AT,
    NFM_DLT_MAP,
} NFM_DOWNLOAD_TYPE;

//TODO: rename to NFM_OBEX_TRANSFER_STATE
typedef enum NFM_DOWNLOAD_STATE_e
{
    NFM_LS_END = 0,
    NFM_LS_START,
    NFM_DLS_ING,
    NFM_LS_STOP,
    NFM_ULS_ING,
} NFM_DOWNLOAD_STATE;

typedef enum NFM_PHONEBOOK_LOCATION_e
{
    NFM_PB_LOC_PHONE,
    NFM_PB_LOC_SIM,
    NFM_PB_LOC_SIM2,
} NFM_PHONEBOOK_LOCATION;

typedef enum NFM_PHONE_TYPE_e
{
    NFM_PHONE_PREF = 0,
    NFM_PHONE_WORK,
    NFM_PHONE_HOME,
    NFM_PHONE_VOICE,
    NFM_PHONE_FAX,
    NFM_PHONE_MSG,
    NFM_PHONE_CELL,
    NFM_PHONE_PAGER,
    NFM_PHONE_BBS,
    NFM_PHONE_MODEM,
    NFM_PHONE_CAR,
    NFM_PHONE_ISDN,
    NFM_PHONE_VIDEO,
    NFM_PHONE_NONE
} NFM_PHONE_TYPE;

typedef struct NFM_PHONE_e
{
    char            *phone_number;
    NFM_PHONE_TYPE  *phone_type;
    unsigned int    phone_type_count;
}NFM_PHONE;

typedef enum NFM_PHONEBOOK_TYPE_e
{
    NFM_PB_TYPE_PB,
    NFM_PB_TYPE_MISSED,
    NFM_PB_TYPE_OUTGOING,
    NFM_PB_TYPE_INCOMING,
    NFM_PB_TYPE_COMBINED,
} NFM_PHONEBOOK_TYPE;

typedef struct NFM_PHONEBOOK_DATA_e
{
    NFM_PHONEBOOK_LOCATION  location;
    NFM_PHONEBOOK_TYPE      type;
    unsigned int            max_count;
}NFM_PHONEBOOK_DATA;

typedef enum NFM_PHONE_DIRECTION_e
{
    NFM_PHONE_MISSED,   // missed calls
    NFM_PHONE_RECEIVED, // incoming calls
    NFM_PHONE_DIALED    // outgoing calls
}NFM_PHONE_DIRECTION;

typedef struct NFM_CALL_e
{
    char *dir;  // missed, incoming, outgoing
    char *date; // yyyyymmdd
    char *time; // hhmmss
}NFM_CALL;

typedef struct NFM_CONTACT_INFO_e
{
    char            *name;
    char            *address;
    NFM_PHONE       *phone;
    unsigned int    phone_count;
    char            *photo_data;
    NFM_CALL        call_info;
    char            *fullname;
} NFM_CONTACT_INFO;

// MAP

/* MAP folder structure */
typedef enum
{
    MAP_FOLDER_STRUCTURE_INBOX = 0,
    MAP_FOLDER_STRUCTURE_SENT,
    MAP_FOLDER_STRUCTURE_OUTBOX
}MAP_FOLDER_STRUCT;

typedef struct NFM_MESSAGE_INFO_e
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
} NFM_MESSAGE_INFO;

typedef enum NFM_MESSAGE_LOCATION_e
{
    NFM_MAP_ALL_MESSAGE,
    NFM_MAP_NEW_MESSAGE,
    NFM_MAP_SPECIFIC_MESSAGE
} NFM_MESSAGE_TYPE;

typedef enum NFM_MESSAGE_SETTING_e
{
    NFM_MAP_SETTING_READ,
    NFM_MAP_SETTING_DELETE
} NFM_MESSAGE_SETTING;

typedef enum NFM_MESSAGE_EVENT_TYPE_e
{
    NFM_MESSAGE_EVENT_NEW_MESSAGE,      // NewMessage
    NFM_MESSAGE_EVENT_DELIVERY_SUCCESS, // Delivery Success
                                        // Message has been successfully delivered to its intended recipient.
    NFM_MESSAGE_EVENT_SENDING_SUCCESS,  // Sending Success
                                        // Message has been successfully sent to a remote network.
    NFM_MESSAGE_EVENT_DELIVERY_FAILURE, // DeliveryFailure
                                        // The delivery of a message to its intended recipient failed.
                                        // This event type shall not be applied for emails.
    NFM_MESSAGE_EVENT_SENDING_FAILURE,  // SendingFailure
                                        // Sending to a remote network failed.
    NFM_MESSAGE_EVENT_MEMORY_FULL,      // MemoryFull
    NFM_MESSAGE_EVENT_MEMORY_AVAILABLE, // MemoryAvailable
                                        // Shall receive only if there was a " MemoryFull" event before.
    NFM_MESSAGE_EVENT_MESSAGE_DELETED,  // MessageDeleted
    NFM_MESSAGE_EVENT_MESSAGE_SHIFT,    // MessageShift
                                        // Shifted on the MSE from one folder to another folder.
    NFM_MESSAGE_EVENT_CONNECTION_LOST,  // Connection Lost
    NFM_MESSAGE_EVENT_NOTIFY_SUCCESS,   // Notify register success
    NFM_MESSAGE_EVENT_SENDING_REJECT,   // Sending Reject
    NFM_MESSAGE_EVENT_SENDING_ACCEPT,   // Sending Accept
    NFM_MESSAGE_EVENT_DELETE_REJECT,    // Delete Reject
    NFM_MESSAGE_EVENT_DELETE_ACCEPT,    // Delete Accept
    NFM_MESSAGE_EVENT_READSTATUS_REJECT,// Set read status Reject
    NFM_MESSAGE_EVENT_READSTATUS_ACCEPT,// Set read status Accept

    NFM_MESSAGE_EVENT_INVALID
}NFM_MESSAGE_EVENT_TYPE;

// SPP
typedef enum NFM_SPP_STATE_e
{
    NFM_SPP_STATE_DISCONNECTED,
    NFM_SPP_STATE_CONNECTED
} NFM_SPP_STATE;

// HID
typedef enum NFM_HID_STATE_e
{
    NFM_HID_STATE_DISCONNECTED,
    NFM_HID_STATE_CONNECTED
} NFM_HID_STATE;

// PAN
typedef enum NFM_PAN_STATE_e
{
    NFM_PAN_STATE_DISCONNECTED,
    NFM_PAN_STATE_BUSY,
    NFM_PAN_STATE_CONNECTED
} NFM_PAN_STATE;


////////////////////////////////////////////////////////////////////////////////////////
//6. Callback Event Function
//
////////////////////////////////////////////////////////////////////////////////////////

/* 6.1.1 NFM_CBK_BLUEZSTATUS
 *
 * Association command: nfmCmdStartup
 *
 * When middleware status change, NFM_CBK_BLUEZSTATUS will trigger with following parameter:
 *
 *      bool on             - middleware (initial/terminate)
 *      NFM_CMD_RET cmd_ret - internal execute result
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_BLUEZSTATUS)(void *nfmCbkParam, const bool on, NFM_CMD_RET cmd_ret);

/* 6.1.2 NFM_CBK_POWERSTATUS
 *
 * Association command: nfmCmdSetPower
 *
 * When middleware modify device power setting,
 * NFM_CBK_POWERSTATUS will trigger with following parameter:
 *
 *      NFM_POWER nfmPower  - device power setting
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_POWERSTATUS)(void *nfmCbkParam, const NFM_POWER nfmPower);

/* 6.2.1 NFM_CBK_INQUIRYSTATUS
 *
 * Association command: nfmCmdInquiryStart, nfmCmdInquiryStop
 *
 * NFM_CBK_INQUIRYSTATUS return scan status.
 * If found device, also return device information:
 *
 *      bool scanning       - scanning or not
 *
 *      NFM_DEVICE device   - device information
 *          char *szName    - device name
 *          BT_ADDR btAddr  - device mac address by hex(big endian)
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_INQUIRYSTATUS)(void *nfmCbkParam, const bool scanning, const NFM_DEVICE device);

/* 6.2.2 NFM_CBK_GETPAIREDLIST
 *
 * Association command: nfmCmdGetPairedList
 *
 * NFM_CBK_GETPAIREDLIST return paired list.
 *
 *      NFM_PAIRED *pPairedList - device information array
 *          char *szName    - device name
 *          BT_ADDR btAddr  - device mac address by hex(big endian)
 *
 *      int len - pPairedList length
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_GETPAIREDLIST)(void *nfmCbkParam, const NFM_PAIRED *pPairedList, const int len);

/* 6.2.3 NFM_CBK_REMOTEPAIRREQUEST
 *
 * Association command: nfmCmdPairRequest, nfmCmdAnswerPair, nfmCmdRefusePair
 *
 * NFM_CBK_REMOTEPAIRREQUEST return pair state and pair result:
 *
 *      int pincode                 - indicate remote device pin code when state is NFM_ANSWERPAIR_CONFIRM
 *      NFM_ANSWERPAIR_STATE state  - pair state
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      bool success                - indicate pair result when state is NFM_ANSWERPAIR_RESULT
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_REMOTEPAIRREQUEST)(void *nfmCbkParam, const int pincode, const NFM_ANSWERPAIR_STATE state, const BT_ADDR ba, const bool success);

/* 6.2.4 NFM_CBK_LOCALSSPNUMBER
 *
 * This callback has been deprecated.
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_LOCALSSPNUMBER)(void *nfmCbkParam, const int pincode, const BT_ADDR ba);


/* 6.2.5 NFM_CBK_DIAGOSTICMESSAGE
 *
 * Association command:
 *
 * NFM_CBK_DIAGNOSTICMESSAGE return audio thread status
 *
 *      code - the return code for audio thread status include A2DP, SCO and Ring
 */
typedef NFM_CBK_RET (*NFM_CBK_DIAGNOSTICMESSAGE)(void *nfmCbkParam, NFM_DIAG_CODE code);

/* 6.3.1 NFM_CBK_CONNECTIONSTATUS
 *
 * Association command: nfmCmdConnectRemoteDevice, nfmCmdDisconnectRemoteDevice
 *
 * When profile connection state change
 * NFM_CBK_CONNECTIONSTATUS will trigger with following parameter:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      NFM_PROFILEID ProfileID     - profile unique ID
 *      NFM_CONNECTION_STATE state  - connection state
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_CONNECTIONSTATUS)(void *nfmCbkParam, const BT_ADDR ba, const NFM_PROFILEID ProfileID, const NFM_CONNECTION_STATE state);

/* 6.3.2 NFM_CBK_AUTOCONNECTIONSTATUS
 *
 * Association command: nfmCmdSetRuntimeAutoConnectionEnable
 *
 * When bluelink doing auto connection after power on or linklost and auto connection state change
 * NFM_CBK_AUTOCONNECTIONSTATUS will trigger with following parameter:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      NFM_AUTO_CONNECTION_STATE state - auto connection state
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AUTOCONNECTIONSTATUS)(void *nfmCbkParam, const BT_ADDR ba, const NFM_AUTO_CONNECTION_STATE state);

/*
 * Association command: nfmCmdConnectRemoteDevice, nfmCmdDisconnectRemoteDevice
 *
 * When the link is disconnected, NFM_CBK_DISCONNECTREASON will be triggered with following parameter:
 *
 *      BT_ADDR ba                      -remote device mac address by hex(big endian)
 *      NFM_DISCONNECT_REASON reason    -disconnect reason
 */
typedef NFM_CBK_RET (*NFM_CBK_DISCONNECTREASON)(void *nfmCbkParam, const BT_ADDR ba, const NFM_DISCONNECT_REASON reason);

/* 6.4.1 NFM_CBK_HANDSFREESTATUS
 *
 * This callback has been deprecated.
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREESTATUS)(void *nfmCbkParam, const BT_ADDR ba, const NFM_HANDSFREE_STATE state);

/* 6.4.2 NFM_CBK_HANDSFREECURRENTCALL
 *
 * Association command: Remote device trigger
 *
 * When HANDFREE profile receive a incoming call signal from Bluez
 * NFM_CBK_HANDSFREECURRENTCALL will trigger with following parameter:
 *
 *      BT_ADDR ba          - remote device mac address by hex(big endian)
 *      char *pwzNumber     - incoming call number
 *      unsigned char idx   - line number
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREECURRENTCALL)(void *nfmCbkParam, const BT_ADDR ba, const char *pwzNumber, unsigned char idx);

/* 6.4.3 NFM_CBK_HANDSFREERING
 *
 * Association command: Remote device trigger
 *
 * When HEADSET profile receive ring signal from Bluez
 * NFM_CBK_HANDSFREERING will trigger with following parameter:
 *
 *      BT_ADDR ba  - remote device mac address by hex(big endian)
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREERING)(void *nfmCbkParam, const BT_ADDR ba);

/* 6.4.4 NFM_CBK_HANDSFREECALLSTARTED
 *
 * Association command: nfmCmdDialCall, nfmCmdAnswerCall, nfmCmdHoldCall
 *
 * This callback has been deprecated.
 * NFM_CBK_HANDSFREECALLACTIVE is recommend.
 *
 * When HANDFREE profile start a call
 * NFM_CBK_HANDSFREECALLSTARTED will trigger with following parameter:
 *
 *      BT_ADDR ba          - remote device mac address by hex(big endian)
 *      char *pwzNumber     - incoming call number
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREECALLSTARTED)(void *nfmCbkParam, const BT_ADDR ba, const char *pwzNumber);

/* 6.4.5 NFM_CBK_HANDSFREECALLREJECTED
 *
 * This callback has been deprecated.
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREECALLREJECTED)(void *nfmCbkParam, const BT_ADDR ba);

/* 6.4.6 NFM_CBK_HANDSFREECALLTERMINATED
 *
 * Association command: nfmCmdRejectCall, nfmCmdTerminateCall, nfmCmdHoldCall
 *
 * When HANDFREE profile terminate a call
 * NFM_CBK_HANDSFREECALLTERMINATED will trigger with following parameter:
 *
 *      BT_ADDR ba          - remote device mac address by hex(big endian)
 *      char *pwzNumber     - incoming call number
 *      unsigned char idx   - line number
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREECALLTERMINATED)(void *nfmCbkParam, const BT_ADDR ba, const char *pwzNumber, unsigned char idx);

/* 6.4.7 NFM_CBK_HANDSFREECALLHOLD
 *
 * Association command: nfmCmdHoldCall
 *
 * When HANDFREE profile hold a call
 * NFM_CBK_HANDSFREECALLHOLD will trigger with following parameter:
 *
 *      BT_ADDR ba          - remote device mac address by hex(big endian)
 *      char *pwzNumber     - incoming call number
 *      unsigned char idx   - line number
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREECALLHOLD)(void *nfmCbkParam, const BT_ADDR ba, const char *pwzNumber, unsigned char idx);

/* 6.4.8 NFM_CBK_HANDSFREECALLACTIVE
 *
 * Association command: nfmCmdDialCall, nfmCmdAnswerCall, nfmCmdHoldCall
 *
 * When HANDFREE profile active a call
 * NFM_CBK_HANDSFREECALLACTIVE will trigger with following parameter:
 *
 *      BT_ADDR ba          - remote device mac address by hex(big endian)
 *      char *pwzNumber     - incoming call number
 *      unsigned char idx   - line number
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREECALLACTIVE)(void *nfmCbkParam, const BT_ADDR ba, const char *pwzNumber, unsigned char idx);

/* 6.4.9 NFM_CBK_HANDSFREEMPTYCALLHOLD
 *
 * Association command: nfmCmdHoldCall
 *
 * When HANDFREE profile hold a con-call
 * NFM_CBK_HANDSFREEMPTYCALLHOLD will trigger with following parameter:
 *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *
 *      NFM_HF_MPTY_INFO *pMptyInfo     - con-call members information array
 *          char *pwzNumber             - phone number of member
 *          unsigned char idx           - line number of member
 *          unsigned char mpty_state    - call state, always 0x01(hold)
 *
 *      unsigned char mpty_line_count   - pMptyInfo length
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREEMPTYCALLHOLD)(void *nfmCbkParam, const BT_ADDR ba, const NFM_HF_MPTY_INFO *pMptyInfo, const unsigned char mpty_line_count);

/* 6.4.10 NFM_CBK_HANDSFREEMPTYCALLACTIVE
 *
 * Association command: nfmCmdHoldCall
 *
 * When HANDFREE profile active a con-call
 * NFM_CBK_HANDSFREEMPTYCALLACTIVE will trigger with following parameter:
 *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *
 *      NFM_HF_MPTY_INFO *pMptyInfo     - con-call members information array
 *          char *pwzNumber             - phone number of member
 *          unsigned char idx           - line number of member
 *          unsigned char mpty_state    - call state, always 0x01(hold)
 *
 *      unsigned char mpty_line_count   - pMptyInfo length
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREEMPTYCALLACTIVE)(void *nfmCbkParam, const BT_ADDR ba, const NFM_HF_MPTY_INFO *pMptyInfo, const unsigned char mpty_line_count);

/* 6.4.11 NFM_CBK_HANDSFREEOUTGOINGCALL
 *
 * Association command: nfmCmdDialCall
 *
 * When HANDFREE profile receive a outgoing call signal from Bluez
 * NFM_CBK_HANDSFREEOUTGOINGCALL will trigger with following parameter:
 *
 *      BT_ADDR ba          - remote device mac address by hex(big endian)
 *      char *pwzNumber     - incoming call number
 *      unsigned char idx   - line number
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREEOUTGOINGCALL)(void *nfmCbkParam, const BT_ADDR ba, const char *pwzNumber, unsigned char idx);

/* 6.4.12 NFM_CBK_HANDSFREESCOSTATUS
 *
 * Association command: nfmCmdDialCall, nfmCmdAnswerCall
 *
 * When HANDFREE profile modify SCO status
 * NFM_CBK_HANDSFREESCOSTATUS will trigger with following parameter:
 *
 *      BT_ADDR ba  - remote device mac address by hex(big endian)
 *      bool on     - SCO (connected/disconnected)
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREESCOSTATUS)(void *nfmCbkParam, const BT_ADDR ba, const bool on);

/* 6.4.13 NFM_CBK_HANDSFREEBATTSTATUSCHANGED
 *
 * Association command: Remote device trigger
 *
 * When HANDFREE profile receive a battery status changed signal from Bluez
 * NFM_CBK_HANDSFREEBATTSTATUSCHANGED will trigger with following parameter:
 *
 *      BT_ADDR ba                          - remote device mac address by hex(big endian)
 *      NFM_HANDSFREE_BATT_STATUS status    - battery status, 0x00 for almost discharged, 0x05 for full charged
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREEBATTSTATUSCHANGED)(void *nfmCbkParam, const BT_ADDR ba, const NFM_HANDSFREE_BATT_STATUS status);

/* 6.4.14 NFM_CBK_HANDSFREESIGNALSTRENGTH
 *
 * Association command: Remote device trigger
 *
 * When HANDFREE profile receive a signal strength level changed signal from Bluez
 * NFM_CBK_HANDSFREESIGNALSTRENGTH will trigger with following parameter:
 *
 *      BT_ADDR ba                                  - remote device mac address by hex(big endian)
 *      NFM_HANDSFREEP_SIGNASL_STRENGTH strength    - signal strength
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREESIGNALSTRENGTH)(void *nfmCbkParam, const BT_ADDR ba, const NFM_HANDSFREEP_SIGNASL_STRENGTH strength);

/* 6.4.15 NFM_CBK_HANDSFREESPKVOL
 *
 * Association command: nfmCmdSetSpeakerVolume
 *
 * When remote device has been change speaker volume
 * NFM_CBK_HANDSFREESPKVOL will trigger with following parameter:
 *
 *      BT_ADDR ba              - remote device mac address by hex(big endian)
 *      unsigned char volume    - speaker volume
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREESPKVOL)(void *nfmCbkParam, const BT_ADDR ba, const unsigned char volume);

/* 6.4.16 NFM_CBK_HANDSFREEMICVOL
 *
 * Association command: nfmCmdSetMicrophoneVolume
 *
 * When remote device has been change microphone volume
 * NFM_CBK_HANDSFREEMICVOL will trigger with following parameter:
 *
 *      BT_ADDR ba              - remote device mac address by hex(big endian)
 *      unsigned char volume    - microphone volume
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREEMICVOL)(void *nfmCbkParam, const BT_ADDR ba, const unsigned char volume);

/* 6.4.17 NFM_CBK_HANDSFREECOMMANDRESPONSE
 *
 * Association command: most of HANDFREE/HEADSET profile command
 *
 * This callback type implement two callback: HandsfreeCommandOK, HandsfreeCommandFail
 *
 * NFM_CBK_HANDSFREECOMMANDRESPONSE return HANDFREE/HEADSET profile command result:
 *
 *      BT_ADDR ba                          - remote device mac address by hex(big endian)
 *      NFM_HANDSFREEP_AT_COMMAND command   - command op_code
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREECOMMANDRESPONSE)(void *nfmCbkParam, const BT_ADDR ba, const NFM_HANDSFREEP_AT_COMMAND command);

/* 6.4.18 NFM_CBK_HANDSFREEMANUFACTURERID
 *
 * Association command: nfmCmdGetDevManufacturerID
 *
 * When HANDFREE profile request the manufacturer ID of device and receive the manufacturer ID from connected AG.
 * NFM_CBK_HANDSFREEMANUFACTURERID will trigger with following parameter:
 *
 *      BT_ADDR ba          - remote device mac address by hex(big endian)
 *      char *manufacturerID- manufacturer identification
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREEMANUFACTURERID)(void *nfmCbkParam, const BT_ADDR ba, const char *manufacturerID);

/* 6.4.19 NFM_CBK_HANDSFREEMANUFACTURERMODELID
 *
 * Association command: nfmCmdGetDevManufacturerModelID
 *
 * When HANDFREE profile request the manufacturer model ID of device and receive the manufacturer model ID from connected AG.
 * NFM_CBK_HANDSFREEMANUFACTURERMODELID will trigger with following parameter:
 *
 *      BT_ADDR ba          - remote device mac address by hex(big endian)
 *      char *manufacturerModelID- manufacturer model identification
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREEMANUFACTURERMODELID)(void *nfmCbkParam, const BT_ADDR ba, const char *manufacturerModelID);

/* 6.4.20 NFM_CBK_HANDSFREEMPTYCALLTERMINATED
 *
 * Association command: nfmCmdRejectCall, nfmCmdTerminateCall, nfmCmdHoldCall
 *
 * When HANDFREE profile terminate the multi-party
 * *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREEMPTYCALLTERMINATED)(void *nfmCbkParam, const BT_ADDR ba);

/* 6.4.21 NFM_CBK_HANDSFREESERVICESTATE
*/
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREESERVICESTATE)(void *nfmCbkParam, const BT_ADDR ba, const unsigned char status);

/* 6.4.22 NFM_CBK_HANDSFREESSUBSCRIBERINFO
*/
typedef NFM_CBK_RET (*NFM_CBK_HANDSFREESUBSCRIBERINFO)(void *nfmCbkParam, const BT_ADDR ba, const char *pwzNumber);

    
/* 6.5.1 NFM_CBK_AVRCPSTATUS
 *
 * This callback has been deprecated.
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPSTATUS)(void *nfmCbkParam, const BT_ADDR ba, const NFM_AVRCP_STATE state);

/* 6.5.2 NFM_CBK_AVRCPRESPONSE
 *
 * Association command: AVRCP 1.0 command
 *
 * NFM_CBK_AVRCPRESPONSE return AVRCP 1.0 command result:
 *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *      NFM_AVRCP_PLAY_OPERATION opCode - command op_code
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPRESPONSE)(void *nfmCbkParam, const BT_ADDR ba, const char *response, const NFM_AVRCP_PLAY_OPERATION opCode);

/* 6.5.3 NFM_CBK_AVRCPGETCAPABILITY
 *
 * Association command: nfmCmdGetCapability
 *
 * When nfmCmdGetCapability result arrive,
 * NFM_CBK_AVRCPGETCAPABILITY will trigger with following parameter:
 *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *      unsigned char capability_count  - amount of capabilities
 *      unsigned char eventID[]         - capability ID
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPGETCAPABILITY)(void *nfmCbkParam, const BT_ADDR ba, const unsigned char capability_count, const unsigned char eventID[]);

/* 6.5.4 NFM_CBK_AVRCPPLAYERSETTINGATTR
 *
 * Association command: nfmCmdGetPlayerSettingAttributes
 *
 * When nfmCmdGetPlayerSettingAttributes result arrive,
 * NFM_CBK_AVRCPPLAYERSETTINGATTR will trigger with following parameter:
 *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *      unsigned char attribute_count   - amount of attributes
 *      unsigned char attributeID[]     - attribute ID
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPPLAYERSETTINGATTR)(void *nfmCbkParam, const BT_ADDR ba, const unsigned char attribute_count, const unsigned char attributeID[]);

/* 6.5.5 NFM_CBK_AVRCPPLAYERSETTINGVALUE
 *
 * Association command: nfmCmdGetPlayerSettingValues
 *
 * When nfmCmdGetPlayerSettingValues result arrive,
 * NFM_CBK_AVRCPPLAYERSETTINGVALUE will trigger with following parameter:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      unsigned char value_count   - amount of values
 *      unsigned char valueID[]     - value ID
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPPLAYERSETTINGVALUE)(void *nfmCbkParam, const BT_ADDR ba, const unsigned char value_count, const unsigned char valueID[]);

/* 6.5.6 NFM_CBK_AVRCPGETCURRENTSETTING
 *
 * Association command: nfmCmdGetCurrentPlayerAttributeValues
 *
 * When nfmCmdGetCurrentPlayerAttributeValues result arrive,
 * NFM_CBK_AVRCPGETCURRENTSETTING will trigger with following parameter:
 *
 *      BT_ADDR ba                              - remote device mac address by hex(big endian)
 *      unsigned char setting_count             - amount of attributes
 *      NFM_PLAYER_SETTING_ID player_setting[]  - attribute information
 *          unsigned char attribute_id          - attribute ID
 *          unsigned char value_id              - current value ID
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPGETCURRENTSETTING)(void *nfmCbkParam, const BT_ADDR ba, const unsigned char setting_count, const NFM_PLAYER_SETTING_ID player_setting[]);

/* 6.5.7 NFM_CBK_AVRCPSETSUCCESS
 *
 * Association command: nfmCmdSetCurrentPlayerAttributeValues
 *
 * When nfmCmdSetCurrentPlayerAttributeValues fail,
 * NFM_CBK_AVRCPCOMMANDERROR(6.5.20) will trigger.
 *
 * When nfmCmdSetCurrentPlayerAttributeValues success,
 * NFM_CBK_AVRCPSETSUCCESS will trigger with following parameter:
 *
 *      BT_ADDR ba  - remote device mac address by hex(big endian)
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPSETSUCCESS)(void *nfmCbkParam, const BT_ADDR ba);

/* 6.5.8 NFM_CBK_AVRCPNOWPLAYING
 *
 * Association command: nfmCmdGetNowPlayingAttributes
 *
 * When nfmCmdGetNowPlayingAttributes result arrive,
 * NFM_CBK_AVRCPNOWPLAYING will trigger with following parameter:
 *
 *      BT_ADDR ba                              - remote device mac address by hex(big endian)
 *      unsigned char attribute_count           - amount of metadata attributes
 *      NFM_NOW_PLAYING_ATTRIBUTE attributes[]  - attribute information
 *          char *media_attribute               - attribute name
 *          unsigned short int charset_id       - character set ID
 *          char *string                        - attribute context
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPNOWPLAYING)(void *nfmCbkParam, const BT_ADDR ba, const unsigned char attribute_count, const NFM_NOW_PLAYING_ATTRIBUTE attributes[]);

/* 6.5.9 NFM_CBK_AVRCPPLAYSTATUS
 *
 * Association command: nfmCmdGetPlayStatus
 *
 * When nfmCmdGetPlayStatus result arrive,
 * NFM_CBK_AVRCPPLAYSTATUS will trigger with following parameter:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      char *song_length           - current media duration
 *      char *song_position         - current media position
 *      NFM_AVRCP_PLAY_STATE state  - current media state
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPPLAYSTATUS)(void *nfmCbkParam, const BT_ADDR ba, const char *song_length, const char *song_position, const NFM_AVRCP_PLAY_STATE state);

/* 6.5.10 NFM_CBK_AVRCPREGISTERFAILED
 *
 * Association command: nfmCmdRegisterEventWatcher
 *
 * If AVRCP register command fail,
 * NFM_CBK_AVRCPREGISTERFAILED will trigger with following parameter:
 *
 *      BT_ADDR ba                          - remote device mac address by hex(big endian)
 *      NFM_AVRCP_SUPPORTED_EVENTS PduId    - event ID witch register fail
 *      char *reason                        - error message
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPREGISTERFAILED)(void *nfmCbkParam, const BT_ADDR ba, const NFM_AVRCP_SUPPORTED_EVENTS PduId, const char *reason);

/* 6.5.11 NFM_CBK_EVENT_PLAYBACKSTATUSCHANGED
 *
 * Association command: nfmCmdRegisterEventWatcher, nfmCmdUnregisterEventWatcher
 *
 * If remote device playback status changed and EVENT_PLAYBACK_STATUS_CHANGED(0x01) registered,
 * NFM_CBK_EVENT_PLAYBACKSTATUSCHANGED will trigger with following parameter:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      NFM_AVRCP_PLAY_STATE state  - playback status ID
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_EVENT_PLAYBACKSTATUSCHANGED)(void *nfmCbkParam, const BT_ADDR ba, const NFM_AVRCP_PLAY_STATE state);

/* 6.5.11 NFM_CBK_EVENT_PLAYSTATUSPREFER
 *
 * Association command: nfmCmdRegisterEventWatcher, nfmCmdUnregisterEventWatcher
 *
 * If remote device playback status changed and EVENT_PLAYBACK_STATUS_CHANGED(0x01) registered,
 * NFM_CBK_EVENT_PLAYSTATUSPREFER will trigger with following parameter:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      NFM_AVRCP_PLAY_STATE state  - playback status ID
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_EVENT_PLAYSTATUSPREFER)(void *nfmCbkParam, const BT_ADDR ba, const NFM_AVRCP_PLAY_STATE state);

/* 6.5.12 NFM_CBK_EVENT_TRACKCHANGED
 *
 * Association command: nfmCmdRegisterEventWatcher, nfmCmdUnregisterEventWatcher
 *
 * If remote device has been changed player track and EVENT_TRACK_CHANGED(0x02) registered,
 * NFM_CBK_EVENT_TRACKCHANGED will trigger with following parameter:
 *
 *      BT_ADDR ba                          - remote device mac address by hex(big endian)
 *      unsigned long long int element_id   - track ID
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_EVENT_TRACKCHANGED)(void *nfmCbkParam, const BT_ADDR ba, const unsigned long long int element_id);

/* 6.5.13 NFM_CBK_EVENT_TRACKREACHEDEND
 *
 * Association command: nfmCmdRegisterEventWatcher, nfmCmdUnregisterEventWatcher
 *
 * If remote device reach player track end and EVENT_TRACK_REACHED_END(0x03) registered,
 * NFM_CBK_EVENT_TRACKREACHEDEND will trigger with following parameter:
 *
 *      BT_ADDR ba      - remote device mac address by hex(big endian)
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_EVENT_TRACKREACHEDEND)(void *nfmCbkParam, const BT_ADDR ba);

/* 6.5.14 NFM_CBK_EVENT_TRACKREACHEDSTART
 *
 * Association command: nfmCmdRegisterEventWatcher, nfmCmdUnregisterEventWatcher
 *
 * If remote device reach player track start and EVENT_TRACK_REACHED_START(0x04) registered,
 * NFM_CBK_EVENT_TRACKREACHEDSTART will trigger with following parameter:
 *
 *      BT_ADDR ba      - remote device mac address by hex(big endian)
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_EVENT_TRACKREACHEDSTART)(void *nfmCbkParam, const BT_ADDR ba);

/* 6.5.15 NFM_CBK_EVENT_PLAYBACKPOSCHANGED
 *
 * Association command: nfmCmdRegisterEventWatcher, nfmCmdUnregisterEventWatcher
 *
 * If EVENT_PLAYBACK_POS_CHANGED(0x05) registered and
 * remote device player position shift a interval witch setting by nfmCmdRegisterEventWatcher,
 * NFM_CBK_EVENT_PLAYBACKPOSCHANGED will trigger with following parameter:
 *
 *      BT_ADDR ba          - remote device mac address by hex(big endian)
 *      char *current_pos   - currect position
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_EVENT_PLAYBACKPOSCHANGED)(void *nfmCbkParam, const BT_ADDR ba, const char *current_pos);

/* 6.5.16 NFM_CBK_EVENT_BATTSTATUSCHANGED
 *
 * Association command: nfmCmdRegisterEventWatcher, nfmCmdUnregisterEventWatcher
 *
 * If remote device change the battery status and EVENT_BATT_STATUS_CHANGED(0x06) registered,
 * NFM_CBK_EVENT_BATTSTATUSCHANGED will trigger with following parameter:
 *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *      NFM_AVRCP_BATT_STATUS status    - battery status
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_EVENT_BATTSTATUSCHANGED)(void *nfmCbkParam, const BT_ADDR ba, const NFM_AVRCP_BATT_STATUS status);

/* 6.5.17 NFM_CBK_EVENT_SYSTEMSTATUSCHANGED
 *
 * Association command: nfmCmdRegisterEventWatcher, nfmCmdUnregisterEventWatcher
 *
 * If remote device change the system power status and EVENT_SYSTEM_STATUS_CHANGED(0x07) registered,
 * NFM_CBK_EVENT_SYSTEMSTATUSCHANGED will trigger with following parameter:
 *
 *      BT_ADDR ba                          - remote device mac address by hex(big endian)
 *      NFM_AVRCP_SYS_POWER_STATUS status   - system power status
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_EVENT_SYSTEMSTATUSCHANGED)(void *nfmCbkParam, const BT_ADDR ba, const NFM_AVRCP_SYS_POWER_STATUS status);

/* 6.5.18 NFM_CBK_EVENT_PLAYERSETTINGCHANGED
 *
 * Association command: nfmCmdRegisterEventWatcher, nfmCmdUnregisterEventWatcher
 *
 * If remote device change the player setting and EVENT_PLAYER_APP_SETTING_CHANGED(0x08) registered,
 * NFM_CBK_EVENT_PLAYERSETTINGCHANGED will trigger with following parameter:
 *
 *      BT_ADDR ba                              - remote device mac address by hex(big endian)
 *      unsigned char setting_count             - amount of setting
 *      NFM_PLAYER_SETTING_ID player_setting[]  - player setting array
 *          unsigned char attribute_id          - attribute ID
 *          unsigned char value_id              - value ID
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_EVENT_PLAYERSETTINGCHANGED)(void *nfmCbkParam, const BT_ADDR ba, const unsigned char setting_count, const NFM_PLAYER_SETTING_ID player_setting[]);

/* 6.5.19 NFM_CBK_EVENT_UIDSCHANGED
 *
 * Association command: nfmCmdRegisterEventWatcher, nfmCmdUnregisterEventWatcher
 *
 * If remote device change the UIDs and EVENT_UIDS_CHANGED(0x0C) registered,
 * NFM_CBK_EVENT_UIDSCHANGED will trigger with following parameter:
 *
 *      BT_ADDR ba                          - remote device mac address by hex(big endian)
 *      unsigned short int uid_counter      - UID counter
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_EVENT_UIDSCHANGED)(void *nfmCbkParam, const BT_ADDR ba, const unsigned short int uid_counter);

/* 6.5.20 NFM_CBK_AVRCPCOMMANDERROR
 *
 * Association command: AVRCP 1.3, 1.4 command
 *
 * NFM_CBK_AVRCPCOMMANDERROR will trigger when AVRCP 1.3, 1.4 command fail:
 *
 *      BT_ADDR ba              - remote device mac address by hex(big endian)
 *      NFM_AVRCP_PDUID PduId   - command PDU ID
 *      char *reason            - error message
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPCOMMANDERROR)(void *nfmCbkParam, const BT_ADDR ba, const NFM_AVRCP_PDUID PduId, const char* reason);

/* 6.5.21 NFM_CBK_AVRCPABORTEDRECEIVING
 *
 * Association command: AVRCP 1.3, 1.4 command, nfmCmdAbortReceivingResponse
 *
 * When Bluez abort to receive AVRCP 1.3, 1.4 command result,
 * NFM_CBK_AVRCPABORTEDRECEIVING will trigger with following parameter:
 *
 *      BT_ADDR ba              - remote device mac address by hex(big endian)
 *      NFM_AVRCP_PDUID PduId   - command PDU ID
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPABORTEDRECEIVING)(void *nfmCbkParam, const BT_ADDR ba, const NFM_AVRCP_PDUID PduId);

/* 6.5.22 NFM_CBK_AVRCPPLAYITEMSUCCESS
 *
 * Association command: nfmCmdPlayItem
 *
 * When nfmCmdPlayItem success,
 * NFM_CBK_AVRCPPLAYITEMSUCCESS will trigger with following parameter:
 *
 *      BT_ADDR ba      - remote device mac address by hex(big endian)
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPPLAYITEMSUCCESS)(void *nfmCbkParam, const BT_ADDR ba);

/* 6.5.23 NFM_CBK_AVRCPADDNOWPLAYINGSUCCESS
 *
 * Association command: nfmCmdAddToNowPlaying
 *
 * When nfmCmdAddToNowPlaying success,
 * NFM_CBK_AVRCPADDNOWPLAYINGSUCCESS will trigger with following parameter:
 *
 *      BT_ADDR ba      - remote device mac address by hex(big endian)
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPADDNOWPLAYINGSUCCESS)(void *nfmCbkParam, const BT_ADDR ba);

/* 6.5.24 NFM_CBK_AVRCPBROWSEDPLAYER
 *
 * Association command: nfmCmdAVRCPSetBrowsedPlayer
 *
 * When nfmCmdAVRCPSetBrowsedPlayer result arrive,
 * NFM_CBK_AVRCPBROWSEDPLAYER will trigger with following parameter:
 *
 *      BT_ADDR ba                                  - remote device mac address by hex(big endian)
 *      NFM_BROWSED_PLAYER_ATTRIBUTE browsed_player - browse information
 *          unsigned short int uid_counter          - UID counter
 *          unsigned int item_count                 - amount of item
 *          unsigned short int charset_id           - character set ID
 *          unsigned char folder_depth              - folder depth
 *          char **folder                           - folder name array
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPBROWSEDPLAYER)(void *nfmCbkParam, const BT_ADDR ba, const NFM_BROWSED_PLAYER_ATTRIBUTE browsed_player);

/* 6.5.25 NFM_CBK_AVRCPMEDIAPLAYERITEM
 *
 * Association command: nfmCmdAVRCPGetFolderItems, nfmCmdAVRCPGetFolderItemsSectional
 *
 * If target folder has player item,
 * each player item will trigger NFM_CBK_AVRCPMEDIAPLAYERITEM with following parameter:
 *
 *      BT_ADDR ba                                  - remote device mac address by hex(big endian)
 *      NFM_PLAYER_ITEM player_item                 - item information
 *          NFM_AVRCP_ITEM_ENTRY last_entry         - indicate this item is last item or not
 *          unsigned short int uid_counter          - UID counter
 *          unsigned short int player_id            - player ID
 *          NFM_PLAYER_ITEM_TYPE type_flags         - item type
 *          NFM_PLAYER_ITEM_SUBTYPE subtype_flags   - item sub-type
 *          NFM_AVRCP_PLAY_STATE play_status_id     - play status ID
 *          unsigned char feature_bit_mask[]        - feature mask(little endian)
 *          unsigned short int charset_id           - character set ID
 *          char *name                              - player name
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPMEDIAPLAYERITEM)(void *nfmCbkParam, const BT_ADDR ba, const NFM_PLAYER_ITEM player_item);

/* 6.5.26 NFM_CBK_AVRCPFOLDERITEM
 *
 * Association command: nfmCmdAVRCPGetFolderItems, nfmCmdAVRCPGetFolderItemsSectional
 *
 * If target folder has folder item,
 * each folder item will trigger NFM_CBK_AVRCPFOLDERITEM with following parameter:
 *
 *      BT_ADDR ba                                  - remote device mac address by hex(big endian)
 *      NFM_FOLDER_ITEM folder_item                 - item information
 *          NFM_AVRCP_ITEM_ENTRY last_entry         - indicate this item is last item or not
 *          unsigned short int uid_counter          - UID counter
 *          unsigned long long int uid              - UID
 *          NFM_FOLDER_ITEM_TYPE folder_type        - folder type
 *          NFM_FOLDER_ITEM_PLAYABLE is_playable    - indicate this folder is playable or not
 *          unsigned short int charset_id           - character set ID
 *          char *folder                            - folder name
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPFOLDERITEM)(void *nfmCbkParam, const BT_ADDR ba, const NFM_FOLDER_ITEM folder_item);

/* 6.5.27 NFM_CBK_AVRCPMEDIAITEM
 *
 * Association command: nfmCmdAVRCPGetFolderItems, nfmCmdAVRCPGetFolderItemsSectional
 *
 * If target folder has media item,
 * each media item will trigger NFM_CBK_AVRCPMEDIAITEM with following parameter:
 *
 *      BT_ADDR ba                              - remote device mac address by hex(big endian)
 *
 *      NFM_MEDIA_ITEM media_item               - item information
 *          NFM_AVRCP_ITEM_ENTRY last_entry     - indicate this item is last item or not
 *          unsigned short int uid_counter      - UID counter
 *          unsigned long long int uid          - UID
 *          NFM_MEDIA_ITEM_TYPE media_type      - meida type
 *          unsigned short int charset_id       - character set ID
 *          char *name                          - media name
 *
 *      unsigned char attribute_count           - amount of attribute
 *
 *      NFM_NOW_PLAYING_ATTRIBUTE attribute[]   - attribute information
 *          const char *media_attribute         - attribute name
 *          unsigned short int charset_id       - character set ID
 *          char *string                        - attribute context
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPMEDIAITEM)(void *nfmCbkParam, const BT_ADDR ba, const NFM_MEDIA_ITEM media_item, const unsigned char attribute_count, NFM_NOW_PLAYING_ATTRIBUTE attribute[]);

/* 6.5.28 NFM_CBK_AVRCPGETFOLDERITEMSSUCCESS
 *
 * Association command: nfmCmdAVRCPGetFolderItems, nfmCmdAVRCPGetFolderItemsSectional
 *
 * If get AVRCP get folder command success,
 * NFM_CBK_AVRCPGETFOLDERITEMSSUCCESS will trigger with following parameter:
 *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *      unsigned short int uid_counter  - UID counter
 *      unsigned int total_item_count   - amount of total item
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPGETFOLDERITEMSSUCCESS)(void *nfmCbkParam, const BT_ADDR ba, const unsigned short int uid_counter, const unsigned int total_item_count);

/* 6.5.29 NFM_CBK_AVRCPCHANGEPATHSUCCESS
 *
 * Association command: nfmCmdAVRCPChangePath
 *
 * When nfmCmdAVRCPChangePath success,
 * NFM_CBK_AVRCPCHANGEPATHSUCCESS will trigger with following parameter:
 *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *      unsigned short int item_count   - amount of item in folder
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPCHANGEPATHSUCCESS)(void *nfmCbkParam, const BT_ADDR ba, const unsigned short int item_count);

/* 6.5.30 NFM_CBK_AVRCPBROWSINGERROR
 *
 * Association command: AVRCP 1.4 command
 *
 * If AVRCP 1.4 command fail,
 * NFM_CBK_AVRCPBROWSINGERROR will trigger with following parameter:
 *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *      NFM_AVRCP_BROWSE_PDUID PduId    - command PDU ID
 *      char *reason                    - error message
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_AVRCPBROWSINGERROR)(void *nfmCbkParam, const BT_ADDR ba, const NFM_AVRCP_BROWSE_PDUID PduId, const char* reason);

/* 6.5.31 NFM_CBK_A2DPSTATUS
 *
 * This callback has been deprecated.
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_A2DPSTATUS)(void *nfmCbkParam, const BT_ADDR ba, const NFM_A2DP_STATE state);

/* 6.5.32 NFM_CBK_A2DPSTREAMSTATUS
 *
 * Association command: Remote device trigger
 *
 * When BlueZ A2DP stream status change,
 * NFM_CBK_A2DPSTREAMSTATUS will trigger with following parameter:
 *
 *      BT_ADDR ba      - remote device mac address by hex(big endian)
 *      bool on         - A2DP stream (on/off)
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_A2DPSTREAMSTATUS)(void *nfmCbkParam, const BT_ADDR ba, const bool on);

/* 6.6.1 NFM_CBK_PHONEBOOKDOWNLOADSTATUS
 *
 * Association command: most of PBAP profile command
 *
 * NFM_CBK_PHONEBOOKDOWNLOADSTATUS return PBAP download status:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      NFM_DOWNLOAD_STATE state    - download state
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_PHONEBOOKDOWNLOADSTATUS)(void *nfmCbkParam, const BT_ADDR ba, const NFM_DOWNLOAD_STATE state);

/* 6.6.2 NFM_CBK_DOWNLOADCONTACT
 *
 * Association command: nfmCmdDownload(NoPhoto)PhonebookStart, nfmCmdDownload(NoPhoto)ContactsStart
 *
 * NFM_CBK_DOWNLOADCONTACT return PBAP phone book context:
 *
 *      BT_ADDR ba                              - remote device mac address by hex(big endian)
 *
 *      NFM_CONTACT_INFO contact[]              - phone book element array
 *          char *name                          - name
 *          char *address                       - address
 *          NFM_PHONE *phone                    - phone information array
 *              char *phone_number              - phone number
 *              NFM_PHONE_TYPE *phone_type      - phone types array
 *              unsigned int phone_type_count   - phone types array length
 *          unsigned int phone_count            - phone information array length
 *          char *photo_data                    - photo data(path)
 *          NFM_CALL call_info                  - call information
 *              char *dir                       - call direction
 *              char *date                      - record data(yyyyymmdd)
 *              char *time                      - record time(hhmmss)
 *          char *fullname                      - full name
 *
 *      size_t contact_len                      - phone book element array length
 *
 *      NFM_PHONEBOOK_DATA data                 - phone book information
 *          NFM_PHONEBOOK_LOCATION location     - phone book location
 *          NFM_PHONEBOOK_TYPE type             - phone book type
 *
 *      NFM_CMD_RET cmd_ret                     - download result
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_DOWNLOADCONTACT)(void *nfmCbkParam, const BT_ADDR ba, const NFM_CONTACT_INFO contact[], const size_t contact_len, const NFM_PHONEBOOK_DATA data, NFM_CMD_RET cmd_ret);

/* 6.6.3 NFM_CBK_CONTACTCOUNT
 *
 * Association command: nfmCmdDownload(NoPhoto)PhonebookStart, nfmCmdDownload(NoPhoto)ContactsStart
 *
 * NFM_CBK_CONTACTCOUNT return amount of content before download:
 *
 *      BT_ADDR ba                          - remote device mac address by hex(big endian)
 *      unsigned short int contact_size     - amount of content in phone book
 *      NFM_PHONEBOOK_DATA data             - phone book information
 *          NFM_PHONEBOOK_LOCATION location - phone book location
 *          NFM_PHONEBOOK_TYPE type         - phone book type
 *      NFM_CMD_RET cmd_ret                 - result of blueZ get size method
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_CONTACTCOUNT)(void *nfmCbkParam, const BT_ADDR ba, const unsigned short int contact_size, const NFM_PHONEBOOK_DATA data, NFM_CMD_RET cmd_ret);

/* 6.7.1 NFM_CBK_MESSAGEDOWNLOADSTATUS
 *
 * Association command: most of MAP profile command
 *
 * NFM_CBK_MESSAGEDOWNLOADSTATUS return MAP download status:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      NFM_DOWNLOAD_STATE state    - download state
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_MESSAGEDOWNLOADSTATUS)(void *nfmCbkParam, const BT_ADDR ba, const NFM_DOWNLOAD_STATE state);

/* 6.7.2 NFM_CBK_DOWNLOADMESSAGE
 *
 * Association command: nfmCmdDownloadMessageStart, nfmCmdDownloadSentStart, nfmCmdMessageStartNotify, nfmCmdDownloadSpecificMsgStart
 *
 * NFM_CBK_DOWNLOADMESSAGE return remote device message:
 *
 *      BT_ADDR ba                      - remote device mac address by hex(big endian)
 *      bool is_new_msg                 - indicate this message is new or not
 *
 *      NFM_MESSAGE_INFO message[]      - message element array
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
 *          char *handle                - message handle ID for nfmCmdSetMessageProperty, nfmCmdDownloadSpecificMsgStart
 *          char *folder                - indicate the folder that the message is in (folder: inbox/sent)
 *
 *      size_t message_len              - message element array length
 *      NFM_CMD_RET cmd_ret             - download result
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_DOWNLOADMESSAGE)(void *nfmCbkParam, const BT_ADDR ba, const bool is_new_msg, const NFM_MESSAGE_INFO message[], const size_t message_len, NFM_CMD_RET cmd_ret);

/* 6.7.3 NFM_CBK_MESSAGENOTIFY
 *
 * Association command: nfmCmdMessageStartNotify, nfmCmdMessageStopNotify
 *
 * If remote device send/receive message and MAP has been registered
 * NFM_CBK_MESSAGENOTIFY will trigger with following parameter:
 *
 *      BT_ADDR ba                          - remote device mac address by hex(big endian)
 *      NFM_MESSAGE_EVENT_TYPE event_type   - message notify event type
 *      char *handle                        - message handle ID for nfmCmdSetMessageProperty, nfmCmdDownloadSpecificMsgStart
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_MESSAGENOTIFY)(void *nfmCbkParam, const BT_ADDR ba, const NFM_MESSAGE_EVENT_TYPE event_type, char *handle);

/* 6.8.1 NFM_CBK_SPPINPUT
 *
 * Association command: Remote device trigger
 *
 * When SPP or SPP_IAP profile receive input,
 * NFM_CBK_SPPSTREAMINPUT will trigger with following parameter:
 *
 *      BT_ADDR ba      - remote device mac address by hex(big endian)
 *      char input      - SPP input by character
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_SPPINPUT)(void *nfmCbkParam, const BT_ADDR ba, const char input, const NFM_PROFILEID ProfileID);

/* 6.8.2 NFM_CBK_SPPSTREAMINPUT
 *
 * Association command: Remote device trigger
 *
 * When SPP or SPP_IAP profile receive input,
 * NFM_CBK_SPPSTREAMINPUT will trigger with following parameter:
 *
 *      BT_ADDR ba      - remote device mac address by hex(big endian)
 *      char *input     - SPP input by string
 *      size_t len      - string length
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_SPPSTREAMINPUT)(void *nfmCbkParam, const BT_ADDR ba, const char *input, size_t len, const NFM_PROFILEID ProfileID);

/* 6.9.1 NFM_CBK_HIDCALIBRATION
 *
 * This callback has been deprecated.
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_HIDCALIBRATION)(void *nfmCbkParam, const BT_ADDR ba, const bool success);

/* 6.10.1 NFM_CBK_OBJECTDOWNLOADSTATUS
 *
 * Association command: most of OPP profile command
 *
 * NFM_CBK_OBJECTDOWNLOADSTATUS return OPP download status:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      NFM_DOWNLOAD_STATE state    - download state
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_OBJECTDOWNLOADSTATUS)(void *nfmCbkParam, const BT_ADDR ba, const NFM_DOWNLOAD_STATE state);

/* 6.10.2 NFM_CBK_OBJECTAUTHORIZE
 *
 * Association command: Remote device trigger
 *
 * If OPP profile receive a authorize from remote device
 * NFM_CBK_OBJECTAUTHORIZE will trigger with following parameter:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      char *filename              - file name
 *      char *filetype              - file type
 *      long long int size          - file size
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_OBJECTAUTHORIZE)(void *nfmCbkParam, const BT_ADDR ba, const char *filename, const char *filetype, long long int size);

/* 6.11.1 NFM_CBK_OBEXTRANSFERRESULT
 *
 * Association command: Remote device trigger
 *
 * If Obex profiles receive a transfer complete signal from remote device
 * NFM_CBK_OBEXTRANSFERRESULT will trigger with following parameter:
 *
 *      BT_ADDR ba                  - remote device mac address by hex(big endian)
 *      NFM_CMD_RET                 - transfer result
 *
 */
typedef NFM_CBK_RET (*NFM_CBK_OBEXTRANSFERRESULT)(void *nfmCbkParam, const BT_ADDR ba, NFM_CMD_RET result);


// Registration of callback functions
typedef struct
{
    NFM_CBK_BLUEZSTATUS                 nfm_cb_BlueZStatus;
    NFM_CBK_POWERSTATUS                 nfm_cb_PowerStatus;
    NFM_CBK_INQUIRYSTATUS               nfm_cb_InquiryStatus;
    NFM_CBK_GETPAIREDLIST               nfm_cb_GetPairedList;
    NFM_CBK_REMOTEPAIRREQUEST           nfm_cb_RemotePairRequest;
    NFM_CBK_LOCALSSPNUMBER              nfm_cb_LocalSSPNumber;      // This function has been deprecated.
    NFM_CBK_DIAGNOSTICMESSAGE           nfm_cb_DiagnosticMessage;
    NFM_CBK_CONNECTIONSTATUS            nfm_cb_ConnectionStatus;
    NFM_CBK_AUTOCONNECTIONSTATUS        nfm_cb_AutoConnectionStatus;
    NFM_CBK_DISCONNECTREASON            nfm_cb_DisconnectReason;

    /* HFP */
    NFM_CBK_HANDSFREESTATUS             nfm_cb_HandsfreeStatus;     // This function has been deprecated.
    NFM_CBK_HANDSFREECURRENTCALL        nfm_cb_HandsfreeCurrentCall;
    NFM_CBK_HANDSFREERING               nfm_cb_HandsfreeRing;
    NFM_CBK_HANDSFREECALLSTARTED        nfm_cb_HandsfreeCallStarted;    // This function has been deprecated.
    NFM_CBK_HANDSFREECALLREJECTED       nfm_cb_HandsfreeCallRejected;   // This function has been deprecated.
    NFM_CBK_HANDSFREECALLTERMINATED     nfm_cb_HandsfreeCallTerminated;
    NFM_CBK_HANDSFREECALLHOLD           nfm_cb_HandsfreeCallHold;
    NFM_CBK_HANDSFREECALLACTIVE         nfm_cb_HandsfreeCallActive;
    NFM_CBK_HANDSFREEMPTYCALLHOLD       nfm_cb_HandsfreeMptyCallHold;
    NFM_CBK_HANDSFREEMPTYCALLACTIVE     nfm_cb_HandsfreeMptyCallActive;
    NFM_CBK_HANDSFREEMPTYCALLTERMINATED nfm_cb_HandsfreeMptyCallTerminated;
    NFM_CBK_HANDSFREEOUTGOINGCALL       nfm_cb_HandsfreeOutgoingCall;
    NFM_CBK_HANDSFREESCOSTATUS          nfm_cb_HandsfreeScoStatus;
    NFM_CBK_HANDSFREEBATTSTATUSCHANGED  nfm_cb_HandsfreeBattStatusChanged;
    NFM_CBK_HANDSFREESIGNALSTRENGTH     nfm_cb_HandsfreeSignalStrength;
    NFM_CBK_HANDSFREESPKVOL             nfm_cb_HandsfreeSpeakerVolume;
    NFM_CBK_HANDSFREEMICVOL             nfm_cb_HandsfreeMicrophoneVolume;
    NFM_CBK_HANDSFREECOMMANDRESPONSE    nfm_cb_HandsfreeCommandOK;
    NFM_CBK_HANDSFREECOMMANDRESPONSE    nfm_cb_HandsfreeCommandFail;
    NFM_CBK_HANDSFREEMANUFACTURERID     nfm_cb_HandsfreeManufacturerID;
    NFM_CBK_HANDSFREEMANUFACTURERMODELID    nfm_cb_HandsfreeManufacturerModelID;
    NFM_CBK_HANDSFREESERVICESTATE       nfm_cb_HandsfreeServiceState;
    NFM_CBK_HANDSFREESUBSCRIBERINFO     nfm_cb_HandsfreeSubscriberInfo;
    
    /* AVRCP */
    NFM_CBK_AVRCPSTATUS                 nfm_cb_AVRCPStatus;     // This function has been deprecated.
    NFM_CBK_AVRCPRESPONSE               nfm_cb_AVRCPResponse;
    NFM_CBK_AVRCPGETCAPABILITY          nfm_cb_AVRCPGetCapability;
    NFM_CBK_AVRCPPLAYERSETTINGATTR      nfm_cb_AVRCPPlayerSettingAttribute;
    NFM_CBK_AVRCPPLAYERSETTINGVALUE     nfm_cb_AVRCPPlayerSettingValue;
    NFM_CBK_AVRCPGETCURRENTSETTING      nfm_cb_AVRCPGetCurrentSetting;
    NFM_CBK_AVRCPSETSUCCESS             nfm_cb_AVRCPSetSuccess;
    NFM_CBK_AVRCPNOWPLAYING             nfm_cb_AVRCPNowPlaying;
    NFM_CBK_AVRCPPLAYSTATUS             nfm_cb_AVRCPPlayStatus;
    NFM_CBK_AVRCPREGISTERFAILED         nfm_cb_AVRCPRegisterFailed;
    NFM_CBK_EVENT_PLAYBACKSTATUSCHANGED nfm_cb_Event_PlaybackStatusChanged;
    NFM_CBK_EVENT_PLAYSTATUSPREFER      nfm_cb_Event_PlayStatusPrefer;
    NFM_CBK_EVENT_TRACKCHANGED          nfm_cb_Event_TrackChanged;
    NFM_CBK_EVENT_TRACKREACHEDEND       nfm_cb_Event_TrackReachedEnd;
    NFM_CBK_EVENT_TRACKREACHEDSTART     nfm_cb_Event_TrackReachedStart;
    NFM_CBK_EVENT_PLAYBACKPOSCHANGED    nfm_cb_Event_PlaybackPosChanged;
    NFM_CBK_EVENT_BATTSTATUSCHANGED     nfm_cb_Event_BattStatusChanged;
    NFM_CBK_EVENT_SYSTEMSTATUSCHANGED   nfm_cb_Event_SystemStatusChanged;
    NFM_CBK_EVENT_PLAYERSETTINGCHANGED  nfm_cb_Event_PlayerSettingChanged;
    NFM_CBK_EVENT_UIDSCHANGED           nfm_cb_Event_UIDsChanged;
    NFM_CBK_AVRCPCOMMANDERROR           nfm_cb_AVRCPCommandError;
    NFM_CBK_AVRCPABORTEDRECEIVING       nfm_cb_AVRCPAbortedReceiving;
    NFM_CBK_AVRCPPLAYITEMSUCCESS        nfm_cb_AVRCPPlayItemSuccess;
    NFM_CBK_AVRCPADDNOWPLAYINGSUCCESS   nfm_cb_AVRCPAddNowPlayingSuccess;
    NFM_CBK_AVRCPBROWSEDPLAYER          nfm_cb_AVRCPBrowsedPlayer;
    NFM_CBK_AVRCPMEDIAPLAYERITEM        nfm_cb_AVRCPMediaPlayerItem;
    NFM_CBK_AVRCPFOLDERITEM             nfm_cb_AVRCPFolderItem;
    NFM_CBK_AVRCPMEDIAITEM              nfm_cb_AVRCPMediaItem;
    NFM_CBK_AVRCPGETFOLDERITEMSSUCCESS  nfm_cb_AVRCPGetFolderItemsSuccess;
    NFM_CBK_AVRCPCHANGEPATHSUCCESS      nfm_cb_AVRCPChangePathSuccess;
    NFM_CBK_AVRCPBROWSINGERROR          nfm_cb_AVRCPBrowsingError;

    /* A2DP */
    NFM_CBK_A2DPSTATUS                  nfm_cb_A2DPStatus;      // This function has been deprecated.
    NFM_CBK_A2DPSTREAMSTATUS            nfm_cb_A2DPStreamStatus;
    /* PBAP */
    NFM_CBK_PHONEBOOKDOWNLOADSTATUS     nfm_cb_PhonebookDownloadStatus;
    NFM_CBK_DOWNLOADCONTACT             nfm_cb_DownloadContact;
    NFM_CBK_CONTACTCOUNT                nfm_cb_ContactCount;
    /* MAP */
    NFM_CBK_MESSAGEDOWNLOADSTATUS       nfm_cb_MessageDownloadStatus;
    NFM_CBK_DOWNLOADMESSAGE             nfm_cb_DownloadMessage;
    NFM_CBK_MESSAGENOTIFY               nfm_cb_MessageNotify;
    /* SPP */
    NFM_CBK_SPPINPUT                    nfm_cb_SPPInput;
    NFM_CBK_SPPSTREAMINPUT              nfm_cb_SPPStreamInput;
    /* HID */
    NFM_CBK_HIDCALIBRATION              nfm_cb_HIDCalibration;
    /* OPP */
    NFM_CBK_OBJECTDOWNLOADSTATUS        nfm_cb_ObjectDownloadStatus;
    NFM_CBK_OBJECTAUTHORIZE             nfm_cb_ObjectAuthorize;
    NFM_CBK_OBEXTRANSFERRESULT          nfm_cb_ObexTransferResult;
} NFM_CALLBACK;

////////////////////////////////////////////////////////////////////////////////////////
//5 Callback Registration Function
//
////////////////////////////////////////////////////////////////////////////////////////

#ifdef __GNUC__
  #define BLUELINK_PUBLIC_API __attribute__ ((visibility ("default")))
#else
  #define BLUELINK_PUBLIC_API 
#endif

//5.1.1 nfmCmdSetCbk_BlueZStatus
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_BlueZStatus(NFM_HANDLE nfmHandle, NFM_CBK_BLUEZSTATUS pfn);

//5.1.2 nfmCmdSetCbk_PowerStatus
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_PowerStatus(NFM_HANDLE nfmHandle, NFM_CBK_POWERSTATUS pfn);

//5.2.1 nfmCmdSetCbk_InquiryStatus
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_InquiryStatus(NFM_HANDLE nfmHandle, NFM_CBK_INQUIRYSTATUS pfn);

//5.2.2 nfmCmdSetCbk_GetPairedList
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_GetPairedList(NFM_HANDLE nfmHandle, NFM_CBK_GETPAIREDLIST pfn);

//5.2.3 nfmCmdSetCbk_RemotePairRequest
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_RemotePairRequest (NFM_HANDLE nfmHandle, NFM_CBK_REMOTEPAIRREQUEST pfn);

//5.2.4 nfmCmdSetCbk_LocalSSPNumber
//This function has been deprecated.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_LocalSSPNumber (NFM_HANDLE nfmHandle, NFM_CBK_LOCALSSPNUMBER pfn);

//5.2.5 nfmCmdSetCbk_RemotePairRequest
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_DiagnosticMessage (NFM_HANDLE nfmHandle, NFM_CBK_DIAGNOSTICMESSAGE pfn);

//5.3.1 nfmCmdSetCbk_ConnectionStatus
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_ConnectionStatus(NFM_HANDLE nfmHandle, NFM_CBK_CONNECTIONSTATUS pfn);

//5.3.2 nfmCmdSetCbk_AutoConnectionStatus
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AutoConnectionStatus(NFM_HANDLE nfmHandle, NFM_CBK_AUTOCONNECTIONSTATUS pfn);

//5.3.3 nfmCmdSetCbk_DisconnectReason
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_DisconnectReason(NFM_HANDLE nfmHandle, NFM_CBK_DISCONNECTREASON pfn);

//5.4.1 nfmCmdSetCbk_HandsfreeStatus
//This function has been deprecated.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeStatus(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREESTATUS pfn);

//5.4.2 nfmCmdSetCbk_HandsfreeCurrentCall
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeCurrentCall(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREECURRENTCALL pfn);

//5.4.3 nfmCmdSetCbk_HandsfreeRing
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeRing(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREERING pfn);

//5.4.4 nfmCmdSetCbk_HandsfreeCallStarted
//This function has been deprecated.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeCallStarted(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREECALLSTARTED pfn);

//5.4.5 nfmCmdSetCbk_HandsfreeCallRejected
//This function has been deprecated.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeCallRejected(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREECALLREJECTED pfn);

//5.4.6 nfmCmdSetCbk_HandsfreeCallTerminated
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeCallTerminated(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREECALLTERMINATED pfn);

//5.4.7 nfmCmdSetCbk_HandsfreeCallHold
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeCallHold(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREECALLHOLD pfn);

//5.4.8 nfmCmdSetCbk_HandsfreeCallActive
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeCallActive(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREECALLACTIVE pfn);

//5.4.9 nfmCmdSetCbk_HandsfreeMptyCallHold
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeMptyCallHold(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREEMPTYCALLHOLD pfn);

//5.4.10 nfmCmdSetCbk_HandsfreeMptyCallActive
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeMptyCallActive(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREEMPTYCALLACTIVE pfn);

//5.4.11 nfmCmdSetCbk_HandsfreeOutgoingCall
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeOutgoingCall(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREEOUTGOINGCALL pfn);

//5.4.12 nfmCmdSetCbk_HandsfreeScoStatus
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeScoStatus(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREESCOSTATUS pfn);

//5.4.13 nfmCmdSetCbk_HandsfreeBattStatusChanged
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeBattStatusChanged(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREEBATTSTATUSCHANGED pfn);

//5.4.14 nfmCmdSetCbk_HandsfreeSignalStrength
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeSignalStrength(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREESIGNALSTRENGTH pfn);

//5.4.15 nfmCmdSetCbk_HandsfreeSpeakerVolume
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeSpeakerVolume(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREESPKVOL pfn);

//5.4.16 nfmCmdSetCbk_HandsfreeMicrophoneVolume
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeMicrophoneVolume(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREEMICVOL pfn);

//5.4.17 nfmCmdSetCbk_HandsfreeCommandOK
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeCommandOK(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREECOMMANDRESPONSE pfn);

//5.4.18 nfmCmdSetCbk_HandsfreeCommandFail
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeCommandFail(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREECOMMANDRESPONSE pfn);

//5.4.19 nfmCmdSetCbk_HandsfreeManufacturerID
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeManufacturerID(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREEMANUFACTURERID pfn);

//5.4.20 nfmCmdSetCbk_HandsfreeManufacturerModelID
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeManufacturerModelID(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREEMANUFACTURERMODELID pfn);

//5.4.21 nfmCmdSetCbk_HandsfreeMptyCallTerminated
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeMptyCallTerminated(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREEMPTYCALLTERMINATED pfn);

//5.4.22 nfmCmdSetCbk_HandsfreeServiceState
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HandsfreeServiceState(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREESERVICESTATE pfn);

//5.4.23 nfmCmdSetCbk_HandsfreeSubscriberInfo
BLUELINK_PUBLIC_API NFM_CMD_RET	nfmCmdSetCbk_HandsfreeSubscriberInfo(NFM_HANDLE nfmHandle, NFM_CBK_HANDSFREESUBSCRIBERINFO pfn);


//5.5.1 nfmCmdSetCbk_AVRCPStatus
//This function has been deprecated.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPStatus(NFM_HANDLE nfmHandle, NFM_CBK_AVRCPSTATUS pfn);

//5.5.2 nfmCmdSetCbk_AVRCPResponse
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPResponse(NFM_HANDLE nfmHandle, NFM_CBK_AVRCPRESPONSE pfn);

//5.5.3 nfmCmdSetCbk_AVRCPGetCapability
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPGetCapability(NFM_HANDLE nfmHandle, NFM_CBK_AVRCPGETCAPABILITY pfn);

//5.5.4 nfmCmdSetCbk_AVRCPPlayerSettingAttribute
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPPlayerSettingAttribute(NFM_HANDLE nfmHandle, NFM_CBK_AVRCPPLAYERSETTINGATTR pfn);

//5.5.5 nfmCmdSetCbk_AVRCPPlayerSettingValue
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPPlayerSettingValue(NFM_HANDLE nfmHandle, NFM_CBK_AVRCPPLAYERSETTINGVALUE pfn);

//5.5.6 nfmCmdSetCbk_AVRCPGetCurrentSetting
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPGetCurrentSetting(NFM_HANDLE nfmHandle, NFM_CBK_AVRCPGETCURRENTSETTING pfn);

//5.5.7 nfmCmdSetCbk_AVRCPSetSuccess
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPSetSuccess(NFM_HANDLE nfmHandle, NFM_CBK_AVRCPSETSUCCESS pfn);

//5.5.8 nfmCmdSetCbk_AVRCPNowPlaying
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPNowPlaying(NFM_HANDLE nfmHandle, NFM_CBK_AVRCPNOWPLAYING pfn);

//5.5.9 nfmCmdSetCbk_AVRCPPlayStatus
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPPlayStatus (NFM_HANDLE nfmHandle, NFM_CBK_AVRCPPLAYSTATUS pfn);

//5.5.10 nfmCmdSetCbk_AVRCPRegisterFailed
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPRegisterFailed(NFM_HANDLE nfmHandle, NFM_CBK_AVRCPREGISTERFAILED pfn);

//5.5.11 nfmCmdSetCbk_Event_PlaybackStatusChanged
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_Event_PlaybackStatusChanged(NFM_HANDLE nfmHandle, NFM_CBK_EVENT_PLAYBACKSTATUSCHANGED pfn);

//5.5.11 nfmCmdSetCbk_Event_PlayStatusPrefer
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_Event_PlayStatusPrefer(NFM_HANDLE nfmHandle, NFM_CBK_EVENT_PLAYSTATUSPREFER pfn);

//5.5.12 nfmCmdSetCbk_Event_TrackChanged
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_Event_TrackChanged(NFM_HANDLE nfmHandle, NFM_CBK_EVENT_TRACKCHANGED pfn);

//5.5.13 nfmCmdSetCbk_Event_TrackReachedEnd
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_Event_TrackReachedEnd(NFM_HANDLE nfmHandle, NFM_CBK_EVENT_TRACKREACHEDEND pfn);

//5.5.14 nfmCmdSetCbk_Event_TrackReachedStart
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_Event_TrackReachedStart(NFM_HANDLE nfmHandle, NFM_CBK_EVENT_TRACKREACHEDSTART pfn);

//5.5.15 nfmCmdSetCbk_Event_PlaybackPosChanged
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_Event_PlaybackPosChanged(NFM_HANDLE nfmHandle, NFM_CBK_EVENT_PLAYBACKPOSCHANGED pfn);

//5.5.16 nfmCmdSetCbk_Event_BattStatusChanged
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_Event_BattStatusChanged(NFM_HANDLE nfmHandle, NFM_CBK_EVENT_BATTSTATUSCHANGED pfn);

//5.5.17 nfmCmdSetCbk_Event_SystemStatusChanged
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_Event_SystemStatusChanged(NFM_HANDLE nfmHandle, NFM_CBK_EVENT_SYSTEMSTATUSCHANGED pfn);

//5.5.18 nfmCmdSetCbk_Event_PlayerSettingChanged
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_Event_PlayerSettingChanged(NFM_HANDLE nfmHandle, NFM_CBK_EVENT_PLAYERSETTINGCHANGED pfn);

//5.5.19 nfmCmdSetCbk_Event_UIDsChanged
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_Event_UIDsChanged(NFM_HANDLE nfmHandle, NFM_CBK_EVENT_UIDSCHANGED pfn);

//5.5.20 nfmCmdSetCbk_AVRCPCommandError
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPCommandError(NFM_HANDLE nfmHandle, NFM_CBK_AVRCPCOMMANDERROR pfn);

//5.5.21 nfmCmdSetCbk_AVRCPAbortedReceiving
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPAbortedReceiving(NFM_HANDLE nfmHandle, NFM_CBK_AVRCPABORTEDRECEIVING pfn);

//5.5.22 nfmCmdSetCbk_AVRCPPlayItemSuccess
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPPlayItemSuccess(NFM_HANDLE nfmHandle, NFM_CBK_AVRCPPLAYITEMSUCCESS pfn);

//5.5.23 nfmCmdSetCbk_AVRCPAddNowPlayingSuccess
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPAddNowPlayingSuccess(NFM_HANDLE nfmHandle, NFM_CBK_AVRCPADDNOWPLAYINGSUCCESS pfn);

//5.5.24 nfmCmdSetCbk_AVRCPBrowsedPlayer
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPBrowsedPlayer(NFM_HANDLE nfmHandle, NFM_CBK_AVRCPBROWSEDPLAYER pfn);

//5.5.25 nfmCmdSetCbk_AVRCPMediaPlayerItem
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPMediaPlayerItem(NFM_HANDLE nfmHandle, NFM_CBK_AVRCPMEDIAPLAYERITEM pfn);

//5.5.26 nfmCmdSetCbk_AVRCPFolderItem
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPFolderItem(NFM_HANDLE nfmHandle, NFM_CBK_AVRCPFOLDERITEM pfn);

//5.5.27 nfmCmdSetCbk_AVRCPMediaItem
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPMediaItem(NFM_HANDLE nfmHandle, NFM_CBK_AVRCPMEDIAITEM pfn);

//5.5.28 nfmCmdSetCbk_AVRCPGetFolderItemsSuccess
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPGetFolderItemsSuccess(NFM_HANDLE nfmHandle, NFM_CBK_AVRCPGETFOLDERITEMSSUCCESS pfn);

//5.5.29 nfmCmdSetCbk_AVRCPChangePathSuccess
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPChangePathSuccess(NFM_HANDLE nfmHandle, NFM_CBK_AVRCPCHANGEPATHSUCCESS pfn);

//5.5.30 nfmCmdSetCbk_AVRCPBrowsingError
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_AVRCPBrowsingError(NFM_HANDLE nfmHandle, NFM_CBK_AVRCPBROWSINGERROR pfn);

//5.5.31 nfmCmdSetCbk_A2DPStatus
//This function has been deprecated.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_A2DPStatus(NFM_HANDLE nfmHandle, NFM_CBK_A2DPSTATUS pfn);

//5.5.32 nfmCmdSetCbk_A2DPStreamStatus
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_A2DPStreamStatus(NFM_HANDLE nfmHandle, NFM_CBK_A2DPSTREAMSTATUS pfn);

//5.6.1 nfmCmdSetCbk_PhonebookDownloadStatus
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_PhonebookDownloadStatus(NFM_HANDLE nfmHandle, NFM_CBK_PHONEBOOKDOWNLOADSTATUS pfn);

//5.6.2 nfmCmdSetCbk_DownloadContact
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_DownloadContact(NFM_HANDLE nfmHandle, NFM_CBK_DOWNLOADCONTACT pfn);

//5.6.3 nfmCmdSetCbk_ContactCount
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_ContactCount(NFM_HANDLE nfmHandle, NFM_CBK_CONTACTCOUNT pfn);

//5.7.1 nfmCmdSetCbk_MessageDownloadStatus
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_MessageDownloadStatus(NFM_HANDLE nfmHandle, NFM_CBK_MESSAGEDOWNLOADSTATUS pfn);

//5.7.2 nfmCmdSetCbk_DownloadMessage
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_DownloadMessage(NFM_HANDLE nfmHandle, NFM_CBK_DOWNLOADMESSAGE pfn);

//5.7.3 nfmCmdSetCbk_MessageNotify
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_MessageNotify(NFM_HANDLE nfmHandle, NFM_CBK_MESSAGENOTIFY pfn);

//5.8.1 nfmCmdSetCbk_SPPInput
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_SPPInput(NFM_HANDLE nfmHandle, NFM_CBK_SPPINPUT pfn);

//5.8.2 nfmCmdSetCbk_SPPStreamInput
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_SPPStreamInput(NFM_HANDLE nfmHandle, NFM_CBK_SPPSTREAMINPUT pfn);

//5.9.1 nfmCmdSetCbk_HIDCalibration
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_HIDCalibration(NFM_HANDLE nfmHandle, NFM_CBK_HIDCALIBRATION pfn);

//5.10.1 nfmCmdSetCbk_ObjectDownloadStatus
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_ObjectDownloadStatus(NFM_HANDLE nfmHandle, NFM_CBK_OBJECTDOWNLOADSTATUS pfn);

//5.10.2 nfmCmdSetCbk_ObjectAuthorize
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_ObjectAuthorize(NFM_HANDLE nfmHandle, NFM_CBK_OBJECTAUTHORIZE pfn);

//5.11.1 nfmCmdSetCbk_ObexTransferResult
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCbk_ObexTransferResult(NFM_HANDLE nfmHandle, NFM_CBK_OBEXTRANSFERRESULT pfn);


////////////////////////////////////////////////////////////////////////////////////////
//4.1 Local Information
//Functions deal with MW startup, device information
////////////////////////////////////////////////////////////////////////////////////////
//4.1.1 nfmCmdStartup
//This is the first middleware function should be called before any other functions.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdStartup(NFM_HANDLE *pnfmHandle, void *nfmCbkParam);

//4.1.2 nfmCmdSetCallback
//Set the callback functions for this middleware.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCallback(NFM_HANDLE nfmHandle, NFM_CALLBACK *nfmCbk);

//4.1.3 nfmCmdEnd
//This is the last middleware function should be called before application termination.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdEnd(NFM_HANDLE *pnfmHandle);

//4.1.4 nfmCmdSetLogEnable
//Set the log option for this middleware.
//Since log is used in nfmCmdStartup, this function may be called earlier to make it silent.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetLogEnable(NFM_HANDLE nfmHandle, bool enable);

//4.1.5 nfmCmdGetMwVersion
//Get the middleware version number.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetMwVersion (NFM_VER *pVer);

//4.1.6 nfmCmdGetLocalBtAddr
//Get the address of local Bluetooth device.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetLocalBtAddr(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.1.7 nfmCmdGetLocalDeviceName
//Get the name of local Bluetooth device.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetLocalDeviceName(NFM_HANDLE nfmHandle, char *pwzDevName,
        size_t nDevName);

//4.1.8 nfmCmdSetLocalDeviceName
//Set the name of local Bluetooth device.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetLocalDeviceName(NFM_HANDLE nfmHandle, const char *pwzDevName);

//4.1.9 nfmCmdGetCOD
//Get the class of device settings of local Bluetooth device.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetCOD(NFM_HANDLE pnfmHandle, unsigned int *pcod);

//4.1.10 nfmCmdGetPower
//Application uses this function to get the current power status of local Bluetooth device.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetPower(NFM_HANDLE nfmHandle, NFM_POWER *pnfmPower);

//4.1.11 nfmCmdSetPower
//Application should power on the Bluetooth device by calling this function.
//To power on, set nfmPower to NFM_POWERON.
//To power off, set nfmPower to NFM_POWEROFF.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetPower(NFM_HANDLE nfmHandle, NFM_POWER nfmPower);

//4.1.12 nfmCmdSetLocalBtAddr
//Set the address of local Bluetooth device.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetLocalBtAddr(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.1.13 nfmCmdSetAudioDeviceName
//Set Audio output device name. Both A2DP and RING
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetAudioDeviceName(NFM_HANDLE nfmHandle, char *AudioDevName);

//4.1.14 nfmCmdSetA2DPDeviceName
//Set A2DP output device name. Only A2DP
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetA2DPDeviceName(NFM_HANDLE nfmHandle, char *A2DPDevName);

//4.1.15 nfmCmdSetRingDeviceName
//Set RING output device name. Only RING
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetRingDeviceName(NFM_HANDLE nfmHandle, char *RingDevName);

//4.1.16 nfmCmdGetA2DPDeviceName
//Get A2DP output device name. Only A2DP
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetA2DPDeviceName(NFM_HANDLE nfmHandle, char *A2DPDevName, int size);

//4.1.17 nfmCmdGetRingDeviceName
//Get RING output device name. Only RING
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetRingDeviceName(NFM_HANDLE nfmHandle, char *RingDevName, int size);

//4.1.18 nfmCmdSetTestMode
//Set Bluetooth Test Mode.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetTestMode(NFM_HANDLE nfmHandle, bool enable);

//4.1.19 nfmCmdSetBrEdrTestMode
//Set BrEdr Test Mode
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetBrEdrTestMode(NFM_HANDLE nfmHandle, unsigned char mode, unsigned short freq, unsigned char power, unsigned char data_type, unsigned char modulation);

////////////////////////////////////////////////////////////////////////////////////////
//4.2 Scanning and Pairing
//Functions deal with scanning devices and pairing
////////////////////////////////////////////////////////////////////////////////////////

//4.2.1 nfmCmdGetScanEnable
//Get the scan enable settings of local Bluetooth device.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetScanEnable(NFM_HANDLE nfmHandle, bool *enable);

//4.2.2 nfmCmdSetScanEnable
//Set the scan enable settings of local Bluetooth device.
//  Parameter mask is a bitwise value,
//  0x01: inquiry scan enable.
//  0x02: page scan enable.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetScanEnable(NFM_HANDLE nfmHandle, bool enable);

//4.2.3 nfmCmdSetScanEnableTime
//Set the scan enable time settings of local Bluetooth device.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetScanEnableTime(NFM_HANDLE nfmHandle, unsigned int enable_time);

//4.2.4 nfmCmdGetPairedList
//Application uses this function to get the previous paired Bluetooth devices.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetPairedList(NFM_HANDLE nfmHandle);

//4.2.5 nfmCmdPairRequest
//Application uses this function to pair a remote Bluetooth device.
// Parameter:
// nPinLength, the maximum value is 16.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdPairRequest(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        int nPinLength, char *ppin);

//4.2.6 nfmCmdDeleteDeviceData
//Application uses this function to delete all storage data (phonebook, photo, msg) of the remote Bluetooth device.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdDeleteDeviceData(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.2.7 nfmCmdDeletePairDevice
//Application uses this function to delete a previous paired remote Bluetooth device.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdDeletePairDevice(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.2.8 nfmCmdAnswerPair
//uses this function to accept a pairing request from remote Bluetooth device.
//The pairing request is an event notified by callback function defined in pfnNfmCbkRemotePairRequest.
//Parameter:
//      nPinLength, the maximum value is 16.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdAnswerPair(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        int nPinLength, char *ppin);

//4.2.9 nfmCmdRefusePair
//Application uses this function to refuse a pairing request from remote Bluetooth device.
//The pairing request is an event notified by callback function defined in pfnNfmCbkRemotePairRequest.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdRefusePair(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.2.10 nfmCmdQueryPairService
//Application uses this function to query the available services supported by remote Bluetooth device.
//NFM_CMD_RET   nfmCmdQueryPairService(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.2.10 nfmCmdInquiryStart
//find other Bluetooth device near its neighborhood.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdInquiryStart(NFM_HANDLE nfmHandle);

//4.2.11 nfmCmdInquiryStop
//cancel the previous inquiry request.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdInquiryStop(NFM_HANDLE nfmHandle);

//4.2.12 nfmCmdGetInquiryInterval
//Get the inquiry interval.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetInquiryInterval(NFM_HANDLE nfmHandle, int *interval);

//4.2.13 nfmCmdIsPaired
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdIsPaired(NFM_HANDLE nfmHandle, BT_ADDR *pba, bool *paired);

//4.2.14 nfmCmdSupportProfilelist
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSupportProfilelist(NFM_HANDLE nfmHandle, BT_ADDR *pba, int *Profile_ID);

//4.2.15 nfmCmdSupportProfileVersion
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSupportProfileVersion(NFM_HANDLE nfmHandle, BT_ADDR *pba, NFM_PROFILEID Profile_ID, NFM_SUPPORT_VERSION *version);

//4.2.16 nfmCmdSetSSPMode
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetSSPMode(NFM_HANDLE nfmHandle, bool enable);

//4.2.16 nfmCmdSetPairedDeviceNum
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetPairDeviceNum(NFM_HANDLE nfmHandle, int num );

//4.2.17 nfmCmdGetPairedDeviceNum
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetPairDeviceNum(NFM_HANDLE nfmHandle, int *num );

////////////////////////////////////////////////////////////////////////////////////////
//4.3 Connecting
//Functions deal with connecting
////////////////////////////////////////////////////////////////////////////////////////

//4.3.1 nfmCmdConnectRemoteDevice
//Activate a service supported by remote Bluetooth device.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdConnectRemoteDevice(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        NFM_PROFILEID ProfileID);

//4.3.2 nfmCmdDisconnectRemoteDevice
//Deactivate a service that was previously activated
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdDisconnectRemoteDevice(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        NFM_PROFILEID ProfileID);

//4.3.3 nfmCmdSetRfcommUUID
//Set UUID before rfcomm connect.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetRfcommUUID(NFM_HANDLE nfmHandle, const char* uuid);

//4.3.4 nfmCmdSetAutoConnectionEnable
//Set auto connection configuration for BT power-on.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetAutoConnectionEnable(NFM_HANDLE nfmHandle, int retry_count);

//4.3.5 nfmCmdGetAutoConnectionEnable
//Get auto connection configuration for BT power-on.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetAutoConnectionEnable(NFM_HANDLE nfmHandle, int* retry_count);

//4.3.6 nfmCmdSetAutoConnectionEnableLinklost
//Set auto connection configuration for linklost.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetAutoConnectionEnableLinklost(NFM_HANDLE nfmHandle, int retry_count);

//4.3.7 nfmCmdGetAutoConnectionEnableLinklost
//Get auto connection configuration for linklost.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetAutoConnectionEnableLinklost(NFM_HANDLE nfmHandle, int* retry_count);

//4.3.8 nfmCmdSetConnectionAllAudioEnable
//Set connection all audio profile configuration
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetConnectionAllAudioEnable(NFM_HANDLE nfmHandle, int enable);

//4.3.9 nfmCmdGetConnectionAllAudioEnable
//Get connection all audio profile configuration
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetConnectionAllAudioEnable(NFM_HANDLE nfmHandle, int* enable);

//4.3.10 nfmCmdSetRemoteDeviceProfile
//Set device enable audio profile
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetRemoteDeviceProfile(NFM_HANDLE nfmHandle, BT_ADDR *pba, NFM_PROFILEID ProfileID);

//4.3.11 nfmCmdGetRemoteDeviceProfile
//Get device enable audio profile
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetRemoteDeviceProfile(NFM_HANDLE nfmHandle, BT_ADDR *pba, int *ProfileID);

//4.3.12 nfmCmdSetRuntimeAutoConnectionEnable
//Set auto connection configuration for now or BT power-on
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetRuntimeAutoConnectionEnable(NFM_HANDLE nfmHandle, int retry_count);

//4.3.13 nfmCmdGetRuntimeAutoConnectionEnable
//Get auto connection configuration for now or BT power-on.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetRuntimeAutoConnectionEnable(NFM_HANDLE nfmHandle, int* retry_count);

//4.3.14 nfmCmdSetAutoConnectionPeriod
//Set auto connection retry period configuration.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetAutoConnectionPeriod(NFM_HANDLE nfmHandle, int retry_period);

//4.3.15 nfmCmdGetAutoConnectionPeriod
//Get auto connection retry period configuration.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetAutoConnectionPeriod(NFM_HANDLE nfmHandle, int* retry_period);

////////////////////////////////////////////////////////////////////////////////////////
//4.4 HFP
//Functions deal with hands-free
////////////////////////////////////////////////////////////////////////////////////////

//4.4.1 nfmCmdHSAnswerCall
//Application uses this function to answer to current incoming call on connected headset AG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdHSAnswerCall(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.4.2 nfmCmdHSHangUpCall
//Application uses this function to terminate the ongoing call through the connected headset AG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdHSHangUpCall(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.4.3 nfmCmdDialCall
//Application uses this function to dial to the given number through the connected AG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdDialCall(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        char *pwzNumber);

//4.4.4 nfmCmdDialLast
//Application uses this function to dial to the last dialed number through the connected AG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdDialLast(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.4.5 nfmCmdSendDTMF
//Application uses this function to send DTMF number through the connected AG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSendDTMF(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        char *pwzNumber);

//4.4.6 nfmCmdAudioTransfer
//iActive, 0: transfer to handsfree unit, 1: transfer to audio gateway.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdAudioTransfer(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        bool iActive);

//4.4.7 nfmCmdAnswerCall
//Application uses this function to answer to current incoming call on connected AG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdAnswerCall(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.4.8 nfmCmdRejectCall
//Application uses this function to reject to current incoming call on connected AG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdRejectCall(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.4.9 nfmCmdTerminateCall
//Application uses this function to terminate the ongoing call through the connected AG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdTerminateCall(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.4.10 nfmCmdSetCallAutoReaction
//Application uses this function to set auto answer to incoming call on connected AG
//Set seconds to -1 to cancel auto answer
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCallAutoReaction(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        NFM_HANDSFREE_AUTO_REACTION action);

//4.4.11 nfmCmdHoldCall
//Application uses this function to send hold call command to connected AG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdHoldCall(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        NFM_HANDSFREE_CALLHELD_OPTION option, unsigned int specified_call_number);

//4.4.12 nfmCmdSetSpeakerVolume
//Application uses this function to set the speaker volume through the connected AG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetSpeakerVolume(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        const char *volume);

//4.4.13 nfmCmdSetMicrophoneVolume
//Application uses this function to set microphone volume through the connected AG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetMicrophoneVolume(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        const char *volume);

//4.4.14 nfmCmdSetPlayRing
//Application uses this function to set ringtone play or not
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetPlayRing(NFM_HANDLE nfmHandle, bool action);

//4.4.15 nfmCmdGetPlayRing
//Application uses this function to get ringtone play or not
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetPlayRing(NFM_HANDLE nfmHandle, bool* action);

//4.4.16 nfmCmdSetVoiceRecognition
//Application uses this function to set Voice Recognition through the connected AG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetVoiceRecognition(NFM_HANDLE nfmHandle, BT_ADDR *pba, int enable);

//4.4.17 nfmCmdSetXAPL
//Application uses this function to set Apple custom AT commands through the connected AG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetXAPL(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        unsigned int vid, unsigned int pid, unsigned int version, unsigned int feature);

//4.4.18 nfmCmdSetSiriEyeFree
//Application uses this function to set Siri eye free mode information through the connected AG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetSiriEyeFree(NFM_HANDLE nfmHandle, BT_ADDR *pba, int enable);

//4.4.19 nfmCmdSetCallAutoReactionTime
//Application uses this function to set the waiting time to do auto answer or reject.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCallAutoReactionTime(NFM_HANDLE nfmHandle, int Sec);

//4.4.20 nfmCmdGetCallAutoReaction
//Application uses this function to get the action which will be taken when there is an incoming call.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetCallAutoReaction(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        NFM_HANDSFREE_AUTO_REACTION *action);
//4.4.21 nfmCmdGetCallAutoReactionTime
//Application uses this function to get the waiting time to do auto answer or reject.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetCallAutoReactionTime(NFM_HANDLE nfmHandle, int *Sec);

//4.4.22 nfmCmdMicrophoneMute
//Application uses this function to set HFP sco microphone mute/unmute (local device).
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdMicrophoneMute(NFM_HANDLE nfmHandle, BT_ADDR *pba, bool opt);

//4.4.23 nfmCmdGetMicMuteStatus
//Application uses this function to get HFP sco microphone status (mute or unmute) (local device).
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetMicMuteStatus(NFM_HANDLE nfmHandle, BT_ADDR *pba, bool *opt);

//4.4.24 nfmCmdGetDevManufacturerID
//Application uses this function to request manufacturer identification to AG which is connected handsfree.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetDevManufacturerID(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.4.25 nfmCmdGetDevManufacturerModelID
//Application uses this function to request manufacturer model identification to AG which is connected handsfree.
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetDevManufacturerModelID(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.4.26 nfmCmdSetSpkRender
//Application uses this function to set HFP sco speaker render or not (local device).
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetSpkRender(NFM_HANDLE nfmHandle, bool action);

//4.4.27 nfmCmdGetSpkRender
//Application uses this function to get HFP sco speaker render status (render or not) (local device).
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetSpkRender(NFM_HANDLE nfmHandle, bool* action);

//4.4.28 nfmCmdSetRejectInBandRing
//Application uses this function to reject in-band ring tone from phone ane play internal ring tone from host or not
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetRejectInBandRing(NFM_HANDLE nfmHandle, bool action);

////////////////////////////////////////////////////////////////////////////////////////
//4.5 A2DP and AVRCP
//Functions deal with A2DP and AVRCP
////////////////////////////////////////////////////////////////////////////////////////

/**********************************************/
/* Functions below need AVRCP higher than 1.0 */
/**********************************************/

//4.5.1 nfmCmdAVRCPControlCommand
//Application uses this function to control the player on connected TG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdAVRCPControlCommand(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        NFM_AVRCP_PLAY_OPERATION opidCode);

/**********************************************/
/* Functions below need AVRCP higher than 1.3 */
/**********************************************/

//4.5.2 nfmCmdGetCapability
//Application uses this function to get the capability of player on connected TG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetCapability (NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.5.3 nfmCmdGetPlayerSettingAttributes
//Application uses this function to get the attributes supported by player on connected TG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetPlayerSettingAttributes(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.5.4 nfmCmdGetPlayerSettingValues
//Application uses this function to get the attribute values supported by player on connected TG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetPlayerSettingValues(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        const unsigned char attr_id);

//4.5.5 nfmCmdGetCurrentPlayerAttributeValues
//Application uses this function to get the current attribute values of player on connected TG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetCurrentPlayerAttributeValues(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.5.6 nfmCmdSetCurrentPlayerAttributeValues
//Application uses this function to set the current attribute values of player on connected TG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetCurrentPlayerAttributeValues(NFM_HANDLE nfmHandle,
        BT_ADDR *pba, unsigned char setting[PLAYER_ATTRIBUTE]);

//4.5.7 nfmCmdGetNowPlayingAttributes
//Application uses this function to get the attributes of currently playing track on connected TG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetNowPlayingAttributes(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.5.8 nfmCmdGetPlayStatus
//Application uses this function to get the play status of player on connected TG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetPlayStatus(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.5.9 nfmCmdRegisterEventWatcher
//Application uses this function to register a event watcher on connected TG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdRegisterEventWatcher(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        NFM_AVRCP_SUPPORTED_EVENTS event, unsigned int playback_interval_sec);

//4.5.10 nfmCmdUnregisterEventWatcher
//Application uses this function to unregister a event watcher on connected TG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdUnregisterEventWatcher(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        NFM_AVRCP_SUPPORTED_EVENTS event);

//4.5.11 nfmCmdAbortReceivingResponse
//Application uses this function to abort receiving response of last request on connected TG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdAbortReceivingResponse(NFM_HANDLE nfmHandle, BT_ADDR *pba);

/**********************************************/
/* Functions below need AVRCP higher than 1.4 */
/**********************************************/

//4.5.12 nfmCmdAVRCPSetBrowsedPlayer
//Application uses this function to set browsed player on connected TG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdAVRCPSetBrowsedPlayer(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        unsigned short int player_id);

//4.5.13 nfmCmdAVRCPGetFolderItems
//Application uses this function to get folder item on connected TG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdAVRCPGetFolderItems(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        const NFM_FOLDER_ITEM_SCOPE_ID scope_id);

//4.5.14 nfmCmdAVRCPGetFolderItemsSectional
//Application uses this function to get folder item sectional on connected TG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdAVRCPGetFolderItemsSectional(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        const NFM_FOLDER_ITEM_SCOPE_ID scope_id, unsigned int start_item_offset, unsigned int max_count);


//4.5.15 nfmCmdAVRCPChangePath
//Application uses this function to change path of current navigating folder on connected TG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdAVRCPChangePath(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        const unsigned short int uid_counter,
        const NFM_AVRCP_CHANGE_FOLDER_DIRECTION direction,
        const unsigned long long int uid);

//4.5.16 nfmCmdPlayItem
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdPlayItem(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        const unsigned long long int uid, const unsigned short int uid_counter);

//4.5.17 nfmCmdAddToNowPlaying
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdAddToNowPlaying(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        const unsigned long long int uid, const unsigned short int uid_counter);

//4.5.18 nfmCmdGetAVRCPSupportVersion
//Application uses this function to get AVRCP version supported by remote TG
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetAVRCPSupportVersion(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        NFM_AVRCP_SUPPORT_VERSION *version);

//4.5.19 nfmCmdSetAVRCPVolumeChanged
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetAVRCPVolumeChanged(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        unsigned char volume);

/**********************************************/
/* Functions below a2dp local processing      */
/**********************************************/

//4.5.19 nfmCmdA2dpMute
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdA2dpMute(NFM_HANDLE nfmHandle, BT_ADDR *pba, bool opt);

//4.5.20 nfmCmdA2dpGetMuteStatus
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdA2dpGetMuteStatus(NFM_HANDLE nfmHandle, BT_ADDR *pba, bool *opt);

////////////////////////////////////////////////////////////////////////////////////////
//4.6 PBAP
//Functions deal with Phonebook downloading
////////////////////////////////////////////////////////////////////////////////////////

//4.6.1 nfmCmdDownloadPhonebookStart
//Application uses this function to download phonebook at given location and type
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdDownloadPhonebookStart(NFM_HANDLE nfmHandle, BT_ADDR *pba, NFM_PHONEBOOK_DATA data);

//4.6.2 nfmCmdDownloadContactsStart
//Application uses this function to download phonebook in phone memory and SIM card
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdDownloadContactsStart(NFM_HANDLE nfmHandle, BT_ADDR *pba, NFM_PHONEBOOK_DATA data);

//4.6.3 nfmCmdDownloadPhonebookStop
//Application uses this function to stop download phonebook
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdDownloadPhonebookStop(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.6.4 nfmCmdPbapTransferCancel
//Application uses this function to cancel download phonebook for BQB test
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdPbapTransferCancel (NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.6.5 nfmCmdDownloadPhonebookState
//Application uses this function to get phonebook download status
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdDownloadPhonebookState(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        NFM_DOWNLOAD_STATE *state, NFM_PHONEBOOK_DATA *data);

//4.6.6 nfmCmdDownloadNoPhotoPhonebookStart
//Application uses this function to download phonebook without photo at given location and type
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdDownloadNoPhotoPhonebookStart(NFM_HANDLE nfmHandle, BT_ADDR *pba, NFM_PHONEBOOK_DATA data);

//4.6.7 nfmCmdDownloadNoPhotoContactsStart
//Application uses this function to download phonebook without photo in phone memory and SIM card
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdDownloadNoPhotoContactsStart(NFM_HANDLE nfmHandle, BT_ADDR *pba, NFM_PHONEBOOK_DATA data);

//4.6.8 nfmCmdDownloadPhonebookState
//Application uses this function to download phonebook without disconnect after download complete
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdDownloadPhonebookAlwaysConnected(NFM_HANDLE nfmHandle, bool enable);


////////////////////////////////////////////////////////////////////////////////////////
//4.7 MAP
//Functions deal with Message downloading
////////////////////////////////////////////////////////////////////////////////////////
//4.7.1 nfmCmdDownloadMessageStart
//Application uses this function to download message in folder inbox
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdDownloadMessageStart(NFM_HANDLE nfmHandle, BT_ADDR *pba, unsigned char is_detail);

//4.7.2 nfmCmdDownloadMessageStop
//Application uses this function to stop download message
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdDownloadMessageStop(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.7.3 nfmCmdPushMessage
//Application uses this function to push message
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdPushMessage(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        char *pwzNumber, char *message);

//4.7.4 nfmCmdMessageStartNotify
//Application uses this function to start receive message notification from remote device
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdMessageStartNotify(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.7.5 nfmCmdMessageStopNotify
//Application uses this function to stop receive message notification from remote device
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdMessageStopNotify(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.7.6 nfmCmdDownloadMessageState
//Application uses this function to get message download status
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdDownloadMessageState(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        NFM_DOWNLOAD_STATE *state, NFM_MESSAGE_TYPE *type);

//4.7.7 nfmCmdSetMessageProperty
//Application uses this function to set message status
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetMessageProperty(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        NFM_MESSAGE_SETTING setting, const char* message_handle);

//4.7.8 nfmCmdDownloadSentStart
//Application uses this function to download message in folder sent
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdDownloadSentStart(NFM_HANDLE nfmHandle, BT_ADDR *pba, unsigned char is_detail);

//4.7.9 nfmCmdDownloadSpecificMsgStart
//Application uses this function to download one specific message in folder inbox/sent by message handle
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdDownloadSpecificMsgStart(NFM_HANDLE nfmHandle, BT_ADDR *pba, const char* message_handle, MAP_FOLDER_STRUCT folder);

//4.7.10 nfmCmdDownloadOutboxStart
//Application uses this function to download message in folder outbox
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdDownloadOutboxStart(NFM_HANDLE nfmHandle, BT_ADDR *pba, unsigned char is_detail);

//4.7.11 nfmCmdSetMAPMaxCount
//Application to set MAP max downloading count
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetMAPMaxCount(NFM_HANDLE nfmHandle, unsigned short map_max_listing_count);

//4.7.12 nfmCmdGetMAPMaxCount
//Application to get MAP max downloading count
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdGetMAPMaxCount(NFM_HANDLE nfmHandle, unsigned short *map_max_listing_count);

////////////////////////////////////////////////////////////////////////////////////////
//4.8 SPP
//Functions deal with serial port simulation
////////////////////////////////////////////////////////////////////////////////////////

//4.8.1 nfmCmdSPPOutput
//Application uses this function to send text to remote device
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSPPOutput(NFM_HANDLE nfmHandle, BT_ADDR *pba, char *output, const NFM_PROFILEID ProfileID);

//4.8.2 nfmCmdSPPStreamOutput
//Application uses this function to send text to remote device
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSPPStreamOutput(NFM_HANDLE nfmHandle, BT_ADDR *pba, char *output, const int len, const NFM_PROFILEID ProfileID);

////////////////////////////////////////////////////////////////////////////////////////
//4.9 HID
//Functions deal with Human Interface Device operation
////////////////////////////////////////////////////////////////////////////////////////

//4.9.1 nfmCmdStartHID
//Application uses this function to BT HID Service
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdStartHID(NFM_HANDLE nfmHandle);

//4.9.2 nfmCmdHIDSendKey
//Application uses this function to send HID key to remote device
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdHIDSendKey(NFM_HANDLE nfmHandle, BT_ADDR *pba, int key);

//4.9.3 nfmCmdHIDSendAbsoluteXY
//Application uses this function to send HID absolute coordinate (x , y) to remote device
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdHIDSendAbsoluteXY(NFM_HANDLE nfmHandle, BT_ADDR *pba, int x, int y, unsigned char button);

//4.9.4 nfmCmdHIDSendRelativeXY
//Application uses this function to send HID relative coordinate (x , y) to remote device
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdHIDSendRelativeXY(NFM_HANDLE nfmHandle, BT_ADDR *pba, int x, int y, unsigned char button);

//4.9.5 nfmCmdHIDEnterCalibration
//Application uses this function to enter calibration mode
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdHIDEnterCalibration(NFM_HANDLE nfmHandle, BT_ADDR *pba);

//4.9.6 nfmCmdHIDCancelCalibration
//Application uses this function to cancel calibration mode
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdHIDCancelCalibration(NFM_HANDLE nfmHandle, BT_ADDR *pba);

////////////////////////////////////////////////////////////////////////////////////////
//4.9 OPP
//Functions deal with object push operation
////////////////////////////////////////////////////////////////////////////////////////

//4.10.1 nfmCmdOPPCreateSession
//Application uses this function to send a file to remote device with absolute path
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdOPPCreateSession(NFM_HANDLE nfmHandle, BT_ADDR *pba, char *file_path);
//4.10.2 nfmCmdOPPCancelSession
//Application uses this function to cancel sending transfer
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdOPPCancelSession(NFM_HANDLE nfmHandle, BT_ADDR *pba);
//4.10.3 nfmCmdOPPAuthorizationReply
//Application uses this function to reply authorization to remote device
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdOPPAuthorizationReply(NFM_HANDLE nfmHandle, BT_ADDR *pba, bool allow);
//4.10.4 nfmCmdOPPCancelReceive
//Application uses this function to cancel receiving transfer
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdOPPCancelReceive(NFM_HANDLE nfmHandle, BT_ADDR *pba);
//4.10.5 nfmCmdOPPSetStoragePath
//Application uses this function to set storage directory by absolute path
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdOPPSetStoragePath(NFM_HANDLE nfmHandle, char *storage_path);
//4.10.6 nfmCmdOPPGetStoragePath
//Application uses this function to get storage directory
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdOPPGetStoragePath(NFM_HANDLE nfmHandle, char **storage_path);

//4.11.1 nfmCmdSetSpeakerVolume2DSP
BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetSpeakerVolume2DSP(NFM_HANDLE nfmHandle, BT_ADDR *pba,
        const char *volume);

BLUELINK_PUBLIC_API NFM_CMD_RET nfmCmdSetPairedDeviceSoft(NFM_PAIRED_SOFT_MODE mode);

 #ifdef __cplusplus
 } // extern "C"
 #endif // __cplusplus

#endif //NFBLUELINK_H


