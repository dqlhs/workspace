/*
 ============================================================================
 Name        : Gst_Audio_Capture.c
 Author      : Jerry
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdbool.h>
#include <gst/gst.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
//#include <gst/app/gstappsrc.h>
//#include <gst/app/gstappsink.h>

#include "Gst_Video_Capture.h"
#include "JenDebug.h"

#define MAX_PIPELINES   8

typedef struct{
    GstElement *pPipeLine;
    GstElement *pAlsaSrc;
    GstElement *pAppSink;
    GstElement *pRtpBin;
    GstElement *pRtpDepay;

    int iSampleRate;
    int iChannel;
    int iIndex;

    GstreamerAudioStreamInputCallback_f pAudioStreamCallback;
}GST_PIPELINE_ST;

static GST_PIPELINE_ST g_PipelineProp[MAX_PIPELINES];

int iGstAudioCaptureLogLevel = WARN_LEVEL;

/*=========================================================================:
 *
 * rtp stream Reciever,for recieve rtp stream
 *
==========================================================================*/
static void on_pad_added(GstElement* object, GstPad* pad, gpointer user_data)
{
	INFO("pad added\n");

	GstCaps *caps;
	GstStructure *str;
	gchar *name;
	GstPad *depaysink;
	GstPadLinkReturn ret;
	GstPad *peerpad;
	GstPad *udppad;

	int *pIndex = (int *)user_data;

	name = gst_pad_get_name(pad);

	//g_assert(str);

	//name = (gchar *)gst_structure_get_name(str);
	INFO("pad name:%s\n",name);

	if(g_strrstr(name, "recv_rtp_src"))
	{
		depaysink = gst_element_get_static_pad(g_PipelineProp[*pIndex].pRtpDepay, "sink");
		g_assert(depaysink);
		peerpad = gst_pad_get_peer(depaysink);
		if(peerpad)
		{
			printf("unlink current pad\n");
			gst_pad_unlink(peerpad,depaysink);
			gst_object_unref(peerpad);
		}
		ret = gst_pad_link(pad, depaysink);
		printf("video gst_pad_link return:%d\n",ret);
		gst_object_unref(depaysink);
	}
	/*
	else if(g_strrstr(name, "send_rtp_src"))
	{
		udppad = gst_element_get_static_pad(g_PlayerParam.pUdpRtpsink, "sink");
		g_assert(depaysink);
		ret = gst_pad_link(pad, udppad);
		printf("video gst_pad_link return:%d\n",ret);
		gst_object_unref(udppad);
	}*/
	g_free(name);
	//gst_caps_unref(caps);
}

static void on_pad_removed(GstElement* object, GstPad* pad, gpointer user_data)
{
	INFO("pad removed\n");

	GstCaps *caps;
	GstStructure *str;
	gchar *name;
	GstPad *depaysink;
	GstPadLinkReturn ret;



	name = gst_pad_get_name(pad);

	//g_assert(str);

	//name = (gchar *)gst_structure_get_name(str);
	INFO("pad name:%s\n",name);

	g_free(name);

	//gst_caps_unref(caps);
}

static void on_recievepad_added(GstElement* object, GstPad* pad, gpointer user_data)
{
	INFO("src pad added\n");

	GstCaps *caps;
	GstStructure *str;
	gchar *name;
	GstPad *depaysink;
	GstPadLinkReturn ret;
	GstPad *peerpad;
	GstPad *udppad;
	GstElement *pRtpDepay;

	g_assert(user_data);

	pRtpDepay = (GstElement *)user_data;
	name = gst_pad_get_name(pad);

	//g_assert(str);

	//name = (gchar *)gst_structure_get_name(str);
	INFO("pad name:%s\n",name);

	//if(g_strrstr(name, "recv_rtp_src"))
	//{
		depaysink = gst_element_get_static_pad(pRtpDepay, "sink");
		g_assert(depaysink);

		if(gst_pad_is_linked (depaysink))
		{
			printf("unlink current pad\n");
			peerpad = gst_pad_get_peer(depaysink);
			gst_pad_unlink(peerpad,depaysink);
			gst_object_unref(peerpad);
		}
		ret = gst_pad_link(pad, depaysink);
		printf("video gst_pad_link return:%d\n",ret);
		gst_object_unref(depaysink);
	//}
	/*else if(g_strrstr(name, "send_rtp_src"))
	{
		udppad = gst_element_get_static_pad(g_PlayerParam.pUdpRtpsink, "sink");
		g_assert(depaysink);
		ret = gst_pad_link(pad, udppad);
		printf("video gst_pad_link return:%d\n",ret);
		gst_object_unref(udppad);
	}*/
	g_free(name);
	//gst_caps_unref(caps);
}

