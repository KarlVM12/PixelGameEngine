#define OLC_PGE_APPLICATION
#include <cmath>
#include <cstdint>
#include <string>
#include <iostream>
#include <streambuf>
#include <sstream>
#include <chrono>
#include <vector>
#include <list>
#include <thread>
#include <atomic>
#include <fstream>
#include <map>
#include <functional>
#include <algorithm>
#include <array>
#include <cstring>
#include "olcPixelGameEngine.h"
#include "Rooms.cpp"

// Override base class with your custom functionality
class Hamilton : public olc::PixelGameEngine
{
public:
	Hamilton()
	{
		// Name your application
		sAppName = "Hamilton";
	}

	int screenSize = 600;

	// Player variables
	const float PLAYER_SPRITE_SCALE = 3.5;
	olc::Sprite* sprHam = nullptr;
	olc::Decal* decHam = nullptr;
	double xPosition = screenSize / 2;
	double yPosition = screenSize / 2;
	double moveSpeed = 0.5;
	olc::vf2d playerCollisionTopLeft, playerCollisionBotRight;
	
	// Key controls
	olc::Key currentKey = olc::Key::NONE;
	bool allowKeyW = true, allowKeyA = true, allowKeyS = true, allowKeyD = true;
	
	// Quill variables for health
	olc::Sprite* sprQuill = nullptr;
	olc::Decal* decQuill = nullptr;


	// Wall variables
	olc::Sprite* sprWall = nullptr;
	olc::Decal* decWall = nullptr;

	// Rooms variable
	Rooms r;

	// Menu variables
	olc::Sprite* sprMenu = nullptr;
	olc::Decal* decMenu = nullptr;

	// Main Menu
	bool MAINMENUACTIVE = true;
	olc::Sprite* sprMainMenuBackground = nullptr;
	olc::Decal* decMainMenuBackground = nullptr;
	


public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here

		// loads player sprite from folder
		sprHam = new olc::Sprite("C:/Users/Karl/Documents/GameSourceArt/AHam.png");
		decHam = new olc::Decal(sprHam);

		// initializes player collision
		playerCollisionTopLeft = { float(xPosition), float(yPosition) };
		playerCollisionBotRight = { float((decHam->sprite->width) * 3.5), float((decHam->sprite->height) * 3.5) };

		// loads quill sprite
		sprQuill = new olc::Sprite("c:/Users/Karl/Documents/GameSourceArt/Quill2_2.png");
		decQuill = new olc::Decal(sprQuill);


		// loads wall sprite
		sprWall = new olc::Sprite("C:/Users/Karl/Documents/GameSourceArt/Wall.png");
		decWall = new olc::Decal(sprWall);

		// initializes each wall in the current room
		for (int i = 0; i < std::size(r.room1); i++)
		{
			for (int j = 0; j < std::size(r.room1[i]); j++)
			{
				r.room1[i][j].setX(float(j * 100));
				r.room1[i][j].setY(float(i * 100));
			}

		}

		// loads menu sprite
		sprMenu = new olc::Sprite("C:/Users/Karl/Documents/GameSourceArt/DialogBox2.png");
		decMenu = new olc::Decal(sprMenu);

		// loads main menu background
		sprMainMenuBackground = new olc::Sprite("C:/Users/Karl/Documents/GameSourceArt/MainMenuBackground.png");
		decMainMenuBackground = new olc::Decal(sprMainMenuBackground);

		/*
		int mmbackground = CreateLayer();
		SetDrawTarget(mmbackground);		
		DrawDecal({ 0.0f, 0.0f }, decMainMenuBackground);
		EnableLayer(mmbackground, true);
		SetDrawTarget(nullptr);
		*/


		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (MAINMENUACTIVE) // Main Menu
		{
			Clear(olc::VERY_DARK_YELLOW);


			std::string startGame = "Start Game";
			DrawString({ 185, 100 }, startGame, olc::BLACK, 3);
			//DrawDecal({ 0.0f, 0.0f }, decMainMenuBackground, { 1.0f , 1.0f });

			olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };

