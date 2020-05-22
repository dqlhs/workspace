#ifndef ALSA_WAV_PLAY_H
#define ALSA_WAV_PLAY_H

int startAlsaPlaying(char *waveFilePath, bool loop = true);
void stopAlsaPlaying();
bool play_audio();
void setringtone(bool state);
#endif // ALSA_WAV_PLAY_H
