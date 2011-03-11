#include "logger.h"
#include <iostream>
#include <fstream>

using namespace AntixUtils;


int main()
{
  try 
  {
    Logger * log = new Logger();
    log->setDataOnly(true);
    log->setLogKey("posDB0");
    vector<AntixUtils::LogItem> items = log->logitems();
    for(unsigned int i=0; i<items.size(); i++)
    {
        cout << items[i].message << endl;
    }  
  } 
  catch (std::exception & e) 
  {
    cerr << "got exception: " << e.what() << endl << "FAIL" << endl;
    return 1;
  }

  cout << endl << "testing completed successfully" << endl;
  return 0;
}