static void on_recievepad_removed(GstElement* object, GstPad* pad, gpointer user_data)
{
	GstCaps *caps;
	GstStructure *str;
	gchar *name;
	GstPad *depaysink;
	GstPadLinkReturn ret;

	name = gst_pad_get_name(pad);

	//g_assert(str);
	//name = (gchar *)gst_structure_get_name(str);
	INFO("pad removed,pad name:%s\n",name);
	g_free(name);
/*
    GstElement *decoder = (GstElement *) user_data;
	depaysink = gst_element_get_static_pad (decoder, "sink");

	// If our converter is already linked, we have nothing to do here
	if (gst_pad_is_linked (depaysink)) {
		g_print("proceeding to unlinking ...\n");
	} else {
		g_print("*** We are already unlinked ***\n");
		gst_object_unref (depaysink);
		return;
	}
	ret = gst_pad_unlink (pad, depaysink);

	if (GST_PAD_LINK_FAILED (ret)) {
		//failed
		g_print("failed to unlink dynamically\n");
	} else {
		//pass
		g_print("dynamically unlink successful\n");
	}
	gst_object_unref (depaysink);
*/
	//gst_caps_unref(caps);
}

int bus_call(GstBus *bus, GstMessage *msg, void *data)
{
	printf("Got message\n");
	printf("Got [%s] message\n", GST_MESSAGE_TYPE_NAME (msg));

	GError *err;
	gchar *debug_info;

	switch(GST_MESSAGE_TYPE(msg)){
		case GST_MESSAGE_ERROR:{
			gst_message_parse_error(msg, &err, &debug_info);
			g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
			g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
			g_clear_error(&err);
			g_free(debug_info);

		}
			break;
		case GST_MESSAGE_EOS:{
			g_print("gstMW : End-Of-Stream reached.\n");
		}
			break;
		default:

			break;
	}
	return 1;
}

static GstFlowReturn callbackNewBuffer (GstElement* appsink, gpointer user_data)
{
	GstSample *gstAudioSample = NULL;
	GstBuffer *gstAudioBuffer = NULL;

	//printf("gstAudioSample[%p]\n",gstAudioSample);

    int *pIndex = (int *)user_data;


	gstAudioSample = gst_app_sink_pull_sample (appsink);
	if(!gstAudioSample)
	{
		ERROR("gstAudioSample null\n");
		return -1;
	}
	gstAudioBuffer = gst_sample_get_buffer (gstAudioSample);
	if(!gstAudioBuffer){
		ERROR("gstAudioSample null\n");
		return -1;
	}

    //g_signal_emit_by_name(appsink, "pull-sample", &gstAudioBuffer);
    if(NULL != gstAudioBuffer)
    {
    	GstMapInfo BufferMap;
    	int iBufferSize = gst_buffer_get_size(gstAudioBuffer);
    	INFO("iBufferSize:%d\n",iBufferSize);

    	gst_buffer_map (gstAudioBuffer, &BufferMap, GST_MAP_READ);

        LOG("cap_cb: %d\n", iBufferSize);
        g_PipelineProp[*pIndex].pAudioStreamCallback(user_data, BufferMap.data, iBufferSize);
        //LOG("pAudioStreamCallback\n");
        gst_buffer_unmap (gstAudioBuffer, &BufferMap);
        //LOG("gst_buffer_unmap\n");
        //gst_buffer_unref(gstAudioBuffer);
    }
    else
    {
        DEBUG("Invalid Audio buffer!\n");
    }

    gst_sample_unref (gstAudioSample);

    //LOG("callbackNewBuffer end\n");
    return GST_FLOW_OK;
}


