#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <pthread.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "AvcVideo.h"   
#include "JenDebug.h"

//===============================// AVC Player Parameters
typedef struct
{
    GstElement *pTinyMediaPlayer;
    GstElement *pAppSource;
    GstElement *pParseCapFilter;
    GstElement *pH264Parse;
    GstElement *pVpuDec;
    GstElement *pVideoSink;
    GstElement *pAudioSink;

    char 		*pURI;

    int 		iAppsrcBlockSize;
    int 		iAppsrcMaxBytes;
    bool		bAppsrcIsLive;
    int			iAppsrcMinPercent;

    bool		bNeedData;

    GstBuffer 	*gstVideoBuffer;

    bool		bNeedRecord;

    guint64 	u64V4lSinkMaxLateness;
    int			iV4lSinkBlocksize;
}stMediaParam;

int iAvcStreamLogLevel = WARN_LEVEL;
stMediaParam g_PlayerParam;

#define DEF_CARPLAY_MODE    0
#define DEF_AIRPLAY_MODE    1

static char LastNALHeader[MAX_NAL_SIZE + 1] = {0, 0, 0, 0, 0};
static bool bIsFisrtNAL = true;
static int iGstVideoStoped = true;
static bool isInited = false;

static int g_iCarPlayType = DEF_CARPLAY_MODE;

#ifdef SAL_PLATFORM_ATLAS7
static char cInitSps = 0;
#endif

//#define USE_CAPS_FILTER_PLUGIN
//#define DEF_DEBUG_TIME
//#define DEF_BUILD_APP
//#define SAL_PLATFORM_ATLAS7
//#define SAL_PLATFORM_IMX6

//======================================================================// Old interface
int InitMediaPlayer(const char *strVideoSink, const char *strAudioSink, int iFlags, bool bSync)
{
    return InitStreamPlayer(bSync);
}

int UninitCarPlay(void)
{
    return UninitStreamPlayer();
}

int StartCarPlayVideo(stVideoAxis *DispAxis, int bShowSurface)
{
    return StartVideoStream(DispAxis, bShowSurface);
}

void StopCarPlayVideo(void)
{
    StopVideoStream();
}

int SetVideoSurfaceAlpha(int iAlpha)
{
    return 0;
}

