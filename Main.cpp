#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Audio.hpp>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <string>
#include <random>
#include <iomanip>
#include <vector>
#include "Menu.h"

using namespace std;
using namespace sf;

// Some constants
#define PICO_WIDTH 83.6f
#define PICO_HEIGHT 96.0f
#define COIN_WIDTH 137.f
#define COIN_HEIGHT 560.f
#define COIN_SCALE 0.2f


//::::::::::::::::::::::::::::::::::::::::::::::::::::: Main Menu Variables :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Font font;
//::::::::::::::::::::::::::::::::::::::::::::::::::::: Main Menu Variables :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//----------------------------------------------------------- OUR EARS ---------------------------------------------------------------------------------------
/////Music of the whole game////////
Music backgroundmusic;

///////////Sounds of the game/////////////////////////
SoundBuffer bufferofcoins;               //buffer of coins
SoundBuffer bufferofwinning;             //buffer of winning
SoundBuffer buffernavigation;            //buffer of navigation
SoundBuffer bufferofgame_over;           //buffer of game_over
SoundBuffer bufferofjump;                //buffer of jump
Sound coinsound;                         //sound of coin
Sound winningsound;                      //sound of winning
Sound navigationsound;                   //sound of keyboard navigation
Sound gameover;                          //sound of game_over
Sound jump;                              //sound of jump
//////////////////////////////////////////////////////////////////////////////////////////

/// SOUND LEVEL GLOBALY FOR SAVE 
int sfxLevel = 2;  // Initial sound level (50%)
int musicLevel = 2; // Initial music level (50%)
int levelchoice = 0; //The level the player will choose , initialy set to level 1
//----------------------------------------------------------- OUR EARS ---------------------------------------------------------------------------------------




/////""""""""""""""""""""""""""""""""""""""""""""""""""""Global Variable For Levels""""""""""""""""""""""""""""""""""""""""""""""""""""""""""" 
// level 1 assets (ground / platforms / halls / roof / stares)
RectangleShape ground1, elevator, leftplatform1, rightplatform1, leftHall, rightHall, roof1;
//door and key
Texture doorTexture;
Sprite doorSprite;
bool hasKey = false;
bool doorOpen = false;

// level 2 assets
RectangleShape standup, highground, arrsteps[13], smallsteps[3], higghsteps[3], ground;

// Levels mouvement and collision
bool isOnGround1 = false, isOnGround2 = false;
float oldX1;
float oldY1;
float oldX2;
float oldY2;
float velocityY1 = 0, velocityY2 = 0, velocityX1 = 0, velocityX2 = 0;
int x_1 = 0, x_2 = 0, y_1 = 0, y_2 = 0; //for animation
const float GRAVITY = 0.8f;
const float JUMP_STRENGTH = -14.0f;
const float MOVE_SPEED = 5.0f;

// Coins Global Variables
const int NUM_COINS = 75;
int optimize_level_coins = 0;
int score = 0;
Texture coinsTexture;

// Character Selection 
int selected_color_pico_1 = 0;
int selected_color_pico_2 = 1;


// Elevator Global Varibales
bool elevator_active = false;
bool elevator_returning = false;
const float ELEVATOR_SPEED = 2.0f;
const float ELEVATOR_RETURN_SPEED = 1.0f;
float elevator_original_y = 0;
float elevator_max_height = 0;


// timer Global Variables
Text timer_text;
RectangleShape timer_box;
Clock game_clock;
float remaining_time = 90.0f;

/////""""""""""""""""""""""""""""""""""""""""""""""""""""Global Variable For Level 1""""""""""""""""""""""""""""""""""""""""""""""""""""""""""" 


//-----------------------Scene number------------------------------------------
int scenenum = 1000;
/*
scenenum controls on what page / scene we are in
when scenenum = 1000                we are in main menu
when scenenum = -1                  we close the game
when scenenum = 0                   we go to level choosing menu
when scenenum = 1                   we go to options page
when scenenum = 2                   we go to about page
when scenenum = 3                   we go to character color select  /// lesa 3leha
when scenenum = 4                   we go to loading screen
when scenenum = 5                   we go to first level
when scenenum = 6                   we go to winning level screen
when scenenum = 7                   we go to second level
when scenenum = 8                   we go to game over screen
*/
//-----------------------Scene number------------------------------------------




//""""""""""""""""""""""""""""""""""""""""" OUR FUCTIONS  """"""""""""""""""""""""""""""""""""""""""""
// Main Menu Functions
void Levelchoose(RenderWindow& window);
void About(RenderWindow& window);
void options(RenderWindow& window);
void CharacterSelect(RenderWindow& window);
void loading_screen(RenderWindow& window);

// Coins Functions
void initializeCoins();
void animateCoins();
void checkCoinCollection(Sprite& player);


// Timer Functions
void initializeTimer();
float updateTimer(RenderWindow& window);


// Level 1
void pico_collision_l1(RenderWindow& window);
void Camera_view_l1(RenderWindow& window);
void level1_assets(RenderWindow& window);
void Level1_Full_Gameplay(RenderWindow& window);


// Level 2 
void pico_collision_l2(RenderWindow& window);
void initializeCoinsLevel2();
void Level2_assets(RenderWindow& window);
void Level2_Full_Gameplay(RenderWindow& window);


// Common between levels
void pico_controls(RenderWindow& window);

// loading screen
void loading_screen(RenderWindow& window);

//Game over
void game_over(RenderWindow& window);

//winning level 1
void level_won(RenderWindow& window);

//""""""""""""""""""""""""""""""""""""""""" OUR FUCTIONS  """"""""""""""""""""""""""""""""""""""""""""

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^Our Structs^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Picos Structure
struct Picos
{
	Texture pico_sheet[6], bigPico[6];
	Sprite pico_animated[6], pico_still[6], pico_select1, pico_select2, loading_screenpico1[6], loading_screenpico2[6];
}gamePicos;


// Coins (Structure - Animation - Constants)
struct Coin {
	Sprite sprite;
	bool collected = false;
	Vector2f originalPosition;
}coins[NUM_COINS];


struct CoinAnimation {
	Clock clock;
	int currentFrame = 0;
	const float frameTime = 0.1f; // 100ms per frame
	const int frameCount = 4;
} coin_animation;


//Key (Structure - Texture)
struct Key {
	Sprite sprite;
	bool visible = false;
} key;

Texture keyTexture;

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^Our Structs For Characters^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


