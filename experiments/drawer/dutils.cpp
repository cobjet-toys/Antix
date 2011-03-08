#include "dutils.h"

float DrawUtils::rtod( const float& r )
{
    return( r * 180.0 / M_PI );
}

float DrawUtils::dtor( const float& d)
{
    return( d * M_PI / 180.0 );
}

//sin

//cos

void DrawUtils::parseOptions(
        int argc,
        char** argv,
        int* teamCount,
        int* puckCount,
        int* popCount,
        int* winSize,
        float* worldSize,
        float* FOVAngle,
        float* FOVRange)
{
    int c;
    while( ( c = getopt( argc, argv, "dlh:a:p:s:f:r:w:")) != -1 )
        switch( c )
        {
            case 'h':
                *teamCount = atoi( optarg );
                printf( "[Antix] home count: %d\n", *teamCount );
                break;

            case 'a':
                *puckCount = atoi( optarg );
                printf( "[Antix] puck count: %d\n", *puckCount );
                break;

            case 'p':
                *popCount = atoi( optarg );
                printf( "[Antix] home_population: %d\n", *popCount );
                break;

            case 's':
                *worldSize = atof( optarg );
                printf( "[Antix] worldsize: %.2f\n", *worldSize );
                break;

            case 'f':
                *FOVAngle = DrawUtils::dtor(atof( optarg )); // degrees to radians
                printf( "[Antix] fov: %.2f\n", *FOVAngle );
                break;

            case 'r':
                *FOVRange = atof( optarg );
                printf( "[Antix] range: %.2f\n", *FOVRange );
                break;

            case 'w':
                *winSize = atoi( optarg );
                printf( "[Antix] winsize: %d\n", *winSize );
                break;

        default:
            break;
        }
}