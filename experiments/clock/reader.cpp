#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <boost/algorithm/string.hpp>

using std::string;
using std::ifstream;
using std::vector;
using std::cout;
using std::endl;
using std::pair;
using std::map;

int num_teams;
int robots_per_team;
int num_client_processes;
int num_grids;
int window_size;
float world_size;
float home_radius;
bool enable_fov;
float fov_angle;
float fov_range;
char* client_num;

int parse(vector<string> lines)
{
    num_teams = atoi( lines[0].c_str() );
    robots_per_team = atoi( lines[1].c_str() );
    num_client_processes = atoi( lines[2].c_str() );
    num_grids = atoi( lines[3].c_str() );
    window_size = atoi( lines[4].c_str() );
    world_size = atof( lines[5].c_str() );
    home_radius = atof( lines[6].c_str() );
    enable_fov =  (atoi(lines[7].c_str()) == 1) ? true : false;
    int i = 8;
    if(enable_fov)
    {
        i = 10;
        fov_angle = atof( lines[8].c_str() );
        fov_range = atof( lines[9].c_str() );
    }

    map<int,int> team_grid_map;
   
    for(; i < lines.size(); i++)
    {
        vector<string> tokens;
        boost::split( tokens, lines[i], boost::is_any_of(","));
        if(tokens[0] == "client" && tokens[1] == client_num)
        {
            for(int line_n = i+1; line_n < lines.size() && lines[line_n][0] != 'c'; line_n++)
            {
                vector<string> key_value_pair;
                boost::split( key_value_pair, lines[line_n], boost::is_any_of(",") );
                cout << key_value_pair[0] << key_value_pair[1] << endl;
                team_grid_map.insert(
                    pair<int,int>( atoi(key_value_pair[0].c_str()), 
                                   atoi(key_value_pair[1].c_str()) ) );
            }
            break;
        }
    }
    map<int,int>::iterator it;
    for( it = team_grid_map.begin(); it != team_grid_map.end(); it++ )
    {
        cout << "Team #: " << (*it).first << " Grid #: " << (*it).second << endl;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    if( argc < 3)
    {
        cout << "Insufficient arguments supplied, please pass a config file and a port number as parameters."
             << endl;
        return -1;
    }
    ifstream file(argv[1]);
    client_num = argv[2];
    string line;
    vector<string> lines;
    while( getline(file, line) )
    {
        cout << line << endl;
        lines.push_back(line);
    }
    cout << lines.size() << endl; 
    parse(lines);
    return 0;
}
