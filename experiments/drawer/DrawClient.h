
#ifndef _DRAWCLIENT_H
#define	_DRAWCLIENT_H

#include "logger/logger.h"
#include "dutils.h"

namespace Network
{
    class DrawClient {

	public:
	    DrawClient(int homes=1, int pucks=5, int homePop=10);
	    virtual ~DrawClient();
	    void update();

	private:
	    int m_totalRobots;
	    int m_totalPucks;
	    int m_totalHomes;
	    AntixUtils::Logger* posDB;
    };
}

#endif	/* _DRAWCLIENT_H */

