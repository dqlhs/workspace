// A simple C example to play a mono or stereo, 16-bit 44KHz
// WAVE file using ALSA. This goes directly to the first
// audio card (ie, its first set of audio out jacks). It
// uses the snd_pcm_writei() mode of outputting waveform data,
// blocking.
//
// Compile as so to create "alsawave":
// gcc -o alsawave alsawave.c -lasound
//
// Run it from a terminal, specifying the name of a WAVE file to play:
// ./alsawave MyWaveFile.wav

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Include the ALSA .H file that defines ALSA functions/data
#include <alsa/asoundlib.h>





#pragma pack (1)
/////////////////////// WAVE File Stuff /////////////////////
// An IFF file header looks like this
typedef struct _FILE_head
{
    unsigned char	ID[4];	// could be {'R', 'I', 'F', 'F'} or {'F', 'O', 'R', 'M'}
    unsigned int	Length;	// Length of subsequent file (including remainder of header). This is in
                                    // Intel reverse byte order if RIFF, Motorola format if FORM.
    unsigned char	Type[4];	// {'W', 'A', 'V', 'E'} or {'A', 'I', 'F', 'F'}
} FILE_head;


// An IFF chunk header looks like this
typedef struct _CHUNK_head
{
    unsigned char ID[4];	// 4 ascii chars that is the chunk ID
    unsigned int	Length;	// Length of subsequent data within this chunk. This is in Intel reverse byte
                            // order if RIFF, Motorola format if FORM. Note: this doesn't include any
                            // extra byte needed to pad the chunk out to an even size.
} CHUNK_head;

// WAVE fmt chunk
typedef struct _FORMAT {
    short				wFormatTag;
    unsigned short	wChannels;
    unsigned int	dwSamplesPerSec;
    unsigned int	dwAvgBytesPerSec;
    unsigned short	wBlockAlign;
    unsigned short	wBitsPerSample;
  // Note: there may be additional fields here, depending upon wFormatTag
} FORMAT;
#pragma pack()






// Size of the audio card hardware buffer. Here we want it
// set to 1024 16-bit sample points. This is relatively
// small in order to minimize latency. If you have trouble
// with underruns, you may need to increase this, and PERIODSIZE
// (trading off lower latency for more stability)
#define BUFFERSIZE	(2*1024)

// How many sample points the ALSA card plays before it calls
// our callback to fill some more of the audio card's hardware
// buffer. Here we want ALSA to call our callback after every
// 64 sample points have been played
#define PERIODSIZE	(2*64)

// Handle to ALSA (audio card's) playback port
snd_pcm_t				*PlaybackHandle = NULL;

// Handle to our callback thread
snd_async_handler_t	*CallbackHandle;

// Points to loaded WAVE file's data
unsigned char			*WavePtr = NULL;

// Size (in frames) of loaded WAVE file's data
snd_pcm_uframes_t		WaveSize;

// Sample rate
unsigned short			WaveRate;

// Bit resolution
unsigned char			WaveBits;

// Number of channels in the wave file
unsigned char			WaveChannels;
bool g_stopRingRun;
// The name of the ALSA port we output to. In this case, we're
// directly writing to hardware card 0,0 (ie, first set of audio
// outputs on the first audio card)
//static const char		SoundCardPortName[] = "plughw:1,0";
//***BEIGN*** 120s modify by zhangjiyang 2017-5-15
//static const char		SoundCardPortName[] = "plug:navigation_dmixer";
//static const char		SoundCardPortName[] = "plughw:0,0";
//static const char		SoundCardPortName[] = "plughw:0,0";
static const char		SoundCardPortName[] = "default";
//***END*** 120s modify by zhangjiyang 2017-5-15

// For WAVE file loading
static const unsigned char Riff[4]	= { 'R', 'I', 'F', 'F' };
static const unsigned char Wave[4] = { 'W', 'A', 'V', 'E' };
static const unsigned char Fmt[4] = { 'f', 'm', 't', ' ' };
static const unsigned char Data[4] = { 'd', 'a', 't', 'a' };


//for print pcm state
static char* pcm_state_str[] = {"SND_PCM_STATE_OPEN", "SND_PCM_STATE_SETUP", "SND_PCM_STATE_PREPARED", "SND_PCM_STATE_RUNNING", 
  "SND_PCM_STATE_XRUN", "SND_PCM_STATE_DRAINING", "SND_PCM_STATE_PAUSED", "SND_PCM_STATE_SUSPENDED", 
  "SND_PCM_STATE_DISCONNECTED", "SND_PCM_STATE_LAST"};

/********************** compareID() *********************
 * Compares the passed ID str (ie, a ptr to 4 Ascii
 * bytes) with the ID at the passed ptr. Returns TRUE if
 * a match, FALSE if not.
 */

static unsigned char compareID(const unsigned char * id, unsigned char * ptr)
{
    register unsigned char i = 4;

    while (i--)
    {
        if ( *(id)++ != *(ptr)++ ) return(0);
    }
    return(1);
}





