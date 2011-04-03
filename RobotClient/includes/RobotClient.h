#ifndef ROBOT_CLIENT_H_ 
#define ROBOT_CLIENT_H_

#include "Client.h"
#include "Messages.h"
#include "Packer.h"
#include <vector>
#include <map>
#include "RobotGame.h"

namespace Network
{

class RobotClient: public Client
{
public:
    RobotClient();
    ~RobotClient();
    virtual int handler(int fd);
    int handleNewGrid(int id); 
    int initGrid(const char * host, const char * port, const int id);
    int initClock(const char * host, const char * port);
    
    void initRobotGame();
    
    void setFOV(float fov);
	void setRadius(float radius);
	void setSensorRange(float sensorRange);
	void setPickupRange(float pickupRange);
	void setWorldSize(float worldSize);
	void setHomeRadius(float homeRadius);
    
private:

    int packHeaderMessage(unsigned char* buffer, uint16_t sender, uint16_t message);
    int sendRobotRequests();

	std::vector<int> m_Grids;
    std::map<int, int> m_GridIdToFd;
    std::map<int, int> m_GridFdToId;
    uint16_t m_HeartBeat;
    int m_ClockFd;
    unsigned int m_ReadyGrids;
    unsigned int m_ReadyActionGrids;
    RobotGame* robotGameInstance;

	uint16_t m_totalGridRequests;
	uint16_t m_totalGridResponses;
	uint16_t m_totalRobotsReceived;
	
	float m_robot_FOV;
    float m_robot_Radius;
    float m_robot_SensorRange;
    float m_robot_PickupRange;
    float m_robot_WorldSize;
    float m_robot_HomeRadius;
	
};
}

#endif