// Main Function
int main()
{
	/// Rendering
	VideoMode desktop = VideoMode::getDesktopMode();
	RenderWindow window(VideoMode::getDesktopMode(), "Pico Park", Style::Fullscreen);
	window.setFramerateLimit(60);
	//////////////////////////////////


	/// Pico Park TExt in the first Page in Start Menu
	Menu menu(desktop.width, desktop.height);
	Text name;
	Font font;
	font.loadFromFile("game_over.ttf");
	name.setFont(font);
	name.setCharacterSize(400);
	name.setFillColor(Color{ 255,153,51 });
	name.setPosition(600, -250);
	name.setString("Pico Park");
	///////////////////////////////////////////////

	////////////////music of the game//////////////?????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
	backgroundmusic.openFromFile("Pico-Park-Music.ogg");
	///////////////////////////////////////////////?????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
	// 
	///////to play the background music of the game/////////////////
	backgroundmusic.setVolume(100);
	backgroundmusic.play();
	backgroundmusic.setLoop(true);
	////////////////////////////////////////////////////////////


	////////////////sounds of the game/////////////
	bufferofcoins.loadFromFile("coin sound.ogg");
	bufferofwinning.loadFromFile("winning sound.wav");
	buffernavigation.loadFromFile("navigation sound.mp3");
	bufferofgame_over.loadFromFile("game over.wav");
	bufferofjump.loadFromFile("jump.mp3");
	coinsound.setBuffer(bufferofcoins);
	winningsound.setBuffer(bufferofwinning);
	navigationsound.setBuffer(buffernavigation);
	gameover.setBuffer(bufferofgame_over);
	jump.setBuffer(bufferofjump);
	navigationsound.setVolume(50);
	///////////////////////////////////////////////////////////////////????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????

	///************************************ ASSETS IN THE GUI*******************************************************
	/// Floor in the first Page in Start Menu
	Sprite floor;
	Texture menu_ground;
	menu_ground.loadFromFile("longboard.png");
	floor.setTexture(menu_ground);
	floor.setPosition(-50, 980);
	floor.setScale(1.5, 1);

	////////////////////////////////////////////////
	// Assets (Picos-Key) for Game_play
	gamePicos.pico_sheet[0].loadFromFile("blue pico sheet.png");
	gamePicos.pico_animated[0].setTexture(gamePicos.pico_sheet[0]);

	gamePicos.pico_sheet[1].loadFromFile("green pico sheet.png");
	gamePicos.pico_animated[1].setTexture(gamePicos.pico_sheet[1]);

	gamePicos.pico_sheet[2].loadFromFile("yellow pico sheet.png");
	gamePicos.pico_animated[2].setTexture(gamePicos.pico_sheet[2]);

	gamePicos.pico_sheet[3].loadFromFile("purple pico sheet.png");
	gamePicos.pico_animated[3].setTexture(gamePicos.pico_sheet[3]);


	gamePicos.pico_sheet[4].loadFromFile("light blue pico.png");
	gamePicos.pico_animated[4].setTexture(gamePicos.pico_sheet[4]);

	gamePicos.pico_sheet[5].loadFromFile("red spritesheet.png");
	gamePicos.pico_animated[5].setTexture(gamePicos.pico_sheet[5]);
	////////////////////////////////////////////////////////////
	 // Adjust scale if needed
	///////////////////////////////////////////////////////////

	// Picos Sprite in the first Page in Start Menu
	gamePicos.bigPico[0].loadFromFile("big blue pico.png");
	gamePicos.pico_still[0].setTexture(gamePicos.bigPico[0]);
	gamePicos.pico_still[0].setPosition(1200, 850);
	gamePicos.pico_still[0].setScale(1.5, 1.5);

	gamePicos.bigPico[1].loadFromFile("big green pico.png");
	gamePicos.pico_still[1].setTexture(gamePicos.bigPico[1]);
	gamePicos.pico_still[1].setPosition(1320, 850);
	gamePicos.pico_still[1].setScale(1.5, 1.5);

	gamePicos.bigPico[2].loadFromFile("big yellow pico.png");
	gamePicos.pico_still[2].setTexture(gamePicos.bigPico[2]);
	gamePicos.pico_still[2].setPosition(1440, 850);
	gamePicos.pico_still[2].setScale(1.5, 1.5);

	gamePicos.bigPico[3].loadFromFile("big purple pico.png");
	gamePicos.pico_still[3].setTexture(gamePicos.bigPico[3]);
	gamePicos.pico_still[3].setPosition(1255, 710);
	gamePicos.pico_still[3].setScale(1.5, 1.5);

	gamePicos.bigPico[4].loadFromFile("big light blue pico.png");
	gamePicos.pico_still[4].setTexture(gamePicos.bigPico[4]);
	gamePicos.pico_still[4].setPosition(1495, 710);
	gamePicos.pico_still[4].setScale(-1.5, 1.5);

	gamePicos.bigPico[5].loadFromFile("big red pico.png");
	gamePicos.pico_still[5].setTexture(gamePicos.bigPico[5]);
	gamePicos.pico_still[5].setPosition(1320, 570);
	gamePicos.pico_still[5].setScale(1.5, 1.5);

	///************************************ ASSETS IN THE GUI*******************************************************

	while (true)
	{
		if (scenenum == 1000) //checks if we are in the main menu
		{
			// el l3ba maftoo7a
			while (window.isOpen()) {
				Event event;
				while (window.pollEvent(event))
				{
					// Closing
					if (event.type == Event::Closed)
					{
						window.close();
						break;
					}
					////////////////


					if (event.type == Event::KeyPressed)
					{
						// Up and Down in Main Menu
						if (event.key.code == Keyboard::Up)
						{
							menu.MoveUp();
							navigationsound.play();
						}
						if (event.key.code == Keyboard::Down)
						{
							menu.MoveDown();
							navigationsound.play();
						}
						//////////////////////////////

						/// For Menu Selection
						if (event.key.code == Keyboard::Return)
						{
							if (menu.pressed() == 0)
							{
								scenenum = 0;   // Go to level selection
								navigationsound.play();
							}
							if (menu.pressed() == 1)
							{
								scenenum = 1;   // Go to options
								navigationsound.play();
							}
							if (menu.pressed() == 2)
							{
								navigationsound.play();
								scenenum = 2;   // Go to about page
							}
							if (menu.pressed() == 3)
							{
								navigationsound.play();
								scenenum = -1;  // Exit game								
							}
						}
						/////////////////////////////
					}
				}
				window.clear(Color::White); // Clear screen with white background

 
				if (scenenum != 1000) {
					break;
				}
				//////////////////////////////////////////////////////////////////////////////////


				// Drawing
				menu.draw(window); // Menu Options
				window.draw(name); // Pico Park in the first Page of Start Menu
				window.draw(floor);// Floor
				// Drawing 6 Picos in the fist page of Start Menu
				for (int i = 0; i < 6; i++)
				{
					window.draw(gamePicos.pico_still[i]);
				}
				window.display();
				/////////////////////////////////////////
			}


			/////////////////////////////////////////////
			if (scenenum == -1) // Exit Game 5als 
			{
				window.close();
				break;
			}
			if (scenenum == 0) // Go to level selection screen FUNCTION
			{
				navigationsound.play();
				Levelchoose(window);
			}
			if (scenenum == 1)  // Go to options screen FUNCTION
			{
				navigationsound.play();
				options(window);
			}
			if (scenenum == 2) // Go to about screen FUNCTION
			{
				navigationsound.play();
				About(window);
			}
			if (scenenum == 3)
			{
				navigationsound.play();
				CharacterSelect(window);
			}
			if (scenenum == 4)
			{
				loading_screen(window);
			}
			if (scenenum == 5)
			{
				optimize_level_coins = 0;  // For optimize coins animation and collection 
				Level1_Full_Gameplay(window);
			}
			if (scenenum == 7)
			{
				optimize_level_coins = 66;  // For optimize coins animation and collection 
				Level2_Full_Gameplay(window);
			}
			if (scenenum == 6)
			{
				level_won(window);
			}
			//////////////////////
		}
	}
}


//**********************************************************LEVELs SCREEN FUNCTION*********************************************************************************
void Levelchoose(RenderWindow& window)
{
	font.loadFromFile("game_over.ttf");

	levelchoice = 0; //initialization to avoid glitches after choosing a level

	Sprite levelbg[2];
	Texture levelbackground[2];
	levelbackground[0].loadFromFile("level1bg.png");
	levelbackground[1].loadFromFile("level2bg.png");

	levelbg[0].setTexture(levelbackground[0]);
	levelbg[0].setScale(0.287, 0.33);
	levelbg[0].setPosition(250, 300);

	levelbg[1].setTexture(levelbackground[1]);
	levelbg[1].setScale(0.287, 0.33);
	levelbg[1].setPosition(950, 300);

	Text title, levelnum[2];
	// Choose level ely fe nos el shasha 
	title.setFont(font);
	title.setCharacterSize(250);
	title.setPosition(650, -100);
	title.setString("Choose level");
	title.setFillColor(Color{ 255,153,51 });
	//////////////////////////////////////

	levelnum[0].setFont(font);
	levelnum[0].setCharacterSize(375);
	levelnum[0].setPosition(450, 150);
	levelnum[0].setString("1");
	levelnum[0].setFillColor(Color{ 175, 90,15 });
	
	levelnum[1].setFont(font);
	levelnum[1].setCharacterSize(375);
	levelnum[1].setPosition(1175, 150);
	levelnum[1].setString("2");
	levelnum[1].setFillColor(Color{ 255,153,51 });
	//////////////////////////////////////////

	// Left Rectangle boarder
	RectangleShape levelborder[2];
	levelborder[0].setSize({ 550,350 });
	levelborder[0].setOutlineColor(Color::Black);
	levelborder[0].setFillColor(Color::Transparent);
	levelborder[0].setOutlineThickness(10);
	levelborder[0].setPosition(250, 300);
	// Right Rectangle boarder 
	levelborder[1].setSize({ 550,350 });
	levelborder[1].setOutlineColor(Color::Black);
	levelborder[1].setFillColor(Color::Transparent);
	levelborder[1].setOutlineThickness(10);
	levelborder[1].setPosition(950, 300);
	//////////////////////////////////////

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
				break;
			}
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
			{
				scenenum = 1000; // Go back to main menu
				navigationsound.play();
				return; // Exit this function
			}
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Left)
			{
				navigationsound.play();
				if (levelchoice - 1 >= -1)
				{
					levelnum[levelchoice].setFillColor(Color{ 255,153,51 });
					levelchoice--;
					if (levelchoice == -1)
					{
						levelchoice = 1;
					}
					levelnum[levelchoice].setFillColor(Color{ 175,90,15 });
				}
			}
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Right)
			{
				navigationsound.play();
				if (levelchoice + 1 >= 1)
				{
					levelnum[levelchoice].setFillColor(Color{ 255,153,51 });
					levelchoice++;
					if (levelchoice == 2)
					{
						levelchoice = 0;
					}
					levelnum[levelchoice].setFillColor(Color{ 175,90,15 });
				}
			}
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)
			{
				navigationsound.play();
				scenenum = 3;
				return;
			}
		}
		window.clear(Color::White);

		// Drawing
		window.draw(levelbg[0]);
		window.draw(levelbg[1]);
		window.draw(levelborder[0]);
		window.draw(levelborder[1]);
		window.draw(levelnum[0]);
		window.draw(levelnum[1]);
		window.draw(title);
		window.display();
	}
}
//**********************************************************LEVELs SCREEN FUNCTION*********************************************************************************


//######################################################### ABOUT SCREEN FUNCTION #################################################################################
void About(RenderWindow& window)
{
	font.loadFromFile("game_over.ttf");
	// Our Names in the About
	Text credits;
	credits.setFont(font);
	credits.setCharacterSize(120);
	credits.setFillColor(Color{ 255,153,51 });
	credits.setPosition(150, 100);
	credits.setString("This game was created by: \n Andrew Maged \n Bassel Ramez \n Bola Nagui \n David Atef \n Mohannad Khaled \n Miguel Maged \n Youssef Hany Barakat");
	// Brief about the game
	Text aboutGame;
	aboutGame.setFont(font);
	aboutGame.setCharacterSize(120);
	aboutGame.setFillColor(Color{ 255,153,51 });
	aboutGame.setPosition(720, 300);
	aboutGame.setString("Pico Park is a multi-player game that recquires \nthe players to collaborate to get the key \nand then escape the level");
	// Rectangle Outline
	RectangleShape outline;
	outline.setOutlineColor(Color::Black);
	outline.setSize({ 1700,800 });
	outline.setPosition(100, 100);
	outline.setOutlineThickness(5);

	//Drawing
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
				break;
			}
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
			{
				navigationsound.play();
				scenenum = 1000; // Go back to main menu
				return; // Exit this function
			}
		}
		window.clear(Color::White);
		window.draw(outline);
		window.draw(credits);
		window.draw(aboutGame);
		window.display();
	}
}
//######################################################### ABOUT SCREEN FUNCTION #################################################################################


