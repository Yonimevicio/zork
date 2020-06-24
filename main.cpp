#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include "globals.h"
#include "world.h"

using namespace std;

#define BACKSPACE "\033[D\033[K"     

// -------------------------------------------------
int main()
{
	char key;
	string player_input;
	vector<string> args;
	args.reserve(10);
	World my_world;
	cout << Cyan_ "Welcome to MyZork!\n" _Cyan;
	cout << "----------------\n";
	cout << "You are sleeping peacefully when suddenly you notice that something is slapping your face.\n";
	cout << "You wake up but there's nobody.\n";
	cout << "Have you dreamed something strange, what have you dreamed of?\n";
	bool selected = false;
	while (!selected) {
		cout << "1 -> I was very hunk and wore giant armor " Red_ "Warrior"  _Red "\n";
		cout << "2 -> I had someone around the neck with a knife " Yellow_ "Rogue"  _Yellow "\n";
		cout << "3 -> It was all very blurry and I couldn't walk straight " Green_ "Drunk Fighter"  _Green "\n";
		cout << "4 -> Was I sleeping inside the dream? " Blue_ "Elder Drowsy"  _Blue "\n";
		int i = 0;
		cin >> i;
		switch (i) 
		{
		case 1:
			selected = my_world.RoleMessage("Warrior");
			break;
		case 2:
			selected = my_world.RoleMessage("Rogue");
			break;
		case 3:
			selected = my_world.RoleMessage("Drunk Fighter");
			break;
		case 4:
			selected = my_world.RoleMessage("Elder Drowsy");
			break;
		default:
			selected = my_world.RoleMessage("");
			break;
		}
	}
	

	args.push_back("look");

	while(1)
	{
		if(_kbhit() != 0)
		{
			key = _getch();
			if(key == '\b') // backspace
			{
				if(player_input.length() > 0)
				{
					player_input.pop_back();
					//cout << BACKSPACE;
					cout << '\b';
					cout << " ";
					cout << '\b';
				}
			}
			else if(key != '\r') // return
			{
				player_input += key;
				cout << key;
			}
			else
				Tokenize(player_input, args);
		}

		if(args.size() > 0 && Same(args[0], "quit"))
			break;
		if (args.size() > 1 && Same(args[0], "look") && Same(args[1], "mirror")) {
			cout << "\n Congratulations you have finished this demo, we hope you liked it" << "\n";
			break;
		}
		if(my_world.Tick(args) == false)
			cout << "\nSorry, I do not understand your command.\n";

		if(args.size() > 0)
		{
			args.clear();
			player_input = "";
			cout << "> ";
		}
	}

	cout << "\nThanks for playing, Bye!\n";
	return 0;
}