#include <raylib.h>
#include "ui.hpp"

bool ui::Button(const float& x, const float& y, const float& width, const float& height, const int& textSize, const char title[], const Color color)
{ 
    float mouseX = GetMouseX(), mouseY = GetMouseY();

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && mouseX > x && mouseY > y && mouseX < x + width && mouseY < y + height)
        DrawRectangle(x, y, width, height, LIGHTGRAY);
    else
        DrawRectangle(x, y, width, height, color);
        
    DrawText(title, x + 2, y + 2, textSize, WHITE);
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mouseX > x && mouseY > y && mouseX < x + width && mouseY < y + height)
        return true; 
    else
        return false;
}

bool ui::Button(const float& x, const float& y, const char title, const Color color)
{ 
    float mouseX = GetMouseX(), mouseY = GetMouseY();

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && mouseX > x && mouseY > y && mouseX < x + 20 && mouseY < y + 20)
        DrawRectangle(x, y, 20, 20, LIGHTGRAY);
    else
        DrawRectangle(x, y, 20, 20, color);

    char toCast[1]; 
    toCast[0] = title;    
    
    DrawText(toCast, x + 2, y + 2, 20, WHITE);
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mouseX > x && mouseY > y && mouseX < x + 20 && mouseY < y + 20)
        return true; 
    else
        return false;
}


bool ui::DragDropZone(const float& x, const float& y, const float& width, const float& height)
{
    float mX = GetMouseX(), mY = GetMouseY(); 
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && mX > x && mY > y && mX < x + width && mY < y + height)
        return true;
    else
        return false;
}

void ui::Slidder(const float x, const float y, const float width, const float height, const char title[], float& value, Color color)
{
    float mouseX = GetMouseX(), mouseY = GetMouseY();
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && mouseX > x && mouseY > y && mouseX < x + width && mouseY < y + height)
    {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
        value = mouseX/(width+x);
    }
    else 
    {
        SetMouseCursor(MOUSE_CURSOR_ARROW);
    }

    DrawRectangle(x, y, width*value, height, color);
    DrawText(TextFormat("%s", title), x + 5, y + height/4, 15, BLACK);
    DrawRectangleLines(x, y, width, height, BLACK);
}