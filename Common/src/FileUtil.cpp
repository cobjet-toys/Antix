#include "FileUtil.h"
#include "SimulationInfo.h"

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

int parseConfigFile(const char* configFileName, ClientList& clients)
{
    std::vector<string> lines = linesInFile(configFileName);
    // Catch incorrect lexical casts if we make them
    try
    {
        int loop_start = 8;
        SimInfo::NUM_TEAMS = lexical_cast<int>( lines[0] );
        SimInfo::ROBOTS_PER_TEAM = lexical_cast<int>( lines[1] );
        SimInfo::NUM_CLIENT_PROCESSES = lexical_cast<int>( lines[2] );
        SimInfo::NUM_GRIDS = lexical_cast<int>( lines[3] );
        SimInfo::WINDOW_SIZE = lexical_cast<int>( lines[4] );
        SimInfo::WORLD_SIZE = lexical_cast<float>( lines[5] );
        SimInfo::HOME_RADIUS = lexical_cast<float>( lines[6] );
        SimInfo::ENABLE_FOV = (lexical_cast<int>( lines[7] ) == 1) ? true : false;
        if(SimInfo::ENABLE_FOV)
        {
            SimInfo::FOV_ANGLE = lexical_cast<float>( lines[8] );
            SimInfo::FOV_RANGE = lexical_cast<float>( lines[9] );
            loop_start = 10;
        }
        
        int cur_client = -1;
        for( std::vector<string>::iterator iter = lines.begin()+loop_start;
             iter != lines.end();
             iter++)
        {
            //cout << (*iter) << endl;
            StringList tokens;
            split(tokens, (*iter), boost::is_any_of(","));
            if(tokens[0] == "client")
            {
                cur_client = lexical_cast<int>( tokens[1] );
            }
            else
            {
                int team_num = lexical_cast<int>( tokens[0] );
                int grid_num = lexical_cast<int>( tokens[1] );
                //cout << "clients[" << cur_client << "]";
                //cout << "\t" << team_num << " " << grid_num << endl;
                clients[cur_client].push_back(TeamGridPair(team_num, grid_num));
                cout << clients[cur_client].size() << endl;
                cout << clients[cur_client].at(0).first << endl;
            }
        }

    }
    catch(bad_lexical_cast& b)
    {
        cout << "Bad cast dog!" << endl;
        return -1;
    }
    cout << "Num Clients: " << clients.size() << endl;
    return 0;
}

int parseServerFile(   const char* serverFileName,
                       ConnectionList& grid_servers, 
                       ConnectionList& robot_clients,
                       ConnectionPair& clock_server,
                       ConnectionPair& draw_server)
{
    StringList lines = linesInFile(serverFileName); 
    for( StringList::iterator iter = lines.begin();
         iter != lines.end(); iter++)
    {
        //cout << (*iter) << endl;
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

} //namespace Antix


