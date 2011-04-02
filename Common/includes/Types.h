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

typedef unsigned int uid;
typedef std::pair<string, string> ConnectionPair;
typedef std::pair<int, int> TeamGridPair;
typedef std::pair<float, float> Location;
typedef std::vector<ConnectionPair> ConnectionList;
typedef std::vector<string> StringList;
typedef ConnectionList::iterator ConListIterator;
typedef std::map<uid, std::vector<TeamGridPair> > ClientList;
typedef std::vector<Msg_SensedObjectGroupItem> SensedItemsList;
typedef std::vector<Msg_RobotInfo> RobotInfoList;
typedef std::vector<uid> IDList;
typedef std::pair<uid, std::vector<Msg_SensedObjectGroupItem> > RobotSensedObjectsPair;
typedef std::list< std::pair<float,float> > ObjectLocationList;

// For checking output of Antix::getTypes()
const int ROBOT = 0;
const int PUCK = 1;

const unsigned int SET_SPEED = 0;
const unsigned int PICKUP = 1;
const unsigned int DROP = 2;

#endif // __TYPES_H__
