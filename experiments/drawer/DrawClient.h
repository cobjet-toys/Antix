
#ifndef _DRAWCLIENT_H
#define	_DRAWCLIENT_H

#include "redis/redislist.h"
#include "dutils.h"

namespace Network
{
    class DrawClient {

	public:
	    DrawClient();
	    virtual ~DrawClient();
      void init(string db_host="localhost", int homes=1, int pucks=5, int homePop=10000);
	    void update();

	private:
	    int m_totalRobots;
	    int m_totalPucks;
	    int m_totalHomes;
	    AntixUtils::Logger* posDB;
    };
}

#endif	/* _DRAWCLIENT_H */

