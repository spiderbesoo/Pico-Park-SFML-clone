#include "Menu.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;
using namespace std;

Menu::Menu(float width, float height)
{
	font.loadFromFile("game_over.ttf");

	for (int i = 0; i < 4; i++)
	{
		mainmenu[i].setFont(font);
		mainmenu[i].setCharacterSize(250);
	}

	mainmenu[0].setFillColor(Color{ 175,90,15 });
	mainmenu[0].setString("Start");
	mainmenu[0].setPosition(50, 100);

	mainmenu[1].setFillColor(Color{ 255,153,51 });
	mainmenu[1].setString("Options");
	mainmenu[1].setPosition(50, 300);

	mainmenu[2].setFillColor(Color{ 255,153,51 });
	mainmenu[2].setString("About");
	mainmenu[2].setPosition(50, 500);

	mainmenu[3].setFillColor(Color{ 255,153,51 });
	mainmenu[3].setString("Exit");
	mainmenu[3].setPosition(50, 700);

	selected = 0;
}

Menu::~Menu() {};

void Menu::Select(int n)
{
	selected = n;
}

void Menu::draw(RenderWindow& window)
{
	for (int i = 0; i < 4; i++)
	{
		window.draw(mainmenu[i]);
	}
}

void Menu::MoveDown()
{
	if (selected + 1 <= 4) //not exit
	{
		mainmenu[selected].setFillColor(Color{ 255,153,51 });
		selected++;
		if (selected == 4)
		{
			selected = 0;
		}
		mainmenu[selected].setFillColor(Color{ 175,90,15 });
	}
}
void Menu::MoveUp()
{
	if (selected - 1 >= -1) //not start
	{
		mainmenu[selected].setFillColor(Color{ 255,153,51 });
		selected--;
	}
	if (selected == -1)
	{
		selected = 3;
	}
	mainmenu[selected].setFillColor(Color{ 175,90,15 });
}
