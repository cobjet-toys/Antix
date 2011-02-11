#ifndef MATH_H_
#define MATH_H_

namespace Math
{
    /**
     * Convert radions 'r' to degrees.
     */
    float rtod( const float& r );

    /**
     * Convert degrees 'd' to radians.
     */
    float dtor( const float& d);

    /**
     * Calcuate the distance assuming the world wraps. 
     */
    float WrapDistance(const float& d, const float& maxSize);
    
    /**
     * Normalize the distance 'd' relative to the max world size.
     */
    float DistanceNormalize(const float& d, const float& maxSize);
    
    /**
     * Normalize the angle 'a'.
     */
    float AngleNormalize(const float& a);
}

#endif
