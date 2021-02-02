#pragma once 

#include <SFML/Graphics.hpp>
#include <math.h>


namespace gbh
{
    namespace math
    {
        /** Calculate the length (magnitude) of a Vector2f. i.e. The length of the line going from (0, 0) to (vector). */
        float length(const sf::Vector2f& vector);
    
        /** Returns a normalized version of 'vector' which is guaranteed to have a length of 1 (unless the input vector is empty). */
        sf::Vector2f normalize(const sf::Vector2f& vector);
    
        /** Convert radians to degrees. */
        float radiansToDegrees(float radians);
    
        /** Convert degrees to radians. */
        float degreesToRadians(float degrees);
    }
}

