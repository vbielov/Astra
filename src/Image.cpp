#include "Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image(const char* pngPath)
{
    // opengl starts at bottom left
	// png format starts at upper left
	// so have to flip texture:

	stbi_set_flip_vertically_on_load(1);
    int BPP;
	unsigned char* localBuffer = stbi_load(pngPath, &width, &height, &BPP, 4); // RGBA - 4 values

    CreateTexture(localBuffer, width, height);
    // WriteImageHeaderFile(localBuffer);

    if(localBuffer)
		stbi_image_free(localBuffer);
}

void Image::CreateTexture(unsigned char* bufferSource, int width, int height)
{
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufferSource);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Image::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Image::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

Image::~Image()
{
    glDeleteTextures(1, &m_id);
}