static inline int GetEnvPropInt(const char *pStrEnv, int iDefault)
{
    const char *pEnv = NULL;

    pEnv = getenv(pStrEnv);
    if(NULL != pEnv)
    {
        return atoi(pEnv);
    }
    else
    {
        return iDefault;
    }
}

static int SetPipelineNull2Playing(GstElement *pPipeLine)
{
    GstStateChangeReturn gstStateReturn = GST_STATE_CHANGE_FAILURE;

    gstStateReturn = gst_element_set_state(pPipeLine, GST_STATE_PLAYING);
    switch (gstStateReturn)
    {
    case GST_STATE_CHANGE_SUCCESS:
        DEBUG("Change state successfully!\n");
        break;
    case GST_STATE_CHANGE_ASYNC:
    case GST_STATE_CHANGE_NO_PREROLL:
        DEBUG("Change state async or no preroll!\n");
        gstStateReturn = gst_element_get_state(pPipeLine, NULL, NULL, 3 * GST_SECOND);
        if (GST_STATE_CHANGE_SUCCESS  != gstStateReturn)
        {
            WARN("Set pipeline to playing failed: %d\n", gstStateReturn);
            gstStateReturn = gst_element_set_state(pPipeLine, GST_STATE_NULL);
            if (GST_STATE_CHANGE_SUCCESS  != gstStateReturn)
            {
                WARN("Set pipeline to NULL failed: %d\n", gstStateReturn);
            }
            return -1;
        }
        break;
    case GST_STATE_CHANGE_FAILURE:
        DEBUG("Change state failed!\n");
        gstStateReturn = gst_element_set_state(pPipeLine, GST_STATE_NULL);
        if (GST_STATE_CHANGE_SUCCESS  != gstStateReturn)
        {
            WARN("Set pipeline to NULL failed: %d\n", gstStateReturn);
        }
        return -2;
        break;
    default:
        break;
    }

    return 0;
}

static inline long long GetEnvPropInt64(const char *pStrEnv, int iDefault)
{
    const char *pEnv = NULL;

    pEnv = getenv(pStrEnv);
    if(NULL != pEnv)
    {
        return atoll(pEnv);
    }
    else
    {
        return iDefault;
    }
}

int init_gstreamer_video_capture(int *argc, char **argv[])
{
    printf("\e[33m[Audio Capture] build on %s, %s !\n\e[0m", __DATE__, __TIME__);

    SetDebugLogLevel("JEN_AUDIO_CAPTURE_LOGLEVEL", INFO_LEVEL);

    INFO("Init audio capture!\n");

    gst_init(argc, argv);

    memset(g_PipelineProp, 0, (MAX_PIPELINES * sizeof(GST_PIPELINE_ST)));

    return 0;
}

static int GetPipelineResource(GST_PIPELINE_ST (pPipelineParamsHeader[]))
{
    int iIndex = 0;
    for(iIndex = 0; iIndex < MAX_PIPELINES; iIndex++)
    {
        if(NULL == pPipelineParamsHeader[iIndex].pPipeLine)
        {
            return iIndex;
        }
    }

    return -1;
}

