#ifndef __TYPES_H__
#define __TYPES_H__


#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using std::string;

namespace Antix
{

const string DEFAULT_REDIS_PORT = "6379";

typedef unsigned int uid;
typedef std::pair<string, string> ConnectionPair;
typedef std::pair<int, int> TeamGridPair;
typedef std::vector<ConnectionPair> ConnectionList;
typedef std::vector<string> StringList;
typedef ConnectionList::iterator ConListIterator;
typedef std::map<uid, std::vector<TeamGridPair> > ClientList;

}

#endif // __TYPES_H__