			if (mouse.x >= 185 && mouse.x <= 425 && mouse.y >= 90 && mouse.y <= 125)
			{
				DrawString({ 185, 100 }, startGame, olc::YELLOW, 3);
				if (GetMouse(0).bPressed)
					MAINMENUACTIVE = false;
			}

		}
		else // Playing Field
		{



			// Clears screen
			Clear(olc::VERY_DARK_GREY);


			// draws wall sprites
			for (int i = 0; i < std::size(r.room1); i++)
			{
				for (int j = 0; j < std::size(r.room1[i]); j++)
				{
					if (r.room1[i][j].type == Tile::TileType::WALL)
					{
						// draws wall sprites
						DrawDecal({ float(j * 100), float(i * 100) }, decWall);

						// Collision rectangle for wall tiles
						//DrawRect({ int(j * 100), int(i * 100) }, { int(r.room1[i][j].tileSize), int(r.room1[i][j].tileSize) }, olc::GREEN);

					}

				}
			}

			bool didPlayerCollide = playerCollide(r.room1);

			// Player collisions with objects which sends the player away from the current key they just pressed
			if (didPlayerCollide)
			{
				if (currentKey == olc::Key::W)
				{
					yPosition++;
					allowKeyW = false;
				}
				else if (currentKey == olc::Key::A)
				{
					xPosition++;
					allowKeyA = false;
				}
				else if (currentKey == olc::Key::S)
				{
					yPosition--;
					allowKeyS = false;
				}
				else if (currentKey == olc::Key::D)
				{
					xPosition--;
					allowKeyD = false;
				}
			}



			// Drawing player collision box
			playerCollisionTopLeft = { float(xPosition), float(yPosition) };
			//DrawRect(playerCollisionTopLeft, playerCollisionBotRight, olc::RED);

			// Position of mouse cursor
			olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };

			// WASD controls affected by movespeed
			if (GetKey(olc::Key::D).bHeld && allowKeyD)
			{
				xPosition += moveSpeed;
				currentKey = olc::Key::D;
			}
			else if (GetKey(olc::Key::A).bHeld && allowKeyA)
			{
				xPosition -= moveSpeed;
				currentKey = olc::Key::A;
			}
			else if (GetKey(olc::Key::W).bHeld && allowKeyW)
			{
				yPosition -= moveSpeed;
				currentKey = olc::Key::W;
			}
			else if (GetKey(olc::Key::S).bHeld && allowKeyS)
			{
				yPosition += moveSpeed;
				currentKey = olc::Key::S;
			}

			// Shift causes player to run
			if (GetKey(olc::Key::SHIFT).bHeld)
				moveSpeed = 0.75;
			else
				moveSpeed = 0.5;

			//Override collision keys
			if (GetKey(olc::Key::E).bHeld)
			{
				xPosition++;
				moveSpeed = 0.5;
			}
			if (GetKey(olc::Key::Q).bHeld)
			{
				xPosition--;
				moveSpeed = 0.5;
			}

			// Keys allowed to press are reset after collision stops them from being allowed
			allowKeyW = true, allowKeyA = true, allowKeyS = true, allowKeyD = true;

			// Draws player sprite
			DrawDecal({ float(xPosition), float(yPosition) }, decHam, { PLAYER_SPRITE_SCALE, PLAYER_SPRITE_SCALE });

			// Menu box that holds quills
			DrawPartialDecal({ 100.0f, 100.0f }, { 48.0f, 64.0f }, decMenu, { 0.0f, 0.0f }, {16.0f, 24.0f});
			DrawPartialDecal({ 148.0f, 100.0f }, { 48.0f, 64.0f }, decMenu, { 8.0f, 0.0f }, { 8.0f, 24.0f });
			DrawPartialDecal({ 196.0f, 100.0f }, { 48.0f, 64.0f }, decMenu, { 8.0f, 0.0f }, { 16.0f, 24.0f });

			// Three quills
			DrawDecal({ 120.0f, 118.0f }, decQuill, {2.0f, 2.0f});
			DrawDecal({ 155.0f, 118.0f }, decQuill, { 2.0f, 2.0f });
			DrawDecal({ 190.0f, 118.0f }, decQuill, { 2.0f, 2.0f });





			// escapes to main menu
			if (GetKey(olc::Key::ESCAPE).bPressed)
			{
				MAINMENUACTIVE = true;
			}

		}
		
		
		return true;
	}

public:
	bool playerCollide(Tile tile[6][6])
	{
		for (int i = 0; i < std::size(r.room1); i++)
		{
			for (int j = 0; j < std::size(r.room1[i]); j++)
			{
				if (r.room1[i][j].type == 1)
				{
					if (xPosition >= tile[i][j].x && xPosition <= (tile[i][j].x + tile[i][j].tileSize) && yPosition >= tile[i][j].y && yPosition <= (tile[i][j].y + tile[i][j].tileSize))
						return true;
					if (xPosition + playerCollisionBotRight.x >= tile[i][j].x && xPosition + playerCollisionBotRight.x <= (tile[i][j].x + tile[i][j].tileSize) && yPosition >= tile[i][j].y && yPosition <= (tile[i][j].y + tile[i][j].tileSize))
						return true;
					if (xPosition >= tile[i][j].x && xPosition <= (tile[i][j].x + tile[i][j].tileSize) && yPosition + playerCollisionBotRight.y >= tile[i][j].y && yPosition + playerCollisionBotRight.y <= (tile[i][j].y + tile[i][j].tileSize))
						return true;
					if (xPosition + playerCollisionBotRight.x >= tile[i][j].x && xPosition + playerCollisionBotRight.x <= (tile[i][j].x + tile[i][j].tileSize) && yPosition + playerCollisionBotRight.y >= tile[i][j].y && yPosition + playerCollisionBotRight.y <= (tile[i][j].y + tile[i][j].tileSize))
						return true;
				}

			}
		}

		return false;
	}

};


int main()
{
	Hamilton demo;
	if (demo.Construct(demo.screenSize, demo.screenSize, 1, 1))
		demo.Start();
	return 0;
}