/********************** waveLoad() *********************
 * Loads a WAVE file.
 *
 * fn =			Filename to load.
 *
 * RETURNS: 0 if success, non-zero if not.
 *
 * NOTE: Sets the global "WavePtr" to an allocated buffer
 * containing the wave data, and "WaveSize" to the size
 * in sample points.
 */

static unsigned char waveLoad(const char *fn)
{
    const char				*message;
    FILE_head				head;
    register int			inHandle;

    if ((inHandle = open(fn, O_RDONLY)) == -1)
        message = "didn't open";

    // Read in IFF File header
    else
    {
        if (read(inHandle, &head, sizeof(FILE_head)) == sizeof(FILE_head))
        {
            // Is it a RIFF and WAVE?
            if (!compareID(&Riff[0], &head.ID[0]) || !compareID(&Wave[0], &head.Type[0]))
            {
                message = "is not a WAVE file";
                goto bad;
            }

            // Read in next chunk header
            while (read(inHandle, &head, sizeof(CHUNK_head)) == sizeof(CHUNK_head))
            {
                // ============================ Is it a fmt chunk? ===============================
                if (compareID(&Fmt[0], &head.ID[0]))
                {
                    FORMAT	format;

                    // Read in the remainder of chunk
                    if (read(inHandle, &format.wFormatTag, sizeof(FORMAT)) != sizeof(FORMAT)) break;

                    // Can't handle compressed WAVE files
                    if (format.wFormatTag != 1)
                    {
                        message = "compressed WAVE not supported";
                        goto bad;
                    }

                    WaveBits = (unsigned char)format.wBitsPerSample;
                    WaveRate = (unsigned short)format.dwSamplesPerSec;
                    WaveChannels = format.wChannels;
                    printf("[bt ringtone] wavebits %d, waverate %d, wavechannel %d\n", WaveBits, WaveRate, WaveChannels);
                }

                // ============================ Is it a data chunk? ===============================
                else if (compareID(&Data[0], &head.ID[0]))
                {
                    // Size of wave data is head.Length. Allocate a buffer and read in the wave data]
                    printf("file len =%d",head.Length);
                    if (!(WavePtr = (unsigned char *)malloc(head.Length)))
                    {
                        message = "won't fit in RAM";
                        goto bad;
                    }

                    if (read(inHandle, WavePtr, head.Length) != head.Length)
                    {
                        free(WavePtr);
                        break;
                    }

                    // Store size (in frames)
                    WaveSize = (head.Length * 8) / ((unsigned int)WaveBits * (unsigned int)WaveChannels);

                    close(inHandle);
                    return(0);
                }

                // ============================ Skip this chunk ===============================
                else
                {
                    if (head.Length & 1) ++head.Length;  // If odd, round it up to account for pad byte
                    lseek(inHandle, head.Length, SEEK_CUR);
                }
            }
        }

        message = "is a bad WAVE file";
bad:	close(inHandle);
    }

    printf("[bt ringtone] %s %s\n", fn, message);
    return(1);
}

/********************** play_audio() **********************
 * Plays the loaded waveform.
 *
 * NOTE: ALSA sound card's handle must be in the global
 * "PlaybackHandle". A pointer to the wave data must be in
 * the global "WavePtr", and its size of "WaveSize".
 */
//unsigned long GetTickCount()
//{
//    struct timespec ts;
//    clock_gettime( CLOCK_MONOTONIC, &ts );
//    unsigned long temp = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
//    return temp;
//}
bool play_audio()
{
    bool ret = false;
    int err, err2, count, count_frame;
    int frame =0;

//    snd_pcm_state_t pcm_state;

    printf("[bt ringtone] waveSize:%d\n", WaveSize);
 // int dump_rst = 0;

    if(PlaybackHandle != NULL)
    {
        count = 0;
        count_frame = 10;
        frame = 10000;//;//frame = wavebits/8 * waverate * wavechannel / 20
        ret =true;

//        do
//        {

//            if (g_stopRingRun)
//                break;
//            if (count <6)
//               err = snd_pcm_writei(PlaybackHandle, WavePtr+frame*count, frame );
//            else
           // err = snd_pcm_writei(PlaybackHandle, WavePtr, WaveSize );
            err = snd_pcm_writei(PlaybackHandle, WavePtr, WaveSize );
            if (err == -EAGAIN || (err >= 0 && err < WaveSize)) 
            {
               snd_pcm_wait(PlaybackHandle, 100);
             }
            else if (err == -EPIPE)
            {
                printf(("[bt ringtone] =====Underrun: %s(%d)=====\n"), snd_strerror(err), err);
                snd_pcm_prepare(PlaybackHandle);
            }
            else if (err == -ESTRPIPE) 
            {
                while ((err = snd_pcm_resume(PlaybackHandle)) == -EAGAIN)
                {
                    printf("[bt ringtone] =====snd_pcm_resume EAGAIN, sleep once=====\n");
                     sleep(1);  /* wait until resume flag is released */
                }
                if (err < 0)
                     snd_pcm_prepare(PlaybackHandle);
          }
         else if (err < 0)
         {
            printf(("[bt ringtone] Write PCM device error: %s(%d)\n"), snd_strerror(err), err);
         //   break;//return;
	    }
//        count ++;// = count_frame;
      //  count_frame++;

//        if(err > 0)
//        {
//             printf(("[bt ringtone] Write PCM d: (%d)\n"), count_frame);
//            count_frame += err;
//            count += err * WaveChannels * WaveBits / 8;
//        }
            
//        } while (count < 7/*&& (times > 0)*/);
 
     }

     printf("[bt ringtone] ======end once======\n");
     return ret;
}


