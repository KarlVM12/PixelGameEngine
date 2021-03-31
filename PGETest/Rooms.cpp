#include "Tile.cpp"
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       

// macros
#define n Tile(Tile::TileType::NOTHING)
#define w Tile(Tile::TileType::WALL)
#define dd Tile(Tile::TileType::DAMAGE)
#define h Tile(Tile::TileType::HEAL)
#define d Tile(Tile::TileType::DOOR)


class Rooms
{
public:

	std::vector<std::vector<Tile>> room1 = { {w,w,w,w,w,w},
											 {w,n,n,n,n,w},
											 {w,n,n,n,n,w},
											 {w,n,n,n,n,w},
											 {w,n,n,dd,n,w},
											 {w,w,w,w,w,w} };

	Rooms() {	}
	
	std::vector<std::vector<Tile>> room2 = { {w,w,w,w,w,w},
											 {w,n,n,n,n,w},
											 {w,n,n,n,n,w},
											 {w,n,n,n,n,w},
											 {w,n,n,n,n,w},
											 {w,w,w,w,w,w} };

	std::vector<std::vector<Tile>> room3 = { {w,w,w,w,w,w},
											 {w,n,n,n,n,w},
											 {w,n,n,n,n,w},
											 {w,n,n,n,n,w},
											 {w,n,n,h,n,w},
											 {w,w,w,w,w,w} };
	
	std::vector<std::vector<Tile>> room4 = { {w,w,d,d,w,w},
											 {w,n,n,n,n,w},
											 {w,n,n,n,n,w},
											 {w,n,n,n,n,w},
											 {w,n,n,n,n,w},
											 {w,w,w,w,w,w} };

	std::vector<std::vector<Tile>> room5 = { {w,w,w,w,w,w},
											 {w,n,n,n,n,w},
											 {w,n,n,n,n,w},
											 {w,n,n,n,n,w},
											 {w,n,n,n,n,w},
											 {w,w,w,w,w,w} };
	
	void randomDoor(std::vector<std::vector<Tile>> &room, std::vector<std::vector<Tile>> &nextRoom, int randNum)
	{
		std::srand(time(NULL));

		int randomIndex = rand() % 4 + 1;

		room[randNum][5] = d;
		nextRoom[randNum][0] = d;
	}


};
