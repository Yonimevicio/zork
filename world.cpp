#include <iostream>
#include "globals.h"
#include "entity.h"
#include "creature.h"
#include "item.h"
#include "exit.h"
#include "room.h"
#include "player.h"
#include "world.h"
#include "rapidxml.hpp"
#include "rapidxml_iterators.hpp"
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"
#include <algorithm> 
#include <string> 
using namespace rapidxml;
char * World::ReadFile(string name) {
	ifstream file;
	file.open(name);
	if (file.is_open()) {
		file.seekg(0, std::ios_base::end);
		int length = file.tellg();
		file.seekg(0, std::ios_base::beg);
		char* buffer = new char[length];
		file.read(buffer, length);
		return buffer;
	} else return "";
	
}
void World::LoadRoomsFromFile(string roomFile) {	
	xml_document<> doc;
	doc.parse<0>(GetXmlDocumentText(roomFile, "Rooms"));
	xml_node<> *node = doc.first_node("Rooms");
	for (xml_node<> *subNode = node->first_node("Room");
		subNode; subNode = subNode->next_sibling())
	{
		char* roomName = subNode->first_node("Name")->value();
		char* roomDesc = subNode->first_node("Description")->value();
		entities.push_back(new Room(roomName,roomDesc));
	}
}
void World::LoadExitsFromFile(string exitFile) {
	xml_document<> doc;
	doc.parse<0>(GetXmlDocumentText(exitFile, "Exits"));
	xml_node<> *node = doc.first_node("Exits");
	for (xml_node<> *subNode = node->first_node("Exit");
		subNode; subNode = subNode->next_sibling())
	{
		char* exitName = subNode->first_node("Name")->value();
		char* exitOpName = subNode->first_node("OppositeName")->value();
		char* exitDesc = subNode->first_node("Description")->value();
		char* exitOrigin= subNode->first_node("Origin")->value();
		char* exitDestination = subNode->first_node("Destination")->value();
		char* exitOneWay = subNode->first_node("OneWay")->value();
		char* exitLocked = subNode->first_node("Locked")->value();
		Room* origin = (Room*)SearchEntity(exitOrigin);
		Room* destination = (Room*)SearchEntity(exitDestination);
		string eLStr = exitLocked;
		string eOStr = exitOneWay;
		Exit* ex = new Exit(exitName, exitOpName, exitDesc, origin, destination, eOStr == "1");
		if (eLStr == "1") ex->locked = true;
		entities.push_back(ex);
	}
}
void World::LoadCreaturesFromFile(string creaturesFile) {
	xml_document<> doc;
	doc.parse<0>(GetXmlDocumentText(creaturesFile,"Creatures"));
	xml_node<> *node = doc.first_node("Creatures");
	for (xml_node<> *subNode = node->first_node("Creature");
		subNode; subNode = subNode->next_sibling())
	{
		char* creatureName = subNode->first_node("Name")->value();
		char* creatureDesc = subNode->first_node("Description")->value();
		char* creatureRoom = subNode->first_node("Room")->value();
		char* creatureHP = subNode->first_node("HP")->value();
		Room* room = (Room*)SearchEntity(creatureRoom);
		Creature* cr = new Creature(creatureName, creatureDesc, room);
		cr->hit_points = atoi(creatureHP);
		entities.push_back(cr);
	}
}
void World::LoadItemsFromFile(string itemsFile) {
	xml_document<> doc;
	doc.parse<0>(GetXmlDocumentText(itemsFile, "Items"));
	xml_node<> *node = doc.first_node("Items");
	for (xml_node<> *subNode = node->first_node("Item");
		subNode; subNode = subNode->next_sibling())
	{
		char* itemName = subNode->first_node("Name")->value();
		char* itemDesc = subNode->first_node("Description")->value();
		char* itemLocation = subNode->first_node("Location")->value();
		char* itemType = subNode->first_node("Type")->value();
		char* itemMinValue = subNode->first_node("MinValue")->value();
		char* itemMaxValue = subNode->first_node("MaxValue")->value();
		char* keyof = subNode->first_node("KeyOf")->value();
		Entity* ent = SearchEntity(itemLocation);
		ItemType itype = COMMON;
		string strType = itemType;
		if (strType == "WEAPON") itype = WEAPON;
		if (strType == "ARMOUR") itype = ARMOUR;
		Item* item = new Item(itemName, itemDesc, ent, itype);
		if (itype != COMMON) {
			item->max_value = atoi(itemMaxValue);
			item->min_value = atoi(itemMinValue);
		}
		Entity* entLocked = SearchEntity(keyof);
		if (entLocked != NULL)((Exit*)entLocked)->key = item;
		entities.push_back(item);
	}
}
void World::LoadRoleSettingsFromFile(string playerFile) {
	xml_document<> doc;
	doc.parse<0>(GetXmlDocumentText(playerFile, "Roles"));
	xml_node<> *node = doc.first_node("Roles");
	for (xml_node<> *subNode = node->first_node("Role");
		subNode; subNode = subNode->next_sibling())
	{
		char* roleName = subNode->first_node("Name")->value();
		char* roleHP = subNode->first_node("HitPoints")->value();
		char* roleDescription = subNode->first_node("Description")->value();
		Room* room = (Room*)SearchEntity("The Woods");
		Player* pl = new Player(roleName, roleDescription, room);
		pl->hit_points = atoi(roleHP);
		availableRols.push_back(pl);
	}
}
void World::AutoEquipCreatures() {
	for (list<Entity*>::const_iterator it = entities.begin(); it != entities.cend(); ++it) {
			if ((*it)->type == CREATURE) ((Creature*)(*it))->AutoEquip();
	}
}
char* World::GetXmlDocumentText(string fileName, string mainTag) {
	char* fileContent = ReadFile(fileName);
	string s = fileContent;
	int i = s.find_last_of(mainTag + ">");
	s.erase(s.begin() + i + 1, s.end());
	strcpy(fileContent, s.c_str());
	return fileContent;
}
bool World::RoleMessage(string rolename) {
	for (list<Player*>::const_iterator it = availableRols.begin(); it != availableRols.cend(); ++it)
	{
		if (Same((*it)->name, rolename)) {
			cout << rolename + "\n";
			cout << (*it)->description + "\n";
			string hp = to_string((*it)->hit_points);
			cout << "Hitpoints: " +  hp  + "\n" ;
			cout << "Are you sure you dreamed this? y/n \n";
			char sure;
			cin >> sure;
			if (sure == 'y') {
				player = (*it);
				player->maxHitpoints = player->hit_points;
				entities.push_back(player);
				return true;
			}
			else return false;
		}

	}
}
Entity* World::SearchEntity(string name) {
	for (list<Entity*>::const_iterator it = entities.begin(); it != entities.cend(); ++it)
	{
		if (Same((*it)->name, name)) {
			if ((*it)->type == ROOM) return (Room*)(*it);
			if ((*it)->type == CREATURE) return (Creature*)(*it);
			if ((*it)->type == ITEM) return (Item*)(*it);
		}else if (Same((*it)->description, name)) return (Exit*)(*it);
	}
	return NULL;
}
// ----------------------------------------------------
World::World()
{
	tick_timer = clock();

	// Rooms ----
	LoadRoomsFromFile("defaultRooms.xml");
	/*Room* forest = new Room("Forest", "You are surrounded by tall trees. It feels like a huge forest someone could get lost easily.");
	Room* house = new Room("House", "You are inside a beautiful but small white house.");
	Room* basement = new Room("Basement", "The basement features old furniture and dim light.");*/
	LoadExitsFromFile("defaultExits.xml");
	LoadCreaturesFromFile("defaultCreatures.xml");
	LoadItemsFromFile("defaultItems.xml");
	LoadRoleSettingsFromFile("defaultRoles.xml");
	AutoEquipCreatures();

}

