#include "FileParser.h"
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include "Config.h"

FileParser::FileParser()
{
	
}

FileParser::~FileParser()
{
	
}

void FileParser::tokenizer(std::string &str, std::vector<std::string> &strVec)
{
    strVec.clear();
    int offset = 0;
    int last = 0;

    if (str.length() > 0)
    {
        while(offset > -1)
        {
            offset = str.find_first_of(" ", offset+1);
            if (offset == 1)
            {
                strVec.push_back(str.substr(last, offset-last));
                if (strVec.back().length() == 0) strVec.pop_back();
            } else {
                if (last> 0) strVec.push_back(str.substr(last+1, offset-last-1));
                else  strVec.push_back(str.substr(last, offset));   
                if (strVec.back().length() == 0) strVec.pop_back();
            }
            last = offset;
        }
    }
        
}

int FileParser::readFile(std::string filename, void *args)
{
	DEBUGPRINT("FILE_PARSER STATUS:\t Parsing file\n");
    char temp[256];
    std::string str;

    std::vector<std::string> fileContainer;
    
    std::ifstream file(filename.c_str(), std::ios::in);
	if (!file.is_open()) return ENOENT;
    while (file.eof() != true)
    {	
        fileContainer.clear();
        file.getline(temp, 256);
        str = temp;
        str = str.substr(0, strlen(temp));
        FileParser::tokenizer(str, fileContainer);
        if (fileContainer.size() > 0)
        {
			DEBUGPRINT("FILE_PARSER STATUS:\t Handeling line %s\n", temp);
            int l_res = handler(fileContainer, args);
			if (l_res < 0) return -1;
        } else {
			DEBUGPRINT("FILE_PARSER STATUS:\t Empty line read %s\n", temp);
		}
    }    
    return 0;
}
