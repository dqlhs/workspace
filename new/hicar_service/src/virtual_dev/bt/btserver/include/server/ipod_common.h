/*
 * ipodcommon.h
 *
 *  Created on: 2013-2-18
 *      Author: wukai
 */

#ifndef IPODCOMMON_H_
#define IPODCOMMON_H_

#include "ipod_def.h"

/**
 * 初始化IPod的相关资源
 */
extern int i_init_ipod();
/**
 * 注册事件回调函数
 */
extern void i_register_event_observer(PIPODObserver p_observer);

/**
 * 注销事件回调函数
 */
extern void i_unregister_event_observer(void);

/**
 * 创建与IPod的连接
 */
extern int i_create_session(ipod_t * ipod_handle);

/**
 * 关闭与IPod的连接
 */
extern int i_close_session(ipod_t * ipod_handle);
/**
 * 获取iPod设备的名称
 */
extern int i_get_model(ipod_t ipod_handle, char * model);

/**
 * 获取iPodlib 版本号
 */
extern int i_get_ipodlib_version(ipod_t ipod_handle, IPODLibVersion * version);
/**
 * 向IPod发送命令
 */
extern int i_send_command(ipod_t ipod_handle, IPODCommandCode cmd_code);
/*
 * 获取当前播放歌曲数
 */
extern int i_get_play_tracks_num(ipod_t ipod_handle);
/**
 * 获取当前播放状态
 */
extern int i_get_play_status(ipod_t ipod_handle, IPODPlayingStatus * ipod_status);

/*
 * 设置随机模式
 * IPODRestoreOnExitMode: 0:iPod拔出后保留设置; 1:iPod拔出后回复原始设置
 */
extern int i_set_shuffle(ipod_t ipod_handle, IPODShuffleMode mode, IPODRestoreOnExitMode restore_onexit);

/**
 * 获取随机模式
 */
extern int i_get_shuffle(ipod_t ipod_handle, IPODShuffleMode * mode);

/**
 * 设置循环模式
 * IPODRestoreOnExitMode: 0:iPod拔出后保留设置; 1:iPod拔出后回复原始设置
 */
extern int i_set_repeat(ipod_t ipod_handle, IPODRepeatMode mode, IPODRestoreOnExitMode restore_onexit);

/**
 * 获取循环模式
 */
extern int i_get_repeat(ipod_t ipod_handle, IPODRepeatMode * mode);

/**
 * 获取当前播放音乐的index值
 */
extern int i_get_current_playing_track_index(ipod_t hid_handle, int * index);

/**
 * 根据index值获取音乐播放歌曲名称
 */
extern int i_get_track_playinfo_by_index(ipod_t ipod_handle, int index, int * nTrackLen);
/**
 * 根据index值获取音乐播放歌曲名称
 */
extern int i_get_track_title_name_by_index(ipod_t ipod_handle, int index, char * name);

/**
 *根据index值获取音乐播放歌曲艺术家（演唱者）名称
 */
extern int i_get_track_artis_name_by_index(ipod_t ipod_handle, int index, char * name);
/**
 * 根据index值获取音乐播放歌曲专辑名称
 */
extern int i_get_track_album_name_by_index(ipod_t ipod_handle, int index, char * name);
/*
 * 获取歌曲专辑图片
 */
extern int i_get_track_artwork_by_index(ipod_t ipod_handle, int format, int index, pArtworkData artwork);
/*
 * reset音乐列表，直接到顶层目录
 */
extern int i_reset_music_list(ipod_t ipod_handle);
extern int i_select_default_play_list(ipod_t ipod_handle, int index);
/*
 *  设备歌曲播放位置 --单位ms
 */
extern int i_set_track_position(ipod_t ipod_handle, int position_ms);
/**
 * 根据索引号打开音乐文件列表，与i_get_music_list配对使用
 * 位于叶子节点时调用此方法可以播放index指向的音乐文件
 * id: 打开iap2功能后有效，表示相关记录的id,不支持iap2时无效
 */
#if 1//IPOD_IAP2_SUPPORT
extern int i_select_music_list(ipod_t ipod_handle, int index, UINT64 id);
#else
extern int i_select_music_list(ipod_t ipod_handle, int index);
#endif

/*
 * 根据起始位置和读取长度获取音乐列表
 */
extern int i_get_music_list(ipod_t ipod_handle, int start_index, int read_count, IPODMusicList * music_list);

#endif /* IPODCOMMON_H_ */