// ----------------------------------------------------
World::~World()
{
	for(list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		delete *it;

	entities.clear();
}

// ----------------------------------------------------
bool World::Tick(vector<string>& args)
{
	bool ret = true;

	if(args.size() > 0 && args[0].length() > 0)
		ret = ParseCommand(args);

	GameLoop();

	return ret;
}

// ----------------------------------------------------
void World::GameLoop()
{
	clock_t now = clock();

	if((now - tick_timer) / CLOCKS_PER_SEC > TICK_FREQUENCY)
	{
		for(list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
			(*it)->Tick();

		tick_timer = now;
	}
}

// ----------------------------------------------------
bool World::ParseCommand(vector<string>& args)
{
	bool ret = true;

	switch(args.size())
	{
		case 1: // commands with no arguments ------------------------------
		{
			if(Same(args[0], "look") || Same(args[0], "l"))
			{
				player->Look(args);
			}
			else if(Same(args[0], "north") || Same(args[0], "n"))
			{
				(args.size() == 1) ? args.push_back("north") : args[1] = "north";
				player->Go(args);
			}
			else if(Same(args[0], "south") || Same(args[0], "s"))
			{
				(args.size() == 1) ? args.push_back("south") : args[1] = "south";
				player->Go(args);
			}
			else if(Same(args[0], "east") || Same(args[0], "e"))
			{
				(args.size() == 1) ? args.push_back("east") : args[1] = "east";
				player->Go(args);
			}
			else if(Same(args[0], "west") || Same(args[0], "w"))
			{
				(args.size() == 1) ? args.push_back("west") : args[1] = "west";
				player->Go(args);
			}
			else if(Same(args[0], "up") || Same(args[0], "u"))
			{
				(args.size() == 1) ? args.push_back("up") : args[1] = "up";
				player->Go(args);
			}
			else if(Same(args[0], "down") || Same(args[0], "d"))
			{
				(args.size() == 1) ? args.push_back("down") : args[1] = "down";
				player->Go(args);
			}
			else if(Same(args[0], "stats") || Same(args[0], "st"))
			{
				player->Stats();
			}
			else if(Same(args[0], "inventory") || Same(args[0], "i"))
			{
				player->Inventory();
			}
			else
				ret = false;
			break;
		}
		case 2: // commands with one argument ------------------------------
		{
			if(Same(args[0], "look") || Same(args[0], "l"))
			{
				player->Look(args);
			}
			else if(Same(args[0], "go"))
			{
				player->Go(args);
			}
			else if(Same(args[0], "take") || Same(args[0], "pick"))
			{
				player->Take(args);
			}
			else if(Same(args[0], "drop") || Same(args[0], "put"))
			{
				player->Drop(args);
			}
			else if(Same(args[0], "equip") || Same(args[0], "eq"))
			{
				player->Equip(args);
			}
			else if(Same(args[0], "unequip") || Same(args[0], "uneq"))
			{
				player->UnEquip(args);
			}
			else if(Same(args[0], "examine") || Same(args[0], "ex"))
			{
				player->Examine(args);
			}
			else if(Same(args[0], "attack") || Same(args[0], "at"))
			{
				player->Attack(args);
			}
			else if(Same(args[0], "loot") || Same(args[0], "lt"))
			{
				player->Loot(args);
			}
			else
				ret = false;
			break;
		}
		case 3: // commands with two arguments ------------------------------
		{
			break;
		}
		case 4: // commands with three arguments ------------------------------
		{
			if(Same(args[0], "unlock") || Same(args[0], "unlk"))
			{
				player->UnLock(args);
			}
			else if(Same(args[0], "lock") || Same(args[0], "lk"))
			{
				player->Lock(args);
			}
			else if(Same(args[0], "take") || Same(args[0], "pick"))
			{
				player->Take(args);
			}
			else if(Same(args[0], "drop") || Same(args[0], "put"))
			{
				player->Drop(args);
			}
			else
				ret = false;
			break;
		}
		default:
		ret =  false;
	}

	return ret;
}