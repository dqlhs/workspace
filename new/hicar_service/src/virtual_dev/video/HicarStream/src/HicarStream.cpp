/*
 * HicarStream.c
 *
 *  Created on: Nov 27, 2019
 *      Author: guxin
 */
#include <sys/types.h>
#include <stdio.h>
#include "AvcVideo.h"
#include "HicarStream.h"



const char *ES_FILE_NAME = "./my.es";
FILE *filePointer = NULL;

void SaveEsFile(const uint8_t* esFrame, int esFrameLength)
{
	if (filePointer == NULL)
	{
		filePointer = fopen(ES_FILE_NAME, "a+");
		fwrite(esFrame, sizeof( char ), esFrameLength, filePointer );
	}
	else
	{
		fwrite(esFrame, sizeof( char ), esFrameLength, filePointer );
	}
}

int handleEvent(int cmdcode, uint32_t len, char *pbuf)
{
    printf("eventId[%d] ,detail[%s]",cmdcode,pbuf);

    return 0;
}


int handleVideoData(const uint8_t* pPacket, uint32_t nPktSize, int64_t timeStamp,
    int64_t duration, VideoFrameInfo *info)
{
	int ret = 0;
    //SaveEsFile(pPacket, nPktSize);
    //printf(" handleVideoData %d  \r\n ", nPktSize);
    ret = FeedH264ToPlayer((void *)pPacket, nPktSize);

    return ret;
}

int initPlayer()
{
	int ret = 0;
	ret = InitStreamPlayer(false);
	if(ret < 0)
		return ret;
	ret = StartVideoStream(NULL, -1);

	return ret;
}

int uinitPlayer()
{
	int ret = 0;
	ret = UninitStreamPlayer();

	return ret;
}
