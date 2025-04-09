#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "miniaudio.h"
#include <stdint.h>

struct AudioSource
{
    ma_device device;
    ma_decoder decoder;
    bool isPlaying;
    float volume;

    AudioSource(void* audioFileData, size_t fileSize, float volume);
    ~AudioSource();
};

class AudioManager
{
public:
    std::vector<AudioSource*> sources;

    static AudioManager& instance();
    void update();
    static void dataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
    
private:
    AudioManager();
    ~AudioManager();
};

class Audio 
{
public:
    Audio(const char* wavFilePath);
    ~Audio();
    void play(float volume = 1.0f);

private:
    void* m_audioFileData;
    ma_uint64 m_fileSize;
};

