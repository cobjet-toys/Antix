#ifndef __TYPES_H__
#define __TYPES_H__


#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Messages.h"

using std::string;

const string DEFAULT_REDIS_PORT = "6379";

typedef struct{
    unsigned int id;
    float x;
    float y;
} sensed_item;

//Action is: 1 - setSpeed, 2 - pickup, 3 - drop
typedef struct{
    int action;
    float speed;
    float angle;
} action;

typedef struct{
    float x_pos;
    float y_pos;
    float speed;
    float angle;
    unsigned int puck_id;
} action_results;

typedef struct{
    unsigned int id;
    float x_pos;
    float y_pos;
    float speed;
    float angle;
    unsigned int puck_id;
} robot_info;

typedef unsigned int uid;
typedef std::pair<string, string> ConnectionPair;
typedef std::pair<int, int> TeamGridPair;
typedef std::pair<float, float> Location;
typedef std::vector<ConnectionPair> ConnectionList;
typedef std::vector<string> StringList;
typedef ConnectionList::iterator ConListIterator;
typedef std::map<uid, std::vector<TeamGridPair> > ClientList;
typedef std::map<int, std::vector<sensed_item> > SensedItems;

// For checking output of Antix::getTypes()
const int ROBOT = 0;
const int PUCK = 1;

#endif // __TYPES_H__
