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

/*
 * Fast Arc-tangent, arc-cos, and arc-sin code. Credit to Richard Vaughan
 * and the internet.
 */
float Math::atan(const float& x, const float& y)
{
    const float piD2( M_PI/2.0 );
    float atan;
    float z = y/x;
    
    if( x == 0.0 )
    {
        if ( y > 0.0 ) return piD2;
        if ( y == 0.0 ) return 0.0;
        return -piD2;
    }
    
    if( fabs( z ) < 1.0 )
    {
        atan = z/(1.0 + 0.28*z*z);
        if ( x < 0.0 )
        {
            if ( y < 0.0 ) return atan - M_PI;
            return atan + M_PI;
        }
    }
    else
    {
        atan = piD2 - z/(z*z + 0.28f);
        if ( y < 0.0f ) return atan - M_PI;
    }

    return atan;
}
  
float Math::sin(const float& t)
{
    const float B = 4/M_PI;
    const float C = -4/(M_PI*M_PI);
    const float P = 0.225;
    const float tp = B * t + C * t * fabs(t);
    return( P * (tp * fabs(tp) - tp) + tp );
}
  
float Math::cos(const float& t)
{
    const float B = 4/M_PI;
    const float C = -4/(M_PI*M_PI);
    const float P = 0.225;
    
    float tp = t + M_PI/2;
    if(tp > M_PI)
    {
        // Original x > M_PI/2
        // Wrap: cos(x) = cos(x - 2 M_PI)
        tp -= 2 * M_PI; 
    }
    
    float y = B * tp + C * tp * fabs(tp); //fast, inprecise
    return( P * (y * fabs(y) - y) + y );
}

