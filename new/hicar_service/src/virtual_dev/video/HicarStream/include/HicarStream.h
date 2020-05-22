/*
 * HicarStream.h
 *
 *  Created on: Nov 27, 2019
 *      Author: guxin
 */

#ifndef HICARSTREAM_H_
#define HICARSTREAM_H_

#include "hicar.h"

int initPlayer();

int uinitPlayer();

int handleVideoData(const uint8_t* pPacket, uint32_t nPktSize, int64_t timeStamp,
    int64_t duration, VideoFrameInfo *info);

int handleEvent(int cmdcode, uint32_t len, char *pbuf);

#endif /* HICARSTREAM_H_ */
