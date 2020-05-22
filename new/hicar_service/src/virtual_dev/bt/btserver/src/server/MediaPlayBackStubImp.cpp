#include "MediaPlayBackStubImp.h"
#include <string.h>
#include <dbus/dbus.h>
#include <stdlib.h>
//#include <unistd.h>
#include "DeviceStubImp.h"
shared_ptr<MediaPlayBackStubImp> MediaPlayBackStubImp::mediaPlayInstance = NULL;
//ExecuteMethodAsyncResultT MediaPlayBackStubImp::m_notifyConnectDisconnectResult =
//		NULL;
//string MediaPlayBackStubImp::avrcpConnectObjPath = "";
#define id_illegal              0x00
#define DEFAULT_POS_INTERVAL    1
Media* MediaPlayBackStubImp::m_interface;
gboolean MediaPlayBackStubImp::m_mutestate;
PlayStatus m_playStatus;
PlayPosition m_palyPosition;
string songLong = "";
extern bool avrcp_registing;
typedef struct{
    const unsigned char	id;					/**< Player Attribute(Value) ID */
    const char			*name;				/**< Attribute Value ID */
}SETTING_ID_TABLE;
unsigned char attribute_list[20]={0};
int tail = 0;
const SETTING_ID_TABLE event_table_t[] =
{
    {id_illegal,						"Illegal"},
    {EVENT_PLAYBACK_STATUS_CHANGED,		"Change in playback status of the current track (0x01)"},
    {EVENT_TRACK_CHANGED,				"Change of current track (0x02)"},
    {EVENT_TRACK_REACHED_END,			"Reached end of a track (0x03)"},
    {EVENT_TRACK_REACHED_START,			"Reached start of a track (0x04)"},
    {EVENT_PLAYBACK_POS_CHANGED,		"Change in playback position (0x05)"},
    {EVENT_BATT_STATUS_CHANGED,			"Change in battery status (0x06)"},
    {EVENT_SYSTEM_STATUS_CHANGED,		"Change in system status (0x07)"},
    {EVENT_PLAYER_APP_SETTING_CHANGED,	"Change in player application setting (0x08)"},
    {EVENT_NOW_PLAYING_CONTENT_CHANGED,	"The content of the Now Playing list has changed (0x09)"},
    {EVENT_AVAILABLE_PLAYER_CHANGED,	"The available players have changed (0x0A)"},
    {EVENT_ADDRESSED_PLAYER_CHANGED,	"The Addressed Player has been changed (0x0B)"},
    {EVENT_UIDS_CHANGED,				"The UIDs have changed (0x0C)"},
    {EVENT_VOLUME_CHANGED,				"The volume has been changed locally on the target (0x0D)"},
    {EVENT_RESERVED,					"Reserved"},
};

const char *player_attr_id_01 = "Equalizer";
const char *player_attr_id_02 = "Repeat";
const char *player_attr_id_03 = "Shuffle";
const char *player_attr_id_04 = "Scan";
const SETTING_ID_TABLE attribute_table_t[] =
{
    {ATTRIBUTE_ILLEGAL,		NULL},
    {ATTRIBUTE_EQUALIZER,	"Equalizer"},
    {ATTRIBUTE_REPEAT_MODE,	"Repeat"},
    {ATTRIBUTE_SHUFFLE,		"Shuffle"},
    {ATTRIBUTE_RESERVED,	"Scan"}
};

const SETTING_ID_TABLE equalizer_table_t[] =
{
    {id_illegal,	NULL},
    {EQUALIZER_OFF,	"OFF"},
    {EQUALIZER_ON,	"ON"}
};

const SETTING_ID_TABLE repeat_table_t[] =
{
    {id_illegal,			NULL},
    {REPEAT_OFF,			"OFF"},
    {REPEAT_SINGLE_TRACK,	"Single"},
    {REPEAT_ALL_TRACK,		"All"},
    {REPEAT_GROUP,			"Group"}
};

const SETTING_ID_TABLE shuffle_table_t[] =
{
    {id_illegal,		NULL},
    {SHUFFLE_OFF,		"OFF"},
    {SHUFFLE_ALL_TRACK,	"All"},
    {SHUFFLE_GROUP,		"Group"}
};

