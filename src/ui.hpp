#pragma once
#include <raylib.h>

namespace ui
{
    bool Button(const float& x, const float& y, const float& width, const float& height, const int& textSize, const char title[], const Color color);
    bool Button(const float& x, const float& y, const char title, const Color color);
    bool DragDropZone(const float& x, const float& y, const float& width, const float& height);
    void Slidder(float x, float y, float width, float heigt, const char title[], float& value, Color color);
}