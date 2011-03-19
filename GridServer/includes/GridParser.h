#ifndef __GRID_PARSER_H__
#define __GRID_PARSER_H__

#include "FileParser.h"
#include <string>

class GridParser: public FileParser
{
public:
	GridParser();
	~GridParser();
private:
	virtual int handler(std::vector<std::string> commands, void *args);
	
};

#endif