const SETTING_ID_TABLE scan_table_t[] =
{
    {id_illegal,		NULL},
    {SCAN_OFF,			"OFF"},
    {SCAN_ALL_TRACK,	"All"},
    {SCAN_GROUP,		"Group"}
};

typedef struct{
    const unsigned char	id;
    const SETTING_ID_TABLE	*table;
    const unsigned char 	count;
}SETTING_TABLES;

const SETTING_TABLES tables_t[] =
{
    {ATTRIBUTE_ILLEGAL,		NULL,				id_illegal},
    {ATTRIBUTE_EQUALIZER,	equalizer_table_t,	EQUALIZER_ON},
    {ATTRIBUTE_REPEAT_MODE,	repeat_table_t,		REPEAT_GROUP},
    {ATTRIBUTE_SHUFFLE,		shuffle_table_t,	SHUFFLE_GROUP},
    {ATTRIBUTE_SCAN,		scan_table_t,		SCAN_GROUP}
};

unsigned long MediaPlayBackStubImp::m_musicmoment = 0;
HSAE_CBK_RET a2dp_status(void *HSAECbkParam, const BT_ADDR ba,
		const HSAE_A2DP_STATE state) {
//	printf("The A2DP status of %02X:%02X:%02X:%02X:%02X:%02X is now ", ba.b[0],
//			ba.b[1], ba.b[2], ba.b[3], ba.b[4], ba.b[5]);

//	if (state == HSAE_A2DP_STATE_DISCONNECTED)
//		printf("disconnected.\n");
//	else if (state == HSAE_A2DP_STATE_CONNECTING)
//		printf("connecting.\n");
//	else if (state == HSAE_A2DP_STATE_READY)
//		printf("connected.\n");
//	else if (state == HSAE_A2DP_STATE_OPEN)
//		printf("opened\n");
//	else
//		printf("in unknown state %d.\n", state);
    LOG(BT_LOG_MEDIA,"state =%d",(int)state);
	return HSAE_CBK_SUCCESS;
}

//HSAE_CBK_RET a2dp_streamstatus( void *HSAECbkParam, const BT_ADDR ba,
//                               const bool on )
//{
//    std::shared_ptr< MediaPlayBackStubImp > m_meida = MediaPlayBackStubImp::getInstance();
//    printf( "The stream status of %02X:%02X:%02X:%02X:%02X:%02X is now ",
//            ba.b[0], ba.b[1], ba.b[2], ba.b[3], ba.b[4], ba.b[5] );
////    BT_ADDR m_newba = ba;
//    g_ba = ba;
//
//    if( on == true )
//    {
//        printf( "start.\n\n" );
//        a2dpstream = true;
//        pthread_t id;
//        int ret;
//        hsaeCmdGetNowPlayingAttributes( NULL, (BT_ADDR *) &g_ba );
//    }
//    else
//    {
//        a2dpstream = false;
//        printf( "stop.\n\n" );
//    }
//    return HSAE_CBK_SUCCESS;
//}\

