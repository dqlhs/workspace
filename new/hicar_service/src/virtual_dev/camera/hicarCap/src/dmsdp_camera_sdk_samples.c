#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "dmsdp_business.h"
#include "dmsdp_camera_data_type.h"
#include "dmsdp_camera_handler.h"
#include "Gst_Video_Capture.h"

//#define LOG_TAG "DMSDP_SDK_CAMERA"

#define VENDOR_CAMERA_ID_MAX_LEN (256)
#define VENDOR_CAMERA_MAX_NUM (16)

const char* g_CameraId = "Camera1";

DMSDPSendBackDataCB g_SendDataCallback = NULL;
static pthread_t g_VideoThread;
static int g_FlagRun = false;
static uint32_t g_Type = 0;
static int pipeIndex = 0;


static FILE *filePointer = NULL;
static bool Recflag = false;

void SaveEsFile(const uint8_t* esFrame, int esFrameLength)
{
	if (filePointer == NULL)
	{
		printf("filePointer null\n");
		filePointer = fopen("/home/root/hicarrecord.h264", "a+");
		if(filePointer)
		{
			fwrite(esFrame, sizeof( char ), esFrameLength, filePointer );
		}
	}
	else
	{
		fwrite(esFrame, sizeof( char ), esFrameLength, filePointer );
	}
}


static void captureCallBack(void *pHandle, const void *inBuffer, int inLen)
{

    if(*((int*)pHandle) >= 0)
    {
    	if (g_FlagRun && g_SendDataCallback != NULL) {
			printf("h264 len=%d\n", inLen);
			if(Recflag)
				SaveEsFile((const uint8_t*)inBuffer, inLen);
			g_SendDataCallback(g_CameraId, strlen(g_CameraId), (const uint8_t*)inBuffer, inLen, g_Type);
		}
    }
}

bool IsCameraIdValid(const char* id, uint32_t idLen)
{
    if ((id == NULL) || (idLen > VENDOR_CAMERA_ID_MAX_LEN) || (idLen == 0)) {
        printf("IsCameraIdValid id invalid len=%d\n", idLen);
        return false;
    }

    if (id[idLen] != '\0') {
    	printf("IsCameraIdValid id no end\n");
        return false;
    }
    return true;
}

int32_t CameraRegisterSendDataCallback(const char* id, uint32_t idLen, const DMSDPSendBackDataCB cb)
{
    /* regist callback senddata */
	printf("CameraRegisterSendDataCallback\n");
    g_SendDataCallback = cb;
    return DMSDP_OK;
}

int32_t OpenCamera(const DMSDPCameraParam* param)
{
	int iRet = 0;
    if (param == NULL) {
        return DMSDP_ERR_INVALID_PARAMETER;
    }

    if (!IsCameraIdValid(param->id, param->idLen)) {
    	printf("OpenCamera camera id invalid\n");
        return DMSDP_ERR_INVALID_PARAMETER;
    }

    const char *pEnv = NULL;
    pEnv = getenv("HICAR_CAMERA_RECORD");
	if(pEnv)
	{
		Recflag = true;
	}
	else
	{
		printf("don't record video file\n");
	}
    /* camera paramter convert*/
    printf("OpenCamera sdk camera=%s,width=%d,height=%d,fps=%d,dataType=%d\n",
        param->id,
        param->width,
        param->height,
        param->fps,
        param->dataFormat);

    /* open camera */
    g_FlagRun = true;
    g_Type = param->dataFormat;

    if (g_Type == H264) {
    	init_gstreamer_video_capture(NULL, NULL);
    	iRet = start_gstreamer_video_capture(&pipeIndex,
											 NULL,
											 &captureCallBack);
		if(iRet < 0)
			printf("Start audio capture failed: %d!\n", iRet);
		else
			printf("Start audio capture Success: %d!\n", iRet);
    } else if (g_Type == H265) {
    	printf("no support for H265\n");
    	return DMSDP_ERR_UNKNOWN_ERROR;
    }


    return DMSDP_OK;
}

