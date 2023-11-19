#pragma once
#include "Image.hpp"

class Font : public Image
{
public:
    Font(const char* atlasPath, int cellSize);
    int getCellSize() const;
private:
    int m_cellSize;
};