HSAE_CBK_RET avrcp_get_setting(void *cbkParam, const BT_ADDR ba,
        const unsigned char setting_count, const HSAE_PLAYER_SETTING_ID player_setting[])
{
    printf( "The AVRCP player setting of %02X:%02X:%02X:%02X:%02X:%02X is:\n",
            ba.b[0], ba.b[1], ba.b[2], ba.b[3], ba.b[4], ba.b[5] );

    unsigned char i;
    for( i = 0; i < setting_count; i++ ){
        unsigned char attr_id = player_setting[i].attribute_id;
        unsigned char val_id = player_setting[i].value_id;
        printf("%10s:%s\n", attribute_table_t[attr_id].name,
                            tables_t[attr_id].table[val_id].name);
    }
    printf( "\n" );

    return HSAE_CBK_SUCCESS;
}
HSAE_CBK_RET avrcp_setting_value(void *cbkParam, const BT_ADDR ba,
        const unsigned char value_count, const unsigned char valueID[])
{
    const unsigned char attribute_id = attribute_list[tail];

    printf( "The AVRCP player setting of %02X:%02X:%02X:%02X:%02X:%02X has %X value in attribute %s:\n",
            ba.b[0], ba.b[1], ba.b[2], ba.b[3], ba.b[4], ba.b[5], value_count, attribute_table_t[attribute_id].name );

    unsigned char i;
    for( i = 0; i < value_count; i++ )
        printf( "\t%-10s", tables_t[attribute_id].table[valueID[i]].name );
    printf( "\n\n" );

    tail = (tail+1) % 20;

    return HSAE_CBK_SUCCESS;
}
HSAE_CBK_RET avrcp_command_error(void *cbkParam, const BT_ADDR ba,
        const HSAE_AVRCP_PDUID PduId, const char* reason)
{
//    printf( "The AVRCP command %X send to %02X:%02X:%02X:%02X:%02X:%02X got error on response:\n\t%s\n\n",
//            PduId, ba.b[0], ba.b[1], ba.b[2], ba.b[3], ba.b[4], ba.b[5], reason );

//    if(PduId == LIST_APP_VALUE_PDU_ID){
//        attribute_list[tail] = 0;
//        tail = (tail+1)%20;
//    }

    return HSAE_CBK_SUCCESS;
}
HSAE_CBK_RET avrcp_event_TrackChanged(void *HSAECbkParam, const BT_ADDR ba,
		const unsigned long long int element_id) {
//	printf(
//			"The AVRCP playing track of %02X:%02X:%02X:%02X:%02X:%02X has been changed to ",
//			ba.b[0], ba.b[1], ba.b[2], ba.b[3], ba.b[4], ba.b[5]);
    LOG(BT_LOG_MEDIA,"");
	hsaeCmdGetNowPlayingAttributes( NULL, (BT_ADDR *) &ba);

//	if (element_id == ELEMENT_ID_PLAYING) {
//		printf("selected track.\n\n");
//	} else if (element_id == ELEMENT_ID_NO_SELECTED) {
//		printf("no selected track.\n\n");
//	} else {
//		printf("UID %llu.\n\n", element_id);
//	}

	return HSAE_CBK_SUCCESS;

}

//HSAE_CBK_RET avrcp_status(void *HSAECbkParam, const BT_ADDR ba,
//		const HSAE_AVRCP_STATE state) {
//    LOG(BT_LOG_MEDIA,"state =%d",(int)state);
//	return HSAE_CBK_SUCCESS;
//}

HSAE_CBK_RET avrcp_now_playing(void *cbkParam, const BT_ADDR ba,
		const unsigned char attribute_count,
		const HSAE_NOW_PLAYING_ATTRIBUTE attributes[]) {
	std::shared_ptr<MediaPlayBackStubImp> m_meida =
			MediaPlayBackStubImp::getInstance();
	PlayAttribute m_attribute;
	unsigned char i;
    string tempchar="";
	for (i = 0; i < attribute_count; ++i) {
         if (attributes[i].string !=NULL)
             tempchar = string(attributes[i].string);
         else
             tempchar ="";
        if (string(attributes[i].media_attribute) == "Play Time") {
            songLong = tempchar;//string(attributes[i].string);
		}
		if (string(attributes[i].media_attribute) == "Title") {
            m_attribute.title = tempchar;//string(attributes[i].string);
		}
		if (string(attributes[i].media_attribute) == "Artist") {
            m_attribute.artist = tempchar;//string(attributes[i].string);
		}
		if (string(attributes[i].media_attribute) == "Album") {
            m_attribute.album = tempchar;//string(attributes[i].string);
		}
		if (string(attributes[i].media_attribute) == "Genre") {
            m_attribute.genre = tempchar;//string(attributes[i].string);
		}
	}
	m_meida->TrancNowPlayAttributes(m_attribute);
    LOG(BT_LOG_MEDIA,"attribute_count =%d title =%s ,artist =%s,album =%s,m_attribute.genre=%s",attribute_count,m_attribute.title.data(),m_attribute.artist.data(),m_attribute.album.data(),m_attribute.genre.data());
	return HSAE_CBK_SUCCESS;
}


HSAE_CBK_RET avrcp_event_PlaybackStatusChanged(void *HSAECbkParam,
		const BT_ADDR ba, const HSAE_AVRCP_PLAY_STATE state) {
	std::shared_ptr<MediaPlayBackStubImp> m_meida =
			MediaPlayBackStubImp::getInstance();

	return HSAE_CBK_SUCCESS;
}
//add by qianzhong 2016-12-26

