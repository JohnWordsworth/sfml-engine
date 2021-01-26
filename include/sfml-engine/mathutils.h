#pragma once 

#include <SFML/Graphics.hpp>
#include <math.h>


namespace gbh
{
    namespace math
    {
        // Calculate the length (magnitude) of a Vector2f
        float length(const sf::Vector2f& vector);
    
        // Normalize a Vector2f so that the resulting vector has length of 1
        sf::Vector2f normalize(const sf::Vector2f& vector);
    
        // Convert radians to degrees
        float radiansToDegrees(float radians);
    
        // Convert degrees to radians
        float degreesToRadians(float degrees);
    }
}

