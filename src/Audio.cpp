#include "Audio.hpp"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

std::vector<Audio::AudioSource*> Audio::s_sources = {};

Audio::Audio(const char* wavFilePath) : m_audioFileData(nullptr), m_fileSize(0)
{
    Audio::s_sources.reserve(10);
    // Open the .wav file
    FILE* pFile = fopen(wavFilePath, "rb");
    if (pFile == NULL) 
    {
        std::cerr << "Audio Error: File open error!" << std::endl;
        fclose(pFile);
        return;
    }

    // Get the file size
    fseek(pFile, 0, SEEK_END);
    m_fileSize = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);

    // Read the file contents into a buffer
    m_audioFileData = malloc(m_fileSize);
    if (m_audioFileData == NULL) 
    {
        std::cerr << "Audio Error: Memory allocation error!" << std::endl;
        m_audioFileData = nullptr;
        fclose(pFile);
        return;
    }

    fread(m_audioFileData, 1, m_fileSize, pFile);
    fclose(pFile);
}

void Audio::Play() 
{
    AudioSource* source = new AudioSource(m_audioFileData, m_fileSize);
    s_sources.push_back(source);
}

Audio::~Audio() 
{
    for(int i = 0; i < s_sources.size(); i++)
    {
        delete s_sources[i];
    }
    s_sources.clear();
    // free memory
}

void Audio::DataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) 
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == nullptr)
        return;

    ma_uint64 frameCurser;
    ma_decoder_get_cursor_in_pcm_frames(pDecoder, &frameCurser);
    ma_uint64 framesLength;
    ma_decoder_get_length_in_pcm_frames(pDecoder, &framesLength);
    if(frameCurser >= framesLength)
    {
        int size = s_sources.size();
        int it = FindIterator(pDevice);
        std::cout << "End" << std::endl;
        if(it != -1)
            s_sources.erase(s_sources.begin() + it);
    }
    
    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, nullptr);
    (void)pInput; // Unused

    // Apply volume
    // float* pSamples = (float*)pOutput;
    // for (ma_uint32 i = 0; i < frameCount * pDevice->playback.channels; ++i) 
    // {
    //     pSamples[i] *= pDecoder->volume;
    // }

}

int Audio::FindIterator(ma_device *pDevice)
{
    for(int i = 0; i < s_sources.size(); i++)
    {
        if (&(s_sources[i]->m_device) == pDevice) 
        {
            // delete s_sources[i];
            return i;
        }
    }
    return -1;
}