HSAE_CBK_RET avrcp_event_PlaybackPosChanged(void *cbkParam, const BT_ADDR ba,
		const char *current_pos) {
	std::shared_ptr<MediaPlayBackStubImp> m_meida =
			MediaPlayBackStubImp::getInstance();
  //  LOG(BT_LOG_MEDIA,"current_pos =%s",current_pos);
	if (current_pos[0] != '\0') {
//		printf(
//				"The AVRCP playing track of %02X:%02X:%02X:%02X:%02X:%02X is now at position %s.\n\n",
//				ba.b[0], ba.b[1], ba.b[2], ba.b[3], ba.b[4], ba.b[5],
//				current_pos);
		m_palyPosition.songLength = songLong;
		m_palyPosition.songPosition = string(current_pos);
		m_playStatus.positon = m_palyPosition;
		m_meida->TrancPlayStatus(m_playStatus);

    } /*else {
		printf(
				"No AVRCP playing track of %02X:%02X:%02X:%02X:%02X:%02X is currently selected.\n\n",
				ba.b[0], ba.b[1], ba.b[2], ba.b[3], ba.b[4], ba.b[5]);
    }*/

	return HSAE_CBK_SUCCESS;
}
HSAE_CBK_RET avrcp_capability(void *cbkParam, const BT_ADDR ba,
        const unsigned char capability_count, const unsigned char eventID[])
{

    LOG(BT_LOG_DEVICE,"capability  has  %d ",capability_count );
    unsigned char i;
    unsigned int interval_sec;
    std::shared_ptr<MediaPlayBackStubImp> m_meida =MediaPlayBackStubImp::getInstance();
    for( i = 0; i < capability_count; i++ ) {
//        if(avrcp_registing) {
            interval_sec = (event_table_t[eventID[i]].id == EVENT_PLAYBACK_POS_CHANGED) ? DEFAULT_POS_INTERVAL : 0;
            hsaeCmdRegisterEventWatcher(NULL,(BT_ADDR *)&ba, (HSAE_AVRCP_SUPPORTED_EVENTS)eventID[i],interval_sec);
          // cout<<"avrcp_capability" <<(HSAE_AVRCP_SUPPORTED_EVENTS)eventID[i]<<endl;
    }
    m_meida->Aapability(capability_count,eventID);
    return HSAE_CBK_SUCCESS;
}
HSAE_CBK_RET avrcp_event_PlayStatusPrefer(void *HSAECbkParam, const BT_ADDR ba,
		const HSAE_AVRCP_PLAY_STATE state) {
	std::shared_ptr<MediaPlayBackStubImp> m_meida =
			MediaPlayBackStubImp::getInstance();

  bool playstate =false;
    LOG(BT_LOG_MEDIA,"state =%d",(int)state);
 if (state == HSAE_PLAY_STATE_PLAYING)
 {
    	playstate = true;

 }
     hsaeCmdGetNowPlayingAttributes(NULL, (BT_ADDR *)&ba);
     m_meida->TrancStreamStatus(playstate);

	return HSAE_CBK_SUCCESS;
}

MediaPlayBackStubImp::MediaPlayBackStubImp() {
//	hsaeCmdSetCbk_A2DPStatus( NULL, a2dp_status);

//	hsaeCmdSetCbk_AVRCPStatus( NULL, avrcp_status);
	hsaeCmdSetCbk_AVRCPNowPlaying( NULL, avrcp_now_playing);
    hsaeCmdSetCbk_Event_PlaybackStatusChanged(NULL, avrcp_event_PlaybackStatusChanged);
    hsaeCmdSetCbk_Event_PlaybackPosChanged( NULL,avrcp_event_PlaybackPosChanged);
	hsaeCmdSetCbk_Event_PlayStatusPrefer( NULL, avrcp_event_PlayStatusPrefer);
	hsaeCmdSetCbk_Event_TrackChanged( NULL, avrcp_event_TrackChanged);
    hsaeCmdSetCbk_AVRCPGetCapability( NULL, avrcp_capability );
    hsaeCmdSetCbk_AVRCPPlayerSettingValue ( NULL, avrcp_setting_value ) ;
    hsaeCmdSetCbk_AVRCPGetCurrentSetting( NULL, avrcp_get_setting );
    m_mutestate =true;
    m_musicmoment =0;
}

