/* 
 * File:   DrawClient.cpp
 * Author: cla18
 * 
 * Created on March 6, 2011, 5:31 PM
 */

#include <boost/detail/container_fwd.hpp>
#include <list>

#include "DrawClient.h"

using namespace Network;

DrawClient::DrawClient()
{
    m_NumOfRobots = 1000;
}

DrawClient::~DrawClient()
{

}

void DrawClient::setNumOfRobots(int x)
{
    m_NumOfRobots = x;
}

int DrawClient::handler(int fd)
{
    unsigned int x =0;
    printf("handeling fd %i\n", fd);

    TcpConnection * curConnection = m_serverList[fd];
    if (curConnection == NULL) return -1; //no socket descriptor

    /* Init Robot Data */
    std::list<float> robotPosX;
    for (int i=0; i<m_NumOfRobots; i++)
    {
        robotPosX.push_back(i%1000);
    }

    for(;;)
    {
        /* Send Message Header */
        Msg_header header = {SENDER_GRIDSERVER, MSG_DRAWROBOT};
        unsigned char buf[header.size];

        printf("status of pack %u of %u\n", pack(buf, "hh", header.sender, header.message), header.size);
        //printf("%i: Sending Message %i\n", x,m_heartbeat);
        if (curConnection->send(buf, header.size) < 0)
        {
            printf("Failed to send header\n");
            return -1;
        }

        /* Generate Robot Data*/
        int perRobotLen = sizeof(float)*3 + sizeof(char)*3;
        int dataLen = m_NumOfRobots * perRobotLen;
        char * dataBuf = 0;
        char * writePos = dataBuf;
        dataBuf = (char*)malloc(dataLen);
        
        for (std::list<float>::iterator i=robotPosX.begin(); i != robotPosX.end(); i++)
        {
            float orientation = 1.0;
            float val = *i;
            writePos += sprintf(writePos, "%f,%f,%f;", val, (float)(((int)val+x)%1000), orientation);
            printf("%f,%f,%f;", val, (float)(((int)val+x)%1000), orientation);
        }        

        /* Send Robot Data */
        //Msg_DrawRobot robotMsg;
        unsigned char msgBuf[dataLen];
        dataLen = pack(msgBuf, "s", dataBuf);
        printf("%i Sending robot data\n", x);
        if (curConnection->send(msgBuf, dataLen) < 0)
        {
            printf("Failed to send heartbeat\n");
            free(dataBuf);
            return -1;
        }

        free(dataBuf);
        x++;
    }
}