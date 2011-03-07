#ifndef _RHASH_H
#define	_RHASH_H


#include "redisclient.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <time.h>
#include <unistd.h>

#ifndef DEFAULT_HOST
#define DEFAULT_HOST    "localhost"
#endif
#ifndef DEFAULT_PORT
#define DEFAULT_PORT    6379
#endif
#ifndef DEFAULT_HASH_DBINDEX
#define DEFAULT_HASH_DBINDEX 1
#endif



using namespace std;
using namespace redis;

namespace AntixUtils
{
    class RHash
    {

    public:
	RHash( const client::string_type & host = DEFAULT_HOST,
                uint16_t port = DEFAULT_PORT,
                client::int_type dbindex = DEFAULT_HASH_DBINDEX)
        {
            this->m_client = new client(host,port,dbindex);
            if (this->m_client == NULL)
            {
                cout << "Logger initialization failed. (HOST="
                        << host << ",PORT=" << port << ",DBINDEX=" << dbindex << ")" << endl;
                exit(1);
            }
        }

	vector< pair<string,string> > hgetall(string key)
	{
	    vector< pair<string,string> > results;
	    try
	    {
		this->m_client->hgetall(key, results);
	    }
            catch (redis_error & e)
            {
                cout << "Error: Failed to trim log. <" << e.what() << ">" << endl;
            }

	    return results;
	}
	
	void hmset(string key, vector< pair<string,string> > field_vals)
	{
	    try
	    {
		this->m_client->hmset(key, field_vals);
	    }
            catch (redis_error & e)
            {
                cout << "Error: Failed to trim log. <" << e.what() << ">" << endl;
            }
	}

	void hdel(string key)
	{
	    try
	    {
		this->m_client->del(key);
	    }
            catch (redis_error & e)
            {
                cout << "Error: Failed to trim log. <" << e.what() << ">" << endl;
            }
	}

	/*
	vector< vector< pair<string,string> > > hgetall(vector<string> keys)
	{
	    vector< vector< pair<string,string> > >  results;

	    try
	    {
		makecmd & cmdInfo = makecmd("HGETALL");
		*cmdInfo << keys;

		//CHECK TO SEE IF CMDINFO HAS KEY!

		command c = new command(cmdInfo);
		this->m_client->exec(c);
	    }
            catch (redis_error & e)
            {
                cout << "Error: Failed to trim log. <" << e.what() << ">" << endl;
            }


	    return results;
	}
	 */

    private:
        client * m_client;
    };
}


#endif	/* _RHASH_H */