/*=========================================================================:

==========================================================================*/
int FeedH264ToPlayer(void *pVideoBuffer, int iBufferSize)
{
#ifdef DEF_DEBUG_TIME
    struct timeval StartTime, FinishTime;
    int iTimeDiv = 0;
    gettimeofday(&StartTime, NULL);
#endif

    LOG("feed start\n");

    static FILE *fp = NULL;
    GstFlowReturn gstResult;
    static int WriteCnt = 0;
    const char *pNALBuffer = NULL;

    if(NULL == pVideoBuffer)
    {
        ERROR("Video buffer address should great than 0!\n");
        return -1;
    }

    if(NULL == g_PlayerParam.pAppSource)
    {
        ERROR("Video srouce has not been initalized!\n");
        return -2;
    }

    if(iBufferSize <= 0)
    {
        ERROR("iBufferSize should not be less than 1!\n");
        return -3;
    }

	pNALBuffer = pVideoBuffer;
	if(  ( (0x7 == (0x1F & pNALBuffer[4])) || (0x8 == (0x1F & pNALBuffer[4])) ) //sps/pps
		&& (0x00 == pNALBuffer[0])
		&& (0x00 == pNALBuffer[1])
		&& (0x00 == pNALBuffer[2])
		&& (0x01 == pNALBuffer[3]) )
	{

/*针对A7平台carlife黑屏问题做walkaround方案
**原因:sps的level id变化，高通硬解码无法在初始化后reconfigure
**方案:如果检测到sps的level　id发生变化，则修改为初始化时的id
*/
#ifdef SAL_PLATFORM_ATLAS7
		char *pTmpNalBuffer = NULL;

		if(0x7 == (0x1F & pNALBuffer[4]) && !cInitSps)
		{
			cInitSps = pNALBuffer[7];
		}
		if(0x7 == (0x1F & pNALBuffer[4]) && cInitSps != pNALBuffer[7] )
		{
			pTmpNalBuffer = pVideoBuffer;
			WARN("sps level id change(%#X),reset level id to %#X",pTmpNalBuffer[7],cInitSps);		
			pTmpNalBuffer[7] = cInitSps;
		}
#endif

		if (0 == memcmp(LastNALHeader, pVideoBuffer, iBufferSize))
		{
			INFO("###### The same NAL Header found!\n");
			return 0;
		}
		else
		{
			if(true == bIsFisrtNAL)
			{
				INFO("--> First NAL Header.\n");
				bIsFisrtNAL = false;
			}
			else
			{
				INFO("--> New NAL Header.\n");
				if(DEF_AIRPLAY_MODE == g_iCarPlayType)
				{
				    INFO("A Mode, we will restart renderor!\n");
				    StopVideoStream();             //--jerry for airplay
				    StartVideoStream(NULL, -1);    //--jerry for airplay
				}
			}

            if(iBufferSize > MAX_NAL_SIZE)
            {
                WARN("NAL size is too large: %d !\n", iBufferSize);
                memcpy(LastNALHeader, pVideoBuffer, MAX_NAL_SIZE);
            }
            else
            {
                memcpy(LastNALHeader, pVideoBuffer, iBufferSize);
            }
        }
    }

    // copy video data
    GstMapInfo BufferMap;

    g_PlayerParam.gstVideoBuffer = gst_buffer_new_and_alloc(iBufferSize);

    gst_buffer_map (g_PlayerParam.gstVideoBuffer, &BufferMap, GST_MAP_WRITE);
    memcpy(BufferMap.data, pVideoBuffer, iBufferSize);
    gst_buffer_unmap (g_PlayerParam.gstVideoBuffer, &BufferMap);
    // wait for need data
    while(false == g_PlayerParam.bNeedData)
    {
        usleep(1000);
        printf(".");
        if(true == iGstVideoStoped)
        {
            DEBUG("Gst Video Stoped!\n");
            gst_buffer_unref(g_PlayerParam.gstVideoBuffer);
            return -5;
        }
    }
    // push data to gstreamer
    DEBUG("bm_push: %d\n", gst_buffer_get_size(g_PlayerParam.gstVideoBuffer));
    g_signal_emit_by_name(g_PlayerParam.pAppSource, "push-buffer", g_PlayerParam.gstVideoBuffer, &gstResult);
    if(GST_FLOW_OK != gstResult)
    {
        ERROR("\n=================================================> Push buffer failed!\n");
    }
    gst_buffer_unref(g_PlayerParam.gstVideoBuffer);

    //g_PlayerParam.bNeedData = false;

    if(true == g_PlayerParam.bNeedRecord)
    {
        if(NULL == fp)
        {
            fp = fopen("/usr/app/gstCarplay.rec", "w");
            if(NULL == fp)
            {
                ERROR("Open file for AVC record failed!\n");
            }
        }

        if(NULL != fp)
        {
            fwrite(pVideoBuffer, 1, iBufferSize, fp);
            DEBUG("==> Rec: %d\n", iBufferSize);
            WriteCnt++;
            if (0 == (WriteCnt % 10))
            {
                DEBUG("==> flush Video data!");
                fflush(fp);
            }
        }
    }
#ifdef DEF_DEBUG_TIME
    gettimeofday(&FinishTime, NULL);
    iTimeDiv = 1000000*(FinishTime.tv_sec - StartTime.tv_sec) + (FinishTime.tv_usec - StartTime.tv_usec);
    if(iTimeDiv >= 200)
    {
        LOG("tdiv>200us: %d\n", iTimeDiv);
    }
#endif

    return 0;
}

/*=========================================================================:

==========================================================================*/
static void callbackNeedDataAppsource(GstElement* appsource,
                                      guint arg0,
                                      gpointer user_data)
{
    DEBUG("Need data!\n");
    g_PlayerParam.bNeedData = true;
}

