#include "../Network/Messages.h"
#include "../Network/Packer.h"
#include "../Network/TCPInterface.h"
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <pthread.h>
#include <unistd.h>

void* ClientLoop(void* args);
void* ServerLoop(void* args);
void* UserInputLoop(void* args);

int main(int argc, char** argv)
{
	//Create our two threads
	pthread_t receiveMsg, receiveInput;
    //Create our connection arguments.
    //First parameter is IP.
    //Second is port.
    //Third is function callback to main loop function. void* func(void*)
    //To call the server, use InitServerConnection(&l_ServerConInfo);
    ConnectArgs l_ServerConInfo = {"localhost", "13337", &ServerLoop};


    //To call the client, use InitClientConnection(&l_ClientConInfo);
    ConnectArgs l_ClientConInfo = {"localhost", "13337", &ClientLoop}; 


	int  thread1, thread2;
	//Spawn thread to wait for socket message
	thread1 = pthread_create( &receiveMsg, NULL, InitClientConnection, (void*)&l_ClientConInfo);
	//Spawn thread to wait for user input
	thread2 = pthread_create( &receiveInput, NULL, UserInputLoop, (void*)0);
    //Initialize our connection.
	//InitConnection((void*)&l_ConInfo);
	
	pthread_join( receiveInput, NULL);
	pthread_join( receiveMsg, NULL);
	
    return 0;
}

void* UserInputLoop(void* args){
	printf("Want to play a game?\n");
	while(1){
		//Wait for user command
	}
	return (void*)0;
}

void* ClientLoop(void* args)
{
    int l_Sockfd = (intptr_t)args;
    //..Client code goes here....
    while(1)
    {
        //Create an empty example message to receive into.
        //Message types are defined in Messages.h
        Msg_controller_ready l_Expected;

        //Create a buffer to receive into, with size of the expected message size.
        //Do NOT use sizeof message. Compilers optimize structs and pad them with extra data. *See Messages.h
        unsigned char l_Buffer[l_Expected.size];

        //Receive our message into the buffer.
        rcvAll(l_Sockfd, l_Buffer, l_Expected.size);

        //Unpack our message from the char buffer into our message struct.
        unpack(l_Buffer, "lff", &l_Expected.load_one, &l_Expected.load_five, &l_Expected.load_fifteem);

        printf("Received an example message with one: %f, five: %f,  and fifteen: %f\n", l_Expected.load_one, l_Expected.load_five, l_Expected.load_fifteen);
		
		//Wait
        pthread_yield();
    }
    return (void*)0;
}


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
