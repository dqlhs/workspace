//
// Created by prow on 2019/11/26.
//

#include "HiCarAudioHal.h"
#include "dmsdp_log.h"
#include <sys/stat.h>

#define PROFILES_KEY_CODEC "codec"
#define PROFILES_KEY_SAMPLERATE "sampleRate"
#define PROFILES_KEY_FORMAT "format"
#define PROFILES_KEY_CHANNEL "channel"

snd_pcm_t ** getAudioHandleById(const char *cardid);
#define MALLOC_CHECK(v) \
do { \
	if (NULL == v) { \
        printf("No memory for this test!\n"); \
        exit(-1);\
    } \
} while(0)

static ssize_t safe_read(int fd, void *buf, size_t count)
{
	ssize_t result = 0, res;

	while (count > 0 ) {
		if ((res = read(fd, buf, count)) == 0)
			break;
		if (res < 0)
			return result > 0 ? result : res;
		count -= res;
		result += res;
		buf = (char *)buf + res;
	}
	return result;
}

int set_hwparams(snd_pcm_t *handle, snd_pcm_hw_params_t *params, snd_pcm_access_t access)
{
	unsigned int rrate;
	snd_pcm_uframes_t size;
	int err, dir;
	int resample = 1;                        /* enable alsa-lib resampling */
	unsigned int buffer_time = 512000; ;     /* ring buffer length in us */
	unsigned int period_time = 10000;  ;     /* period time in us */

	snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;//SND_PCM_FORMAT_S24_3LE;

	unsigned int rate = 44100;
	unsigned int channels = 2;

	/* choose all parameters */
	err = snd_pcm_hw_params_any(handle, params);
	if (err < 0) {
		printf("Broken configuration for playback: no configurations available: %s\n", snd_strerror(err));
		return err;
	}

	/* set hardware resampling */
	err = snd_pcm_hw_params_set_rate_resample(handle, params, resample);
	if (err < 0) {
		printf("Resampling setup failed for playback: %s\n", snd_strerror(err));
		return err;
	}

	/* set the interleaved read/write format */
	err = snd_pcm_hw_params_set_access(handle, params, access);
	if (err < 0) {
		printf("Access type not available for playback: %s\n", snd_strerror(err));
		return err;
	}

	/* set the sample format */
	err = snd_pcm_hw_params_set_format(handle, params, format);
	if (err < 0) {
		printf("Sample format not available for playback: %s\n", snd_strerror(err));
		return err;
	}

	/* set the count of channels */
	err = snd_pcm_hw_params_set_channels(handle, params, channels);
	if (err < 0) {
		printf("Channels count (%i) not available for playbacks: %s\n", channels, snd_strerror(err));
		return err;
	}

	/* set the stream rate */
	rrate = rate;
	err = snd_pcm_hw_params_set_rate_near(handle, params, &rrate, 0);
	if (err < 0) {
		printf("Rate %iHz not available for playback: %s\n", rate, snd_strerror(err));
		return err;
	}
	if (rrate != rate) {
		printf("Rate doesn't match (requested %iHz, get %iHz)\n", rate, err);
		return -EINVAL;
	}

	/* set the buffer time */
	err = snd_pcm_hw_params_set_buffer_time_near(handle, params, &buffer_time, &dir);
	if (err < 0) {
		printf("Unable to set buffer time %i for playback: %s\n", buffer_time, snd_strerror(err));
		return err;
	}

	err = snd_pcm_hw_params_get_buffer_size(params, &size);
	if (err < 0) {
		printf("Unable to get buffer size for playback: %s\n", snd_strerror(err));
		return err;
	}
	/* set the period time */
	err = snd_pcm_hw_params_set_period_time_near(handle, params, &period_time, &dir);
	if (err < 0) {
		printf("Unable to set period time %i for playback: %s\n", period_time, snd_strerror(err));
		return err;
	}

	err = snd_pcm_hw_params_get_period_size(params, &size, &dir);
	if (err < 0) {
		printf("Unable to get period size for playback: %s\n", snd_strerror(err));
		return err;
	}

	err = snd_pcm_hw_params(handle, params);
	if (err < 0) {
		printf("Unable to set hw params for playback: %s\n", snd_strerror(err));
		return err;
	}
	return 0;
}

