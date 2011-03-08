#include "DrawClient.h"

/*
 * 
 */
int main(int argc, char** argv)
{
    int m_TotalTeamCount=0, m_TotalPuckCount=0, m_RobotPopCount=0, m_WindowSize=0;
    float m_WorldSize=0, m_FOV=0, m_SensorRange=0;
    
    DrawUtils::parseOptions(argc, argv, &m_TotalTeamCount, &m_TotalPuckCount,
            &m_RobotPopCount, &m_WindowSize, &m_WorldSize, &m_FOV, &m_SensorRange);

    Network::DrawClient * dCli;
    if (argc == 1)
    {
        dCli = new Network::DrawClient();
    }
    else
    {
        dCli = new Network::DrawClient(m_TotalTeamCount, m_TotalPuckCount, m_RobotPopCount);
    }

    dCli->update();    

    return (EXIT_SUCCESS);
}

