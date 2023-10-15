#pragma once
#include <iostream>
#include "miniaudio.h"

// TODO: https://github.com/mackron/miniaudio/tree/master use low level API 

class Audio
{
public:
    Audio();
    ~Audio();

    void Play(const char* wavFile);
    void Stop();
private:
    ma_engine m_engine;
};