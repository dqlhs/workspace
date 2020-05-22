/*
 * ipoddef.h
 *
 *  Created on: 2013-3-4
 *      Author: wukai
 */

#ifndef IPODDEF_H_
#define IPODDEF_H_

#include <stdarg.h>
#include <pthread.h>
#include <stdbool.h>

#define IPOD_LIB_VERSION "V4.1.0"
#define IPOD_LIB_TIME       "2014.12.31"

//#define MFIVERSION                  //MFI version define

#define SUCCESS 0
#define FAILURE -1
#define IPOD_IAP2_SUPPORT 1

//#define IPOD_PLATFORM_ANDROID

#ifdef IPOD_PLATFORM_ANDROID
#include <android/log.h>
#endif
/**
 * 日志级别
 */
#define LOG_LEVEL_ERROR    	0
#define LOG_LEVEL_WARN 		1
#define LOG_LEVEL_INFO 		2
#define LOG_LEVEL_DEBUG		3

#define IPOD_TRACK_NAME_MAX_LEN   128

#define PACKET_MAX_LENGTH 	1024
#define HID_HEAD_LENGTH		2

int log_level ;
static char debug_time_buf[50] ;

#define NONE			"\033[m"
#define DARY_GRAY	"\033[1;30m"
#define RED			"\033[0;32;31m"
#define LIGHT_RED	"\033[1;31m"
#define GREEN		"\033[0;32;32m"
#define LIGHT_GREEN	"\033[1;32m"
#define BROWN		"\033[0;33m"
#define YELLOW		"\033[1;33m"
#define BLUE			"\033[0;32;34m"
#define LIGHT_BLUE	"\033[1;34m"
#define PURPLE		"\033[0;35m"
#define LIGHT_PURPLE	"\033[1;35m"
#define CYAN			"\033[0;36m"
#define LIGHT_CYAN	"\033[1;36m"
#define LIGHT_GRAY	"\033[0;37m"
#define WHITE		"\033[1;37m"


/**
 * 默认日志级别
 */
#ifndef _LOG_LEVEL
#define _LOG_LEVEL LOG_LEVEL_INFO
#endif

#ifdef IPOD_PLATFORM_ANDROID
#define TAG "libipod-iap2"
#define IS_DEBUG 1
#define ERROR(fmt, args...) if(IS_DEBUG) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)
#define WARN(fmt, args...) if(IS_DEBUG) __android_log_print(ANDROID_LOG_WARN, TAG, fmt, ##args)
#define INFO(fmt, args...) if(IS_DEBUG) __android_log_print(ANDROID_LOG_INFO, TAG, fmt, ##args)
#define DEBUG(fmt, args...) if(IS_DEBUG) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define VERBOSE(fmt, args...) if(IS_DEBUG) __android_log_print(ANDROID_LOG_VERBOSE, TAG, fmt, ##args)
#else
/**
 * Error日志打印
 */