/*=========================================================================:

========================================================================.==*/
static void callbackEnoughDataAppsource(GstElement* object,
                                        gpointer user_data)
{
    DEBUG("Enough data!\n");
#ifdef DEF_BUILD_APP
    g_PlayerParam.bNeedData = false;
#endif
}

/*=========================================================================:

==========================================================================*/
int StartVideoStream(stVideoAxis *DispAxis, int bShowSurface)
{
    INFO("StartCarPlayVideo..., show: %d\n", bShowSurface);

    SetDisplayAxis(DispAxis);

    gst_element_set_state(g_PlayerParam.pTinyMediaPlayer, GST_STATE_PLAYING);

    iGstVideoStoped = false;

    INFO("StartCarPlayVideo OK\n");

    return 0;
}

/*=========================================================================:

==========================================================================*/
void StopVideoStream(void)
{
    INFO("StopCarPlayVideo...\n");

    gst_element_set_state(g_PlayerParam.pTinyMediaPlayer, GST_STATE_NULL);
    //gst_element_get_state(g_PlayerParam.pTinyMediaPlayer, NULL, NULL, 5);

    memset(LastNALHeader, 0, MAX_NAL_SIZE);
    if(DEF_CARPLAY_MODE == g_iCarPlayType)
    {
        INFO("C Mode, we will set bIsFisrtNAL!\n");
        bIsFisrtNAL = true;
    }
    else if(DEF_AIRPLAY_MODE == g_iCarPlayType)
    {
        INFO("A Mode, we will not set bIsFisrtNAL!\n");
    }

    iGstVideoStoped = true;

    INFO("StopCarPlayVideo OK\n");
}
/**
 * Call this to set appsrc
 * @return true if success, false if not.
 */
static bool  setAppSrc(){
    char *pStrEnv = NULL;
    guint64 u64MaxBytes = 0;

    // default value
    g_PlayerParam.bNeedData = false;
    g_PlayerParam.gstVideoBuffer = NULL;

    g_PlayerParam.pURI = NULL;

    g_PlayerParam.iAppsrcBlockSize = 100*1024;
    g_PlayerParam.iAppsrcMaxBytes = -1;
    g_PlayerParam.bAppsrcIsLive = true;
    g_PlayerParam.iAppsrcMinPercent = 0;

    // Get value from env
    if(NULL != (pStrEnv = getenv("JEN_AVCPLY_APPSRC_BLKSIZE")))
    {
        g_PlayerParam.iAppsrcBlockSize = atoi(pStrEnv);
    }
    INFO("iAppsrcBlockSize: %d\n", g_PlayerParam.iAppsrcBlockSize);

    if(NULL != (pStrEnv = getenv("JEN_AVCPLY_APPSRC_MAXBS")))
    {
        g_PlayerParam.iAppsrcMaxBytes = atoi(pStrEnv);
    }
    INFO("iAppsrcMaxBytes: %d\n", g_PlayerParam.iAppsrcMaxBytes);

    if(NULL != (pStrEnv = getenv("JEN_AVCPLY_APPSRC_LIVE")))
    {
        g_PlayerParam.bAppsrcIsLive = atoi(pStrEnv);
    }
    INFO("bAppsrcIsLive: %d\n", g_PlayerParam.bAppsrcIsLive);

    if(NULL != (pStrEnv = getenv("JEN_AVCPLY_APPSRC_MINPCT")))
    {
        g_PlayerParam.iAppsrcMinPercent = atoi(pStrEnv);
    }
    INFO("iAppsrcMinPercent: %d\n", g_PlayerParam.iAppsrcMinPercent);

    // set appsrc
    g_PlayerParam.pAppSource = gst_element_factory_make("appsrc", "TinyAppsrc");
    if(NULL == g_PlayerParam.pAppSource)
    {
        ERROR("pAppSource == NULL!\n");
        return false;
    }

    INFO("appsrc blocksize: %d\n", g_PlayerParam.iAppsrcBlockSize);
    g_object_set(g_PlayerParam.pAppSource, "blocksize", g_PlayerParam.iAppsrcBlockSize, NULL);

    if(g_PlayerParam.iAppsrcMaxBytes >= 0)
    {
        INFO("Now set max-bytes: %d\n", g_PlayerParam.iAppsrcMaxBytes);
        g_object_set(g_PlayerParam.pAppSource, "max-bytes", g_PlayerParam.iAppsrcMaxBytes, NULL);

        DEBUG("Now get max-bytes...\n");

        g_object_get(G_OBJECT(g_PlayerParam.pAppSource), "max-bytes", &u64MaxBytes, NULL);
        INFO("max-bytes: %llu !\n", u64MaxBytes);
    }
    INFO("appsrc is-live: %d\n", g_PlayerParam.bAppsrcIsLive);
    g_object_set(g_PlayerParam.pAppSource, "is-live", g_PlayerParam.bAppsrcIsLive, NULL);

    INFO("appsrc min-percent: %d\n", g_PlayerParam.iAppsrcMinPercent);
    g_object_set(g_PlayerParam.pAppSource, "min-percent", g_PlayerParam.iAppsrcMinPercent, NULL);
    g_signal_connect(g_PlayerParam.pAppSource, "need-data", G_CALLBACK(callbackNeedDataAppsource), NULL);
    g_signal_connect(g_PlayerParam.pAppSource, "enough-data", G_CALLBACK(callbackEnoughDataAppsource), NULL);

    return true;
}