std::shared_ptr<MediaPlayBackStubImp> MediaPlayBackStubImp::getInstance() {
	if (mediaPlayInstance == NULL) {
		mediaPlayInstance = make_shared<MediaPlayBackStubImp>();
		mediaPlayInstance->getInstance();
	}
	return mediaPlayInstance;

}
MediaPlayBackStubImp::~MediaPlayBackStubImp() {

}

void MediaPlayBackStubImp::on_bus_acquired(GDBusConnection *connection,
		const gchar *name, gpointer user_data) {
	GError **error;

//创建一个skeleton对象,以注册输出对象
	m_interface = media_skeleton_new();

//利用g_signal_connect连接回调函数进行方法处理
	g_signal_connect(m_interface, "handle-next-song", //signal_name, 在.c文件中的_default_init查找
			G_CALLBACK(next),//具体的方法处理函数, 注意增加输入参数
			user_data);
	g_signal_connect(m_interface, "handle-pause-music", //signal_name, 在.c文件中的_default_init查找
			G_CALLBACK(pause),//具体的方法处理函数, 注意增加输入参数
			user_data);
	g_signal_connect(m_interface, "handle-play-music", //signal_name, 在.c文件中的_default_init查找
			G_CALLBACK(play),//具体的方法处理函数, 注意增加输入参数
			user_data);
	g_signal_connect(m_interface, "handle-last-song", //signal_name, 在.c文件中的_default_init查找
			G_CALLBACK(previous),//具体的方法处理函数, 注意增加输入参数
			user_data);
	g_signal_connect(m_interface, "handle-mute-music", //signal_name, 在.c文件中的_default_init查找
			G_CALLBACK(mute),//具体的方法处理函数, 注意增加输入参数
			user_data);
    g_signal_connect(m_interface, "handle_fast_forwardstart", //signal_name, 在.c文件中的_default_init查找
            G_CALLBACK(fastForwardstart),//具体的方法处理函数, 注意增加输入参数
			user_data);
    g_signal_connect(m_interface, "handle_fast_forwardstop", //signal_name, 在.c文件中的_default_init查找
            G_CALLBACK(fastForwardstop),//具体的方法处理函数, 注意增加输入参数
            user_data);
    g_signal_connect(m_interface, "handle-rewindstart", //signal_name, 在.c文件中的_default_init查找
            G_CALLBACK(rewindstart),//具体的方法处理函数, 注意增加输入参数
			user_data);
    g_signal_connect(m_interface, "handle-rewindstop", //signal_name, 在.c文件中的_default_init查找
            G_CALLBACK(rewindstop),//具体的方法处理函数, 注意增加输入参数
            user_data);
    g_signal_connect(m_interface, "handle_setplayer", //signal_name, 在.c文件中的_default_init查找
            G_CALLBACK(setplayer),//具体的方法处理函数, 注意增加输入参数
            user_data);
	media_set_music_mute_state(m_interface, m_mutestate);

//输出interface到总线上
	if (!g_dbus_interface_skeleton_export(
			G_DBUS_INTERFACE_SKELETON(m_interface), connection,
			"/com/hsaeyz/bluetooth/media", //object_path
			error)) {
		/* handle error */
	}

}

