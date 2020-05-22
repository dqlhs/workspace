/*
 ============================================================================
 Name        : Gst_Audio_Capture.c
 Author      : Jerry
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#ifndef __GST_AUDIO_CAPTURE_H__
#define __GST_AUDIO_CAPTURE_H__

#ifdef __cplusplus
extern "C"{
#endif

#define MAX_PIPELINES   8

//---------------------------------------------------------------------------------------------------------------------------
/*! @function   GstreamerAudioStreamInputCallback_f
    @abstract   Sets a function to be called for audio input or output (depending on the direction of the stream).

    @param      inBuffer        For input, the buffer contains the audio received from the hardware (e.g. microphone).
                                For output, the callback is expected to write audio data to be played to this buffer.

    @param      inLen           Number of bytes in the buffer.

*/
typedef void( *GstreamerAudioStreamInputCallback_f )(
                void *pAppleHandle,
                const void *inBuffer,
                int         inLen);

/*! @function   init_gstreamer_audio_capture
    @abstract   init gstreamer capture
    @param      card_id :set the capture card id
    @return  : init  result      0:init success -1 : failed
*/
int  init_gstreamer_video_capture(int *argc, char **argv[]);

/*! @function   open gstremer audio capture
    @abstract   init gstreamer capture
    @param      rate capture rate 48000\44100\32000\16000 etc
    @param      channels    stereo :2    mono:1
    @param      endianness    1234/4321: endian;   0, ignore
    @param      is_signed     true: signed; false: not signed;  is_signed < 0: ignore
    @param      width        width <= 0: ignore; width > 0: use
    @param      depth        depth <= 0: ignore; depth > 0: use
    @param     callback ;   capture callback function
    @return  : init  result  0:init success -1 : failed
*/
int start_gstreamer_video_capture(int *index, const char *device, GstreamerAudioStreamInputCallback_f callback);

/*! @function   close_gstreamer_captue
    @abstract   release the gstreamer capture resource

    @return  : init  result      0:init success;  <0 : failed
*/
int close_gstreamer_video_captue(int index);

void uinit_gstreamer_video_capture(int index);

#ifdef __cplusplus
}
#endif

#endif
