
#ifndef _DRAWCLIENT_H
#define	_DRAWCLIENT_H

#include "logger/logger.h"

namespace Network
{
    class DrawClient {

	public:
	    DrawClient();
	    //virtual ~DrawClient();
	    void update();

	private:
	    int m_totalRobots;
	    int m_totalPucks;
	    int m_totalHomes;
	    AntixUtils::Logger* posDB;


	    //AntixUtils::RHash* posDB;

	    #define POS_DB_NAME		"posDB"
	    #define FIELD_ID		"id"
	    #define FIELD_POSX		"posX"
	    #define FIELD_POSY		"posY"
	    #define FIELD_ORIENTATION	"or"
	    #define FIELD_HOME		"home"
	    #define FIELD_HASPUCK	"has_puck"
    };
}

#endif	/* _DRAWCLIENT_H */