//**********************************************************Option Screen FUNCTION*********************************************************************************
void options(RenderWindow& window)
{
	font.loadFromFile("game_over.ttf");
	// Sound effects volume label
	Text sfx;
	sfx.setFont(font);
	sfx.setCharacterSize(150);
	sfx.setString("Sound effects volume: ");
	sfx.setFillColor(Color{ 255, 153, 51 });
	sfx.setPosition(100, 50);

	// Music volume label
	Text music;
	music.setFont(font);
	music.setCharacterSize(150);
	music.setString("Music volume: ");
	music.setFillColor(Color{ 255, 153, 51 });
	music.setPosition(100, 450);
	//////////////////////////////////////////
	//
	////<<<<<BARS SETTINGS>>>>
	// Volume bar settings
	int selectedBar = 0; // 0 = Sound Effects, 1 = Music
	const int levelCount = 5; // 5 levels for each bar (0%, 25%, 50%, 75%, 100%)
	float barWidth = 800.0f; // Width of the bars
	float barHeight = 50.0f; // Height of the bars
	float sectionWidth = barWidth / levelCount; // Width of each level segment
	Color barColor = Color{ 255, 153, 51 }; // Orange color used for the bar fill
	Color shadowColor = Color(150, 100, 50); // Shadow color (darkened orange)

	// Create percentage text for sound effects
	Text sfxPercentage;
	sfxPercentage.setFont(font);
	sfxPercentage.setCharacterSize(100);
	sfxPercentage.setFillColor(Color::Black);

	// Create percentage text for music
	Text musicPercentage;
	musicPercentage.setFont(font);
	musicPercentage.setCharacterSize(100);
	musicPercentage.setFillColor(Color::Black);
	////////////////////////////////////////////////////////////////////////

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
				break;
			}

			// Handle key presses
			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Escape)
				{
					navigationsound.play();
					// Save current levels to global variables before exiting
					// Exit to main menu (or whatever logic you need)
					scenenum = 1000;
					return; // Exit this function
				}

				// Switch between Sound Effects and Music with Up/Down keys
				if (event.key.code == Keyboard::Up)
				{
					selectedBar = 0; // Sound effects selected
					navigationsound.play();
				}
				if (event.key.code == Keyboard::Down)
				{
					selectedBar = 1; // Music selected
					navigationsound.play();
				}

				// Adjust Sound Effects or Music with Left/Right keys
				if (event.key.code == Keyboard::Left)
				{
					if (selectedBar == 0) {
						sfxLevel = max(0, sfxLevel - 1); // Decrease sound effects volume
						coinsound.setVolume(sfxLevel * 25);
						winningsound.setVolume(sfxLevel * 25);
						coinsound.play();
					}
					if (selectedBar == 1) {
						musicLevel = max(0, musicLevel - 1);
						backgroundmusic.setVolume(musicLevel * 25);
					}// Decrease music volume
				}
				if (event.key.code == Keyboard::Right)
				{
					if (selectedBar == 0)
					{
						sfxLevel = std::min(levelCount - 1, sfxLevel + 1);
						coinsound.setVolume(sfxLevel * 25);
						winningsound.setVolume(sfxLevel * 25);
						coinsound.play();
					} // Increase sound effects volume
					if (selectedBar == 1)
					{
						musicLevel = std::min(levelCount - 1, musicLevel + 1);
						backgroundmusic.setVolume(musicLevel * 25);
					}// Increase music volume
				}
			}
		}

		window.clear(Color::White);

		////<<<<<BARS RECTANGLES>>>>
		// SFX Volume Bar (Top)
		RectangleShape sfxBarBack(Vector2f(barWidth, barHeight));
		sfxBarBack.setPosition(100, 250);
		sfxBarBack.setFillColor(Color(200, 200, 200)); // gray background

		// Shadow effect for selected bar
		RectangleShape sfxBarShadow(Vector2f(sectionWidth * (sfxLevel + 1), barHeight));
		sfxBarShadow.setPosition(105, 255); // Slightly offset the shadow
		sfxBarShadow.setFillColor(shadowColor);

		// Actual Sound Effects Volume Bar (Orange)
		RectangleShape sfxBarFill(Vector2f(sectionWidth * (sfxLevel + 1), barHeight));
		sfxBarFill.setPosition(100, 250);
		//////////////////////////////////////////////////

		// If sfxLevel is 0, make the bar empty (transparent)
		if (sfxLevel == 0)
		{
			sfxBarFill.setFillColor(Color::Transparent); // Empty bar for 0% sound level
		}
		else
		{
			sfxBarFill.setFillColor(barColor); // Fill with color for non-zero levels
		}

		// Draw the shadow only if the sound effects bar is selected
		if (selectedBar == 0)
		{
			window.draw(sfxBarShadow); // Draw the shadow on the selected bar
		}

		// Set the percentage text for sound effects volume
		sfxPercentage.setString(to_string(sfxLevel * 25) + "%");
		sfxPercentage.setPosition(950, 250); // Position the percentage text on the right of the bar

		// Music Volume Bar (Bottom)
		RectangleShape musicBarBack(Vector2f(barWidth, barHeight));
		musicBarBack.setPosition(100, 650);
		musicBarBack.setFillColor(Color(200, 200, 200)); // gray background

		// Shadow effect for selected bar (even for 0%)
		RectangleShape musicBarShadow(Vector2f(sectionWidth * (musicLevel + 1), barHeight));
		musicBarShadow.setPosition(105, 655); // Slightly offset the shadow
		musicBarShadow.setFillColor(shadowColor);

		// Actual Music Volume Bar (Orange)
		RectangleShape musicBarFill(Vector2f(sectionWidth * (musicLevel + 1), barHeight));
		musicBarFill.setPosition(100, 650);

		// If musicLevel is 0, make the bar empty (transparent)
		if (musicLevel == 0)
		{
			musicBarFill.setFillColor(Color::Transparent); // Empty bar for 0% music level
		}
		else
		{
			musicBarFill.setFillColor(barColor); // Fill with color for non-zero levels
		}

		// Draw the shadow only if the music bar is selected
		if (selectedBar == 1)
		{
			window.draw(musicBarShadow); // Draw the shadow on the selected bar
		}

		// Set the percentage text for music volume
		musicPercentage.setString(std::to_string(musicLevel * 25) + "%");
		musicPercentage.setPosition(950, 650); // Position the percentage text on the right of the bar

		// Draw the bars without shadow for 0% levels
		window.draw(sfxBarBack);
		window.draw(sfxBarFill);
		window.draw(sfxPercentage); // Draw the sound effects percentage text

		window.draw(musicBarBack);
		window.draw(musicBarFill);
		window.draw(musicPercentage); // Draw the music percentage text

		window.draw(sfx);  // Sound effects label
		window.draw(music); // Music label

		window.display();
	}
}
//**********************************************************Option Screen FUNCTION*********************************************************************************

//##########################################################Character Select screen Function#######################################################################
void CharacterSelect(RenderWindow& window)
{
	Texture pico_selection, selectors;
	pico_selection.loadFromFile("pico select sheet.png");
	selectors.loadFromFile("selectors.png");

	gamePicos.pico_select1.setTexture(pico_selection);
	gamePicos.pico_select2.setTexture(pico_selection);
	gamePicos.pico_select2.setPosition(600, 500);
	gamePicos.pico_select1.setPosition(1200, 500);
	gamePicos.pico_select1.setTextureRect(IntRect(0, 0, 105, 96));
	gamePicos.pico_select2.setTextureRect(IntRect(105, 0, 105, 96));

	Font font;
	font.loadFromFile("game_over.ttf");
	Text instructions, error;
	instructions.setFont(font);
	instructions.setCharacterSize(120);
	instructions.setPosition(250, 750);
	instructions.setFillColor(Color{ 255,153,51 });
	instructions.setString("Use left and right arrow keys to choose first player \nUse A and D to choose second player\nYou cannot choose 2 same colours");

	Sprite select_arrows1, select_arrows2;
	select_arrows1.setTexture(selectors);
	select_arrows2.setTexture(selectors);
	select_arrows1.setPosition(490, 490);
	select_arrows2.setPosition(1090, 490);

	selected_color_pico_1 = 0; //initialization to start on different colors
	selected_color_pico_2 = 1;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
				break;
			}
			window.clear(Color::White);
			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Escape)
				{
					navigationsound.play();
					scenenum = 1000;
					return;
				}
				if (event.key.code == Keyboard::Right)
				{
					navigationsound.play();
					selected_color_pico_1 = (selected_color_pico_1 + 1) % 6;
					gamePicos.pico_select1.setTextureRect(IntRect(selected_color_pico_1 * 105, 0, 105, 96));
					gamePicos.pico_select1.setPosition(1200 - (selected_color_pico_1 * 5), 500);
				}
				if (event.key.code == Keyboard::Left)
				{
					navigationsound.play();
					selected_color_pico_1 = (selected_color_pico_1 + 5) % 6;
					gamePicos.pico_select1.setTextureRect(IntRect(selected_color_pico_1 * 105, 0, 105, 96));
					gamePicos.pico_select1.setPosition(1200 - (selected_color_pico_1 * 5), 500);
				}
				if (event.key.code == Keyboard::D)
				{
					navigationsound.play();
					selected_color_pico_2 = (selected_color_pico_2 + 1) % 6;
					gamePicos.pico_select2.setTextureRect(IntRect(selected_color_pico_2 * 105, 0, 105, 96));
					gamePicos.pico_select2.setPosition(600 - (selected_color_pico_2 * 5), 500);
				}
				if (event.key.code == Keyboard::A)
				{
					navigationsound.play();
					selected_color_pico_2 = (selected_color_pico_2 + 5) % 6;
					gamePicos.pico_select2.setTextureRect(IntRect(selected_color_pico_2 * 105, 0, 105, 96));
					gamePicos.pico_select2.setPosition(600 - (selected_color_pico_2 * 5), 500);
				}
				if (event.key.code == Keyboard::Enter)
				{
					navigationsound.play();
					if (selected_color_pico_1 == selected_color_pico_2)
					{
						continue;
					}
					navigationsound.play();
					if (levelchoice == 0)
					{
						scenenum = 5;  // go level 1
						return;
					}
					if (levelchoice == 1)
					{
						scenenum = 7;  // go level 2 
						return;
					}

				}
			}
			window.draw(gamePicos.pico_select1);
			window.draw(gamePicos.pico_select2);
			window.draw(select_arrows1);
			window.draw(select_arrows2);
			window.draw(instructions);
			window.display();
		}

	}
}
//##########################################################Character Select screen Function#######################################################################

