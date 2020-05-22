#ifndef _MEDIAPLAYBACKSTUBIMP_H_
#define _MEDIAPLAYBACKSTUBIMP_H_
//#include "com/hsae/bluetooth/MediaPlayBack/MediaPlayStubDefault.h"
#include "Media/media.h"

#include "HsaeTypes.h"
#include <iostream>
#include <memory>
#include <stdio.h>
#include "DeviceStubImp.h"
using namespace std;
using namespace com::hsae::bluetooth::util::HsaeTypes;

#define MEDIA_PLAYER_PROPERTIES_STATUS              "Status"
/*Media Player property*/
#define MEDIA_PLAYER_PROPERTIES_EQUALIZER           "Equalizer"
#define MEDIA_PLAYER_PROPERTIES_REPEAT              "Repeat"
#define MEDIA_PLAYER_PROPERTIES_SHUFFLE             "Shuffle"
#define MEDIA_PLAYER_PROPERTIES_SCAN                "Scan"
#define MEDIA_PLAYER_PROPERTIES_STATUS              "Status"
#define MEDIA_PLAYER_PROPERTIES_POSITION            "Position"
#define MEDIA_PLAYER_PROPERTIES_TRACK               "Track"
#define MEDIA_PLAYER_PROPERTIES_DEVICE              "Device"
#define MEDIA_PLAYER_PROPERTIES_NAME                "Name"
#define MEDIA_PLAYER_PROPERTIES_TYPE                "Type"
#define MEDIA_PLAYER_PROPERTIES_SUBTYPE             "Subtype"
#define MEDIA_PLAYER_PROPERTIES_BROWSABLE           "Browsable"
#define MEDIA_PLAYER_PROPERTIES_SEARCHABLE          "Searchable"
#define MEDIA_PLAYER_PROPERTIES_PLAYLIST            "Playlist"

/*Track attribute*/
#define MEDIA_TRACK_ATTR_TITLE                  "Title"
#define MEDIA_TRACK_ATTR_ARTIST                 "Artist"
#define MEDIA_TRACK_ATTR_ALBUM                  "Album"
#define MEDIA_TRACK_ATTR_GENRE                  "Genre"
#define MEDIA_TRACK_ATTR_TRACK_NUMBER           "TrackNumber"
#define MEDIA_TRACK_ATTR_NUMBER_OF_TRACK        "NumberOfTracks"        //total track number
#define MEDIA_TRACK_ATTR_DURATION               "Duration"              //playing time
class MediaPlayBackStubImp //: public MediaPlayStubDefault
{
private:
//    static ExecuteMethodAsyncResultT m_notifyConnectDisconnectResult;
//    static void A2dpConnectResult(bool result, void *user_data);
//    static void A2dpDisconnectResult(bool result, void *user_data);
//    static void A2dpConnectDisconnectAsyncResult(DBusPendingCall *pending, void *user_data);
//    static void AvrcpConnectResult(DBusPendingCall *call, void *user_data);
//    static void AvrcpDisconnectResult(DBusPendingCall *call, void *user_data);
public:
    MediaPlayBackStubImp();
    ~MediaPlayBackStubImp();

    static std::shared_ptr<MediaPlayBackStubImp > mediaPlayInstance;
    static std::shared_ptr<MediaPlayBackStubImp > getInstance();
    static Media* m_interface;
    static gboolean m_mutestate;
    /**
    *commonapi methods and broadcast
    */
//    void fastForward();
    //method
    static gboolean next(Media *interface,GDBusMethodInvocation *invocation);        //nextSong
    static gboolean pause(Media *interface,GDBusMethodInvocation *invocation);       //pauseMusic
    static gboolean play(Media *interface,GDBusMethodInvocation *invocation);        //playMusic
    static gboolean previous(Media *interface,GDBusMethodInvocation *invocation);    //lastSong
    static gboolean mute(Media *interface,GDBusMethodInvocation *invocation,gboolean arg_state);        //muteMusic
    static gboolean fastForwardstart(Media *interface,GDBusMethodInvocation *invocation);
    static gboolean fastForwardstop(Media *interface,GDBusMethodInvocation *invocation);
    static gboolean rewindstart(Media *interface,GDBusMethodInvocation *invocation);
    static gboolean rewindstop(Media *interface,GDBusMethodInvocation *invocation);
   //add by lhm 20170907
    static gboolean setplayer(Media *interface,GDBusMethodInvocation *invocation,gint id,gint value);


    //    void rewind();
//    void stop();
    //signal
    void TrancNowPlayAttributes(PlayAttribute playAttribute); //PlayAttributesChanged
    void TrancPlayStatus(PlayStatus playStatus);              //PlayStatusChanged

    void TrancStreamStatus(bool streamStatus);                                    //streamStatusChanged

    void Aapability(const unsigned char capability_count, const unsigned char eventID[]);
//    void TrancPlayStatus(PlayStatus playStatus);
    static void on_bus_acquired (GDBusConnection *connection,const gchar *name,gpointer user_data);
    /**
     * inner logic method and var
//     */
//    string musicObjPath ="";//a2dp
//    string musicPlayerObjPath  = "";//playerpath
//    static string avrcpConnectObjPath;//avrcpPath
//    void A2dpConnect(const char* path);
//    void A2dpConnectAsync(const char *devicePath, ExecuteMethodAsyncResultT callback);
//    void A2dpDisConnect(const char* path);
//    void A2dpDisConnectAsync(const char *devicePath, ExecuteMethodAsyncResultT callback);
    //avrcp
//    void AvrcpConnect(char* path);
//    void AvrcpDisConnect(char* path);
    static unsigned long m_musicmoment;
    static unsigned long GetTickCount();
};
//}
//}
//}
//}
#endif

