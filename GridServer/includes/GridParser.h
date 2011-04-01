#ifndef __GRID_PARSER_H__
#define __GRID_PARSER_H__

#include "FileParser.h"
#include <string>

class GridParser: public FileParser
{
public:
	GridParser();
	~GridParser();
	int verify();
private:
	virtual int handler(std::vector<std::string> commands, void *args);

	bool m_hasNumTeams;
	bool m_hasRobotsPerTeam;
	bool m_hasIdRange;
	bool m_clock;
	
};

#endif