/*********************** free_wave_data() *********************
 * Frees any wave data we loaded.
 *
 * NOTE: A pointer to the wave data be in the global
 * "WavePtr".
 */

static void free_wave_data(void)
{
    printf("[bt ringtone] free_wave_data +++\n");
    if (WavePtr) free(WavePtr);
    WavePtr = 0;
    printf("[bt ringtone] free_wave_data ---\n");
}


int startAlsaPlaying(char *waveFilePath, bool loop)
{
    int times = 10;
    register int err = 0;
    register int format_wavebit = 0;
    // No wave data loaded yet
    WavePtr = 0;

    printf("[bt ringtone] ======startAlsaPlaying +++ ======\n");

    // Load the wave file
    if (!waveLoad(waveFilePath))
    {
        printf("[bt ringtone] wave load finished, start open device\n");
        times = 10;
        while(1)
        {
            if(times <= 0)
            {
                printf("[bt ringtone] Failed to open device for the past 10 attempts!\n");
                // Free the WAVE data      
                free_wave_data();
                return -1;
            }
            
            // Open audio card we wish to use for playback
            if ((err = snd_pcm_open(&PlaybackHandle, &SoundCardPortName[0], SND_PCM_STREAM_PLAYBACK, 0)) < 0)
            {
                printf("[bt ringtone] Can't open audio %s: %s, once more\n", &SoundCardPortName[0], snd_strerror(err));
                times --;
                usleep(100000);
                continue;
            }
            
            printf("[bt ringtone] ===successed in snd_pcm_open %x, pcm state is %s===\n", PlaybackHandle, pcm_state_str[snd_pcm_state(PlaybackHandle)]);
            break;
        } /* while(1) */

        switch (WaveBits)
        {
            case 8:
                format_wavebit = SND_PCM_FORMAT_U8;
                break;

            case 16:
                format_wavebit = SND_PCM_FORMAT_S16;
                break;

            case 24:
                format_wavebit = SND_PCM_FORMAT_S24;
                break;

            case 32:
                format_wavebit = SND_PCM_FORMAT_S32;
                break;
        }

        times = 10;
        while(1)
        {
            if(times <= 0)
            {
                // Close sound card
                printf("[bt ringtone] Failed in set_param for 10 attemps\n");
                snd_pcm_close(PlaybackHandle);
                PlaybackHandle = NULL;
                
                // Free the WAVE data      
                free_wave_data();

                return -1;
            }
            if ((err = snd_pcm_set_params(PlaybackHandle, (_snd_pcm_format)format_wavebit, SND_PCM_ACCESS_RW_INTERLEAVED, WaveChannels, WaveRate, 1, 500000)) < 0)//WaveRate
            {
                printf("[bt ringtone] Can't set sound parameters: %s, once more\n", snd_strerror(err));
                times --;
                usleep(100000);
                continue;
            }


            /*OK for open pcm and set pcm*/
            printf("[bt ringtone] ===successed in snd_pcm_set_params %x, pcm state is %s===\n", PlaybackHandle, pcm_state_str[snd_pcm_state(PlaybackHandle)]);
            break;
        } /* while(1) */
    }

    snd_pcm_uframes_t period = 0, buf = 0;

    snd_pcm_get_params(PlaybackHandle, &buf, &period);
    if (PlaybackHandle ==NULL)
        g_stopRingRun = true;
    else
        g_stopRingRun =false;
    printf("[bt ringtone] buf size %d frames, period size %d frames\n", buf, period);
    printf("[bt ringtone] ======startAlsaPlaying --- ======\n");

    return 0;
}

void stopAlsaPlaying()
{
    int times = 10;
    int err = 0;

    printf("[bt ringtone] ======stopAlsaPlaying +++ ======\n");
      
    if (PlaybackHandle)
    {    
        printf("[bt ringtone] ======snd_pcm_drop======\n");
        snd_pcm_drop(PlaybackHandle);

        // Close sound card
        printf("[bt ringtone] ======snd_pcm_close %x======\n", PlaybackHandle);
        while(times > 0)
        {
            err = snd_pcm_close(PlaybackHandle);
            if(err < 0)
            {
                printf("Failed to snd_pcm_close: %s, once more\n", snd_strerror(err));
                times --;
                usleep(100000);
                continue;
            }

            printf("[bt ringtone] ===successed in snd_pcm_close %x===\n", PlaybackHandle);
            PlaybackHandle = NULL;
            break;
        }

        if(times <= 0)
        {
            printf("[bt ringtone] Failed to close device for the past 10 times\n");
        }
    }

    // Free the WAVE data      
    free_wave_data();

    return;
}


void setringtone(bool state)
{
    g_stopRingRun =state;
}
