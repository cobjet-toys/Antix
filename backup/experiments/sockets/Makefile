#compiler
CC = g++

#used to manage your libs add them here
SERVER_LIBS =
SERVER_INCLUDES =

SERVER_SRC = server.cpp Pack_Unpack.cpp
# do not modify this line
SERVER_OBJECTS = $(SERVER_SRC:.cpp=.o)

SERVER_BINARY = server


#used to manage your libs add them here
CLIENT_LIBS =
CLIENT_INCLUDES =

CLIENT_SRC = client.cpp Pack_Unpack.cpp
# do not modify this line
CLIENT_OBJECTS = $(CLIENT_SRC:.cpp=.o)

CLIENT_BINARY = client

#-------------------------------------------

server: server_depends $(SERVER_OBJECTS)
	$(CC) -o $(SERVER_BINARY) $(SERVER_OBJECTS) -L$(SERVER_LIBS) -I$(SERVER_INCLUDES) 

server_depends:
	$(CC) -M $(SERVER_SRC) > server_depends

server_clean:
	rm $(SERVER_BINARY) $(SERVER_OBJECTS)

include server_depends

#-------------------------------------------

client: client_depends $(CLIENT_OBJECTS)
	$(CC) -o $(CLIENT_BINARY) $(CLIENT_OBJECTS) -L$(CLIENT_LIBS) -I$(CLIENT_INCLUDES) 

client_depends:
	$(CC) -M $(CLIENT_SRC) > client_depends

client_clean:
	rm $(CLIENT_BINARY) $(CLIENT_OBJECTS)

include client_depends

#-------------------------------------------

clean:
	rm $(SERVER_BINARY) $(SERVER_OBJECTS) $(CLIENT_BINARY) $(CLIENT_OBJECTS) client_depends server_depends