/**
 * Call this to set Transform
 * @return true if success, false if not.
 */
static bool setTransform(){
    char *pStrEnv = NULL;
    GstCaps *x264Caps;
    g_PlayerParam.pParseCapFilter = gst_element_factory_make("capsfilter", "TinyParseCapFilter");
    if(NULL == g_PlayerParam.pParseCapFilter)
    {
        ERROR("pParseCapFilter == NULL!\n");
        return false;
    }
    x264Caps = gst_caps_from_string("video/x-h264, parsed=true, alignment=au, stream-format=byte-stream");
    g_object_set(g_PlayerParam.pParseCapFilter, "caps", x264Caps, NULL);
    gst_caps_unref(x264Caps);
    return true;
}
/**
 * Call this to set Parse
 * @return true if success, false if not.
 */
static bool setParse(){
    g_PlayerParam.pH264Parse = gst_element_factory_make("h264parse", "TinyH264Parse");
    if(NULL == g_PlayerParam.pH264Parse)
    {
        ERROR("pH264Parse == NULL!\n");
        return false;
    }
    return true;
}
/**
 * Call this to set VideoDecoder
 * @return true if success, false if not.
 */
static bool setVideoDecoder(){
    char *pStrEnv = NULL;

    // set VideoDecoder
    if(NULL != (pStrEnv = getenv("JEN_AVCPLY_AVC_DEC")))
    {
        INFO("Use AvcDec plugin as JEN_AVCPLY_AVC_DEC: %s\n", pStrEnv);
        g_PlayerParam.pVpuDec = gst_element_factory_make(pStrEnv, "TinyVpuDec");
    }
    else
    {
#ifdef SAL_PLATFORM_ATLAS7
        g_PlayerParam.pVpuDec = gst_element_factory_make("omxh264dec", "TinyVpuDec");
#elif defined(SAL_PLATFORM_IMX6)
        g_PlayerParam.pVpuDec = gst_element_factory_make("vpudec", "TinyVpuDec");
#else
#error "SAL_PLATFORM_X is not defined"
#endif
    }
    if(NULL == g_PlayerParam.pVpuDec)
    {
        ERROR("pVpuDec == NULL!\n");
        return false;
    }

#ifdef SAL_PLATFORM_IMX6
    if(NULL != (pStrEnv = getenv("JEN_AVCPLY_VPU_FRAMEPLUS")))
    {
        int iVpudecFramePlus = atoi(pStrEnv);
        INFO("JEN_AVCPLY_VPU_FRAMEPLUS: %d\n", iVpudecFramePlus);
        g_object_set(g_PlayerParam.pVpuDec, "frame-plus", iVpudecFramePlus, NULL);
    }
#endif
    if(NULL != (pStrEnv = getenv("JEN_AVCPLY_VPU_REORDER")))
    {
        int iVpudecReorder = atoi(pStrEnv);
        INFO("JEN_AVCPLY_VPU_REORDER: %d\n", iVpudecReorder);
        g_object_set(g_PlayerParam.pVpuDec, "enable-reorder", iVpudecReorder, NULL);
    }
    return true;
}

