#include "logger.h"
#include <iostream>
#include <fstream>

using namespace AntixUtils;


int main()
{
  try 
  {
    Logger & log = * (new Logger());
    ofstream myfile;
    myfile.open ("redis_report.csv");    
    if (!myfile.is_open() || !myfile.good())
    {
	cout << "File failed to be created." << endl;
	return 1;
    }

    myfile << "Host" << '\t' << "Time" << '\t' << "Message" << endl;

    vector<AntixUtils::LogItem> items = log.logitems();
    for(unsigned int i=0; i<items.size(); i++)
    {
        time_t t = time(NULL);
        string timestr = asctime(localtime(&t));
        myfile << items[i].host << '\t' << timestr.substr(0,timestr.length()-1) << '\t' << items[i].message << endl;
        cout << items[i].host << '\t' << timestr.substr(0,timestr.length()-1) << '\t' << items[i].message << endl;
    }

    myfile.close();    
  } 
  catch (std::exception & e) 
  {
    cerr << "got exception: " << e.what() << endl << "FAIL" << endl;
    return 1;
  }

  cout << endl << "testing completed successfully" << endl;
  return 0;
}