int32_t CloseCamera(const char* id, uint32_t idLen)
{
    if (!IsCameraIdValid(id, idLen)) {
    	printf("CloseCamera camera id invalid\n");
        return DMSDP_ERR_INVALID_PARAMETER;
    }

    printf("CloseCamera\n");

    /* close camera */
    g_FlagRun = false;
    close_gstreamer_video_captue(pipeIndex);
    uinit_gstreamer_video_capture(pipeIndex);
    
    /* unregist senddata callback */
    g_SendDataCallback = NULL;

    return DMSDP_OK;
}

int32_t CameraUpdateConfig(const char* id, uint32_t idLen, int32_t key, char* value, uint32_t valLen)
{
    if (!IsCameraIdValid(id, idLen)) {
    	printf("CloseCamera camera id invalid\n");
        return DMSDP_ERR_INVALID_PARAMETER;
    }

    /* update config fps */
    printf("CameraUpdateConfig key=%d,value=%s\n", key, value);
    return DMSDP_OK;
}

void CameraReleaseCameraAbility(DMSDPCameraCapabilities* ability, uint32_t num)
{
    /* release the resource if any */
    if (ability == NULL) {
        return;
    }

    uint32_t number = num;
    if (num > VENDOR_CAMERA_MAX_NUM) {
        number = num;
    }

    uint32_t i;
    for (i = 0; i < number; i++) {
        DMSDPCameraCapabilities* cameraAbilityItem = &ability[i];
        if (cameraAbilityItem->id != NULL) {
            free(cameraAbilityItem->id);
            cameraAbilityItem->id = NULL;
        }

        if (cameraAbilityItem->abilities == NULL) {
            continue;
        }

        uint32_t j = 0;
        for (j = 0; j < cameraAbilityItem->abilityNum; j++) {
            DMSDPCameraAbility* abilityitem = &cameraAbilityItem->abilities[j];
            if (abilityitem->supportFpsRanges != NULL) {
                free(abilityitem->supportFpsRanges);
                abilityitem->supportFpsRanges = NULL;
            }

            if (abilityitem->supportSizes != NULL) {
                free(abilityitem->supportSizes);
                abilityitem->supportSizes = NULL;
            }
            free(cameraAbilityItem->abilities);
            cameraAbilityItem->abilities = NULL;
        }
    }
    free(ability);
    return;
}

void CameraRelease(int32_t type, void* ptr, uint32_t num)
{
	printf("CameraRelease type=%d\n", type);
    switch (type) {
        case RELEASE_TYPE_QUERY_ABILITY:
            CameraReleaseCameraAbility((DMSDPCameraCapabilities*)ptr, num);
            break;
        default:
            break;
    }
    return;
}