//////////////////////////////////////////////////////////// LOADING SCREEN////////////////////////////////////////////////////////////////////////////////////////
void loading_screen(RenderWindow& window)
{
	font.loadFromFile("game_over.ttf"); 
	Text game_name, movement1, movement2;
	game_name.setFont(font);
	game_name.setCharacterSize(550);
	game_name.setPosition(435, 0);
	game_name.setFillColor(Color{ 255,153,51 });
	game_name.setString("Pico Park");

	movement1.setFont(font);
	movement2.setFont(font);
	movement1.setCharacterSize(100);
	movement1.setFillColor(Color{ 255,153,51 });
	movement1.setPosition(1480, 350);
	movement1.setString("Player 2 moves\nwith the arrow keys");

	movement2.setCharacterSize(100);
	movement2.setFillColor(Color{ 255,153,51 });
	movement2.setPosition(70, 350);
	movement2.setString("Player 1 moves\nwith the WAD keys");

	for (int i = 0; i < 6; i++) // loading textures into picos
	{
		gamePicos.loading_screenpico1[i].setTexture(gamePicos.pico_sheet[i]);
		gamePicos.loading_screenpico2[i].setTexture(gamePicos.pico_sheet[i]);
	}

	gamePicos.loading_screenpico1[selected_color_pico_1].setPosition(150, 820); //putting picos one above the loading bar and the other on top of the first pico
	gamePicos.loading_screenpico2[selected_color_pico_2].setPosition(150, 700);

	int pico_animation = 0; //animation for sprite sheet
	Clock animation_clock; //animation speed
	const float animation_speed = 0.1; //animation fps 0.1 = 10fps

	Clock loading_timer;
	const float loading_time = 10.0; // total loading time
	float loading_progress = 0; // how much has loaded

	RectangleShape loading_bar, unloaded;
	loading_bar.setFillColor(Color(255, 153, 51));
	loading_bar.setSize({ 1600, 100 });
	loading_bar.setPosition(150, 900);
	loading_bar.setScale(0, 1); // to make the loading bar start empty

	unloaded.setFillColor(Color(128, 128, 128));
	unloaded.setSize({ 1600, 100 });
	unloaded.setPosition(150, 900);

	Sprite movement_keys1, movement_keys2;
	Texture keys1, keys2;

	keys1.loadFromFile("arrow keys.png");
	keys2.loadFromFile("WAD keys.png");

	movement_keys1.setTexture(keys1);
	movement_keys2.setTexture(keys2);
	movement_keys1.setPosition(1550, 200);
	movement_keys2.setPosition(100, 200);
	movement_keys1.setScale(1.5, 1.5);
	movement_keys2.setScale(1.5, 1.5);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		loading_progress = loading_timer.getElapsedTime().asSeconds() / loading_time;

		if (animation_clock.getElapsedTime().asSeconds() > animation_speed)
		{
			pico_animation = (pico_animation + 1) % 10; // animation for the sprite sheet
			animation_clock.restart(); // timer reset to make animation smooth
		}

		gamePicos.loading_screenpico1[selected_color_pico_1].setTextureRect(
			IntRect(pico_animation * PICO_WIDTH, 0, PICO_WIDTH, PICO_HEIGHT));
		gamePicos.loading_screenpico2[selected_color_pico_2].setTextureRect(
			IntRect(pico_animation * PICO_WIDTH, 0, PICO_WIDTH, PICO_HEIGHT));

		float target_x = 150 + (1600 * loading_progress); // the loading end of the loading bar
		gamePicos.loading_screenpico1[selected_color_pico_1].setPosition(target_x - gamePicos.loading_screenpico1[selected_color_pico_1].getGlobalBounds().width, 800);
		gamePicos.loading_screenpico2[selected_color_pico_2].setPosition(target_x - gamePicos.loading_screenpico2[selected_color_pico_2].getGlobalBounds().width, 720);

		if (loading_progress >= 1)
		{
			loading_progress = 1;
			loading_bar.setScale(loading_progress, 1);
			if (levelchoice == 0)
			{
				scenenum = 5;
				return;
			}
			if (levelchoice == 1)
			{
				scenenum = 6;
				return;
			}
		}

		loading_bar.setScale(loading_progress, 1);

		window.clear(Color::White);
		window.draw(game_name);
		window.draw(movement1);
		window.draw(movement2);
		window.draw(movement_keys1);
		window.draw(movement_keys2);
		window.draw(unloaded);
		window.draw(gamePicos.loading_screenpico1[selected_color_pico_1]);
		window.draw(gamePicos.loading_screenpico2[selected_color_pico_2]);
		window.draw(loading_bar);
		window.display();
	}

}
//////////////////////////////////////////////////////////// LOADING SCREEN////////////////////////////////////////////////////////////////////////////////////////

//********************************************************** Level1 Platforms*************************************************************************************
void level1_assets(RenderWindow& window) {

	if (!keyTexture.loadFromFile("key.png")) {
		cout << "Failed to load key texture!" << endl;
	}
	key.sprite.setTexture(keyTexture);
	key.sprite.setPosition(1600.f, 250.f);
	key.sprite.setScale(0.3f, 0.3f);

	ground1.setSize(Vector2f(2854, 73.5));
	ground1.setFillColor(Color{ 255,153,51 });
	ground1.setPosition(0.f, 646.5f);


	elevator.setSize(Vector2f(PICO_WIDTH * 2 - 50, 6));
	elevator.setFillColor(Color::Red);
	elevator.setPosition(1260, 646.5f); 
	elevator_original_y = elevator.getPosition().y;
	elevator_max_height = 400.f; // 50px below left platform



	leftplatform1.setSize(Vector2f(1260, 73.5));
	leftplatform1.setFillColor(Color{ 255,153,51 });
	leftplatform1.setPosition(0.f, 320.f);

	rightplatform1.setSize(Vector2f(1260, 73.5));
	rightplatform1.setFillColor(Color{ 255,153,51 });
	rightplatform1.setPosition(1576.f, 320.f);

	leftHall.setSize(Vector2f(78, 1248));
	leftHall.setFillColor(Color{ 255,153,51 });
	leftHall.setPosition(0.f, 0.f);

	rightHall.setSize(Vector2f(100, 1248));
	rightHall.setFillColor(Color{ 255,153,51 });
	rightHall.setPosition(2820.f, 0.f);

	roof1.setSize(Vector2f(2854, 73.5));
	roof1.setFillColor(Color{ 255,153,51 });
	roof1.setPosition(0.f, 0.f);



	//door and key
	doorTexture.loadFromFile("door.png");
	doorSprite.setTexture(doorTexture);
	doorSprite.setTextureRect(sf::IntRect(0, 0, 559, 424));
	doorSprite.setScale(0.5, 0.5);
	doorSprite.setPosition(2530, 435);


}
//********************************************************** Level1 Platforms*************************************************************************************

//---------------------------------------------------------- Timer ( 2 FUnctions )--------------------------------------------------------------------------------
// Initialize Timer
void initializeTimer() {

	// Timer text
	timer_text.setFont(font);
	timer_text.setCharacterSize(110); 
	timer_text.setFillColor(Color::White);
	timer_text.setStyle(Text::Bold);
	timer_text.setString("01:30"); // Initial timer text

	// Rectangle Boarder
	timer_box.setSize(Vector2f(200, 80)); // Width x Height
	timer_box.setFillColor(Color::Transparent);
	timer_box.setOutlineThickness(5.f);    //Outline
	timer_box.setOutlineColor(Color::White);
	timer_box.setPosition(20.f, 20.f); // fo2 3la el 4mal

	// Position text inside the box
	timer_text.setPosition(timer_box.getPosition().x + 50.f, timer_box.getPosition().y + -50.f);
}
// Update timer
float updateTimer(RenderWindow& window) {
	// el w2t ely 3ada
	float elapsed = game_clock.getElapsedTime().asSeconds();

	// Subtract elapsed time ( count down ) time 
	float currentTime = remaining_time - elapsed;

	if (currentTime <= 0) {
		currentTime = 0;
	}


	// make it an integer num m4 b decimal
	int totalSeconds = static_cast<int>(currentTime);
	int minutes = totalSeconds / 60;
	int seconds = totalSeconds % 60;

	timer_text.setString(
		(minutes < 10 ? "0" : "") + to_string(minutes) + ":" +
		(seconds < 10 ? "0" : "") + to_string(seconds)
	);
	return currentTime;
}
//---------------------------------------------------------- Timer ( 2 FUnctions )--------------------------------------------------------------------------------

///{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{ COINS 3 FUNCTIONS }}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}
void initializeCoins() {

	// Load coins texture
	if (!coinsTexture.loadFromFile("Coins Sprite Sheet.png")) {
		cout << "Failed to load coins texture!" << endl;
		return;
	}

	// Reset ALL coins to uncollected state
	for (int i = 0; i < NUM_COINS; i++) {
		coins[i].collected = false;
	}

	// Initialize coin positions (same as your original code)
	float spacingX = 60.f;
	float spacingY = 280.f;
	int coinIndex = 0;

	// 20 coins above Right Platform
	float startX1 = rightplatform1.getPosition().x + 50.f;
	float startY1 = rightplatform1.getPosition().y - 100.f;

	for (int i = 0; i < 20; i++, coinIndex++) {
		coins[coinIndex].sprite.setTexture(coinsTexture);
		coins[coinIndex].sprite.setScale(COIN_SCALE, COIN_SCALE);
		coins[coinIndex].sprite.setTextureRect(IntRect(0, 0, COIN_WIDTH, COIN_HEIGHT));

		float yOffset = (i % 2 == 0) ? 0 : -spacingY / 2;
		coins[coinIndex].sprite.setPosition(
			startX1 + i * spacingX,
			startY1 + yOffset
		);
		coins[coinIndex].originalPosition = coins[coinIndex].sprite.getPosition();
	}

	// 20 coins above Left Platform
	float startX2 = leftplatform1.getPosition().x + 80.f;
	float startY2 = leftplatform1.getPosition().y - 100.f;

	for (int i = 0; i < 20; i++, coinIndex++) {
		coins[coinIndex].sprite.setTexture(coinsTexture);
		coins[coinIndex].sprite.setScale(COIN_SCALE, COIN_SCALE);
		coins[coinIndex].sprite.setTextureRect(IntRect(0, 0, COIN_WIDTH, COIN_HEIGHT));

		float yOffset = (i % 2 == 0) ? 0 : -spacingY / 2;
		coins[coinIndex].sprite.setPosition(
			startX2 + i * spacingX,
			startY2 + yOffset
		);
		coins[coinIndex].originalPosition = coins[coinIndex].sprite.getPosition();
	}

	// 20 coins above the ground and below Left Platform
	float startX3 = leftplatform1.getPosition().x + 100.f;
	float startY3 = ground1.getPosition().y - 100.f;
	for (int i = 0; i < 20; i++, coinIndex++) {
		coins[coinIndex].sprite.setTexture(coinsTexture);
		coins[coinIndex].sprite.setScale(COIN_SCALE, COIN_SCALE);
		coins[coinIndex].sprite.setTextureRect(IntRect(0, 0, COIN_WIDTH, COIN_HEIGHT));

		float yOffset = (i % 2 == 0) ? 0 : -spacingY / 2;
		coins[coinIndex].sprite.setPosition(
			startX3 + i * spacingX,
			startY3 + yOffset
		);
		coins[coinIndex].originalPosition = coins[coinIndex].sprite.getPosition();
	}

	// 15 coins above the ground and below the Right Platform
	float startX4 = rightplatform1.getPosition().x + 100.f;
	float startY4 = ground1.getPosition().y - 100.f;
	for (int i = 0; i < 15; i++, coinIndex++) {
		coins[coinIndex].sprite.setTexture(coinsTexture);
		coins[coinIndex].sprite.setScale(COIN_SCALE, COIN_SCALE);
		coins[coinIndex].sprite.setTextureRect(IntRect(0, 0, COIN_WIDTH, COIN_HEIGHT));

		float yOffset = (i % 2 == 0) ? 0 : -spacingY / 2;
		coins[coinIndex].sprite.setPosition(
			startX4 + i * spacingX,
			startY4 + yOffset
		);
		coins[coinIndex].originalPosition = coins[coinIndex].sprite.getPosition();
	}

	// Reset key visibility
	key.visible = false;
}