#define ERROR(format,...)  \
		do {						\
			debug_get_time_str(debug_time_buf);     \
			if(log_level>=LOG_LEVEL_ERROR) \
			{			\
				fprintf(stderr, RED"%s[Thread-%ld][ERROR]"NONE,debug_time_buf, (long)pthread_self());\
				fprintf(stderr,format,##__VA_ARGS__); \
			}						\
			}							\
		while(0)

/**
 * WARN日志打印
 */
#define WARN(format,...)  \
	do {						\
		debug_get_time_str(debug_time_buf);     \
		if(log_level>=LOG_LEVEL_WARN) \
		{									\
			fprintf(stdout, YELLOW"%s[Thread-%ld][WARN]"NONE,debug_time_buf, (long)pthread_self());\
			fprintf(stdout,format,##__VA_ARGS__); \
		}						\
	}							\
	while(0)


/**
 * Info日志打印
 */

#define INFO(format,...)  \
	do {				\
		debug_get_time_str(debug_time_buf);     \
		if(log_level>=LOG_LEVEL_INFO) \
		{									\
			fprintf(stdout, GREEN"%s[Thread-%ld][INFO]"NONE,debug_time_buf,(long)pthread_self());\
			fprintf(stdout,format,##__VA_ARGS__); \
		}												\
	}													\
	while(0)

/**
 * Debug日志打印
 */

#define DEBUG(format, ...)		\
		do {				\
			if(log_level>=LOG_LEVEL_DEBUG) \
			{									\
				fprintf(stdout,format,##__VA_ARGS__); \
			}												\
		}													\
		while(0)

#endif

/* INT is processor specific in length may vary in size */
typedef signed int          INT;
typedef signed char         INT8;
typedef signed short int    INT16;
typedef signed long int     INT32;
typedef signed long long    INT64;

/* UINT is processor specific in length may vary in size */
typedef unsigned int        UINT;
typedef unsigned char       UINT8;
typedef unsigned short int  UINT16;
/* 24-bit type only available on C18 */
#if defined(__18CXX)
typedef unsigned short long UINT24;
#endif
typedef unsigned long int   UINT32;     /* other name for 32-bit integer */
typedef unsigned long long int UINT64;

typedef unsigned int ipod_t;
/*
 * longo id
 */
typedef enum
{
    LINGO_GENERAL					= 0x00U,
    LINGO_MICROPHONE				= 0x01U,
    LINGO_SIMPLE_REMOTE			= 0x02U,
    LINGO_DISPLAY_REMOTE			= 0x03U,
    LINGO_EXTENDED_INTERFACE		= 0x04U,
    LINGO_ACCESSORY_POWER			= 0x05U,
    LINGO_USB_HOST_CONTROL		= 0x06U,
    LINGO_RF_TUNER					= 0x07U,
    LINGO_ACCESSORY_EQUALIZER		= 0x08U,
    LINGO_SPORTS					= 0x09U,
    LINGO_DIGITAL_AUDIO			= 0x0AU,
    LINGO_STORAGE					= 0x0CU,
    LINGO_LOCATION					= 0x0EU,
    LINGO_MAX						= 0x0FU
} IPOD_LINGOS;
/**
 * 播放状态类型
 */
typedef enum
{
    IPOD_DR_PLAYBACK_STOPPED 				= 0x00U,
    IPOD_DR_PLAYING 						= 0x01U,
    IPOD_DR_PLAYBACK_PAUSED 				= 0x02U,
    IPOD_DR_FAST_FORWARD 					= 0x03U,
    IPOD_DR_FAST_REWIND 					= 0x04U,
    IPOD_DR_END_FAST_FORWARD_OR_REWIND 	= 0x05U,
    IPOD_DR_MAX_PLAYSTATUS 				= 0x7FU
} IPODPlayStatusType;


/**
 * 播放状态上报消息
 */
typedef struct _IPODPlayStatus
{
    /* 歌曲总长度 */
    int track_length;
    /*播放进度*/
    int track_postion;
    /*播放状态*/
    IPODPlayStatusType status;
} IPODPlayingStatus;

/**
 * 连接状态
 */
typedef enum
{
    IPOD_CONNECTED,
    IPOD_DISCONNECTED
} IPODConnectStatus;

/**
 * 连接状态
 */
typedef enum
{
    IPOD_STANDARD_OPERATING_MODE,
    IPOD_EXTENDED_INTERFACE_MODE,
    IPOD_FULL_SCREEN_MODE,
    IPOD_OUT_SAFE_ACTION_MODE
} IPODUIMode;

/**
 * ipod数据库层级模式
 */
typedef enum
{
    IPOD_AUDIO_HIERARCHY = 0x01U,
    IPOD_VIDEO_HIERARCHY = 0x02U,
}
IPODHierarchySection;
/**
 * 随机模式
 */
typedef enum
{
    IPOD_SHUFFLE_OFF,
    IPOD_SHUFFLE_TRACKS,
    IPOD_SHUFFLE_ALBUMS,
    IPOD_SHUFFLE_ORDERCHANGE //按上述三种状态顺序改变
} IPODShuffleMode;

/**
 * 重复模式
 */
typedef enum
{
    IPOD_REPEAT_OFF,
    IPOD_REPEAT_ONE_TRACK,
    IPOD_REPEAT_ALL_TRACKS,
    IPOD_REPEAT_ORDERCHANGE //按上述三种状态顺序改变
} IPODRepeatMode;

/**
 *iPod拔出后设置监听模式
 */
typedef enum
{
    IPOD_MONITOR_ON_EXIT_YES,
    IPOD_MONITOR_ON_EXIT_NO
} IPODMonitorOnExitMode;

/**
 *iPod拔出后设置存储模式
 */
typedef enum
{
    IPOD_RESTORE_ON_EXIT_STORED, //iPod拔出后保留设置
    IPOD_RESTORE_ON_EXIT_REVERT	 //iPod拔出后回复原始设置
} IPODRestoreOnExitMode;

/*
 * 命令编码
 */
typedef enum
{
    IPOD_CMD_PLAYPAUSE 		= 0x01U,
    IPOD_CMD_STOP 				= 0x02U,
    IPOD_CMD_NEXTTRACK 		= 0x03U,
    IPOD_CMD_PREVIOUSTRACK 	= 0x04U,
    IPOD_CMD_STARTFF 			= 0x05U,
    IPOD_CMD_STARTREW 			= 0x06U,
    IPOD_CMD_ENDFFREW 			= 0x07U,
    IPOD_CMD_NEXT 				= 0x08U, //1.06
    IPOD_CMD_PREVIOUS 			= 0x09U, //1.06
    IPOD_CMD_NEWPLAY 			= 0x0AU, //1.13
    IPOD_CMD_NEWPAUSE 			= 0x0BU	 //1.13
} IPODCommandCode;

/**
 * 目录类型
 */
typedef enum
{
    IPOD_ALL,
    IPOD_PLAYLIST,
    IPOD_ARTIST,
    IPOD_ALBUM,
    IPOD_GENRE,
    IPOD_SONGS,
    IPOD_COMPOSER,
    IPOD_AUDIOBOOK,
    IPOD_PODCAST
} IPODCategoryType;

/**
 * 排序类型
 */
typedef enum
{
    IPOD_SORT_ASC,
    IPOD_SORT_DESC,
    IPOD_SORT_DEFAULT
} IPODSortType;
/*
 * display remote ipod status info
 */
typedef enum
{
    IPOD_DR_STATUS_TRACK_POS_MS 					= 0x00U,
    IPOD_DR_STATUS_PLAYBACK_INDEX				= 0x01U,
    IPOD_DR_STATUS_CHAPTER_INFO 					= 0x02U,
    IPOD_DR_STATUS_PLAY_STATUS 					= 0x03U,
    IPOD_DR_STATUS_MUTE_AND_VOLUME 				= 0x04U,
    IPOD_DR_STATUS_POWER_AND_BATTERY 			= 0x05U,
    IPOD_DR_STATUS_EQ_SETTING 					= 0x06U,
    IPOD_DR_STATUS_SHUFFLE_SETTING 				= 0x07U,
    IPOD_DR_STATUS_REPEAT_SETTING 				= 0x08U,
    IPOD_DR_STATUS_DATA_AND_TIME 				= 0x09U,
    IPOD_DR_STATUS_ALARM_STATE_AND_DATE 			= 0x0AU,
    IPOD_DR_STATUS_BACKLIGHT 						= 0x0BU,
    IPOD_DR_STATUS_HOLD_SWITCH 					= 0x0CU,
    IPOD_DR_STATUS_SOUND_CHECK 					= 0x0DU,
    IPOD_DR_STATUS_AUDIO_BOOK_SPEED 				= 0x0EU,
    IPOD_DR_STATUS_TRACK_POS_S 					= 0x0FU,
    IPOD_DR_STATUS_MUTE_UI_ABSOLUTE_VOL 			= 0x10U,
    IPOD_DR_STATUS_TRACK_CAPABILITIES 			= 0x11U,
    IPOD_DR_STATUS_PLAYBACK_ENGINE_CONTENTS	 	= 0x12U,

    IPOD_STATUS_INFO_MAX 							= 0x7FU

} IPOD_DR_STATUS_INFO;


/**
 * 设置ipod 播放状态上报消息类型（可以通过设置类型对上报数据进行过滤）
 */
typedef enum
{
    DISABLENOTIFICATION	 									= 0x00,  //Disable all change notification
    ENABLENOTIFICATION 										= 0x01,	//Enable all change notification

    ONE_BYTE_FORMAT 											= 0x00,  //Disable all change notification
    FOUR_BYTE_FORMAT 											= 0x01,	//Enable all change notification
    IPOD_EVT_BASIC_PLAY_STATUS	 							= (0x01 << 0),
    IPOD_EVT_EXTENDED_PLAY_STATUS_CHANGES 					= (0x01 << 1),
    IPOD_EVT_TRACK_INDEX 										= (0x01 << 2),
    IPOD_EVT_TRACK_TIME_OFFSET_MSEC 							= (0x01 << 3),
    IPOD_EVT_TRACK_TIME_OFFSET_SEC 							= (0x01 << 4),
    IPOD_EVT_CHAPTER_INDEX 									= (0x01 << 5),
    IPOD_EVT_CHAPTER_OFFSET_MSEC 							= (0x01 << 6),
    IPOD_EVT_CHAPTER_OFFSET_SEC 								= (0x01 << 7),
    IPOD_EVT_TRACK_UNIQUE_ID 									= (0x01 << 8),
    IPOD_EVT_TRACK_MEDIA_TYPE 								= (0x01 << 9),
    IPOD_EVT_TRACK_LYRICS_READY 								= (0x01 << 10),
    IPOD_EVT_TRACK_CAPABILITIES_CHANGED 						= (0x01 << 11),
    IPOD_EVT_TRACK_PLAYBACK_ENGINE_CONTENTS_CHANGED 		= (0x01 << 12),

    IPOD_EVT_MAX 												= 0x7FFFFFFF
} SetPlayStatusNotification;
/**
 * 设置ipod 显示远程事件上报消息类型（可以通过设置类型对上报数据进行过滤）
 */
typedef enum
{
    IPOD_DRE_TRACK_TIME_POS_MSEC                       = (0x01 << 0),
    IPOD_DRE_TRACK_PLAY_INDEX 			   		      = (0x01 << 1),
    IPOD_DRE_CHAPTER_INDEX				 	          = (0x01 << 2),
    IPOD_DRE_PLAY_STATUS 					              = (0x01 << 3),
    IPOD_DRE_MUTE_OR_UI_VOLUME					      = (0x01 << 4),
    IPOD_DRE_POWER_BATTERY					          = (0x01 << 5),
    IPOD_DRE_EQUALIZER_SETTING					      = (0x01 << 6),
    IPOD_DRE_SHUFLE_SETTING					          = (0x01 << 7),
    IPOD_DRE_REPEAT_SETTING					          = (0x01 << 8),
    IPOD_DRE_DATE_TIME_SETTING					      = (0x01 << 9),
    IPOD_DRE_ALARM_SETTING						      = (0x01 << 10), //DO NOT USE
    IPOD_DRE_BACKLIGHT_LEVEL						      = (0x01 << 11),
    IPOD_DRE_HOLD_SWITCH_STATE						  = (0x01 << 12),
    IPOD_DRE_SOURCE_CHECK_STATE						  = (0x01 << 13),
    IPOD_DRE_AUDIOBOOK_SPEED						      = (0x01 << 14),
    IPOD_DRE_TRACK_TIME_POS_SEC                        = (0x01 << 15),
    IPOD_DRE_MUTE_OR_UI_OR_ABSOLUTE_VOLUME 			  = (0x01 << 16),
    IPOD_DRE_TRACK_CAPABILITIES 					      = (0x01 << 17),
    IPOD_DRE_PLAYBACK_ENGINE_CONTENTS 				  = (0x01 << 18),

    IPOD_DRE_MAX 										  = 0x7FFFFFFF
} SetDisplayRemoteEventNotification;
/**
 * 播放状态上报类型
 */
typedef enum
{
    IPOD_STATUS_EXTENED_PLAYBACK_STOPPED 			= 0x02U,
    IPOD_STATUS_EXTENED_FFW_STARTED 					= 0x05U,
    IPOD_STATUS_EXTENED_REW_STARTED					= 0x06U,
    IPOD_STATUS_EXTENED_FFW_REW_STOPED 				= 0x07U,
    IPOD_STATUS_EXTENED_PLAYBACK_PLAYING 			= 0x0AU,
    IPOD_STATUS_EXTENED_PLAYBACK_PAUSED 				= 0x0BU,
    IPOD_STATUS_EXTENED_MAX_PLAYSTATUS 				= 0x7FU
} IPODPlayStatusTypeExtend;

/**
 * ipod 播放状态上报消息类型
 */
typedef enum
{
    STOPPED 								= 0x00000000U,	//播放停止
    TRACKCHANGED 							= 0x00000001U,	//歌曲切换
    FFSEEKSTOP 							= 0x00000002U,	//快进停止
    REWSEEKSTOP 							= 0x00000003U,	//快退停止
    TRACKTIME_OFFSET_MS 					= 0x00000004U,	//上报播放时间（ms）
    CHAPTERINDEX 							= 0x00000005U,	//章节索引号(用于有声读物)
    PLAYBACK_STATUS_EXT 					= 0x00000006U,  	//Extended play state changes，see IPODPlayStatusType
    TRACKTIME_OFFSET_S 					= 0x00000007U,	//上报播放时间（单位：s）
    CHAPTER_TIME_OFFSET_MSEC			 	= 0x00000008U,
    CHAPTER_TIME_OFFSET_SEC 				= 0x00000009U,	//
    TRACK_UNIQUE_ID 						= 0x0000000AU,	//歌曲独特标识
    TRACK_PLAYBACK_MODE 					= 0x0000000BU,	//当前歌曲类型 0：audio 1:video
    TRACK_LYRICS_READY 					= 0x0000000CU,	//下载的歌词可用
    TRACK_CAPABILITIES_CHANGED 			= 0x0000000DU,	//歌曲属性改变
    PLAYBACK_ENGINE_CONTNENTS_CHANGED 	= 0x0000000EU,	//歌曲在新的播放列表中的index值
    NOTIFICATION_MAX 						= 0x7FFFFFFFU

} NotificationPlayStatus;

typedef enum
{
    // 2bits per pixel
    IPOD_PIXEL_MONOCHROME = 0x01U,

    // RGB 565 color little-endian, 16bpp
    IPOD_PIXEL_RGB_565_LITTLE = 0x02U,

    // RGB 565 color big-endian, 16bpp
    IPOD_PIXEL_RGB_565_BIG = 0x03U,

    IPOD_PIXEL_MAX = 0x7FFFFFFFU

} IPOD_PIXEL_FORMAT;

typedef enum
{
    MONOCHROME_2BPP = 0x01,
    RGB565_LITTLE_ENDIAN_16BPP,
    RGB565_BIG_ENDIAN_16BPP
//	MAX_PIXEL_FORMAT = 0x7F
} DisplayPixelFormat;
typedef enum
{
    IPOD_NOTI_BITMASK_RESERVED_BIT0							= (0x01U << 0),
    IPOD_NOTI_BITMASK_RESERVED_BIT1							= (0x01U << 1),
    IPOD_NOTI_BITMASK_FLOW_CONTROL							= (0x01U << 2),
    IPOD_NOTI_BITMASK_RADIO_TAGGING_STATUS					= (0x01U << 3),
    IPOD_NOTI_BITMASK_CAMERA_STATUS							= (0x01U << 4),
    IPOD_NOTI_BITMASK_CHARGING_INFO							= (0x01U << 5),
    IPOD_NOTI_BITMASK_RESERVED_BIT6							= (0x01U << 6),
    IPOD_NOTI_BITMASK_RESERVED_BIT7							= (0x01U << 7),
    IPOD_NOTI_BITMASK_RESERVED_BIT8							= (0x01U << 8),
    IPOD_NOTI_BITMASK_DATABASE_CHANGED						= (0x01U << 9),
    IPOD_NOTI_BITMASK_NOWPLAYINGFOCUSAPP_STATUS				= (0x01U << 10),
    IPOD_NOTI_BITMASK_SSESSIONSPACEAVAILABLE_STATUS		= (0x01U << 11),
    IPOD_NOTI_BITMASK_RESERVED_BIT12							= (0x01U << 12),
    IPOD_NOTI_BITMASK_COMMAND_COMPLETED						= (0x01U << 13),
    IPOD_NOTI_BITMASK_RESERVED_BIT14							= (0x01U << 14),
    IPOD_NOTI_BITMASK_STATUS_IPOD_OUT_MODE					= (0x01U << 15),
    IPOD_NOTI_BITMASK_RESERVED_BIT16							= (0x01U << 16),
    IPOD_NOTI_BITMASK_BT_CONNECTION_STATUS					= (0x01U << 17),
    IPOD_NOTI_BITMASK_RESERVED_BIT18							= (0x01U << 18),
    IPOD_NOTI_BITMASK_NOWPLAYINGAPPDISPLAYNAME_STATUS		= (0x01U << 19),
    IPOD_NOTI_BITMASK_ASSISTIVETOUCH_STATUS					= (0x01U << 20)

} IPOD_NOTI_BITMASK_STATUS;

/**
 * IPod连接状态改变的回调函数
 */
typedef void (*onConnectedStatusChanged)(IPODConnectStatus /*new_status*/);

/**
 * IPod源改变的回调函数
 */
typedef void (*onSourceChanged)(int /*new_index*/);

/**
 * IPod音轨时间轴改变的回调函数
 */
typedef void (*onTrackProgress)(int /*position*/);

/**
 * IPod播放状态改变的回调函数
 */
typedef void (*onPlayStatusChanged)(IPODPlayStatusType/*IPODPlayStatusTypeExtend new_status*/);

/**
 * IPod随机模式改变的回调函数
 */
typedef void (*onShuffleModeChanged)(IPODShuffleMode /*new_mode*/);

/**
 * IPod重复模式改变的回调函数
 */
typedef void (*onRepeatModeChanged)(IPODRepeatMode /*new_mode*/);

/**
 * IPod采样率改变的回调函数
 */
typedef void (*onSampleRateChanged)(int /*new_rate*/);

/**
 * 专辑图片保存路径
 */
typedef void (*onArtworkChanged)(char * /*artwork_path*/);

#if IPOD_IAP2_SUPPORT
/**
 * 媒体库更新进度
 */
typedef void (*onUpdateProgressChanged)(int /*progress*/, int /*success*/);

typedef void (*onAssistiveEnableChanged) (int);
#endif

/**
 * iPod事件回调函数结构体
 */
typedef struct _IPODObserver
{
    onConnectedStatusChanged on_connected_status_changed;
    onSourceChanged	on_source_changed;
    onTrackProgress on_track_progress;
    onPlayStatusChanged on_play_status_changed;
    onShuffleModeChanged on_shuffle_mode_changed;
    onRepeatModeChanged on_repeat_mode_changed;
    onSampleRateChanged on_sample_rate_changed;
    onArtworkChanged on_artwork_changed;
#if IPOD_IAP2_SUPPORT
    onUpdateProgressChanged on_update_progress_changed;
    onAssistiveEnableChanged on_assistive_enable_changed;
#endif
} IPODObserver;

typedef IPODObserver * PIPODObserver;

/**
 * ipod database存储记录数据类型
 */
typedef struct _IPODMusicSource
{
    int index;
#if IPOD_IAP2_SUPPORT
    UINT64 id;
#endif
    char name[IPOD_TRACK_NAME_MAX_LEN];
} IPODMusicSource;

/**
 * ipodlib 版本信息
 */
typedef struct _IPODLibVersion
{
    char date[128];
    char version[128];
} IPODLibVersion;



/**
 * iPod音乐列表结构
 * size：列表长度
 * totolnum：当前层级下记录总数
 * start：开始序号
 * end：结束序号
 * elements：指向IPODMusicSource的指针
 * categorytype：当前层级的类型
 */
typedef struct _IPODMusicList
{
    int size;
    int totolnum;
    int start;
    int end;
    IPODMusicSource * elements;
    IPODCategoryType categorytype;
} IPODMusicList;
/*
 * ipod options for lingo 各个lingo支持的功能
 */
typedef struct
{
    IPOD_LINGOS lingo;
    union
    {
        long long options;
        char option[8];
    };
} IAP_OPTIONS_FOR_LINGO;

/*
 * ipod software version
 */
typedef struct
{
    char lingo_id;
    char major_version_num;
    char minor_version_num;
} IAP_IPOD_PROTOCOL_VERSION;

/*
 * ipod software version
 */
typedef struct
{
    char major_version_num;
    char minor_version_num;
    char revision_version;
} IAP_IPOD_SOFTWARE_VERSION;
/*
 * ipod HID descriptor
 */
typedef struct
{
    UINT8        descrprior[49];   // 0xFF
} IAP_HID_DESCRIPTOR;

/*
 * ipod HID report
 */
typedef struct
{
    UINT8        report[3];   // 0xFF
} IAP_HID_REPORT;

/*
 * 专辑插图格式
 */
typedef struct
{
    short FormatID;
    DisplayPixelFormat PixelFormat;
    short ImageWidth;
    short ImageHeight;
} ArtworkFormat, *pArtworkFormat;

/*
 * 专辑插图数据
 */
typedef struct
{
    char	cPixelFormat;
    short sWidth;
    short sHeight;
    short sTopLeftX;
    short sTopLeftY;
    short sBottomRightX;
    short sBottomRightY;
    unsigned int uiRowSize;
    unsigned int uiTotalSize;
    unsigned char * pImageData;
} ArtworkData, *pArtworkData;

#endif /* IPODDEF_H_ */
