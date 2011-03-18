#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>
#include "client.pb.h"
using namespace std;

void PromptForMessage(Client::RobotMessage* rmsg){
    
cout << "Pretend to be a server sending a message to a client!\n";

cout << "Set neighbor robot\n";
for(int i = 0; i < 10000; i++) {
    Client::RobotMessage::NeighborRobots* neighbor = rmsg->add_neighbors();

    int id = 1;

    neighbor->set_id(id);
		
		
    float pos = 10;

    neighbor->set_position(pos);

    float speed = 100;
    neighbor->set_speed(speed);

} 


}

int main(int argc, char* argv[]) {

  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) {
    cerr << "Usage:  " << argv[0] << " MessagesFile" << endl;
    return -1;
  }

  Client::Client client;

{
    // Read the existing address book.
    fstream input(argv[1], ios::in | ios::binary);
    if (!input) {
      cout << argv[1] << ": File not found.  Creating a new file." << endl;
    } else if (!client.ParseFromIstream(&input)) {
      cerr << "Failed to parse messages." << endl;
      return -1;
    }
  }

  // add a message
  PromptForMessage(client.add_message());

  {
    // Write the new address book back to disk.
    fstream output(argv[1], ios::out | ios::trunc | ios::binary);
    if (!client.SerializeToOstream(&output)) {
      cerr << "Failed to write messages file." << endl;
      return -1;
    }
  }

  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