/**
 * Call this to set videosink
 * @param bSyncOnClock  Sync on the clock
                        flags: readable, writable
                        Boolean. Default: true
 * @return true if success, false if not.
 */
static bool setVideoSink(bool bSyncOnClock){
    char *pStrEnv = NULL;
    // default value
    g_PlayerParam.u64V4lSinkMaxLateness = 5000000000;//60000000000;
    g_PlayerParam.iV4lSinkBlocksize = 100*1024;

    // Get value from env
    if(NULL != (pStrEnv = getenv("JEN_AVCPLY_SINK_MAXLTS")))
    {
        g_PlayerParam.u64V4lSinkMaxLateness = atol(pStrEnv);
    }
    INFO("u64MfwSinkMaxLateness: %llu\n", g_PlayerParam.u64V4lSinkMaxLateness);

    if(NULL != (pStrEnv = getenv("JEN_AVCPLY_MFWSINK_BLKSIZE")))
    {
        g_PlayerParam.iV4lSinkBlocksize = atoi(pStrEnv);
    }
    INFO("iV4lSinkBlocksize: %d\n", g_PlayerParam.iV4lSinkBlocksize);

    // set videosink
    if(NULL != (pStrEnv = getenv("JEN_AVCPLY_VIDEO_SINK")))
    {
        INFO("Use video-sink plugin as JEN_AVCPLY_VIDEO_SINK: %s\n", pStrEnv);
        g_PlayerParam.pVideoSink = gst_element_factory_make(pStrEnv, "TinyVideoSink");
    }
    else
    {
#ifdef SAL_PLATFORM_ATLAS7
        g_PlayerParam.pVideoSink = gst_element_factory_make("v4l2sink", "TinyVideoSink");
#elif defined(SAL_PLATFORM_IMX6)
        g_PlayerParam.pVideoSink = gst_element_factory_make("overlaysink", "TinyVideoSink");
#else
#error "SAL_PLATFORM_X is not defined"
#endif
    }
    if(NULL == g_PlayerParam.pVideoSink)
    {
        ERROR("pVideoSink == NULL!\n");
        return false;
    }

    DEBUG("==> SYNC: %d !\n", bSyncOnClock);
    g_object_set(g_PlayerParam.pVideoSink, "sync", bSyncOnClock, NULL); //--ddd

    INFO("max-lateness: %llu\n", g_PlayerParam.u64V4lSinkMaxLateness);
    g_object_set(g_PlayerParam.pVideoSink, "max-lateness", g_PlayerParam.u64V4lSinkMaxLateness, NULL);

    INFO("v4lsink blocksize: %d\n", g_PlayerParam.iV4lSinkBlocksize);
    g_object_set(g_PlayerParam.pVideoSink, "blocksize", g_PlayerParam.iV4lSinkBlocksize, NULL);

    return true;
}

