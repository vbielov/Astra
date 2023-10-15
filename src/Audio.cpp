#include "Audio.hpp"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

Audio::Audio() : m_engine()
{
    ma_result result;

    result = ma_engine_init(NULL, &m_engine);
    if (result != MA_SUCCESS)
    {
        std::cout << "Error: miniaudio was not inited" << std::endl;
        return;
    }
}

Audio::~Audio()
{
    ma_engine_uninit(&m_engine);
}

void Audio::Play(const char* wavFile)
{
    ma_engine_play_sound(&m_engine, wavFile, NULL);
}

void Audio::Stop()
{

}
