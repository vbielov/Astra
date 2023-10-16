#pragma once
#include <iostream>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdint.h>
#include <algorithm>
#include "miniaudio.h"

// TODO: https://github.com/mackron/miniaudio/tree/master use low level API 

// class Audio
// {
// public:
//     Audio();
//     ~Audio();

//     void Play(const char* wavFilePath, float volume = 1.0f);
//     void Stop();
// private:
//     ma_engine m_engine;
// };



class Audio 
{
public:
    Audio(const char* wavFilePath);
    ~Audio();
    void Play();

private:
    void* m_audioFileData;
    ma_uint64 m_fileSize;

    static void DataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
    

    struct AudioSource
    {
        ma_device m_device;
        ma_decoder m_decoder;
        ma_device_config m_deviceConfig;

        AudioSource(void* m_audioFileData, ma_uint64 m_fileSize)
        {
            ma_result result = ma_decoder_init_memory(m_audioFileData, m_fileSize, NULL, &m_decoder);
            if (result != MA_SUCCESS)
            {
                std::cerr << "Audio Error: Failed to initialize decoder" << std::endl;
                return;
            }

            m_deviceConfig = ma_device_config_init(ma_device_type_playback);
            m_deviceConfig.playback.format   = m_decoder.outputFormat;
            m_deviceConfig.playback.channels = m_decoder.outputChannels;
            m_deviceConfig.sampleRate        = m_decoder.outputSampleRate;
            m_deviceConfig.dataCallback      = Audio::DataCallback;
            m_deviceConfig.pUserData         = &m_decoder;

            result = ma_device_init(NULL, &m_deviceConfig, &m_device);
            if (result != MA_SUCCESS) 
            {
                std::cerr << "Audio Error: Failed to initialize playback device" << std::endl;
                return;
            }

            result = ma_device_start(&m_device);
            if (result != MA_SUCCESS) 
            {
                std::cerr << "Audio Error: Failed to start playback device" << std::endl;
                ma_device_uninit(&m_device);
                return;
            }
        }
        ~AudioSource()
        {
            ma_device_uninit(&m_device);
            ma_decoder_uninit(&m_decoder);
        }
    };

    static int FindIterator(ma_device* pDevice);
public:
    static std::vector<AudioSource*> s_sources;
};

