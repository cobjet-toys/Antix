#include "../Network/TCPInterface.h"
#include <stdio.h>
#include <stdlib.h>
#include <map>

//void* ClientLoop(void* args);
void* ServerLoop(void* args);

int main(int argc, char** argv)
{
    //Create our connection arguments.
    //First parameter is IP.
    //Second is port.
    //Third is function callback to main loop function. void* func(void*)
    //To build the server, add -DSERVER to g++ flags
//#ifdef SERVER
    ConnectArgs l_ConInfo = {"localhost", "13337", &ServerLoop};
//#endif

    //To build the client, add -DCLIENT to g++ flags
//#ifdef CLIENT
//    ConnectArgs l_ConInfo = {"localhost", "13337", &ClientLoop}; 
//#endif

    //Initialize our connection. 
    InitConnection((void*)&l_ConInfo);

    return 0;
}
/*
void* ClientLoop(void* args)
{
    int l_Sockfd = (intptr_t)args;
    //..Client code goes here....
    while(1)
    {
        //Create an empty example message to receive into.
        //Message types are defined in Messages.h
        Msg_example l_Expected;

        //Create a buffer to receive into, with size of the expected message size.
        //Do NOT use sizeof message. Compilers optimize structs and pad them with extra data. *See Messages.h
        unsigned char l_Buffer[l_Expected.size];

        //Receive our message into the buffer.
        rcvAll(l_Sockfd, l_Buffer, l_Expected.size);

        //Unpack our message from the char buffer into our message struct.
        unpack(l_Buffer, "lff", &l_Expected.id, &l_Expected.xpos, &l_Expected.ypos);

        printf("Received an example message with id: %d, xpos: %f,  and ypos: %f\n", l_Expected.id, l_Expected.xpos, l_Expected.ypos);
        exit(0);
    }
    return (void*)0;
}
*/

//Server loop needs to accept incoming connections.
//Not part of TCPInterface because we can handle new connections in many different ways.
//So we leave it to be implemented as needed.
//E.g. fork() to new process, new thread, kqueue, poll etc..
void* ServerLoop(void* args)
{
    int l_Sockfd = (intptr_t)args, l_Newfd;
    socklen_t l_ConnectSize;
    struct sockaddr_storage l_ConnectAddr;

	std::map<int, int> l_Machines;
    while(1)
    {
        //Listen and accept new connections.
        l_ConnectSize = sizeof l_ConnectAddr;
        l_Newfd = accept(l_Sockfd, (struct sockaddr *)&l_ConnectAddr, &l_ConnectSize);
        if (l_Newfd == -1) {
            perror("Log: Error accepting connection.");
            continue;
        }
       //...Handle our new connection...
       printf("Log: Received new connection.\n");

	   l_Machines[l_Newfd]=1;
       //Create a new message to send.
       Msg_example l_Example = {1337, 3.14159, 2.71828};

       //Create a new buffer to pack our message into.
       //Do NOT use sizeof message. Compilers optimize structs and pad them with extra data. *See Messages.h
       unsigned char l_Buffer[l_Example.size];

       //Pack our message into the char buffer.
       pack(l_Buffer, "lff", l_Example.id, l_Example.xpos, l_Example.ypos);


		//Keep track of which machines available, whats running on each machine
		//decide which machine to send client or server
		//Hash  available machines
		//Use file descriptor as key
		
		//
		
       //Send our message to the client.
       sendAll(l_Newfd, l_Buffer, l_Example.size);
    }
    return (void*)0;
}
