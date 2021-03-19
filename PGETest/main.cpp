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

	const float PLAYER_SPRITE_SCALE = 3.5;
	olc::Sprite* sprHam = nullptr;
	olc::Decal*  decHam = nullptr;
	double xPosition = screenSize / 2;
	double yPosition = screenSize / 2;
	double moveSpeed = 0.5;
	olc::vf2d playerCollisionTopLeft, playerCollisionBotRight;

	olc::Sprite* sprWall = nullptr;
	olc::Decal*  decWall = nullptr;

	Rooms r;

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		sprHam = new olc::Sprite("C:/Users/Karl/Documents/GameSourceArt/AHam.png");
		decHam = new olc::Decal(sprHam);

		playerCollisionTopLeft = { float(xPosition), float(yPosition) };
		playerCollisionBotRight = { float((decHam->sprite->width) * 3.5), float((decHam->sprite->height) * 3.5) };
		

		sprWall = new olc::Sprite("C:/Users/Karl/Documents/GameSourceArt/Wall.png");
		decWall = new olc::Decal(sprWall);

		for (int i = 0; i < std::size(r.room1); i++)
		{
			for (int j = 0; j < std::size(r.room1[i]); j++)
			{
				r.room1[i][j].setX(float(j * 100));
				r.room1[i][j].setY(float(i * 100));
			}

		}
		

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Called once per frame, draws random coloured pixels
		//for (int x = 0; x < ScreenWidth(); x++)
			//for (int y = 0; y < ScreenHeight(); y++)
			//	Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));
		
		Clear(olc::VERY_DARK_GREY);

		
		int wallCounter = 0;
		for (int i = 0; i < std::size(r.room1); i++)
		{
			for (int j = 0; j < std::size(r.room1[i]); j++)
			{
				if (r.room1[i][j].type == Tile::TileType::WALL)
				{
					//DrawDecal({ float(j * 100), float(i * 100) }, decWall);


					// Collision rectangle for wall tiles
					DrawRect({ int(j * 100), int(i * 100) }, { int(r.room1[i][j].tileSize), int(r.room1[i][j].tileSize) },olc::GREEN);
					
					/*
					if (playerCollide(r.room1[i][j]))
						moveSpeed = 0;
					else
						moveSpeed = 0.5;
					*/	
				}
				
			}
		}

		
		if (playerCollide(r.room1))
			moveSpeed = 0;
		else
			moveSpeed = 0.5;
			
	
		playerCollisionTopLeft = { float(xPosition), float(yPosition) };
		DrawRect( playerCollisionTopLeft, playerCollisionBotRight, olc::RED);
		
		olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };

		if (GetKey(olc::Key::D).bHeld)
			xPosition += moveSpeed;
		if (GetKey(olc::Key::A).bHeld)
			xPosition -= moveSpeed;
		if (GetKey(olc::Key::W).bHeld)
			yPosition -= moveSpeed;
		if (GetKey(olc::Key::S).bHeld)
			yPosition += moveSpeed;
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



		DrawDecal({ float(xPosition), float(yPosition) }, decHam, { PLAYER_SPRITE_SCALE, PLAYER_SPRITE_SCALE });
		
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