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

	olc::Sprite* sprHam = nullptr;
	olc::Decal*  decHam = nullptr;
	double xPosition = screenSize / 2;
	double yPosition = screenSize / 2;
	double moveSpeed = 0.5;

	olc::Sprite* sprWall = nullptr;
	olc::Decal*  decWall = nullptr;

	Rooms r;

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		sprHam = new olc::Sprite("C:/Users/Karl/Documents/GameSourceArt/AHam.png");
		decHam = new olc::Decal(sprHam);
		

		sprWall = new olc::Sprite("C:/Users/Karl/Documents/GameSourceArt/Wall.png");
		decWall = new olc::Decal(sprWall);


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

					r.room1[i][j].setX(float(j * 100));
					r.room1[i][j].setY(float(i * 100));

					DrawRect(r.room1[i][j].topLeftCollisionBox, r.room1[i][j].botRightCollisionBox, olc::GREEN);

				}
				

			}
		}
		
		DrawRect( float(xPosition), float(yPosition) , (decHam->sprite->width)* 3.5, (decHam->sprite->height) * 3.5, olc::RED);
		
		olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };

		if (GetKey(olc::Key::D).bHeld)
			xPosition += moveSpeed;
		if (GetKey(olc::Key::A).bHeld)
			xPosition -= moveSpeed;
		if (GetKey(olc::Key::W).bHeld)
			yPosition -= moveSpeed;
		if (GetKey(olc::Key::S).bHeld)
			yPosition += moveSpeed;
		
		DrawDecal({ float(xPosition), float(yPosition) }, decHam, { 3.5f, 3.5f });
		
		return true;
	}

};


int main()
{
	Hamilton demo;
	if (demo.Construct(demo.screenSize, demo.screenSize, 1, 1))
		demo.Start();
	return 0;
}