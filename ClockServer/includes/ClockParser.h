#ifndef __CLOCK_PARSER_H__
#define __CLOCK_PARSER_H__

#include "FileParser.h"
#include <string>

class ClockParser: public FileParser
{

public:
    ClockParser();
    ~ClockParser();
private:
    virtual int handler(std::vector<std::string> commands, void *args);

};

#endif
