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
	list<Player*> availableRols;
	Player* player;
	bool World::RoleMessage(string rolename);
private:
	char* ReadFile(string name);
	void LoadRoomsFromFile(string roomFile);
	void LoadExitsFromFile(string exitFile);
	void LoadCreaturesFromFile(string creaturesFile);
	void LoadItemsFromFile(string itemsFile);
	void LoadRoleSettingsFromFile(string playerFile);
	void AutoEquipCreatures();
	char* GetXmlDocumentText(string fileName, string mainTag);
	Entity* SearchEntity(string name);
	clock_t tick_timer;
	list<Entity*> entities;
};

#endif //__World__