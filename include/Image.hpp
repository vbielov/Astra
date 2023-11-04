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

    void bind(unsigned int slot) const;
    void unbind() const;

private:
    unsigned int m_id;

    void createTexture(unsigned char* bufferSource, int width, int height);
};