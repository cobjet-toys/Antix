#ifndef __CONTROLLER_PARSER_H__
#define __CONTROLLER_PARSER_H__

#include "FileParser.h"
#include <string>

class ControllerParser: public FileParser
{
public:
	ControllerParser();
	~ControllerParser();
private:
	virtual int handler(std::vector<std::string> commands, void *args);
	
};

#endif