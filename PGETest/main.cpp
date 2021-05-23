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
	olc::Sprite* sprHamSheet = nullptr;
	olc::Decal* decHamSheet = nullptr;
	int currentColumnOfHamSheet = 0; 
	float elapsedTime = 0.0f;
	double xPosition = screenSize / 2;
	double yPosition = screenSize / 2;
	double moveSpeed = 0.5;
	olc::vf2d playerCollisionTopLeft, playerCollisionBotRight;
	int playerHealth = 3;
	
	// Second Player? Burr sprites
	olc::Sprite* sprBurr = nullptr;
	olc::Decal* decBurr = nullptr;
	boolean collideWithBurr = false;

	// Talk with Burr
	std::vector<std::string> talkOptions = { "Press F to talk", "Hamilton: Pardon me, are\nyou Aaron Burr, Sir?", "Burr: That depends, Who's\nasking?"};
	int currentTalkOption = 0;

	// Key controls
	olc::Key currentKey = olc::Key::NONE;
	bool allowKeyW = true, allowKeyA = true, allowKeyS = true, allowKeyD = true;
	
	// Quill variables for health
	olc::Sprite* sprQuill = nullptr;
	olc::Decal* decQuill = nullptr;


	// Wall variables
	olc::Sprite* sprWall = nullptr;
	olc::Decal* decWall = nullptr;

	// Door variables
	olc::Sprite* sprDoor = nullptr;
	olc::Decal* decDoor = nullptr;

	// Rooms variable
	Rooms r;
	int currentRoomNumber = 0;
	std::vector<std::vector<Tile>> currentRoom;
	std::vector<std::vector<std::vector<Tile>>> listOfRooms;
	

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

		// loads ham sprite sheet from folder
		sprHamSheet = new olc::Sprite("C:/Users/Karl/Documents/GameSourceArt/AHamSpriteSheet.png");
		decHamSheet = new olc::Decal(sprHamSheet);

		// initializes player collision
		playerCollisionTopLeft = { float(xPosition), float(yPosition) };
		playerCollisionBotRight = { float((decHam->sprite->width) * 3.5), float((decHam->sprite->height) * 3.5) };

		// loads Burr sprites
		sprBurr = new olc::Sprite("c:/Users/Karl/Documents/GameSourceArt/ABurr.png");
		decBurr = new olc::Decal(sprBurr);


		// loads quill sprite
		sprQuill = new olc::Sprite("c:/Users/Karl/Documents/GameSourceArt/Quill2_2.png");
		decQuill = new olc::Decal(sprQuill);


		// loads wall sprite
		sprWall = new olc::Sprite("C:/Users/Karl/Documents/GameSourceArt/Wall.png");
		decWall = new olc::Decal(sprWall);

		// initializes each wall in the current room
		
		
		// Randomizez the two doors between the three rooms
		srand(time(NULL));
		r.randomDoor(r.room1,r.room2, rand() % 4 + 1);
		r.randomDoor(r.room2, r.room3, rand() % 4 + 1);

		// initializes the listOfRooms as well as loads the current room
		listOfRooms = { r.room1, r.room2, r.room3 };
		currentRoom = listOfRooms[currentRoomNumber];
		loadRoom(currentRoom);
		
		
		// loads door sprite
		sprDoor = new olc::Sprite("C:/Users/Karl/Documents/GameSourceArt/Door.png");
		decDoor = new olc::Decal(sprDoor);

		// loads menu sprite
		sprMenu = new olc::Sprite("C:/Users/Karl/Documents/GameSourceArt/DialogBox2.png");
		decMenu = new olc::Decal(sprMenu);

		// loads main menu background
		sprMainMenuBackground = new olc::Sprite("C:/Users/Karl/Documents/GameSourceArt/MainMenuBackground.png");
		decMainMenuBackground = new olc::Decal(sprMainMenuBackground);

		



		//int mmbackground = CreateLayer();
		//SetDrawTarget(mmbackground);		
		//DrawDecal({ 0.0f, 0.0f }, decMainMenuBackground);
		//EnableLayer(mmbackground, true);
		//SetDrawTarget(nullptr);
		


		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (MAINMENUACTIVE) // Main Menu
		{
			Clear(olc::YELLOW);

			std::string pixelton = "Pixelton";
			std::string startGame = "Start Game";
			//DrawDecal({ 0.0f, 0.0f }, decMainMenuBackground, { 1.0f , 1.0f });
			DrawStringDecal({ 185, 100 }, pixelton, olc::BLACK, { 3.0f, 3.0f });

			olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };

			// if mouse is in the borders of the Game Title (pixelton)
			if (mouse.x >= 185 && mouse.x <= 425 && mouse.y >= 90 && mouse.y <= 125)
			{
				DrawStringDecal({ 185, 100 }, pixelton, olc::WHITE, {3.0f, 3.0f});
				if (GetMouse(0).bPressed)
					MAINMENUACTIVE = false;
			}

		}
		else // Playing Field
		{



			// Clears screen
			Clear(olc::VERY_DARK_GREY);


			// draws wall sprites
			for (int i = 0; i < currentRoom.size(); i++)
			{
				for (int j = 0; j < currentRoom[i].size(); j++)
				{
					if (currentRoom[i][j].type == Tile::TileType::WALL)
					{
						// draws wall sprites
						DrawDecal({ float(j * 100), float(i * 100) }, decWall);

						// Collision rectangle for wall tiles
						//DrawRect({ int(j * 100), int(i * 100) }, { int(r.room1[i][j].tileSize), int(r.room1[i][j].tileSize) }, olc::GREEN);

					}
					else if (currentRoom[i][j].type == Tile::Tile::DAMAGE)
					{
						DrawRect({ (j * 100), (i * 100) }, { 100, 100 }, olc::RED);
					}
					else if (currentRoom[i][j].type == Tile::Tile::HEAL)
					{
						DrawRect({ (j * 100), (i * 100) }, { 100, 100 }, olc::GREEN);
					}
					else if (currentRoom[i][j].type == Tile::Tile::DOOR)
					{
						DrawDecal({ float(j * 100), float(i * 100) }, decDoor);
					}
					else if (currentRoom[i][j].type == Tile::Tile::BURR)
					{
						DrawDecal({ float(j * 100), float(i * 100) }, decBurr, { PLAYER_SPRITE_SCALE, PLAYER_SPRITE_SCALE });
					}

				}
			}


			// Player Collision
			std::pair<bool, int> didPlayerCollide = playerCollide(currentRoom);


			// Player collisions with objects which sends the player away from the current key they just pressed
			if (didPlayerCollide.first) // first contains the bool of collision
			{
				// Collision based on the tile hit
				if(didPlayerCollide.second == Tile::TileType::DAMAGE) // second contains the TileType it collided with
					playerHealth--;
				if (didPlayerCollide.second == Tile::TileType::HEAL) // second contains the TileType it collided with
					playerHealth++;
				if (didPlayerCollide.second == Tile::TileType::DOOR) // second contains the TileType it collided with
				{
					// Gets rid of Burr text upon entering a new room
					collideWithBurr = false;
					currentTalkOption = 0;

					// Depending if going right or left, change room accordingly
					if (currentKey == olc::Key::D) // if going right into a door, makes sure player loads on left side of room
					{
						currentRoomNumber++;

						currentRoom = listOfRooms[currentRoomNumber];
						loadRoom(currentRoom);
						xPosition -= 350;
					}
					else if (currentKey == olc::Key::A) // if going left into a door, makes sure player loads on right side of room
					{
						currentRoomNumber--;
	
						currentRoom = listOfRooms[currentRoomNumber];
						loadRoom(currentRoom);
						xPosition += 350;

					}
				}
				
				
				// Possible Solution is to get a boolean in each room that disappears after you leave a room
				//	It would become true once you hit burr and then reset when you enter a new room
				
				if (didPlayerCollide.second == Tile::TileType::BURR || collideWithBurr == true)
				{
					collideWithBurr = true;

					/*
					std::string talkOption = "Press F to talk";
					DrawString({ 180, 400  }, talkOption, olc::WHITE, 2);
					if (GetKey(olc::Key::F).bPressed)
					{
						talkOption = "Pardon me, Are you Aaron Burr, Sir?";
						DrawString({ 180, 400 }, talkOption, olc::WHITE, 2);
					}
					*/
				}
				
				// determines which key to not allow based on collision
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


			// Talking with Burr
			if (collideWithBurr)
			{
				// Selects current dialogue based on how many times player has pressed F
				std::string talkOption = talkOptions[currentTalkOption];

				//Drawing box that holds text
				DrawPartialDecal({ 70.0f, 510.0f }, { 50.0f, 80.0f }, decMenu, { 0.0f, 0.0f }, { 16.0f, 24.0f }); // left curved edge
				DrawPartialDecal({ 110.0f, 510.0f }, { 380.0f, 80.0f }, decMenu, { 8.0f, 0.0f }, { 8.0f, 24.0f }); // middle long
				DrawPartialDecal({ 490.0f, 510.0f }, { 60.0f, 80.0f }, decMenu, { 8.0f, 0.0f }, { 24.0f, 24.0f }); // right curved edge

				// Draws current Dialogue to text bos
				DrawStringDecal({ 90, 530 }, talkOption, olc::WHITE, { 2.0f , 2.0f });
				
				// If player hits F, currentTalkOption is increased, incrementing to next dialogue
				if (GetKey(olc::Key::F).bPressed)
				{
					currentTalkOption++;
					
					// if at last talk option, exit talk dialog
					if (currentTalkOption >= talkOptions.size())
					{
						collideWithBurr = false;
						currentTalkOption = 0;
					}
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
				currentKey = olc::Key::D;;
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
			DrawPartialDecal({ float(xPosition), float(yPosition) }, { 13.0f * PLAYER_SPRITE_SCALE, 27.0f * PLAYER_SPRITE_SCALE }, decHamSheet, { (currentColumnOfHamSheet) * 13.0f, 0.0f }, { 13.0f, 27.0f });
			
			// Cycles through the Sprite Sheet for hamilton when it gets to the end of the sprites 
			if (currentColumnOfHamSheet >= 4) // there are 4 different sprites on the sprite sheets and it starts at 0
				currentColumnOfHamSheet = 0;
			

			// Menu box that holds quills
			DrawPartialDecal({ 10.0f, 10.0f }, { 48.0f, 64.0f }, decMenu, { 0.0f, 0.0f }, {16.0f, 24.0f});
			DrawPartialDecal({ 58.0f, 10.0f }, { 48.0f, 64.0f }, decMenu, { 8.0f, 0.0f }, { 8.0f, 24.0f });
			DrawPartialDecal({ 106.0f, 10.0f }, { 48.0f, 64.0f }, decMenu, { 8.0f, 0.0f }, { 16.0f, 24.0f });

			// Three quills
			if (playerHealth == 3)
			{
				DrawDecal({ 30.0f, 28.0f }, decQuill, { 2.0f, 2.0f });
				DrawDecal({ 65.0f, 28.0f }, decQuill, { 2.0f, 2.0f });
				DrawDecal({ 100.0f, 28.0f }, decQuill, { 2.0f, 2.0f });

			}
			else if (playerHealth == 2)
			{
				DrawDecal({ 30.0f, 28.0f }, decQuill, { 2.0f, 2.0f });
				DrawDecal({ 65.0f, 28.0f }, decQuill, { 2.0f, 2.0f });

			}
			else if (playerHealth == 1)
			{
				DrawDecal({ 30.0f, 28.0f }, decQuill, { 2.0f, 2.0f });
			}

			// Health bounds
			if (playerHealth < 0)
				playerHealth = 0;
			else if (playerHealth > 3)
				playerHealth = 3;



			// escapes to main menu
			if (GetKey(olc::Key::ESCAPE).bPressed)
			{
				MAINMENUACTIVE = true;
			}

		}
	
		// Adds time to elapsedTime every 0.1 seconds so the graphic states of Ham can cycle through the sprite sheet
		elapsedTime += fElapsedTime;
		if (elapsedTime >= 0.1f)
		{
			elapsedTime = 0;
			currentColumnOfHamSheet++;
		}
		
		return true;
	}

public:
	std::pair<bool, int> playerCollide(std::vector<std::vector<Tile>> tile)
	{
		for (int i = 0; i < tile.size(); i++)
		{
			for (int j = 0; j < tile[i].size(); j++)
			{
				if (tile[i][j].type > 0)
				{
					if (xPosition >= tile[i][j].x && xPosition <= (tile[i][j].x + tile[i][j].tileSize) && yPosition >= tile[i][j].y && yPosition <= (tile[i][j].y + tile[i][j].tileSize))
						return std::make_pair(true, tile[i][j].type);

					if (xPosition + playerCollisionBotRight.x >= tile[i][j].x && xPosition + playerCollisionBotRight.x <= (tile[i][j].x + tile[i][j].tileSize) && yPosition >= tile[i][j].y && yPosition <= (tile[i][j].y + tile[i][j].tileSize))
						return std::make_pair(true, tile[i][j].type);

					if (xPosition >= tile[i][j].x && xPosition <= (tile[i][j].x + tile[i][j].tileSize) && yPosition + playerCollisionBotRight.y >= tile[i][j].y && yPosition + playerCollisionBotRight.y <= (tile[i][j].y + tile[i][j].tileSize))
						return std::make_pair(true, tile[i][j].type);

					if (xPosition + playerCollisionBotRight.x >= tile[i][j].x && xPosition + playerCollisionBotRight.x <= (tile[i][j].x + tile[i][j].tileSize) && yPosition + playerCollisionBotRight.y >= tile[i][j].y && yPosition + playerCollisionBotRight.y <= (tile[i][j].y + tile[i][j].tileSize))
						return std::make_pair(true, tile[i][j].type);
				}

			}
		}

		return std::make_pair(false, 0);
	}

	void switchRooms(std::vector<std::vector<Tile>> currentroom, std::vector<std::vector<Tile>> newRoom)
	{
		// reaassigns all the tiles in currentRoom to the new rooms
		currentroom = newRoom;

		// remakes all the collisions boxes when switching rooms
		for (int i = 0; i < currentroom.size(); i++)
		{
			for (int j = 0; j < currentroom[i].size(); j++)
			{
				newRoom[i][j].setX(float(j * 100));
				newRoom[i][j].setY(float(i * 100));
			}

		}

		
	}

	void loadRoom(std::vector<std::vector<Tile>> &vect)
	{
			for (int i = 0; i < vect.size(); i++)
			{
				for (int j = 0; j < vect[i].size(); j++)
				{
					vect[i][j].setX(float(j * 100));
					vect[i][j].setY(float(i * 100));
				}

			}
	}

};


int main()
{
	Hamilton demo;
	if (demo.Construct(demo.screenSize, demo.screenSize, 1, 1))
		demo.Start();
	return 0;
}