// Animate coins
void animateCoins() {
	// time to change coin animation
	if (coin_animation.clock.getElapsedTime().asSeconds() > coin_animation.frameTime) {
		// animation +1 and  % 4
		coin_animation.currentFrame = (coin_animation.currentFrame + 1) % coin_animation.frameCount;

		for (int i = 0; i < NUM_COINS - optimize_level_coins; i++) { 
			if (!coins[i].collected) {
				// continue animation if it still not taken
				coins[i].sprite.setTextureRect(IntRect(coin_animation.currentFrame * COIN_WIDTH, 0, COIN_WIDTH, COIN_HEIGHT));
				coins[i].sprite.setPosition(coins[i].originalPosition.x, coins[i].originalPosition.y);
			}
		}
		coin_animation.clock.restart();
	}
}
// Check coin collection and update score
void checkCoinCollection(Sprite& player) {
	for (int i = 0; i < NUM_COINS - optimize_level_coins; i++) { //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		// exist and taken --> bool true and score++ --> play sound effect --> bool visibilty key = true
		if (!coins[i].collected &&
			player.getGlobalBounds().intersects(coins[i].sprite.getGlobalBounds())) {

			coins[i].collected = true;
			score++; // 1 point per coin

			//*********************************************************************************************************************  Play sound effect
			coinsound.setVolume(sfxLevel * 25); // Scale volume based on settings
			coinsound.play();

			cout << "Score: " << score << endl;
			// Check if all coins are collected
			if (score == NUM_COINS - optimize_level_coins) {
				key.visible = true;
				
				cout << "All coins collected! Key appeared!" << endl;
			}
		}
	}
}
///{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{ COINS 3 FUNCTIONS }}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}

// Camera View
void Camera_view_l1(RenderWindow& window) {
	View camera;
	float centerX = (gamePicos.pico_animated[selected_color_pico_1].getPosition().x +
		gamePicos.pico_animated[selected_color_pico_2].getPosition().x) / 2.f;

	centerX = max(centerX, 1280.f / 2.f);
	centerX = min(centerX, 2854.f - 1280.f / 2.f);

	camera.setCenter(centerX, 720.f / 2.f);
	camera.setSize(1280.f, 720.f);
	window.setView(camera);
}

