#ifndef _DRAWSERVER_H
#define	_DRAWSERVER_H

#include "logger/logger.h"

namespace Network
{
    class DrawServer {

	public:
	    DrawServer();
	    virtual ~DrawServer();
	    void draw();

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

#endif	/* _DRAWSERVER_H */

