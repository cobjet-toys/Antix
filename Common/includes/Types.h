#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <string>

using std::string;

namespace Antix
{

const string DEFAULT_REDIS_PORT = "6379";

typedef std::pair<string, string> ConnectionPair;
typedef std::vector< ConnectionPair > ConnectionList;
typedef ConnectionList::iterator ConListIterator;

}