// Updated pico_controls function
void pico_collision_l1(RenderWindow& window) {
	// Save old positions
	oldX1 = gamePicos.pico_animated[selected_color_pico_1].getPosition().x;
	oldY1 = gamePicos.pico_animated[selected_color_pico_1].getPosition().y;
	oldX2 = gamePicos.pico_animated[selected_color_pico_2].getPosition().x;
	oldY2 = gamePicos.pico_animated[selected_color_pico_2].getPosition().y;

	// Reset ground flags
	isOnGround1 = false;
	isOnGround2 = false;
	bool isJumping1 = true;
	bool isJumping2 = true;

	// Check boolean picos 
	bool p1_on = gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().intersects(elevator.getGlobalBounds());
	bool p2_on = gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().intersects(elevator.getGlobalBounds());

	// If both players are standing on the elevator
	if (p1_on && p2_on) {
		elevator_returning = false; // Don't go down
		float y = elevator.getPosition().y;

		if (y > elevator_max_height) {
			float moveUp = std::min(ELEVATOR_SPEED, y - elevator_max_height);
			elevator.move(0, -moveUp); // Go up
			gamePicos.pico_animated[selected_color_pico_1].move(0, -moveUp);
			gamePicos.pico_animated[selected_color_pico_2].move(0, -moveUp);
		}
	}
	// If one player steps off, start going down
	else if (!p1_on || !p2_on) {
		elevator_returning = true;
	}

	// If elevator should return to the bottom
	if (elevator_returning) {
		float y = elevator.getPosition().y;

		if (y < elevator_original_y) {
			float moveDown = std::min(ELEVATOR_SPEED, elevator_original_y - y);
			elevator.move(0, moveDown); // Go down
		}
		else {
			elevator_returning = false; // Done returning
		}
	}
	// Character 1 collision ground and platforms
	if (gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().intersects(ground1.getGlobalBounds())) {
		gamePicos.pico_animated[selected_color_pico_1].setPosition(gamePicos.pico_animated[selected_color_pico_1].getPosition().x, ground1.getPosition().y - gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().height);
		velocityY1 = 0;
		isOnGround1 = true;
	}
	if (gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().left < leftHall.getGlobalBounds().left + leftHall.getGlobalBounds().width) {
		gamePicos.pico_animated[selected_color_pico_1].setPosition(
			leftHall.getGlobalBounds().left + leftHall.getGlobalBounds().width, // Aligns left edge with wall's right edge
			gamePicos.pico_animated[selected_color_pico_1].getPosition().y
		);
		velocityX1 = 0;
	}

	// (Optional) Same for Pico 2
	if (gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().left < leftHall.getGlobalBounds().left + leftHall.getGlobalBounds().width) {
		gamePicos.pico_animated[selected_color_pico_2].setPosition(
			leftHall.getGlobalBounds().left + leftHall.getGlobalBounds().width,
			gamePicos.pico_animated[selected_color_pico_2].getPosition().y
		);
		velocityX2 = 0;
	}
	if (gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().left + gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().width > rightHall.getGlobalBounds().left) {
		gamePicos.pico_animated[selected_color_pico_1].setPosition(
			rightHall.getGlobalBounds().left - gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().width, // Align right edge with wall
			gamePicos.pico_animated[selected_color_pico_1].getPosition().y
		);
		velocityX1 = 0;
	}
	if (gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().left + gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().width > rightHall.getGlobalBounds().left) {
		gamePicos.pico_animated[selected_color_pico_2].setPosition(
			rightHall.getGlobalBounds().left - gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().width,
			gamePicos.pico_animated[selected_color_pico_2].getPosition().y
		);
		velocityX2 = 0;
	}
	if (gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().top < roof1.getGlobalBounds().top + roof1.getGlobalBounds().height) {
		// Set Pico's top to exactly match ceiling bottom
		gamePicos.pico_animated[selected_color_pico_1].setPosition(
			gamePicos.pico_animated[selected_color_pico_1].getPosition().x,
			roof1.getGlobalBounds().top + roof1.getGlobalBounds().height
		);
		velocityY1 = 0;  // Stop vertical movement
		isJumping1 = false; // Optional: Reset jump state if needed
	}

	if (gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().top < roof1.getGlobalBounds().top + roof1.getGlobalBounds().height) {
		gamePicos.pico_animated[selected_color_pico_2].setPosition(
			gamePicos.pico_animated[selected_color_pico_2].getPosition().x,
			roof1.getGlobalBounds().top + roof1.getGlobalBounds().height
		);
		velocityY2 = 0;
		isJumping2 = false;
	}

	if (gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().intersects(leftplatform1.getGlobalBounds())) {
		/// hna bgeb el 7dod el pico wel platform///////////////////////
		FloatRect picoBounds = gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds();
		FloatRect platformBounds = leftplatform1.getGlobalBounds();
		///////////////////////////////////////// 
		float overlapLeft = picoBounds.left + picoBounds.width - platformBounds.left;
		//////////////////////////////////////////////////////////////////////////////
		float overlapRight = platformBounds.left + platformBounds.width - picoBounds.left;
		float overlapTop = picoBounds.top + picoBounds.height - platformBounds.top;
		float overlapBottom = platformBounds.top + platformBounds.height - picoBounds.top;

		float minOverlap = min({ overlapLeft, overlapRight, overlapTop, overlapBottom });
		////////////////////////////////////////////////////////////////////////////////

		if (minOverlap == overlapTop) {

			gamePicos.pico_animated[selected_color_pico_1].setPosition(picoBounds.left, platformBounds.top - picoBounds.height);
			velocityY1 = 0;
			isOnGround1 = true;
		}
		/////////////////////////////////////////////////////////////////////////////////////////

		else if (minOverlap == overlapBottom) {

			gamePicos.pico_animated[selected_color_pico_1].setPosition(picoBounds.left, platformBounds.top + platformBounds.height);
			velocityY1 = 0;
		}
		////////////////////////////////////////////////////////////////////
		else if (minOverlap == overlapLeft) {

			gamePicos.pico_animated[selected_color_pico_1].setPosition(platformBounds.left - picoBounds.width, picoBounds.top);
			velocityX1 = 0;
		}
		else if (minOverlap == overlapRight) {

			gamePicos.pico_animated[selected_color_pico_1].setPosition(platformBounds.left + platformBounds.width, picoBounds.top);
			velocityX1 = 0;
		}
	}

	if (gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().intersects(rightplatform1.getGlobalBounds())) {
		FloatRect picoBounds = gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds();
		FloatRect platformBounds = rightplatform1.getGlobalBounds();

		float overlapLeft = picoBounds.left + picoBounds.width - platformBounds.left;
		float overlapRight = platformBounds.left + platformBounds.width - picoBounds.left;
		float overlapTop = picoBounds.top + picoBounds.height - platformBounds.top;
		float overlapBottom = platformBounds.top + platformBounds.height - picoBounds.top;

		float minOverlap = min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

		if (minOverlap == overlapTop) {
			gamePicos.pico_animated[selected_color_pico_1].setPosition(picoBounds.left, platformBounds.top - picoBounds.height);
			velocityY1 = 0;
			isOnGround1 = true;
		}
		else if (minOverlap == overlapBottom) {
			gamePicos.pico_animated[selected_color_pico_1].setPosition(picoBounds.left, platformBounds.top + platformBounds.height);
			velocityY1 = 0;
		}
		else if (minOverlap == overlapLeft) {
			gamePicos.pico_animated[selected_color_pico_1].setPosition(platformBounds.left - picoBounds.width, picoBounds.top);
			velocityX1 = 0;
		}
		else if (minOverlap == overlapRight) {
			gamePicos.pico_animated[selected_color_pico_1].setPosition(platformBounds.left + platformBounds.width, picoBounds.top);
			velocityX1 = 0;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////

	if (gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().intersects(leftplatform1.getGlobalBounds())) {

		FloatRect picoBounds = gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds();
		FloatRect platformBounds = leftplatform1.getGlobalBounds();
		///////////////////////////////////////// 
		float overlapLeft = picoBounds.left + picoBounds.width - platformBounds.left;
		//////////////////////////////////////////////////////////////////////////////
		float overlapRight = platformBounds.left + platformBounds.width - picoBounds.left;
		float overlapTop = picoBounds.top + picoBounds.height - platformBounds.top;
		float overlapBottom = platformBounds.top + platformBounds.height - picoBounds.top;

		//////asghr value ll tsadomat el 4 w dh hysa3dna n3rf el tsadom gy mnen
		float minOverlap = min({ overlapLeft, overlapRight, overlapTop, overlapBottom });
		////////////////////////////////////////////////////////////////////////////////

		if (minOverlap == overlapTop) {

			gamePicos.pico_animated[selected_color_pico_2].setPosition(picoBounds.left, platformBounds.top - picoBounds.height);
			velocityY2 = 0;
			isOnGround2 = true;
		}
		/////////////////////////////////////////////////////////////////////////////////////////

		else if (minOverlap == overlapBottom) {

			gamePicos.pico_animated[selected_color_pico_2].setPosition(picoBounds.left, platformBounds.top + platformBounds.height);
			velocityY2 = 0;
		}
		////////////////////////////////////////////////////////////////////

		else if (minOverlap == overlapLeft) {

			gamePicos.pico_animated[selected_color_pico_2].setPosition(platformBounds.left - picoBounds.width, picoBounds.top);
			velocityX2 = 0;
		}
		else if (minOverlap == overlapRight) {

			gamePicos.pico_animated[selected_color_pico_2].setPosition(platformBounds.left + platformBounds.width, picoBounds.top);
			velocityX2 = 0;
		}
	}

	if (gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().intersects(rightplatform1.getGlobalBounds())) {
		FloatRect picoBounds = gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds();
		FloatRect platformBounds = rightplatform1.getGlobalBounds();

		float overlapLeft = picoBounds.left + picoBounds.width - platformBounds.left;
		float overlapRight = platformBounds.left + platformBounds.width - picoBounds.left;
		float overlapTop = picoBounds.top + picoBounds.height - platformBounds.top;
		float overlapBottom = platformBounds.top + platformBounds.height - picoBounds.top;

		float minOverlap = min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

		if (minOverlap == overlapTop) {
			gamePicos.pico_animated[selected_color_pico_2].setPosition(picoBounds.left, platformBounds.top - picoBounds.height);
			velocityY2 = 0;
			isOnGround2 = true;
		}
		else if (minOverlap == overlapBottom) {
			gamePicos.pico_animated[selected_color_pico_2].setPosition(picoBounds.left, platformBounds.top + platformBounds.height);
			velocityY2 = 0;
		}
		else if (minOverlap == overlapLeft) {
			gamePicos.pico_animated[selected_color_pico_2].setPosition(platformBounds.left - picoBounds.width, picoBounds.top);
			velocityX2 = 0;
		}
		else if (minOverlap == overlapRight) {
			gamePicos.pico_animated[selected_color_pico_2].setPosition(platformBounds.left + platformBounds.width, picoBounds.top);
			velocityX2 = 0;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////



	else if (gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().intersects(elevator.getGlobalBounds())) {
		gamePicos.pico_animated[selected_color_pico_1].setPosition(gamePicos.pico_animated[selected_color_pico_1].getPosition().x, elevator.getPosition().y - gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().height);
		velocityY1 = 0;
		isOnGround1 = true;
	}

	// Character 2 collision ground and platforms
	if (gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().intersects(ground1.getGlobalBounds())) {
		gamePicos.pico_animated[selected_color_pico_2].setPosition(gamePicos.pico_animated[selected_color_pico_2].getPosition().x, ground1.getPosition().y - gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().height);
		velocityY2 = 0;
		isOnGround2 = true;
	}
	else if (gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().intersects(leftplatform1.getGlobalBounds())) {
		gamePicos.pico_animated[selected_color_pico_2].setPosition(gamePicos.pico_animated[selected_color_pico_2].getPosition().x, leftplatform1.getPosition().y - gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().height);
		velocityY2 = 0;
		isOnGround2 = true;
	}
	else if (gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().intersects(rightplatform1.getGlobalBounds())) {
		gamePicos.pico_animated[selected_color_pico_2].setPosition(gamePicos.pico_animated[selected_color_pico_2].getPosition().x, rightplatform1.getPosition().y - gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().height);
		velocityY2 = 0;
		isOnGround2 = true;
	}
	else if (gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().intersects(elevator.getGlobalBounds())) {
		gamePicos.pico_animated[selected_color_pico_2].setPosition(gamePicos.pico_animated[selected_color_pico_2].getPosition().x, elevator.getPosition().y - gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().height);
		velocityY2 = 0;
		isOnGround2 = true;
	}
	else if (gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().intersects(elevator.getGlobalBounds()) && gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().intersects(elevator.getGlobalBounds())) {
		velocityY1 = 0;
		velocityY2 = 0;
		isOnGround1 = true;
		isOnGround2 = true;
		elevator.move(0, -2);
	}

	///////////////////////////////////////////////////////
	// Collision Check: Prevent Left & Right Overlapping
    // Check collision between players
	if (gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().intersects(
		gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds())) {

		FloatRect p1 = gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds();
		FloatRect p2 = gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds();

		// Calculate overlap
		float overlapX = min(p1.left + p1.width, p2.left + p2.width) - max(p1.left, p2.left);
		float overlapY = min(p1.top + p1.height, p2.top + p2.height) - max(p1.top, p2.top);

		// Resolve based on smaller overlap
		if (overlapX < overlapY) {
			// Horizontal collision
			if (p1.left < p2.left) {
				gamePicos.pico_animated[selected_color_pico_1].move(-overlapX / 2, 0);
				gamePicos.pico_animated[selected_color_pico_2].move(overlapX / 2, 0);
			}
			else {
				gamePicos.pico_animated[selected_color_pico_1].move(overlapX / 2, 0);
				gamePicos.pico_animated[selected_color_pico_2].move(-overlapX / 2, 0);
			}
		}
		else {
			// Vertical collision
			if (p1.top < p2.top) {
				gamePicos.pico_animated[selected_color_pico_1].move(0, -overlapY / 2);
				gamePicos.pico_animated[selected_color_pico_2].move(0, overlapY / 2);
				if (velocityY1 > 0) velocityY1 = 0;
				if (velocityY2 < 0) velocityY2 = 0;
				isOnGround1 = true;
			}
			else {
				gamePicos.pico_animated[selected_color_pico_1].move(0, overlapY / 2);
				gamePicos.pico_animated[selected_color_pico_2].move(0, -overlapY / 2);
				if (velocityY1 < 0) velocityY1 = 0;
				if (velocityY2 > 0) velocityY2 = 0;
				isOnGround2 = true;
			}
		}
	}
}

void pico_controls(RenderWindow& window) {
	// Apply gravity
	velocityY1 += GRAVITY;
	velocityY2 += GRAVITY;

	// Character 1 controls
	if (Keyboard::isKeyPressed(Keyboard::Up) && isOnGround1) {
		jump.play();
		velocityY1 = JUMP_STRENGTH;
		isOnGround1 = false;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right)) {
		gamePicos.pico_animated[selected_color_pico_1].move(MOVE_SPEED, 0);
		x_1 = (x_1 + 1) % 9;
		y_1 = 0;
		gamePicos.pico_animated[selected_color_pico_1].setTextureRect(IntRect(x_1 * PICO_WIDTH, y_1 * PICO_HEIGHT, PICO_WIDTH, PICO_HEIGHT));
	}
	if (Keyboard::isKeyPressed(Keyboard::Left)) {
		gamePicos.pico_animated[selected_color_pico_1].move(-MOVE_SPEED, 0);
		x_1 = (x_1 + 1) % 9;
		y_1 = 1;
		gamePicos.pico_animated[selected_color_pico_1].setTextureRect(IntRect(x_1 * PICO_WIDTH, y_1 * PICO_HEIGHT, PICO_WIDTH, PICO_HEIGHT));
	}

	// Character 2 controls
	if (Keyboard::isKeyPressed(Keyboard::W) && isOnGround2) {
		jump.play();
		velocityY2 = JUMP_STRENGTH;
		isOnGround2 = false;
	}
	if (Keyboard::isKeyPressed(Keyboard::D)) {
		gamePicos.pico_animated[selected_color_pico_2].move(MOVE_SPEED, 0);
		x_2 = (x_2 + 1) % 9;
		y_2 = 0;
		gamePicos.pico_animated[selected_color_pico_2].setTextureRect(IntRect(x_2 * PICO_WIDTH, y_2 * PICO_HEIGHT, PICO_WIDTH, PICO_HEIGHT));
	}
	if (Keyboard::isKeyPressed(Keyboard::A)) {
		gamePicos.pico_animated[selected_color_pico_2].move(-MOVE_SPEED, 0);
		x_2 = (x_2 + 1) % 9;
		y_2 = 1;
		gamePicos.pico_animated[selected_color_pico_2].setTextureRect(IntRect(x_2 * PICO_WIDTH, y_2 * PICO_HEIGHT, PICO_WIDTH, PICO_HEIGHT));
	}

	// Apply vertical movement
	gamePicos.pico_animated[selected_color_pico_1].move(0, velocityY1);
	gamePicos.pico_animated[selected_color_pico_2].move(0, velocityY2);
}

// Main Level Gameplay
void Level1_Full_Gameplay(RenderWindow& window) {

	loading_screen(window); // loading window view

	// Reset door state and key
	doorOpen = false;
	hasKey = false;
	key.visible = false;

	doorSprite.setTextureRect(sf::IntRect(0, 0, 559, 424)); // Reset to closed door frame


	// Initialize platforms
	level1_assets(window);

	// Initialize players
	gamePicos.pico_animated[selected_color_pico_1].setTextureRect(IntRect(0, 0, PICO_WIDTH, PICO_HEIGHT));
	gamePicos.pico_animated[selected_color_pico_1].setPosition(1600.f, 550.f);
	gamePicos.pico_animated[selected_color_pico_2].setTextureRect(IntRect(0, 0, PICO_WIDTH, PICO_HEIGHT));
	gamePicos.pico_animated[selected_color_pico_2].setPosition(1500.f, 550.f);

	gamePicos.pico_animated[selected_color_pico_1].setScale(1.0f, 1.0f);
	gamePicos.pico_animated[selected_color_pico_2].setScale(1.0f, 1.0f);

	// Reset states
	velocityY1 = velocityY2 = 0;
	isOnGround1 = isOnGround2 = false;
	x_1 = y_1 = x_2 = y_2 = 0;
	score = 0;
	key.visible = false;  // Key starts invisible

	// Reset elevator
	elevator_active = false;
	elevator_returning = false;
	elevator.setPosition(1360, 646.5f);

	hasKey = false;


	// Initialize game elements
	initializeCoins();
	initializeTimer();
	game_clock.restart();

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::MouseButtonPressed) {
				Vector2i mousePos = Mouse::getPosition(window);
				cout << "Mouse Clicked at: X=" << mousePos.x << ", Y=" << mousePos.y << endl;
			}
		}

		// Update game state
		pico_controls(window);
		pico_collision_l1(window);
		animateCoins();

		// Check coin collection for both players
		checkCoinCollection(gamePicos.pico_animated[selected_color_pico_1]);
		checkCoinCollection(gamePicos.pico_animated[selected_color_pico_2]);

		// Check key collection if visible
		if (key.visible) {
			// Check collision with player 1 or 2
			if (gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().intersects(key.sprite.getGlobalBounds()) ||
				gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().intersects(key.sprite.getGlobalBounds())) {
				key.visible = false;  // Key disappears when touched
				hasKey = true;        // Mark that the key was collected
				cout << "Key collected!" << endl;
			}
		}

		// Open door if key was collected
		if (hasKey && !doorOpen) {
			doorSprite.setTextureRect(sf::IntRect(559, 0, 559, 424)); // Switch to 'open' frame			
			doorOpen = true;
			cout << "Door opened!" << endl;
		}
		if (doorOpen) {
			if (gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().intersects(doorSprite.getGlobalBounds()) &&
				gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().intersects(doorSprite.getGlobalBounds()))
			{
				winningsound.play();
				level_won(window);
				scenenum;
				return;
			}
		}
		updateTimer(window);
		if (updateTimer(window) == 0)
		{
			gameover.play();
			game_over(window);
			scenenum;
			return;
		}
		Camera_view_l1(window);

		// Drawing
		window.clear(Color::White);

		// Draw platforms
		window.draw(ground1);
		window.draw(elevator);
		window.draw(leftplatform1);
		window.draw(rightplatform1);
		window.draw(leftHall);
		window.draw(rightHall);
		window.draw(roof1);
		window.draw(doorSprite);

		// Draw coins
		for (int i = 0; i < NUM_COINS; i++) {
			if (!coins[i].collected) {
				window.draw(coins[i].sprite);
			}
		}

		// Draw players
		window.draw(gamePicos.pico_animated[selected_color_pico_1]);
		window.draw(gamePicos.pico_animated[selected_color_pico_2]);

		// Draw key if visible
		if (key.visible) {
			window.draw(key.sprite);
		}

		// Draw timer (AFTER setting the camera view)
		window.setView(window.getDefaultView());
		window.draw(timer_box);
		window.draw(timer_text);
		window.setView(window.getView());

		window.display();
	}
}
void Level2_assets(RenderWindow& window) {
	const float window_width = window.getSize().y;
	const float window_height = window.getSize().x;
	////////////

	if (!keyTexture.loadFromFile("key.png")) {
		cout << "Failed to load key texture!" << endl;
	}
	key.sprite.setTexture(keyTexture);
	key.sprite.setPosition(1200.f, 400.f);
	key.sprite.setScale(0.3f, 0.3f);

	doorTexture.loadFromFile("door.png");
	doorSprite.setTexture(doorTexture);
	doorSprite.setTextureRect(sf::IntRect(0, 0, 559, 424));
	doorSprite.setScale(0.5, 0.5);
	doorSprite.setPosition(3440, 630);

	arrsteps[0].setSize(Vector2f(200, 50));
	arrsteps[0].setFillColor({ 255,153,51 });
	for (int i = 1; i <= 12; i++) {
		if (i == 8) {
			arrsteps[i].setSize(Vector2f(300, 50));
		}
		arrsteps[i].setSize(Vector2f(150, 50));
		arrsteps[i].setFillColor(Color{ 255,153,51 });
	}

	arrsteps[9].setSize({ 305,50 });

	arrsteps[0].setPosition(15.0, 745);
	arrsteps[1].setPosition(350, 745);
	arrsteps[2].setPosition(600, 745);
	arrsteps[3].setPosition(850, 670);
	arrsteps[4].setPosition(1040, 545);
	arrsteps[5].setPosition(1300, 545);
	arrsteps[6].setPosition(1470, 670);
	arrsteps[7].setPosition(1675, 745);
	arrsteps[8].setPosition(1930, 745);
	arrsteps[9].setPosition(2160, 820);
	arrsteps[10].setPosition(20320, 880);
	arrsteps[11].setPosition(3220, 880);
	arrsteps[12].setPosition(3520, 840);

	for (int i = 0; i < 3; i++) {
		smallsteps[i].setSize(Vector2f(100, PICO_HEIGHT));
		smallsteps[i].setFillColor({ 255,153,51 });
		higghsteps[i].setSize(Vector2f(100, PICO_HEIGHT * 2));
		higghsteps[i].setFillColor({ 255,153,51 });

	}
	smallsteps[0].setPosition(2500, 1000);
	higghsteps[0].setPosition(2600, 920);
	smallsteps[1].setPosition(2700, 1000);
	higghsteps[1].setPosition(2800, 920);
	smallsteps[2].setPosition(2900, 1000);
	higghsteps[2].setPosition(3000, 920);

}


