#include <math.h>
#include "ourmath.hpp"

float Distance2D(const Vector2 &a, const Vector2 &b) 
{ 
    return sqrtf(powf(a.x - b.x, 2.f) + powf(a.y - b.y, 2.f));
}

Vector2 Lerp(Vector2 a, Vector2 b, float t)
{
    Vector2 result;
    result.x = (1.f - t) * a.x + t * b.x;
    result.y = (1.f - t) * a.y + t * b.y;
    return result;
}