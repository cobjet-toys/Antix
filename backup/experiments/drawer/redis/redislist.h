/*
 * File:   Logger.h
 * Author: cla18
 *
 * Created on February 3, 2011, 10:03 PM
 */

#ifndef _LOGGER_H
#define	_LOGGER_H

#include "redisclient.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <time.h>
#include <unistd.h>
#include "../dutils.h"

#define DEFAULT_HOST    "localhost"
#define DEFAULT_PORT    6379
#define DEFAULT_DBINDEX 0
#define DEFAULT_LOG_KEY "log"
#define DEFAULT_TIMEOUT 2


using namespace std;
using namespace redis;

namespace AntixUtils
{
    class LogItem
    {

    private:
        static string CURRENT_HOST;
        static string getCurrentHost()
        {
            char hostName[255];
            bzero(hostName, 255);
            return gethostname(hostName, 255) == -1 ?  "unknown" : hostName;
        }

    public:
        string  message;
        tm      timestamp;
        string  host;
        bool    dataOnly;

        LogItem(string hostName, tm time, string msg)
	{
            this->host = hostName;
            this->timestamp = time;
            this->message = msg;
	}
        LogItem(string msg="", bool data=false)
            :message(msg), dataOnly(data)
        {
            //this->host = CURRENT_HOST;
	    this->host = getCurrentHost();
            time_t t = time(NULL);
            this->timestamp = *(localtime(&t));
        }
        LogItem(const LogItem &item)
        {
            this->host = item.host;
            this->timestamp = item.timestamp;
            this->message = item.message;
            this->dataOnly = item.dataOnly;
        }

	static void init()
	{
	    LogItem::CURRENT_HOST = LogItem::getCurrentHost();
	}

        /***
         * Returns a string in the following format:
         * <host>@<timestamp>: <message>
         * The timestamp is formatted according to ctime()
         * */
        string print()
        {
            if (this->dataOnly)
                return this->message;

            time_t t = mktime(&timestamp);
            string timestr = asctime(localtime(&t));
            return host + "@" + timestr.substr(0,timestr.length()-1) + ": " + message;
        }

        /***
         * Populates the LogItem with the formatted string according to the same
         * format used in print()
         * */
        void rprint(string data)
        {
            if (this->dataOnly)
            {
                this->message = data;
                return;
            }

            int index = data.find('@',0);
            if (index < 0) return;

            this->host = data.substr(0,index);
            index++;
            const char* remains = strptime(data.substr(index,data.length() - index).c_str(),"%c", &this->timestamp);
            if (remains != NULL)
                this->message = remains+1;
        }
    };

    

    class Logger
    {
    public:
        Logger(string host = DEFAULT_HOST, uint16_t port = DEFAULT_PORT, int dbindex = DEFAULT_DBINDEX)
        {
            this->m_client = new client(host,port,dbindex);
            if (!this->m_client)
            {
                cout << "Logger initialization failed. (HOST="
                        << host << ",PORT=" << port << ",DBINDEX=" << dbindex << ")" << endl;
                exit(1);
            }

            this->m_logKey = DEFAULT_LOG_KEY;
            this->m_maxLogItems = -1;
            this->m_len = this->m_client->llen(this->m_logKey);
        }        

        void setLogKey(string newKey)
        {
            this->m_logKey = newKey;
        }

        void setDataOnly(bool val)
        {
            this->m_dataOnly = val;
        }

        int append(LogItem * item)
        {
            try
            {
                this->m_len = this->m_client->rpush(this->m_logKey, item->print());
                if (this->m_maxLogItems > 0 && this->m_len > this->m_maxLogItems)
                {
                    m_client->lpop(this->m_logKey);
                    this->m_len--;
                }
            } catch (redis_error & e)
            {
                cout << "Error: Failed to append log item. <" << e.what() << ">" << endl;
            }

            return this->m_len;
        }

        int append(string message)
        {
            return append(new LogItem(message, this->m_dataOnly));
        }

        int trim(int remainingStartIndex, int remainingEndIndex)
        {
            try
            {
                this->m_client->ltrim(this->m_logKey, remainingStartIndex, remainingEndIndex);
            } catch (redis_error & e)
            {
                cout << "Error: Failed to trim log. <" << e.what() << ">" << endl;
            }

            this->m_len = this->m_client->llen(this->m_logKey);
            return this->m_len;
        }

        int clear()
        {
            return trim(this->m_len+1, this->m_len+1);
        }

        /**
         * Deletes all the keys in the current database connection
         **/
        void flushall()
        {
            try
            {
                this->m_client->flushall();

            } catch (redis_error & e)
            {
                cout << "Error: Failed to delete all keys from current DB. <" << e.what() << ">" << endl;
            }

            this->m_len = 0;
        }

        vector<LogItem> logitems()
        {
            // Start clock (for testing purposes) //
            clock_t start = clock();

            client::string_vector items;
            int len = this->m_client->lrange(this->m_logKey, 0, -1, items);

            vector<LogItem> results;
            for (int i=0; i<len; i++)
            {
                LogItem item;
                item.dataOnly = this->m_dataOnly;
                item.rprint(items[i]);
                results.push_back(item);
            }
  
            // Print time in (ms) that it took to fetch data 
            double elapsed = (clock() - start)/(double)CLOCKS_PER_SEC*MILLISECS_IN_SECOND;
            printf("Fetch: %fms\n", elapsed);

            return results;
        }

        LogItem* blpop()
        {
            try
            {
                string msg = this->m_client->blpop(this->m_logKey, DEFAULT_TIMEOUT);
                if (msg != client::missing_value())
                {
                    return new LogItem(msg, this->m_dataOnly);
                }
            } catch (redis_error & e)
            {
                cout << "Error: Failed to pop an item from the list. <" << e.what() << ">" << endl;
            }

            return NULL;
        }

        int size()
        {
            return m_len;
        }

    private:
        client * m_client;
        string m_logKey;
        bool m_dataOnly;
        int m_maxLogItems;
        int m_len;
    };
}

#endif	/* _LOGGER_H */

