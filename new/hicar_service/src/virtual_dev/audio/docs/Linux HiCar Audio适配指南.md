# 本文档的目的

本文档用于说明Linux下HiCar Audio相关的适配接口及其适配方法。



# Audio接口的集成方式

华为以头文件的方式规定了一系列的数据格式以及接口，车机端需要以共享库的形式实现华为规定的接口供其调用，目前在Linux车机上， 我司使用的是纯ALSA接口去播放声音，所以接口的集成目标是通过ALSA将车机的播放能力暴露给华为侧，同时对其隐藏车机的声卡细节。



# Linux车机的声音架构

![AudioArchitect](C:\Users\wanghao\Desktop\Workspace\HiCar\AudioArchitect.png)



在播放声音前，应用需要调用AudioManager的接口进行切源， 再通道Alsa接口播放相关的声音，同时现在HiCar所有的声音都是通过**多媒体通道**进行播放的。现在应用层已经封装了声音操作相关的组件 AudioClient, 相关应用直接调用即可。录音和回放都是使用的**default**声卡。

## 目前使用的声音参数

1. 回放：44100Hz 立体声 16位小端采样
2. 录音:   16000Hz 单声道 16位小端采样



# Audio接口数据结构说明

其中有的数据类型目前没有用到，就不加说明了。

## DMSDPAudioDevice

------

### 用途

声音设备类型枚举

### 定义

```c
typedef enum {
    AUDIO_DEVICE_TYPE_SPEAKER = 0u,
    AUDIO_DEVICE_TYPE_MIC = 1u,
    AUDIO_DEVICE_TYPE_MODEM_SPEAKER = 2u,
    AUDIO_DEVICE_TYPE_MODEM_MIC = 3u
} DMSDPAudioDevice;
```

### 说明

1. AUDIO_DEVICE_TYPE_SPEAKER: 扬声器
2. AUDIO_DEVICE_TYPE_MIC: 麦克风



### DMSDPAudioSampleRates

------

### 用途

声音的波特率枚举

### 定义

```c
typedef enum {
    AUDIO_SAMPLE_RATE_8K  = 8000u,
    AUDIO_SAMPLE_RATE_11K = 11025u,
    AUDIO_SAMPLE_RATE_12K = 12000u,     
    AUDIO_SAMPLE_RATE_16K = 16000u,
    AUDIO_SAMPLE_RATE_22K = 22050u,
    AUDIO_SAMPLE_RATE_24K = 24000u,
    AUDIO_SAMPLE_RATE_32K = 32000u,
    AUDIO_SAMPLE_RATE_44_1K = 44100u,
    AUDIO_SAMPLE_RATE_48K = 48000u
} DMSDPAudioSampleRates;
```



### DMSDPAudioChannelMasks

### 用途

声道数枚举

### 定义

```c
typedef enum {
    AUDIO_CHANNEL_OUT_MONO = 0x1u,
    AUDIO_CHANNEL_OUT_STEREO = 0x3u,
    AUDIO_CHANNEL_IN_MONO = 0x10u,
    AUDIO_CHANNEL_IN_STEREO = 0xCu
} DMSDPAudioChannelMasks;
```

### 说明

1. AUDIO_CHANNEL_OUT_MONO: 输出单声道
2. AUDIO_CHANNEL_OUT_STEREO: 输出立体声
3. AUDIO_CHANNEL_IN_MONO: 录音单声道
4. AUDIO_CHANNEL_IN_STEREO: 录音立体声



## DMSDPAudioFormats

### 用途

PCM格式枚举

### 定义

```c
typedef enum {
    AUDIO_FORMAT_PCM_16_BIT = 0x1u,
    AUDIO_FORMAT_PCM_8_BIT = 0x2u,
    AUDIO_FORMAT_PCM_32_BIT = 0x3u,
    AUDIO_FORMAT_PCM_8_24_BIT = 0x4u
} DMSDPAudioFormats;
```

