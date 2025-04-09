#include "Font.hpp"

Font::Font(const char* atlasPath, int cellSize)
    : Image(atlasPath), m_cellSize(cellSize) {}

int Font::getCellSize() const
{
    return m_cellSize;
}