int32_t GetCameraCapability(DMSDPCameraCapabilities** capabilities, uint32_t* num)
{
    uint32_t cameraIndex = 0;
    uint32_t cameraNum = 1; /* sign how many cameras here*/
    *num = cameraNum;
    *capabilities = (DMSDPCameraCapabilities*)malloc((sizeof(DMSDPCameraCapabilities)) * cameraNum);
    if (*capabilities == NULL) {
        return DMSDP_ERR_NO_MEMORY;
    }
    memset(*capabilities, 0, (sizeof(DMSDPCameraCapabilities)) * cameraNum);
    DMSDPCameraCapabilities* items = *capabilities;

    /*********************** get Camera ID ***************************/
    char* id = malloc(VENDOR_CAMERA_ID_MAX_LEN);
    if (id == NULL) {
        CameraRelease(RELEASE_TYPE_QUERY_ABILITY, *capabilities, cameraNum);
        return DMSDP_ERR_NO_MEMORY;
    }
    memset(id, 0, VENDOR_CAMERA_ID_MAX_LEN);
    uint32_t idLen = strlen(g_CameraId);
    memcpy(id, g_CameraId, idLen);

    items[cameraIndex].id = id;
    items[cameraIndex].idLen = idLen;

    /*********************** get ability ***************************/
    uint32_t abilityNum = 1; /* sign how many abilites here*/
    items[cameraIndex].abilityNum = abilityNum;
    DMSDPCameraAbility* ability = (DMSDPCameraAbility*)malloc((sizeof(DMSDPCameraAbility)) * abilityNum);
    if (ability == NULL) {
        CameraRelease(RELEASE_TYPE_QUERY_ABILITY, *capabilities, cameraNum);
        *capabilities = NULL;
        return DMSDP_ERR_NO_MEMORY;
    }
    memset(ability, 0, (sizeof(DMSDPCameraAbility)) * abilityNum);
    items[cameraIndex].abilities = ability;
    /**********************************************************************************************/
    /* ability index 0 */
    uint32_t abilityIndex = 0;
    ability[abilityIndex].dataType = H264; /* sign video datat type here DMSDPCameraDataType */

    /********************* get ranges *********************/
    uint32_t fpsRangeNum = 1; /* sign how many fpsrange here*/
    ability[abilityIndex].fpsRangeNum = fpsRangeNum;
    DMSDPFpsRange* fpsRanges = (DMSDPFpsRange*)malloc((sizeof(DMSDPFpsRange)) * fpsRangeNum);
    if (fpsRanges == NULL) {
        CameraRelease(RELEASE_TYPE_QUERY_ABILITY, *capabilities, cameraNum);
        *capabilities = NULL;
        return DMSDP_ERR_NO_MEMORY;
    }
    memset(fpsRanges, 0, (sizeof(DMSDPFpsRange)) * fpsRangeNum);

    uint32_t fpsRangeIndex = 0;
    /* fps range index 0*/
    fpsRanges[fpsRangeIndex].min = 15 * 1000;
    fpsRanges[fpsRangeIndex].max = 30 * 1000;

    /* fps range index 1*/

    /* ... */
    ability[abilityIndex].supportFpsRanges = fpsRanges;
    /********************* get sizes *********************/
    uint32_t sizeNum = 3; /* sign how many size here*/
    ability[abilityIndex].sizeNum = sizeNum;
    DMSDPSize* sizes = (DMSDPSize*)malloc((sizeof(DMSDPSize)) * sizeNum);
    if (sizes == NULL) {
        CameraRelease(RELEASE_TYPE_QUERY_ABILITY, *capabilities, cameraNum);
        *capabilities = NULL;
        return DMSDP_ERR_NO_MEMORY;
    }
    memset(sizes, 0, (sizeof(DMSDPSize)) * sizeNum);

    uint32_t sizeIndex = 0;
    /* size index 0*/
    //sizes[sizeIndex].width = 640;
    //sizes[sizeIndex].height = 480;
    //sizeIndex++;

    /* size index 1*/
    sizes[sizeIndex].width = 1280;
    sizes[sizeIndex].height = 720;
    sizeIndex++;

    /* size index 2*/
    //sizes[sizeIndex].width = 1920;
    //sizes[sizeIndex].height = 1080;

    /* ... */
    ability[abilityIndex].supportSizes = sizes;
    /**********************************************************************************************/

    /* ability index 1 */

    /* ability index 2 */

    /* ... */

    /* next camera */
    return DMSDP_OK;
}

const char* GetCameraCapabilityString(uint32_t* len)
{
    /* reserved for ide automatic no need to impl*/
    return NULL;
}

int32_t CameraRegisterListener(const DMSDPListener* listener)
{
    /* regist service state/action listener*/
    return DMSDP_OK;
}

int32_t CameraBusinessControl(uint32_t cmd, void* inputPara, uint32_t inLen, void* outputPara, uint32_t outLen)
{
    /* reserved */
    return DMSDP_OK;
}

int32_t DMSDPGetCameraHandler(DMSDPCameraHandler* cameraHandler)
{
    memset(cameraHandler, 0, sizeof(DMSDPCameraHandler));
    cameraHandler->RegisterSendDataCallback = CameraRegisterSendDataCallback;
    cameraHandler->OpenCamera = OpenCamera;
    cameraHandler->CloseCamera = CloseCamera;
    cameraHandler->UpdateConfig = CameraUpdateConfig;
    cameraHandler->GetCameraCapability = GetCameraCapability;
    cameraHandler->GetCameraCapabilityString = GetCameraCapabilityString;
    cameraHandler->Release = CameraRelease;
    cameraHandler->RegisterListener = CameraRegisterListener;
    cameraHandler->BusinessControl = CameraBusinessControl;
    return DMSDP_OK;
}