void initializeCoinsLevel2() {
	coinsound.setBuffer(bufferofcoins);

	if (!coinsTexture.loadFromFile("Coins Sprite Sheet.png")) {
		cout << "Failed to load coins texture!" << endl;
		return;
	}

	// Reset ALL coins to uncollected state
	for (int i = 0; i < NUM_COINS; i++) {
		coins[i].collected = false;
	}

	// List of platforms to place coins above
	vector<RectangleShape*> platforms = {
		&arrsteps[1], &arrsteps[2], &arrsteps[3], &arrsteps[5],
		&arrsteps[6], &arrsteps[8], &arrsteps[9],
		&smallsteps[0], &smallsteps[1], &smallsteps[2]
	};

	// Place 1 coin above each platform (10 total coins)
	for (int i = 0; i < 10; i++) {
		coins[i].sprite.setTexture(coinsTexture);
		coins[i].sprite.setScale(COIN_SCALE, COIN_SCALE);
		coins[i].sprite.setTextureRect(IntRect(0, 0, COIN_WIDTH, COIN_HEIGHT));

		float platformCenterX = platforms[i]->getPosition().x + platforms[i]->getSize().x / 2;
		coins[i].sprite.setPosition(
			platformCenterX - (COIN_WIDTH * COIN_SCALE) / 2,
			platforms[i]->getPosition().y - 100.f
		);
		coins[i].originalPosition = coins[i].sprite.getPosition();
	}

	// Mark remaining coins as collected (inactive)
	for (int i = 10; i < NUM_COINS; i++) {
		coins[i].collected = true;
	}

	// Reset key visibility
	key.visible = false;
}


