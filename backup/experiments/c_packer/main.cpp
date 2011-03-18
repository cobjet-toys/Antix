#include <stdio.h>
#include <time.h>
#include "Pack_Unpack.h"

#define MAX_MESSAGES 1000000
#define MILLISECS_IN_SECOND 1000.0

int main(int argc, char** argv)
{
    unsigned char lBuffer[1024];
    int32_t lId;
    float32_t lXPos;
    float32_t lYPos;

       
    clock_t lBegin, lEnd;
    //Number of milliseconds to compute.
    int32_t lDiffTime;

    int16_t lPacketsize;

    lBegin = clock();
    //Pack an int, float, and float into a string buffer.
    for (int i = 0; i < MAX_MESSAGES; i++)
               lPacketsize = pack(lBuffer, "lff", (int32_t)1, (float32_t)0.234, (float32_t)0.543);
    lEnd = clock();

    lDiffTime = (float32_t)(lEnd - lBegin)/(float32_t)CLOCKS_PER_SEC*MILLISECS_IN_SECOND;

    printf("It took %d milliseconds to pack %d messages.\n", lDiffTime, MAX_MESSAGES);
    printf("Packet size is %hd bytes\n\n", lPacketsize);
    
    lBegin = clock();
    for (int i = 0; i < MAX_MESSAGES; i++)
        unpack(lBuffer, "lff", &lId, &lXPos, &lYPos);
    lEnd = clock();

    lDiffTime = (float32_t)(lEnd - lBegin)/(float32_t)CLOCKS_PER_SEC*MILLISECS_IN_SECOND;

    printf("It took %d milliseconds to unpack %d messages.\n", lDiffTime, MAX_MESSAGES);
    printf("Id: %d, XPos: %f, Ypos: %f\n", lId, lXPos, lYPos);
    return 0;
}
