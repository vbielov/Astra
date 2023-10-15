#pragma once
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// TODO: Add .bmp & header images 

class Image 
{
public:
    int width;
    int height;
    
    Image(const char* pngPath);
    ~Image();

    void Bind(unsigned int slot) const;
    void Unbind() const;

private:
    unsigned int m_id;

    void CreateTexture(unsigned char* bufferSource, int width, int height);
};