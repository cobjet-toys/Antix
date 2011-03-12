#ifndef __UTIL_H__
#define __UTIL_H__

#include "Types.h"

using std::ifstream;
using std::cout;
using std::endl;

using boost::split;
using boost::lexical_cast;

namespace Antix
{

std::vector<string> linesInFile(const char* fileName)
{
    ifstream file(fileName);
    string line;
    std::vector<string> lines;
    while( getline(file, line) )
    {
        lines.push_back(line);
    }
    return lines;
}

int parseServerFile( const char* serverFileName,
                       ConnectionList& grid_servers, 
                       ConnectionList& robot_clients,
                       ConnectionPair& clock_server,
                       ConnectionPair& draw_server)
{
    std::vector<string> lines = linesInFile(serverFileName); 
    for( std::vector<string>::iterator iter = lines.begin();
         iter != lines.end(); iter++)
    {
        cout << (*iter) << endl;
        std::vector<string> tokens;
        split(tokens, (*iter), boost::is_any_of(",") );
        if( tokens[0] == "clock")
        {
            clock_server.first = tokens[1];
            clock_server.second = tokens[2];
        }
        else if( tokens[0] == "grid")
        {
            ConnectionPair grid(tokens[1], tokens[2]);
            grid_servers.push_back(grid);
        }
        else if( tokens[0] == "drawer")
        {
            draw_server.first = tokens[1];
            draw_server.second = DEFAULT_REDIS_PORT;
        }
        else if( tokens[0] == "client")
        {
            ConnectionPair client(tokens[1], "");
            robot_clients.push_back(client);
        }
    }
    return 0;
}

} // namespace Antix

#endif // __UTIL_H__