void pico_collision_l2(RenderWindow& window) {
	// Save old positions
	oldX1 = gamePicos.pico_animated[selected_color_pico_1].getPosition().x;
	oldY1 = gamePicos.pico_animated[selected_color_pico_1].getPosition().y;
	oldX2 = gamePicos.pico_animated[selected_color_pico_2].getPosition().x;
	oldY2 = gamePicos.pico_animated[selected_color_pico_2].getPosition().y;

	// Reset ground flags
	isOnGround1 = false;
	isOnGround2 = false;

	// Helper lambda for solid block collision
	auto resolveCollision = [](Sprite& player, float& velocityY, bool& isOnGround, const FloatRect& block) {
		FloatRect playerBounds = player.getGlobalBounds();

		// Overlap distances
		float fromLeft = playerBounds.left + playerBounds.width - block.left;
		float fromRight = block.left + block.width - playerBounds.left;
		float fromTop = playerBounds.top + playerBounds.height - block.top;
		float fromBottom = block.top + block.height - playerBounds.top;

		// Minimal overlap direction
		float minX = min(fromLeft, fromRight);
		float minY = min(fromTop, fromBottom);

		if (minX < minY) {
			// Horizontal collision
			if (fromLeft < fromRight) {
				player.setPosition(block.left - playerBounds.width, player.getPosition().y);
			}
			else {
				player.setPosition(block.left + block.width, player.getPosition().y);
			}
		}
		else {
			// Vertical collision
			if (fromTop < fromBottom) {
				player.setPosition(player.getPosition().x, block.top - playerBounds.height);
				velocityY = 0;
				isOnGround = true;
			}
			else {
				player.setPosition(player.getPosition().x, block.top + block.height);
				velocityY = 0; // optional: for head bump
			}
		}
		};

	// All platforms
	for (int i = 0; i < 13; i++) {
		if (gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().intersects(arrsteps[i].getGlobalBounds())) {
			resolveCollision(gamePicos.pico_animated[selected_color_pico_1], velocityY1, isOnGround1, arrsteps[i].getGlobalBounds());
		}
		if (gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().intersects(arrsteps[i].getGlobalBounds())) {
			resolveCollision(gamePicos.pico_animated[selected_color_pico_2], velocityY2, isOnGround2, arrsteps[i].getGlobalBounds());
		}
	}

	for (int i = 0; i < 3; i++) {
		if (gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().intersects(smallsteps[i].getGlobalBounds())) {
			resolveCollision(gamePicos.pico_animated[selected_color_pico_1], velocityY1, isOnGround1, smallsteps[i].getGlobalBounds());
		}
		if (gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().intersects(smallsteps[i].getGlobalBounds())) {
			resolveCollision(gamePicos.pico_animated[selected_color_pico_2], velocityY2, isOnGround2, smallsteps[i].getGlobalBounds());
		}
		if (gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().intersects(higghsteps[i].getGlobalBounds())) {
			resolveCollision(gamePicos.pico_animated[selected_color_pico_1], velocityY1, isOnGround1, higghsteps[i].getGlobalBounds());
		}
		if (gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().intersects(higghsteps[i].getGlobalBounds())) {
			resolveCollision(gamePicos.pico_animated[selected_color_pico_2], velocityY2, isOnGround2, higghsteps[i].getGlobalBounds());
		}
	}

	// Collision between players
	if (gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().intersects(
		gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds())) {

		FloatRect p1 = gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds();
		FloatRect p2 = gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds();

		float overlapX = min(p1.left + p1.width, p2.left + p2.width) - max(p1.left, p2.left);
		float overlapY = min(p1.top + p1.height, p2.top + p2.height) - max(p1.top, p2.top);

		if (overlapX < overlapY) {
			if (p1.left < p2.left) {
				gamePicos.pico_animated[selected_color_pico_1].move(-overlapX / 2, 0);
				gamePicos.pico_animated[selected_color_pico_2].move(overlapX / 2, 0);
			}
			else {
				gamePicos.pico_animated[selected_color_pico_1].move(overlapX / 2, 0);
				gamePicos.pico_animated[selected_color_pico_2].move(-overlapX / 2, 0);
			}
		}
		else {
			if (p1.top < p2.top) {
				gamePicos.pico_animated[selected_color_pico_1].move(0, -overlapY / 2);
				gamePicos.pico_animated[selected_color_pico_2].move(0, overlapY / 2);
				if (velocityY1 > 0) velocityY1 = 0;
				if (velocityY2 < 0) velocityY2 = 0;
				isOnGround1 = true;
			}
			else {
				gamePicos.pico_animated[selected_color_pico_1].move(0, overlapY / 2);
				gamePicos.pico_animated[selected_color_pico_2].move(0, -overlapY / 2);
				if (velocityY1 < 0) velocityY1 = 0;
				if (velocityY2 > 0) velocityY2 = 0;
				isOnGround2 = true;
			}
		}
	}
}


void Level2_Full_Gameplay(RenderWindow& window) {
	loading_screen(window);

	// Reset door state
	doorOpen = false;
	hasKey = false;
	key.visible = false;
	doorSprite.setTextureRect(sf::IntRect(0, 0, 559, 424)); // Reset to closed door frame

	const int SCREEN_WIDTH = 1920;
	const int SCREEN_HEIGHT = 1080;
	int currentScreen = 0;

	View cameraView(FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	cameraView.setCenter(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);

	Level2_assets(window);
	initializeCoinsLevel2();

	// Initialize players
	gamePicos.pico_animated[selected_color_pico_1].setTextureRect(IntRect(0, 0, PICO_WIDTH, PICO_HEIGHT));
	gamePicos.pico_animated[selected_color_pico_1].setPosition(15, 200);
	gamePicos.pico_animated[selected_color_pico_1].setScale(0.6f, 0.6f);
	gamePicos.pico_animated[selected_color_pico_2].setTextureRect(IntRect(0, 0, PICO_WIDTH, PICO_HEIGHT));
	gamePicos.pico_animated[selected_color_pico_2].setPosition(35.f, 300);
	gamePicos.pico_animated[selected_color_pico_2].setScale(0.6f, 0.6f);

	// Reset player states
	velocityY1 = velocityY2 = 0;
	isOnGround1 = isOnGround2 = false;
	x_1 = y_1 = x_2 = y_2 = 0;
	score = 0;

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}

		// Update game state
		pico_controls(window);
		pico_collision_l2(window);
		animateCoins();

		// Check coin collection for both players
		checkCoinCollection(gamePicos.pico_animated[selected_color_pico_1]);
		checkCoinCollection(gamePicos.pico_animated[selected_color_pico_2]);

		// Screen transition logic — move only if BOTH players cross the boundary
		Vector2f player1Pos = gamePicos.pico_animated[selected_color_pico_1].getPosition();
		Vector2f player2Pos = gamePicos.pico_animated[selected_color_pico_2].getPosition();

		if (currentScreen == 0 &&
			player1Pos.x >= SCREEN_WIDTH &&
			player2Pos.x >= SCREEN_WIDTH) {
			currentScreen = 1;
			cameraView.setCenter(SCREEN_WIDTH * 1.5f, SCREEN_HEIGHT / 2.f);
		}
		else if (currentScreen == 1 &&
			player1Pos.x <= SCREEN_WIDTH &&
			player2Pos.x <= SCREEN_WIDTH) {
			currentScreen = 0;
			cameraView.setCenter(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
		}

		// Apply camera view
		window.setView(cameraView);

		// Drawing
		window.clear(Color::White);

		// Draw platforms
		for (int i = 0; i < 13; i++) {
			window.draw(arrsteps[i]);
		}
		for (int i = 0; i < 3; i++) {
			window.draw(smallsteps[i]);
			window.draw(higghsteps[i]);
		}

		// Draw coins
		for (int i = 0; i < NUM_COINS - optimize_level_coins; i++) {
			if (!coins[i].collected) {
				window.draw(coins[i].sprite);
			}
		}

		if (player1Pos.y > 1100 || player2Pos.y > 1100)
		{
			gameover.play();
			currentScreen = 0;
			cameraView.setCenter(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
			window.setView(cameraView);
			game_over(window);
			scenenum;
			return;
		}

		window.draw(doorSprite);

		// Draw players
		window.draw(gamePicos.pico_animated[selected_color_pico_1]);
		window.draw(gamePicos.pico_animated[selected_color_pico_2]);

		// Key interaction
		if (key.visible) {
			if (gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().intersects(key.sprite.getGlobalBounds()) ||
				gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().intersects(key.sprite.getGlobalBounds())) {
				key.visible = false;
				hasKey = true;
				cout << "Key collected!" << endl;
			}
		}

		// Door logic
		if (hasKey && !doorOpen) {
			doorSprite.setPosition(3473, 630); // to reposition the door to be centered with the platform
			doorSprite.setTextureRect(sf::IntRect(559, 0, 559, 424)); // open			
			doorOpen = true;
			cout << "Door opened!" << endl;
		}
		if (doorOpen)
		{
			if (gamePicos.pico_animated[selected_color_pico_1].getGlobalBounds().intersects(doorSprite.getGlobalBounds()) &&
				gamePicos.pico_animated[selected_color_pico_2].getGlobalBounds().intersects(doorSprite.getGlobalBounds()))
			{
				winningsound.play();
				currentScreen = 0;
				cameraView.setCenter(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
				window.setView(cameraView);
				level_won(window);
				scenenum;
				return;
			}
		}

		if (key.visible) {
			window.draw(key.sprite);
		}

		window.display();
	}
}



void game_over(RenderWindow& window)
{
	Text game_over, instructions;

	game_over.setFont(font);
	game_over.setCharacterSize(800);
	game_over.setFillColor(Color{ 255,153,51 });
	game_over.setString("Game Over");
	game_over.setPosition(150, -300);

	instructions.setFont(font);
	instructions.setCharacterSize(200);
	instructions.setFillColor(Color{ 255,153,51 });
	instructions.setString("Press Enter to go back to main menu");
	instructions.setPosition(300, 650);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)
			{
				scenenum = 1000;
				return;
			}
		}
		window.clear(Color::White);
		window.draw(game_over);
		window.draw(instructions);
		window.display();
	}
}

void level_won(RenderWindow& window)
{
	Text winning, instructions, instructions1;

	winning.setFont(font);
	winning.setCharacterSize(800);
	winning.setFillColor(Color{ 255,153,51 });
	winning.setPosition(250, -300);
	winning.setString("You Win");

	instructions.setFont(font);
	instructions.setCharacterSize(200);
	instructions.setFillColor(Color{ 255,153,51 });
	instructions.setString("Press Enter to go to the next level");
	instructions.setPosition(300, 650);

	instructions1.setFont(font);
	instructions1.setCharacterSize(200);
	instructions1.setFillColor(Color{ 255,153,51 });
	instructions1.setString("\nPress Esc to go back to main menu");
	instructions1.setPosition(300, 650);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Escape)
				{
					scenenum = 1000;
					return;
				}
				if (levelchoice == 0)
				{
					if (event.key.code == Keyboard::Enter)
					{
						levelchoice++;
						scenenum = 7;
						return;
					}
				}
			}
		}
		window.clear(Color::White);
		window.draw(winning);
		if (levelchoice == 0)
		{
			window.draw(instructions);
		}
		window.draw(instructions1);
		window.display();
	}
}
