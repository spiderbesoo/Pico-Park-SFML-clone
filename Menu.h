#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;
using namespace std;
class Menu
{
public:
	Text mainmenu[4];
	Menu(float width, float height);
	void draw(RenderWindow& window);
	void MoveUp();
	void MoveDown();
	void Select(int n);
	int pressed()
	{
		return selected;
	}
	~Menu();

private:
	int selected;
	Font font;
};