gboolean MediaPlayBackStubImp::next(Media *interface,GDBusMethodInvocation *invocation)
{
     unsigned long temp =GetTickCount()-m_musicmoment;
     LOG(BT_LOG_MEDIA,"timeelasp =%ld",temp);
//    if (temp>1000)
//    {

//        hsaeCmdAVRCPControlCommand( NULL, &DeviceStubImp::hfp_connect_addr,
//            HSAE_PLAY_OP_FWD);

//        m_musicmoment =GetTickCount();
//    }
//    char test[100]={0};
//    memset(test,10,100);
//    test[0]=0x44;
//    test[1]=0x4C;
//    test[2]=0x36;
//    test[3]=0x0;
//    test[4]=0x01;
//    test[5]=0x0;
//    test[6]=0x0;
//    test[7]=0x0;
//    test[8]=0x0;
//    test[9]=0x0D;
//    test[10]=0x0A;

//    SPPStubImp::btSppInstance->senddata(NULL,test,100);
    media_complete_next_song(interface, invocation);
	return TRUE;
}
gboolean MediaPlayBackStubImp::pause(Media *interface,GDBusMethodInvocation *invocation)
{
   unsigned long temp =GetTickCount()-m_musicmoment;
   LOG(BT_LOG_MEDIA,"timeelasp =%ld",temp);
   if (temp>1000)
   {
        hsaeCmdAVRCPControlCommand( NULL, &DeviceStubImp::hfp_connect_addr,
                HSAE_PLAY_OP_PAUSE);

         m_musicmoment =GetTickCount();
   }
   media_complete_pause_music(interface, invocation);
   return TRUE;
}
gboolean MediaPlayBackStubImp::play(Media *interface,GDBusMethodInvocation *invocation)
{
    unsigned long temp =GetTickCount()-m_musicmoment;
    LOG(BT_LOG_MEDIA,"timeelasp =%ld",temp);
    if (temp>1000)
    {
        hsaeCmdAVRCPControlCommand( NULL, &DeviceStubImp::hfp_connect_addr,
                HSAE_PLAY_OP_PLAY);


        m_musicmoment =GetTickCount();
    }
      media_complete_play_music(interface, invocation);
	return TRUE;
}
gboolean MediaPlayBackStubImp::previous(Media *interface,GDBusMethodInvocation *invocation)
{
    unsigned long temp =GetTickCount()-m_musicmoment;
    LOG(BT_LOG_MEDIA,"timeelasp =%ld",temp);
    if (temp>1000)
    {
        hsaeCmdAVRCPControlCommand( NULL, &DeviceStubImp::hfp_connect_addr,
                HSAE_PLAY_OP_BWD);

       m_musicmoment =GetTickCount();
    }
    media_complete_last_song(interface, invocation);
	return TRUE;
}
gboolean MediaPlayBackStubImp::mute(Media *interface,GDBusMethodInvocation *invocation,gboolean arg_state)
{
    LOG(BT_LOG_MEDIA,"arg_state =%d",arg_state);

    HSAE_CMD_RET ret2 = hsaeCmdA2dpMute(NULL, &DeviceStubImp::hfp_connect_addr, false);

//    bool mute;

//    HSAE_CMD_RET cmd_ret = hsaeCmdA2dpGetMuteStatus( NULL, &DeviceStubImp::hfp_connect_addr, &mute );
//    LOG(BT_LOG_MEDIA,"arg_state2=%d",mute);
    if (ret2 == HSAE_CMD_SUCCESS )
       media_set_music_mute_state(interface,arg_state);
	media_complete_mute_music(interface, invocation);
    return TRUE;;
}

