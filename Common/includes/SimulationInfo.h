class SimInfo
{
public:
	SimInfo() {};
	~SimInfo() {};
	
	static int NUM_TEAMS;
	static int ROBOTS_PER_TEAM;
	static int NUM_CLIENT_PROCESSES;
	static int NUM_GRIDS;
	static int WINDOW_SIZE;
	static float WORLD_SIZE;
	static float HOME_RADIUS;
	static bool ENABLE_FOV;
	static float FOV_ANGLE;
	static float FOV_RANGE;
	static char* CLIENT_NUM;
};
