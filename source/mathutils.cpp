#include "sfml-engine/mathutils.h"

#define _USE_MATH_DEFINES
#include <math.h>


float gbh::math::length(const sf::Vector2f& vector)
{
    return sqrt((vector.x * vector.x) + (vector.y * vector.y));
}


sf::Vector2f gbh::math::normalize(const sf::Vector2f& vector)
{
    float length = gbh::math::length(vector);
    
    if (length == 0)
    {
        return vector;
    }
    
    return sf::Vector2f(vector.x / length, vector.y / length);
}


float gbh::math::radiansToDegrees(float radians)
{
    return radians * (180.0f / (float)M_PI);
}


float gbh::math::degreesToRadians(float degrees)
{
    return degrees / (180.0f / (float)M_PI);
}