gboolean MediaPlayBackStubImp::fastForwardstart(Media *interface,GDBusMethodInvocation *invocation)
{
    unsigned long temp =GetTickCount()-m_musicmoment;
    LOG(BT_LOG_MEDIA,"timeelasp =%ld",temp);
    bool ret1 = false;
    if (temp>1000)
    {
        HSAE_CMD_RET ret2  = hsaeCmdAVRCPControlCommand( NULL, &DeviceStubImp::hfp_connect_addr,
                HSAE_PLAY_OP_START_FAST_FWD);
         ret1 = (ret2==HSAE_CMD_SUCCESS)?true:false;
         m_musicmoment =GetTickCount();
    }
     media_complete_fast_forwardstart(interface, invocation);
    return TRUE;

}
gboolean MediaPlayBackStubImp::fastForwardstop(Media *interface, GDBusMethodInvocation *invocation)
 {
    unsigned long temp =GetTickCount()-m_musicmoment;
    LOG(BT_LOG_MEDIA,"timeelasp =%ld",temp);
     bool ret1 = false;
//    if (temp>1000)
//    {
        HSAE_CMD_RET ret2 = hsaeCmdAVRCPControlCommand( NULL, &DeviceStubImp::hfp_connect_addr,
                HSAE_PLAY_OP_STOP_FAST_FWD);
         ret1 = (ret2==HSAE_CMD_SUCCESS)?true:false;
         m_musicmoment =GetTickCount();
//    }
     media_complete_fast_forwardstop(interface, invocation);
    return TRUE;

}
gboolean MediaPlayBackStubImp::rewindstart(Media *interface,GDBusMethodInvocation *invocation)
{
    unsigned long temp =GetTickCount()-m_musicmoment;
    LOG(BT_LOG_MEDIA,"timeelasp =%ld",temp);
     bool ret1 = false;
    if (temp>1000)
    {
        HSAE_CMD_RET ret2 = hsaeCmdAVRCPControlCommand( NULL, &DeviceStubImp::hfp_connect_addr,
                HSAE_PLAY_OP_START_REV);

         m_musicmoment =GetTickCount();
         ret1 = (ret2==HSAE_CMD_SUCCESS)?true:false;
    }
    media_complete_rewindstart(interface, invocation);
    return TRUE;
}
gboolean MediaPlayBackStubImp::rewindstop(Media *interface, GDBusMethodInvocation *invocation)
 {
    unsigned long temp =GetTickCount()-m_musicmoment;
    LOG(BT_LOG_MEDIA,"timeelasp =%ld",temp);
      bool ret1 = false;
//    if (temp>1000)
//    {
        HSAE_CMD_RET ret2 = hsaeCmdAVRCPControlCommand( NULL, &DeviceStubImp::hfp_connect_addr,
                HSAE_PLAY_OP_STOP_REV);
         ret1 = (ret2==HSAE_CMD_SUCCESS)?true:false;
         m_musicmoment =GetTickCount();
//    }
    media_complete_rewindstop(interface, invocation);
    return TRUE;
}
gboolean MediaPlayBackStubImp::setplayer(Media *interface,GDBusMethodInvocation *invocation,gint id,gint value)
{
       LOG(BT_LOG_MEDIA,"setplayer =%d, value =%d ",id,value);

       media_complete_setplayer(interface, invocation);
       return TRUE;
}




void MediaPlayBackStubImp::TrancNowPlayAttributes(PlayAttribute playAttribute) {
	const gchar *arg_title = playAttribute.title.c_str();
	const gchar *arg_artist = playAttribute.artist.c_str();
	const gchar *arg_album = playAttribute.album.c_str();
	const gchar *arg_genre = playAttribute.genre.c_str();
	gint arg_trackNumber = playAttribute.trackNumber;
	if (m_interface != NULL) {
		media_emit_play_attributes_changed(m_interface, arg_title, arg_artist,arg_album, arg_genre, arg_trackNumber);
				
	}
}
void MediaPlayBackStubImp::TrancPlayStatus(PlayStatus playStatus) {
	gint arg_playState = (int) playStatus.playState;
	const gchar *arg_songLength = playStatus.positon.songLength.c_str();
	const gchar *arg_playPosition = playStatus.positon.songPosition.c_str();
	if (m_interface != NULL) {
		media_emit_play_status_changed(m_interface, arg_playState,
				arg_songLength, arg_playPosition);
	}
}
void MediaPlayBackStubImp::TrancStreamStatus(const bool streamStatus) {
//  fireStreamStatusChangeEvent(streamStatus);
	if (m_interface != NULL) {
        cout<<"MediaPlayBackStubImp::TrancStreamStatus"<<endl;
		media_emit_stream_status_changed(m_interface, streamStatus);
    }
}

void MediaPlayBackStubImp::Aapability(const unsigned char capability_count, const unsigned char eventID[])
{
     LOG(BT_LOG_DEVICE,"capability  has  %d ",capability_count );
//     GVariant * buf=NULL;
//     memcpy(buf,eventID,capability_count);
     gchar arg_eventname[capability_count+1]={0} ;
     memcpy(&arg_eventname,eventID,capability_count);
     LOG(BT_LOG_DEVICE,"capability  has11  %d ",capability_count );
     if (m_interface != NULL) {
         media_emit_capability_changed(m_interface,(int)capability_count,arg_eventname);
         }
}

unsigned long MediaPlayBackStubImp::GetTickCount()
{
    struct timespec ts;
    clock_gettime( CLOCK_MONOTONIC, &ts );
    unsigned long temp = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    return temp;
}

