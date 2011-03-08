#ifndef _DUTILS_H
#define	_DUTILS_H

#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define MILLISECS_IN_SECOND 1000

#define POS_DB_NAME         "posDB"
#define POS_PATTERN         "%f %f %f %d %c"
#define MAX_POS_KEYS        3


namespace DrawUtils
{
    float rtod( const float& r );
    float dtor( const float& d);

    //sin

    //cos

    void parseOptions(
            int argc,
            char** argv,
            int* teamCount,
            int* puckCount,
            int* popCount,
            int* winSize,
            float* worldSize,
            float* FOVAngle,
            float* FOVRange);
}

#endif	/* _DUTILS_H */