/*=========================================================================: 

==========================================================================*/
int InitStreamPlayer(bool bSyncOnClock)
{
    printf("\n\e[33m ==> %s: Build Time: %s-%s! \e[0m\n", __FUNCTION__, __DATE__, __TIME__);


    char *pStrEnv = NULL;
    bool bRetValue = false;

    const char *pCarplayMode = NULL;

    if(isInited){
        WARN("%s has already inited!\n",__FUNCTION__);
        return 0;
    }

    gst_init(NULL, NULL);

    memset(&g_PlayerParam, 0, sizeof(stMediaParam));

    SetDebugLogLevel("JEN_AVC_STREAM_LOGLEVEL", WARN_LEVEL);

    pCarplayMode = getenv("JEN_CARPLAY_LIB_MODE");
    if(NULL != pCarplayMode)
    {
        g_iCarPlayType = atoi(pCarplayMode);
    }

    // set record
    g_PlayerParam.bNeedRecord = false;
    if(NULL != (pStrEnv = getenv("JEN_AVCPLY_RECORD")))
    {
        g_PlayerParam.bNeedRecord = atoi(pStrEnv);
    }
    INFO("bNeedRecord: %d\n", g_PlayerParam.bNeedRecord);

    //======================================================// pipeline
    g_PlayerParam.pTinyMediaPlayer = gst_pipeline_new("TinyMediaPlayer");
    if(NULL == g_PlayerParam.pTinyMediaPlayer)
    {
        ERROR("pTinyMediaPlayer == NULL!\n");
        return -1;
    }

    if(!setAppSrc()) return -1;
    if(!setTransform()) return -1;
    if(!setParse()) return -1;
    if(!setVideoDecoder()) return -1;
    if(!setVideoSink(bSyncOnClock)) return -1;

    //======================================================// Link pipeline
    gst_bin_add_many(GST_BIN(g_PlayerParam.pTinyMediaPlayer),
                     g_PlayerParam.pAppSource,
                     g_PlayerParam.pH264Parse,
                     g_PlayerParam.pParseCapFilter,
                     g_PlayerParam.pVpuDec,
                     g_PlayerParam.pVideoSink,
                     NULL);

    bRetValue = gst_element_link_many(
                g_PlayerParam.pAppSource,
                g_PlayerParam.pH264Parse,
                g_PlayerParam.pParseCapFilter,
                g_PlayerParam.pVpuDec,
                g_PlayerParam.pVideoSink,
                NULL);

    if(false == bRetValue)
    {
        ERROR("==> gst_element_link_many failed!\n");
        return -1;
    }

    isInited = true;

    return 0;
}

/*=========================================================================: 

==========================================================================*/
int UninitStreamPlayer(void)
{
    DEBUG("UninitStreamPlayer...\n");

#ifdef DEF_DEBUG_TIME
    struct timeval StartTime, FinishTime;
    gettimeofday(&StartTime, NULL);
    DEBUG ("Start: %d-%d\n", StartTime.tv_sec, StartTime.tv_usec);
#endif

	StopVideoStream();
#ifdef SAL_PLATFORM_ATLAS7
	cInitSps = 0;
#endif
	gst_object_unref(g_PlayerParam.pTinyMediaPlayer);

#ifdef DEF_DEBUG_TIME
    gettimeofday(&FinishTime, NULL);
    DEBUG ("Finish: %d-%d\n", FinishTime.tv_sec, FinishTime.tv_usec);
#endif

    isInited = false;

    return 0;
}

/*=========================================================================:

==========================================================================*/
int SetDisplayAxis(stVideoAxis *DisplayAxis)
{
    if(NULL == DisplayAxis)
    {
        DEBUG("SetDisplayAxis: NULL parameter, we will do nothing!\n");
        return 0;
    }

    DEBUG("Set surface window: %d, %d, %d, %d\n", DisplayAxis->axis_left, DisplayAxis->axis_top, DisplayAxis->disp_width, DisplayAxis->disp_height);
    g_object_set(G_OBJECT(g_PlayerParam.pVideoSink), "overlay-left", DisplayAxis->axis_left, NULL);
    g_object_set(G_OBJECT(g_PlayerParam.pVideoSink), "overlay-top", DisplayAxis->axis_top, NULL);
    g_object_set(G_OBJECT(g_PlayerParam.pVideoSink), "overlay-width", DisplayAxis->disp_width, NULL);
    g_object_set(G_OBJECT(g_PlayerParam.pVideoSink), "overlay-height", DisplayAxis->disp_height, NULL);

    return 0;
}