### 说明

第1~3项分别代表了16位、8位、32位的小端PCM格式



## DMSDPAudioCodec

### 用途

声音编码格式枚举

### 定义

```c
typedef enum {
    FORMAT_AAC = 0,
    FORMAT_PCM = 1,
    FORMAT_G711A = 2,
} DMSDPAudioCodec;
```

### 说明

FORMAT_PCM: PCM格式编码，我们的系统只支持这种编码。



## DMSDPAudioCodecs

### 用途

DMSDPAudioCodec的容器封装

### 定义

```c
typedef struct {
    uint32_t num;
    DMSDPAudioCodec* codecs;
} DMSDPAudioCodecs;
```

### 说明

1. num: DMSDPAudioCodec的数量

2. codecs: 第一个DMSDPAudioCodec的地址



## DMSDPAudioProfile

### 用途

声卡能力描述结构体

### 定义

```c
typedef struct {
    DMSDPAudioSampleRates sampleRates;
    DMSDPAudioChannelMasks channelMask;
    DMSDPAudioFormats format;
} DMSDPAudioProfile;
```

### 说明

1. sampleRates: 采样率

2. channelMask: 声道数

3. format: pcm格式



## DMSDPAudioProfiles

### 用途

DMSDPAudioProfile的容器封装

### 定义

```c
typedef struct {
    uint32_t num;
    DMSDPAudioProfile* profiles;
} DMSDPAudioProfiles;
```

### 说明：

1. num: DMSDPAudioProfile的数量

2. profiles: 第一个DMSDPAudioProfile的地址



## DMSDPProfile

### 用途

音频参数描述结构体

### 定义

```c
typedef struct {
    char* key;
    uint32_t keyLen;
    uint32_t value;
} DMSDPProfile;
```

### 说明

1. key: 参数名，为ASCII字符串，合法取值为: "channel",  "format",  "sampleRate",  "codec"

   ”channel“: 通道数

   "format": PCM格式

   "sampleRate": 采样率

   "codec": 声音编码格式

2. keyLen: key的长度

3. value: 根据key的取值不同，对应的合法值见前面的相应结构体



## DMSDPProfiles

### 用途

DMSDPProfile的容器封装

### 定义

```c
typedef struct {
    uint32_t num;
    DMSDPProfile* profiles;
} DMSDPProfiles;
```

### 说明

1. num: DMSDPProfile的数量

2. profiles: 第一个DMSDPProfile的地址



## DMSDPAudioCapabilities 

### 用途

声卡描述结构体

### 定义

```c
typedef struct {
    char* id;
    uint32_t idLen;
    DMSDPAudioDevice type;
    DMSDPAudioProfiles profiles;
    DMSDPAudioCodecs codecs;
    DMSDPReserveds reserveds;
} DMSDPAudioCapabilities;
```

### 说明

1. id: 声卡名称的ASCII字符串
2. idLen: 声卡名称的长度
3. type: 声卡的设备类型
4. profiles: 声卡的能力
5. codecs: 声卡的编码格式
6. reserveds: 保留字段，现阶段不用



## DMSDPVirtualStreamDataHeader

### 用途

描述声音数据的相关信息

### 定义

```c
typedef struct {
    uint32_t streamType;
    uint32_t length;
    DMSDPReserveds reserveds;
} DMSDPVirtualStreamDataHeader;
```

### 说明

1. streamType: 声音的流类型
2. length: 数据的长度
3. reserveds: 保留字段



## DMSDPVirtualStreamData

### 用途

声音数据结构体

### 定义

```c
typedef struct  {
    uint8_t* data;
    int64_t timeStampUs;
    uint32_t usage;
    DMSDPVirtualStreamDataHeader* header;
} DMSDPVirtualStreamData;
```

### 说明

1.  data: 声音数据
2.  timeStampUs: 时间戳
3.  usage: 声音的用途，此字段和音频焦点样式有关，目前没用到
4.  header: 声音数据的相关信息



