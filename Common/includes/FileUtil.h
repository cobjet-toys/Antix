#ifndef __FILEUTIL_H__
#define __FILEUTIL_H__

#include "Types.h"

using std::ifstream;
using std::cout;
using std::endl;

using boost::split;
using boost::lexical_cast;
using boost::bad_lexical_cast;

namespace Antix
{

std::vector<string> linesInFile(const char* fileName);
int parseConfigFile(const char* configFileName, ClientList& clients);
int parseServerFile(   const char* serverFileName,
                       ConnectionList& grid_servers, 
                       ConnectionList& robot_clients,
                       ConnectionPair& clock_server,
                       ConnectionPair& draw_server);

} // namespace Antix

#endif // __FILEUTIL_H__
