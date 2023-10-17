#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "miniaudio.h"

class Audio 
{
public:
    Audio(const char* wavFilePath);
    ~Audio();
    void Play(float volume = 1.0f);
    static void Update();

private:
    void* m_audioFileData;
    ma_uint64 m_fileSize;

    static void DataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
    
    struct AudioSource
    {
        ma_device device;
        ma_decoder decoder;
        bool isPlaying;
        float volume;

        AudioSource(void* audioFileData, size_t fileSize, float volume);
        ~AudioSource();
    };

public:
    static std::vector<AudioSource*> s_sources;
};