## DMSDPAudioHandler

### 用途

描述了车机端应该实现的接口函数

### 定义

```c
typedef struct {
    /*
     * 获取声卡能力，车机侧要通过该接口提供声卡能力的信息
     */
    int32_t (*GetAudioCapability)(DMSDPAudioCapabilities** capabilities, uint32_t* num);
    /**
     * 获取描述声卡能力的json字符串
     * len: 字符串长度
     * 返回值: 字符串
     */
    const char* (*GetAudioCapabilityString)(uint32_t* len);

    /*
     * 设置声音参数
     * Id: 声卡id
     * idLen: 声卡id长度
     * sessionId: 会话id，
     * streamType: 声音类型
     * profiles: 声音参数
     */
    int32_t (*SetAudioParameter)(const char* id, uint32_t idLen, int32_t sessionId, uint32_t streamType, const DMSDPProfiles* profiles);

    /*
     * 播放声音
     * id: 声卡名称
     * idLen: 声卡名称长度
     * sessionId: 会话id
     * data: 声音数据
     */
    int32_t (*WriteStreamBuffer)(const char* id, uint32_t idLen, int32_t sessionId, DMSDPVirtualStreamData* data);

    /*
     * 关闭播放声卡
     * id: 声卡名称
     * idLen: 声卡名称长度
     * sessionId: 会话id
     */
    int32_t (*CloseAudioTrack)(const char* id, uint32_t idLen, int32_t sessionId);

    /*
     * 打开录音声卡
     * id: 声卡名称
     * idLen: 声卡名称长度
     * sessionId: 会话id
     * inputSource: 输入源
     * profiles: 声音参数
     * 打开录音声卡在SetAudioParameter实现了，所以本接口暂时为空实现。
     */
    int32_t (*OpenAudioRecord)(
        const char* id, uint32_t idLen, int32_t sessionId, int32_t inputSource, const DMSDPProfiles* profiles);

    /*
     * 读取Mic数据
     * id: 声卡名称
     * idLen: 声卡名称长度
     * sessionId: 会话id
     * data:声音数据
     */
    int32_t (*ReadStreamBuffer)(const char* id, uint32_t idLen, int32_t sessionId, DMSDPVirtualStreamData* data);

    /*
     * 关闭录音声卡
     * id: 声卡名称
     * idLen: 声卡名称长度
     * sessionId: 会话id
     */
    int32_t (*CloseAudioRecord)(const char* id, uint32_t idLen, int32_t sessionId);

    /**
     * 释放资源
     * 暂时是空实现
     */
    void (*Release)(int32_t type, void* ptr, uint32_t num);

    /**
     * 注册监听
     * 暂时是空实现
     */
    int32_t (*RegisterListener)(const DMSDPListener* listener);

    /**
     * 业务控制
     * 暂时是空实现
     */
    int32_t (*BusinessControl)(uint32_t cmd, void* inputPara, uint32_t inLen, void* outputPara, uint32_t outLen);
} DMSDPAudioHandler;
```

### 说明

1. 每个接口都必须实现
2. 返回值类型为`int32_t`的，请查阅`dmsdp_business.h`



# 集成方法

车机侧实现了`DMSDPAudioHandler`中的函数后，再实现如下函数：

```c
int32_t DMSDPGetAudioHandler(DMSDPAudioHandler* audioHandler);
```

在该函数中，车机侧必须用通过入参`audioHandler`将实现的回调函数的地址传给华为侧。



# 集成注意事项

1. 必须谨慎处理华为侧的buffer边界，防止越界，会造成程序发生意外的行为
2. 有关Alsa编程和声卡配置的有关技术细节，请访问[Alsa](https://alsa-project.org/wiki/Main_Page)官方网站。
3. 声卡配置文件位于`/etc/asound.conf`
4. 不需要考虑华为侧传过来的buffer释放问题。