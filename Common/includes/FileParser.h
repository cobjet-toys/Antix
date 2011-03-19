#ifndef __FILE_PARSER__
#define __FILE_PARSER__

#include <string>
#include <vector>
#include <fstream>
#include <string.h>

/*
 * Don't hate me I implemented this back in 08  eww ifstream, sting :P
 */

class FileParser {
	
public:
	
	FileParser();
	~FileParser();
	int readFile(std::string filename, void *args);
	virtual int handler(std::vector<std::string> commands, void *args) = 0;
	
private:
	void tokenizer(std::string &str, std::vector<std::string> &strVec);
	
};

#endif