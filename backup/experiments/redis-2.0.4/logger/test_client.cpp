#include "redisclient.h"
#include "logger.h"

#include <iostream>

using namespace antix;

int main()
{
  try 
  {
    logger & log = * (new logger());
    log.clear();
    log.append(new logitem("log message 1"));
    log.append(new logitem("log message 2"));
    log.append(new logitem("log message 3"));

    vector<antix::logitem> items = log.logitems();
    for(unsigned int i=0; i<items.size(); i++)
        cout << items[i].print() << endl;
  } 
  catch (redis::redis_error & e) 
  {
    cerr << "got exception: " << e.what() << endl << "FAIL" << endl;
    return 1;
  }

  cout << endl << "testing completed successfully" << endl;
  return 0;
}
