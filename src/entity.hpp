#pragma once
#include <raylib.h>

namespace entity
{
    class Entity
    {
    public:
        float x;
        float y;
        Entity(float xV = 0, float yV = 0) : x(xV), y(yV) {}
        virtual ~Entity() {}
        virtual void Draw(Texture2D& tileSheet) {}  
    };
}