int WriteStreamBuffer2(int buflen, int period_size, uint8_t *buf, snd_pcm_t *playback_handle) {
	int count = (buflen/period_size)+((buflen%period_size)?1:0);
	int leftLen = 0;
	int rc = 0;

			int i = 0;
			for(i=0; i < count; i++){



				leftLen = (i == count) ? (buflen - (period_size * count)) : period_size;
				DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_INFO, "AudioHAL", "left len: %d", leftLen);



				rc = snd_pcm_writei(playback_handle, buf + period_size * i, leftLen >> 2);
				DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_INFO, "AudioHAL", "write len1: %d", rc);
				if (rc == -EAGAIN || (rc >= 0 && (size_t)rc < (leftLen >> 2)))
				{
					printf("=====again: %s(%d)=====\n", snd_strerror(rc), rc);
					snd_pcm_wait(playback_handle, 100);
				}
				else if (rc == -EPIPE)
				{
					printf("=====Underrun: %s(%d)=====\n", snd_strerror(rc), rc);
					snd_pcm_prepare(playback_handle);
				}
				else if (rc == -ESTRPIPE)
				{
					while ((rc = snd_pcm_resume(playback_handle)) == -EAGAIN)
					{
						printf("=====snd_pcm_resume, sleep =====\n");
						sleep(1);  /* wait until resume flag is released */
					}
					if (rc < 0)
						snd_pcm_prepare(playback_handle);
				}
			}
	return 0;
}

int main(int argc, char *argv[]) {
	char cardid[64] = {0};
	int profileIndex = 0;
	int32_t r = DMSDP_OK;
	int fd = -1;
	long rc;

	snd_pcm_hw_params_t *hwparams;
	snd_pcm_hw_params_alloca(&hwparams);

	int cardid_len = strlen("MediaCard1");
	memcpy(cardid, "MediaCard1", strlen("MediaCard1"));

	DMSDPProfiles params;
	params.num = 3;
	params.profiles = (DMSDPProfile *) malloc(sizeof(DMSDPProfile) * params.num);
	MALLOC_CHECK(params.profiles);

	params.profiles[profileIndex].keyLen = strlen(PROFILES_KEY_FORMAT);
	params.profiles[profileIndex].key = malloc(sizeof(char) * params.profiles[profileIndex].keyLen);
	MALLOC_CHECK(params.profiles[profileIndex].key);
	memcpy(params.profiles[profileIndex].key, PROFILES_KEY_FORMAT, params.profiles[profileIndex].keyLen);
	params.profiles[profileIndex].value = AUDIO_FORMAT_PCM_16_BIT;

	profileIndex++;

	params.profiles[profileIndex].keyLen = strlen(PROFILES_KEY_CHANNEL);
	params.profiles[profileIndex].key = malloc(sizeof(char) * params.profiles[profileIndex].keyLen);
	MALLOC_CHECK(params.profiles[profileIndex].key);
	memcpy(params.profiles[profileIndex].key, PROFILES_KEY_CHANNEL, params.profiles[profileIndex].keyLen);
	params.profiles[profileIndex].value = AUDIO_CHANNEL_OUT_STEREO;

	profileIndex++;

	params.profiles[profileIndex].keyLen = strlen(PROFILES_KEY_SAMPLERATE);
	params.profiles[profileIndex].key = malloc(sizeof(char) * params.profiles[profileIndex].keyLen);
	MALLOC_CHECK(params.profiles[profileIndex].key);
	memcpy(params.profiles[profileIndex].key, PROFILES_KEY_SAMPLERATE, params.profiles[profileIndex].keyLen);
	params.profiles[profileIndex].value = AUDIO_SAMPLE_RATE_44_1K;

	r = SetAudioParameter(cardid, cardid_len, 0, 0, &params);

	fd = open(argv[1], O_RDONLY, 0);
	if (fd < 0) {
		printf("File open failed!\n");
		exit(-1);
	}

	struct stat st;
	stat(argv[1], &st);
	__off_t file_len = st.st_size;
	long period_size = getPeriodBytes(cardid);
	//long period_size = 1024 * 2;
	DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_INFO, "AudioHAL", "the period_bytes is:%d", period_size);
	if (period_size <= 0) {
		DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_INFO, "AudioHAL", "the period_size is:%d", period_size);
		exit(-1);
	}

	unsigned long buflen = sizeof(uint8_t) * period_size;
	uint8_t *buf = malloc(buflen);
	MALLOC_CHECK(buf);

	int count;
	unsigned long leftLen = 0;
	snd_pcm_t *playback_handle = * (getAudioHandleById(cardid));
//    snd_pcm_t *playback_handle;
//    snd_pcm_open(&playback_handle, "default", SND_PCM_STREAM_PLAYBACK, 0);

//	set_hwparams(playback_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED);
	DMSDPVirtualStreamDataHeader header;
	DMSDPVirtualStreamData data;
	data.header = &header;
	do {
		rc = safe_read(fd, buf, buflen);

		if (rc > 0) {
			data.header->length = rc;
			data.data = buf;
			//r = WriteStreamBuffer2(buflen, period_size, buf, playback_handle);
			r = WriteStreamBuffer(cardid, cardid_len, 0, &data);
//
		}
		memset(data.data, 0, buflen);
	} while (rc > 0);

	printf("play finished!\n");
	close(fd);

	return 0;
}