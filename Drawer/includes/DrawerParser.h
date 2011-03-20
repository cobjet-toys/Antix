#ifndef __DRAWER_PARSER_H__
#define __DRAWER_PARSER_H__

#include "FileParser.h"
#include <string>

class DrawerParser: public FileParser
{

public:
	DrawerParser();
	~DrawerParser();
private:
	virtual int handler(std::vector<std::string> commands, void *args);
	
};

#endif
