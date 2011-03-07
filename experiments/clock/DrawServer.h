/* 
 * File:   DrawServer.h
 * Author: cla18
 *
 * Created on March 6, 2011, 1:16 PM
 */

#include "Server.h"
#include "Messages.h"
#include "Packer.h"
//#include "../../Robotix/include/Game/Home.h"
//#include "../../Robotix/include/Math/Position.h"

#ifndef _DRAWSERVER_H
#define	_DRAWSERVER_H

namespace Network
{
    class DrawServer : public Server
    {
        public:
            DrawServer();
            DrawServer(Msg_InitDrawer init);
            virtual int handler(int fd);

        private:
	    void initGraphics(int argc, char** argv);
	    void drawHome(float xloc, float yloc, float radius, float colR, float colG, float colB);

            bool m_EnableFOV;
            float m_FoVAngle;
            float m_FoVRange;
            float m_WorldSize;
            float m_WindowSize;

            //std::vector<Game::Home*> m_Homes;
    };
}

#endif	/* _DRAWSERVER_H */