#ifdef VIDEOTESTSRC
int start_gstreamer_video_capture(int *index, const char *device, GstreamerAudioStreamInputCallback_f callback)
{
    gboolean bGstResult = false;
    GstCaps *GstAppSrcCaps = NULL;
    GstCaps *GstAppSinkCaps = NULL;
    int iRet = -1;
    int iPipelineIndex = 0;
    const char *pEnv = NULL;
    int framerate = 0;

    INFO("Start video capture!\n");

    iPipelineIndex = GetPipelineResource(g_PipelineProp);
    if(iPipelineIndex < 0)
    {
        ERROR("GetPipelineResource: too many pipelines!\n");
        return -1;
    }
    else
    {
        DEBUG("GetPResource, iIndex: %d !\n", iPipelineIndex);
    }

    *index = iPipelineIndex;
    memset(&g_PipelineProp[iPipelineIndex], 0, sizeof(GST_PIPELINE_ST));
    g_PipelineProp[iPipelineIndex].iIndex = iPipelineIndex;

    if(NULL != callback)
    {
        g_PipelineProp[iPipelineIndex].pAudioStreamCallback = callback;
    }
    else
    {
        ERROR("Invalid callback address!\n");
        return -1;
    }


    GstAppSinkCaps = gst_caps_new_simple("video/x-h264", NULL);

    if(GstAppSinkCaps == NULL)
    {
    	ERROR("gst_caps_new_simple failed!\n");
    	return -13;
    }

    GstAppSrcCaps = gst_caps_new_simple("video/x-raw", NULL);
    if(GstAppSrcCaps == NULL)
	{
		ERROR("GstAppSrcCaps_new_simple failed!\n");
		return -13;
	}
//================================================// Set Caps
    /*
     * "rate",     G_TYPE_INT, g_PipelineProp[iPipelineIndex].iSampleRate,
        "channels", G_TYPE_INT, g_PipelineProp[iPipelineIndex].iChannel,
        "endianness", G_TYPE_INT, endianness,
        "signed", G_TYPE_BOOLEAN, is_signed,
        "width", G_TYPE_INT, width,
        "depth", G_TYPE_INT, depth,
     */


#if 1

    pEnv = getenv("CAPTURE_FPS");
    if(pEnv)
    {
    	framerate = atoi(pEnv);
    }
    else
    {
    	framerate = 30;
    }
    gst_caps_set_simple(GstAppSrcCaps, "width",G_TYPE_INT, 1280, NULL);
	gst_caps_set_simple(GstAppSrcCaps, "height",G_TYPE_INT, 720, NULL);
	gst_caps_set_simple(GstAppSrcCaps, "framerate",GST_TYPE_FRACTION, framerate, 1, NULL);


	gst_caps_set_simple(GstAppSinkCaps, "stream-format",G_TYPE_STRING, "byte-stream", NULL);


#else
    if(endianness > 0)
    {
        gst_caps_set_simple(GstAppSrcCaps, "endianness", G_TYPE_INT, endianness, NULL);
        DEBUG("GstAppSrcCaps: %s\n", gst_caps_to_string(GstAppSrcCaps));
    }

    if(is_signed >= 0)
    {
        gst_caps_set_simple(GstAppSrcCaps, "signed", G_TYPE_BOOLEAN, is_signed, NULL);
        DEBUG("GstAppSrcCaps: %s\n", gst_caps_to_string(GstAppSrcCaps));
    }

    if(width > 0)
    {
        gst_caps_set_simple(GstAppSrcCaps, "width", G_TYPE_INT, width, NULL);
        DEBUG("GstAppSrcCaps: %s\n", gst_caps_to_string(GstAppSrcCaps));
    }

    if(depth > 0)
    {
        gst_caps_set_simple(GstAppSrcCaps, "depth", G_TYPE_INT, depth, NULL);
        DEBUG("GstAppSrcCaps: %s\n", gst_caps_to_string(GstAppSrcCaps));
    }
#endif
    WARN("GstAppSinkCaps: %s\n", gst_caps_to_string(GstAppSinkCaps));
    WARN("GstAppSrcCaps: %s\n", gst_caps_to_string(GstAppSrcCaps));
//================================================// End of set caps
    g_PipelineProp[iPipelineIndex].pPipeLine = gst_pipeline_new("JAudioCapture2Stream");
    if(NULL == g_PipelineProp[iPipelineIndex].pPipeLine)
    {
        ERROR("Make pipeline failed!\n");
        return -3;
    }
//================================================// alsasrc
    g_PipelineProp[iPipelineIndex].pAlsaSrc = gst_element_factory_make("videotestsrc", "Jvideotestsrc");
    if(NULL == g_PipelineProp[iPipelineIndex].pAlsaSrc)
    {
        ERROR("Make audio src failed!\n");
        return -7;
    }


    if(NULL != device)
    {
        g_object_set(g_PipelineProp[iPipelineIndex].pAlsaSrc, "device", device, NULL);
    }
//================================================// appsink
    g_PipelineProp[iPipelineIndex].pAppSink = gst_element_factory_make("appsink", "JAudioStreamSink");
    if(NULL == g_PipelineProp[iPipelineIndex].pAppSink)
    {
        ERROR("Make audio starem sink failed!\n");
        return -9;
    }

    g_object_set(g_PipelineProp[iPipelineIndex].pAppSink, "caps",        GstAppSinkCaps, NULL);
    g_object_set(g_PipelineProp[iPipelineIndex].pAppSink, "sync",        GetEnvPropInt("JEN_PCMREC_APPSK_SYNC",  false),  NULL);
    g_object_set(g_PipelineProp[iPipelineIndex].pAppSink, "emit-signals", true, NULL);
    g_signal_connect(g_PipelineProp[iPipelineIndex].pAppSink, "new-sample", G_CALLBACK (callbackNewBuffer), &g_PipelineProp[iPipelineIndex].iIndex);
    //g_signal_connect(AppSink, "eos", G_CALLBACK(cbVideoEosHandler), hVideoPLugin);

//================================================// check props
    static int iPropTemp;
    static long long llPropTemp;
    static unsigned int uiPropTemp;
    static unsigned long ulPropTemp;


//================================================// End of check props
#if 0
    GstElement *pWaveEnc = gst_element_factory_make("wavenc", "JWaveEnc");
    GstElement *pFilesink = gst_element_factory_make("filesink", "JFileSink");
    g_object_set(pFilesink, "location", "output.wav", NULL);

    gst_bin_add_many(g_PipelineProp[iPipelineIndex].pPipeLine, g_PipelineProp[iPipelineIndex].pAlsaSrc, pWaveEnc, pFilesink, NULL);

    bGstResult = gst_element_link_many(g_PipelineProp[iPipelineIndex].pAlsaSrc, pWaveEnc, pFilesink, NULL);
    if(false == bGstResult)
    {
        ERROR("Link elements failed!\n");
        return -11;
    }
#else
    GstElement *pCapsFilter = gst_element_factory_make("capsfilter", "JCapsFilter");
    if(NULL == pCapsFilter)
    {
	ERROR("create capsfilter failed!\n");
	return -10;
    }

    g_object_set(pCapsFilter, "caps", GstAppSrcCaps, NULL);

    GstElement *pGstEncoder = gst_element_factory_make("vpuenc_h264", "JGstEncoder");
	if(NULL == pGstEncoder)
	{
		ERROR("create encoder failed!\n");
		return -11;
	}

	GstElement *pGstConvert = gst_element_factory_make("videoconvert", "JVideoConvert");
	if(NULL == pGstConvert)
	{
		ERROR("create videoconvert failed!\n");
		return -12;
	}

    gst_bin_add_many((GstBin *)g_PipelineProp[iPipelineIndex].pPipeLine, 
			g_PipelineProp[iPipelineIndex].pAlsaSrc,
			pCapsFilter,
			pGstConvert,
			pGstEncoder,
			g_PipelineProp[iPipelineIndex].pAppSink, NULL);

    bGstResult = gst_element_link_many(g_PipelineProp[iPipelineIndex].pAlsaSrc, 
    				pCapsFilter,
					pGstConvert,
					pGstEncoder,
					g_PipelineProp[iPipelineIndex].pAppSink, NULL);
    if(false == bGstResult)
    {
        ERROR("Link elements failed!\n");
        return -11;
    }
#endif

    iRet = SetPipelineNull2Playing(g_PipelineProp[iPipelineIndex].pPipeLine);
    if(iRet < 0)
    {
        return iRet;
    }

    return 0;
}
#elifdef RTSPSRC
int start_gstreamer_video_capture(int *index, const char *device, GstreamerAudioStreamInputCallback_f callback)
{
    gboolean bGstResult = false;
    GstCaps *GstAppSrcCaps = NULL;
    GstCaps *GstDepayCaps = NULL;
    int iRet = -1;
    int iPipelineIndex = 0;
    const char *pEnv = NULL;
    int port = 0;
    const char *uri = NULL;

    INFO("Start video capture!\n");

    iPipelineIndex = GetPipelineResource(g_PipelineProp);
    if(iPipelineIndex < 0)
    {
        ERROR("GetPipelineResource: too many pipelines!\n");
        return -1;
    }
    else
    {
        DEBUG("GetPResource, iIndex: %d !\n", iPipelineIndex);
    }

    *index = iPipelineIndex;
    memset(&g_PipelineProp[iPipelineIndex], 0, sizeof(GST_PIPELINE_ST));
    g_PipelineProp[iPipelineIndex].iIndex = iPipelineIndex;

    if(NULL != callback)
    {
        g_PipelineProp[iPipelineIndex].pAudioStreamCallback = callback;
    }
    else
    {
        ERROR("Invalid callback address!\n");
        return -1;
    }


//================================================// End of set caps
    g_PipelineProp[iPipelineIndex].pPipeLine = gst_pipeline_new("JAudioCapture2Stream");
    if(NULL == g_PipelineProp[iPipelineIndex].pPipeLine)
    {
        ERROR("Make pipeline failed!\n");
        return -3;
    }
//================================================// alsasrc
    //g_PipelineProp[iPipelineIndex].pAlsaSrc = gst_element_factory_make("udpsrc", "Judpsrc");
    g_PipelineProp[iPipelineIndex].pAlsaSrc = gst_element_factory_make("rtspsrc", "Jrtspsrc");
    if(NULL == g_PipelineProp[iPipelineIndex].pAlsaSrc)
    {
        ERROR("Make rtspsrc failed!\n");
        return -7;
    }

	/*GstAppSrcCaps = gst_caps_from_string("application/x-rtp,media=video,payload=96,clock-rate=90000,encoding-name=H264");
	g_object_set(g_PipelineProp[iPipelineIndex].pAlsaSrc, "caps", GstAppSrcCaps, NULL);
	gst_caps_unref(GstAppSrcCaps);*/

	pEnv = getenv("HICAR_URI");
	if(pEnv)
	{
		//port = atoi(pEnv);
		uri = pEnv;
	}
	else
	{
		//port = 6664;
		uri = "rtsp://127.0.0.1:6664/myvideo";
	}
	//g_object_set(g_PipelineProp[iPipelineIndex].pAlsaSrc, "port", port, NULL);
	INFO("rtspsrc location[%s]\n",uri);
	g_object_set(g_PipelineProp[iPipelineIndex].pAlsaSrc, "location", uri, NULL);
	g_object_set(g_PipelineProp[iPipelineIndex].pAlsaSrc, "latency", 50, NULL);
	g_object_set (g_PipelineProp[iPipelineIndex].pAlsaSrc, "do-rtcp", TRUE, NULL);

//================================================// appsink
    g_PipelineProp[iPipelineIndex].pAppSink = gst_element_factory_make("appsink", "JAudioStreamSink");
    if(NULL == g_PipelineProp[iPipelineIndex].pAppSink)
    {
        ERROR("Make audio starem sink failed!\n");
        return -9;
    }

    //g_object_set(g_PipelineProp[iPipelineIndex].pAppSink, "caps",        GstAppSinkCaps, NULL);
    g_object_set(g_PipelineProp[iPipelineIndex].pAppSink, "sync",        GetEnvPropInt("JEN_PCMREC_APPSK_SYNC",  false),  NULL);
    g_object_set(g_PipelineProp[iPipelineIndex].pAppSink, "emit-signals", true, NULL);
    g_signal_connect(g_PipelineProp[iPipelineIndex].pAppSink, "new-sample", G_CALLBACK (callbackNewBuffer), &g_PipelineProp[iPipelineIndex].iIndex);
    //g_signal_connect(AppSink, "eos", G_CALLBACK(cbVideoEosHandler), hVideoPLugin);

//================================================// check props
    static int iPropTemp;
    static long long llPropTemp;
    static unsigned int uiPropTemp;
    static unsigned long ulPropTemp;


//================================================// End of check props
#if 0
    GstElement *pWaveEnc = gst_element_factory_make("wavenc", "JWaveEnc");
    GstElement *pFilesink = gst_element_factory_make("filesink", "JFileSink");
    g_object_set(pFilesink, "location", "output.wav", NULL);

    gst_bin_add_many(g_PipelineProp[iPipelineIndex].pPipeLine, g_PipelineProp[iPipelineIndex].pAlsaSrc, pWaveEnc, pFilesink, NULL);

    bGstResult = gst_element_link_many(g_PipelineProp[iPipelineIndex].pAlsaSrc, pWaveEnc, pFilesink, NULL);
    if(false == bGstResult)
    {
        ERROR("Link elements failed!\n");
        return -11;
    }
#else
    GstElement *pCapsFilter = gst_element_factory_make("capsfilter", "JCapsFilter");
    if(NULL == pCapsFilter)
    {
	ERROR("create capsfilter failed!\n");
	return -10;
    }

    GstDepayCaps = gst_caps_from_string("video/x-h264, stream-format=byte-stream");
    g_object_set(pCapsFilter, "caps", GstDepayCaps, NULL);
    gst_caps_unref(GstDepayCaps);

    //GstElement *pRtpBin = gst_element_factory_make("rtpbin", "TinyRtpbin");
    GstElement *pRtpDepay = gst_element_factory_make("rtph264depay", "TinyRtpdepay");

    g_signal_connect(g_PipelineProp[iPipelineIndex].pAlsaSrc, "pad-added", G_CALLBACK(on_recievepad_added), pRtpDepay);
    g_signal_connect(g_PipelineProp[iPipelineIndex].pAlsaSrc, "pad-removed", G_CALLBACK(on_recievepad_removed), NULL);

    gst_bin_add_many((GstBin *)g_PipelineProp[iPipelineIndex].pPipeLine,
			g_PipelineProp[iPipelineIndex].pAlsaSrc,
			//pRtpBin,
			pRtpDepay,
			pCapsFilter,
			g_PipelineProp[iPipelineIndex].pAppSink, NULL);

    /*bGstResult = gst_element_link_many(
    			g_PipelineProp[iPipelineIndex].pAlsaSrc,
       			NULL);
	if(false == bGstResult)
	{
		ERROR("==> 3 gst_element_link_many failed!\n");
		return -3;
	}*/

	bGstResult = gst_element_link_many(
			//g_PipelineProp[iPipelineIndex].pAlsaSrc,
			pRtpDepay,
			pCapsFilter,
			g_PipelineProp[iPipelineIndex].pAppSink,
			NULL);
	if(false == bGstResult)
	{
		ERROR("==> 4 gst_element_link_many failed!\n");
		return -4;
	}
#endif

	gst_element_set_state(g_PipelineProp[iPipelineIndex].pPipeLine, GST_STATE_PLAYING);

    return 0;
}
/*rtp and rtcp */
#else
int start_gstreamer_video_capture(int *index, const char *device, GstreamerAudioStreamInputCallback_f callback)
{
	GstCaps *x264Caps;
	char *pStrEnv = NULL;
	bool bRetValue = false;
	guint64 u64MaxBytes = 0;
	const char *pCarplayMode = NULL;
	guint bus_watch_id;
	GstBus *MsgBus;
	int iPipelineIndex = 0;
	GstCaps *GstDepayCaps = NULL;


	INFO("Start video capture!\n");

	iPipelineIndex = GetPipelineResource(g_PipelineProp);
	if(iPipelineIndex < 0)
	{
		ERROR("GetPipelineResource: too many pipelines!\n");
		return -1;
	}
	else
	{
		DEBUG("GetPResource, iIndex: %d !\n", iPipelineIndex);
	}

	*index = iPipelineIndex;
	memset(&g_PipelineProp[iPipelineIndex], 0, sizeof(GST_PIPELINE_ST));
	g_PipelineProp[iPipelineIndex].iIndex = iPipelineIndex;

	if(NULL != callback)
	{
		g_PipelineProp[iPipelineIndex].pAudioStreamCallback = callback;
	}
	else
	{
		ERROR("Invalid callback address!\n");
		return -1;
	}


//================================================// End of set caps
	g_PipelineProp[iPipelineIndex].pPipeLine = gst_pipeline_new("JAudioCapture2Stream");
	if(NULL == g_PipelineProp[iPipelineIndex].pPipeLine)
	{
		ERROR("Make pipeline failed!\n");
		return -3;
	}

	MsgBus = gst_element_get_bus(g_PipelineProp[iPipelineIndex].pPipeLine);
	bus_watch_id = gst_bus_add_watch(MsgBus, bus_call, NULL);
	//==============================// appsrc
	g_PipelineProp[iPipelineIndex].pAlsaSrc = gst_element_factory_make("udpsrc", "TinyUdpsrc");
	if(NULL == g_PipelineProp[iPipelineIndex].pAlsaSrc)
	{
		ERROR("appsrc == NULL!\n");
		return -2;
	}
	x264Caps = gst_caps_from_string("application/x-rtp,media=video,payload=96,clock-rate=90000,encoding-name=H264");
	g_object_set(g_PipelineProp[iPipelineIndex].pAlsaSrc, "caps", x264Caps, NULL);
	gst_caps_unref(x264Caps);

	g_object_set(g_PipelineProp[iPipelineIndex].pAlsaSrc, "port", 6664, NULL);

	g_PipelineProp[iPipelineIndex].pRtpBin = gst_element_factory_make("rtpbin", "TinyRtpbin");
	g_signal_connect(g_PipelineProp[iPipelineIndex].pRtpBin, "pad-added", G_CALLBACK(on_pad_added), &g_PipelineProp[iPipelineIndex].iIndex);
	g_signal_connect(g_PipelineProp[iPipelineIndex].pRtpBin, "pad-removed", G_CALLBACK(on_pad_removed), &g_PipelineProp[iPipelineIndex].iIndex);


	g_PipelineProp[iPipelineIndex].pRtpDepay = gst_element_factory_make("rtph264depay", "TinyRtpdepay");
	GstElement *pCapsFilter = gst_element_factory_make("capsfilter", "JCapsFilter");
	if(NULL == pCapsFilter)
	{
	ERROR("create capsfilter failed!\n");
	return -10;
	}

	GstDepayCaps = gst_caps_from_string("video/x-h264, stream-format=byte-stream");
	g_object_set(pCapsFilter, "caps", GstDepayCaps, NULL);
	gst_caps_unref(GstDepayCaps);

	//================================================// appsink
	g_PipelineProp[iPipelineIndex].pAppSink = gst_element_factory_make("appsink", "JAudioStreamSink");
	if(NULL == g_PipelineProp[iPipelineIndex].pAppSink)
	{
		ERROR("Make audio starem sink failed!\n");
		return -9;
	}

	//g_object_set(g_PipelineProp[iPipelineIndex].pAppSink, "caps",        GstAppSinkCaps, NULL);
	g_object_set(g_PipelineProp[iPipelineIndex].pAppSink, "sync",        GetEnvPropInt("JEN_PCMREC_APPSK_SYNC",  false),  NULL);
	g_object_set(g_PipelineProp[iPipelineIndex].pAppSink, "emit-signals", true, NULL);
	g_signal_connect(g_PipelineProp[iPipelineIndex].pAppSink, "new-sample", G_CALLBACK (callbackNewBuffer), &g_PipelineProp[iPipelineIndex].iIndex);


	gst_bin_add_many(GST_BIN(g_PipelineProp[iPipelineIndex].pPipeLine),
			g_PipelineProp[iPipelineIndex].pAlsaSrc,
			g_PipelineProp[iPipelineIndex].pRtpBin,
			g_PipelineProp[iPipelineIndex].pRtpDepay,
			pCapsFilter,
			g_PipelineProp[iPipelineIndex].pAppSink,
				NULL);


	bRetValue = gst_element_link_many(
			g_PipelineProp[iPipelineIndex].pAlsaSrc,
			g_PipelineProp[iPipelineIndex].pRtpBin,
			NULL);
	if(false == bRetValue)
	{
		ERROR("==> 3 gst_element_link_many failed!\n");
		return -3;
	}

	bRetValue = gst_element_link_many(
			g_PipelineProp[iPipelineIndex].pRtpDepay,
			pCapsFilter,
			g_PipelineProp[iPipelineIndex].pAppSink,
			NULL);
	if(false == bRetValue)
	{
		ERROR("==> 4 gst_element_link_many failed!\n");
		return -4;
	}

	gst_element_set_state(g_PipelineProp[iPipelineIndex].pPipeLine, GST_STATE_PLAYING);
	return 0;
}
#endif

int close_gstreamer_video_captue(int index)
{

	gst_element_set_state(g_PipelineProp[index].pPipeLine, GST_STATE_NULL);
	gst_element_get_state(g_PipelineProp[index].pPipeLine, NULL, NULL, 2 * GST_SECOND);


    return 0;

}


void uinit_gstreamer_video_capture(int index)
{
	if(NULL != g_PipelineProp[index].pPipeLine)
	{
		gst_object_unref(g_PipelineProp[index].pPipeLine);
	    g_PipelineProp[index].pPipeLine = NULL;
	}
}
