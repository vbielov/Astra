#include "Audio.hpp"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

AudioSource::AudioSource(void *audioFileData, size_t fileSize, float volume) 
    : device(), decoder(), isPlaying(false), volume(volume)
{
    ma_result result = ma_decoder_init_memory(audioFileData, fileSize, NULL, &decoder);
    if (result != MA_SUCCESS)
    {
        std::cerr << "Audio Error: Failed to initialize decoder" << std::endl;
        return;
    }

    ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate        = decoder.outputSampleRate;
    deviceConfig.dataCallback      = AudioManager::dataCallback;
    deviceConfig.pUserData         = &decoder;

    result = ma_device_init(NULL, &deviceConfig, &device);
    if (result != MA_SUCCESS) 
    {
        std::cerr << "Audio Error: Failed to initialize playback device" << std::endl;
        return;
    }

    result = ma_device_start(&device);
    if (result != MA_SUCCESS) 
    {
        std::cerr << "Audio Error: Failed to start playback device" << std::endl;
        ma_device_uninit(&device);
        return;
    }

    isPlaying = true;
}

AudioSource::~AudioSource()
{
    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);
}

AudioManager::AudioManager()
    : sources()
{

}

AudioManager &AudioManager::instance()
{
    static AudioManager INSTANCE;
    return INSTANCE;
}

AudioManager::~AudioManager()
{
    for(AudioSource* const& source : sources)
    {
        delete source;
    }

    sources.clear();
}

void AudioManager::update()
{
    // Deletes sounds that are done playing
    std::vector<AudioSource*>::iterator sourceIt;
    for(sourceIt = sources.begin(); sourceIt != sources.end();)
    {
        if((*sourceIt)->isPlaying == false)
        {
            AudioSource* ptr = *sourceIt;
            sources.erase(sourceIt);
            delete ptr;
            continue;
        }
        ++sourceIt;
    }
}

void AudioManager::dataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) 
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == nullptr)
        return;

    // Find pointer
    AudioSource* sourcePointer = nullptr;
    AudioManager* audioManager = &AudioManager::instance();
    for(AudioSource* const& source : audioManager->sources)
    {
        if(&(source->device) == pDevice)
        {
            sourcePointer = source;
        }
    }

    // Mark audio source as ready to delete, 
    // because calling ma_device_uninit in callback is forbidden
    ma_uint64 frameCurser, framesLength;
    ma_decoder_get_cursor_in_pcm_frames(pDecoder, &frameCurser);
    ma_decoder_get_length_in_pcm_frames(pDecoder, &framesLength);
    if(sourcePointer != nullptr && frameCurser >= framesLength)
    {
        sourcePointer->isPlaying = false;
        return;
    }

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, nullptr);
    
    // NOTE: Apply volume (only after read_pcm)
    // NOTE: Only supports ma_format_s16 audio
    int16_t* pOutputBuffer = (int16_t*)pOutput;
    if(sourcePointer != nullptr)
    {
        for (ma_uint32 i = 0; i < frameCount * pDevice->playback.channels; ++i) 
        {
            pOutputBuffer[i] = (int16_t)(pOutputBuffer[i] * sourcePointer->volume);
        }
    }
    
    (void)pInput; // Unused
}

Audio::Audio(const char* wavFilePath) 
    : m_audioFileData(nullptr), m_fileSize(0)
{
    // TODO: Make it C++.

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

Audio::~Audio() 
{
    free(m_audioFileData);
}

void Audio::play(float volume) 
{
    // Removes sounds from the front and pushes new to the back.
    // Didn't use Queue because need to remove AudioSource's from middle of vector
    AudioManager* audioManager = &AudioManager::instance();
    const int MAX_SOUND_SOURCES = 8;
    if(audioManager->sources.size() >= MAX_SOUND_SOURCES)
    {
        delete audioManager->sources.front();
        audioManager->sources.erase(audioManager->sources.begin());
    }

    AudioSource* source = new AudioSource(m_audioFileData, m_fileSize, volume);
    audioManager->sources.push_back(source);
}
