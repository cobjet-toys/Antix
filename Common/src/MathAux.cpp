#include "MathAux.h"
#include <math.h>

using namespace Math;

float Math::rtod( const float& r )
{ 
    return( r * 180.0 / M_PI );
}

float Math::dtor( const float& d)
{
    return( d * M_PI / 180.0 ); 
}

float Math::WrapDistance(const float& d, const float& maxSize)
{
    float halfWorld = maxSize * 0.5;
    float distance = d;

    if (distance > halfWorld)
        distance -= maxSize;
    else if (distance < -halfWorld)
        distance += maxSize;

    return distance;
}

float Math::DistanceNormalize(const float& d, const float& maxSize)
{
    float distance = d;

    while (distance < 0)
        distance += maxSize;
    while (distance > maxSize)
        distance -= maxSize;

    return distance;
}

float Math::AngleNormalize(const float& a)
{
    float angle = a;

    while ( angle < -M_PI)
        angle += 2.0*M_PI;
    while (angle > M_PI)
        angle -= 2.0*M_PI;

    return angle;
}


