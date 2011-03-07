#ifndef _DRAWCLIENT_H
#define	_DRAWCLIENT_H

#include "Client.h"
#include "Messages.h"
#include "Packer.h"

namespace Network
{

class DrawClient: public Client
{
    public:
        DrawClient();
        virtual ~DrawClient();
        virtual int handler(int fd);

        void setNumOfRobots(int x);

    private:
        int m_NumOfRobots;
    };
}

#endif	/* _DRAWCLIENT_H */

