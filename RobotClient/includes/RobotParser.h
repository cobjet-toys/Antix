#ifndef __ROBOT_PARSER_H__
#define __ROBOT_PARSER_H__

#include "FileParser.h"
#include <string>

class RobotParser: public FileParser
{

public:
	RobotParser();
	~RobotParser();
private:
	virtual int handler(std::vector<std::string> commands, void *args);
	
};

#endif