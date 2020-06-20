#ifndef __World__
#define __World__

#include <string>
#include <list>
#include <time.h>
#include "room.h"
#define TICK_FREQUENCY 0.5f

using namespace std;

class Entity;
class Player;

class World
{
public :

	World();
	~World();

	bool Tick(vector<string>& args);
	bool ParseCommand(vector<string>& args);
	void GameLoop();
	
	

private:
	char* ReadFile(string name);
	void LoadRoomsFromFile(string roomFile);
	void LoadExitsFromFile(string exitFile);
	Room* SearchRoom(string name);
	clock_t tick_timer;
	list<Entity*> entities;
	Player* player;
};

#endif //__World__