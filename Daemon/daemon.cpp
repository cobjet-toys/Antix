#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>

#include "../Network/TCPInterface.h"
#include "../Network/Messages.h"
#include "../Network/Packer.h"

#define RUNNING_DIR "/tmp"
#define LOCK_FILE "exampled.lock"
#define LOG_FILE "exampled.log"

void* MsgControllerWithReady(void* args);

void log_message(char* filename, char* message)
{
    FILE *logfile;
    logfile=fopen(filename, "a");
    if(!logfile) return;
    fprintf(logfile, "%s\n", message);
    fclose(logfile);
}

void signal_handler(int sig)
{
    switch(sig){

        case SIGHUP:
            log_message(LOG_FILE, "Hangup signal caught.");
            break;

        case SIGTERM:
            log_message(LOG_FILE, "Terminate signal caught.");
            exit(0);
            break;
        }
}

void daemonize()
{
    int i, lfp;
    char str[10];

    // If parent pid is 1, the process is already a daemon 
    // pid = 1 --> the init process
    if(getppid() == 1) return;

    i = fork();
    if(i < 0) exit(1); // fork error
    if(i > 0) exit(0); // exit parent

    // child continues as daemon
    setsid(); // get new process group

    for(i=getdtablesize(); i>0; --i) close(i); // close all descriptors
    i=open("/dev/null", O_RDWR); // open stdin
    dup(i); // open stdout
    dup(i); // open stderr

    umask(027); // set file permissions for new files created
    chdir(RUNNING_DIR); // change running directory

    // open or create lock file
    // this ensures that only one daemon will run at one time
    lfp = open(LOCK_FILE, O_RDWR | O_CREAT, 0640);
    if(lfp < 0) exit(1); // cannot open
    if(lockf(lfp, F_TLOCK, 0) < 0) exit(0); // cannot lock

    // now only one daemon instance can continue:
    sprintf(str, "%d\n", getpid());
    write(lfp, str, strlen(str)); // record the pid in the lock file

    // ignore child and tty signals
    signal(SIGCHLD, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    // catch hangup and kill signals
    signal(SIGHUP, signal_handler);
    signal(SIGTERM, signal_handler);
}

main()
{
    daemonize();
    log_message(LOG_FILE, "Daemon proccess started.");

    // Connects to controller
    // Sends ready message, including CPU load average
    ConnectArgs l_ServerConInfo = {"localhost", "13337", &MsgControllerWithReady};
    InitServerConnection((void*)&l_ServerConInfo);


    // Execute command
    // int ret;
    // ret = execlp("firefox", "firefox", NULL);
}

// Messages the controller that it's ready to receive commands
// Sends the CPU load average to help the controller decide what processes to run on this machine
void* MsgControllerWithReady(void* args)
{
    // Get the CPU load average
    double load[3];
    if(getloadavg(load, 3) < 0)
    {
        // Couldn't get the load average, defaulting to -1
        load[0] = -1; load[1] = -1; load[2] = -1;
        log_message(LOG_FILE, "Unable to get CPU load average data.");
    }
    log_message(LOG_FILE, "Got CPU load average data.");

    int l_Sockfd = (intptr_t)args, l_Newfd;
    socklen_t l_ConnectSize;
    struct sockaddr_storage l_ConnectAddr;

    // Listen and accept new connections.
    l_ConnectSize = sizeof l_ConnectAddr;
    l_Newfd = accept(l_Sockfd, (struct sockaddr *)&l_ConnectAddr, &l_ConnectSize);
    if (l_Newfd == -1) {
        log_message(LOG_FILE, "TCP: Error accepting connection.");
        return (void*)0;
    }
   // ...Handle our new connection...
   log_message(LOG_FILE, "TCP: Received new connection.");

   // Create the ready message to send
   Msg_controller_ready l_Ready = {(float)(load[0]), (float)(load[1]), (float)(load[2])};

   // Create a new buffer to pack our message into.
   unsigned char l_Buffer[l_Ready.size];

   // Pack our message into the char buffer.
   pack(l_Buffer, "fff", l_Ready.load_one, l_Ready.load_five, l_Ready.load_fifteen);
    
   // Send our message to the client.
   sendAll(l_Newfd, l_Buffer, l_Ready.size);
   log_message(LOG_FILE, "Sent ready message to controller.");

   return (void*)0;
}
