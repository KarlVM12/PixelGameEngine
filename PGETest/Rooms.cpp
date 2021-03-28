#include "Tile.cpp"
#include <vector>

// macros
#define n Tile(Tile::TileType::NOTHING)
#define w Tile(Tile::TileType::WALL)
#define dd Tile(Tile::TileType::DAMAGE)
#define h Tile(Tile::TileType::HEAL)
#define d Tile(Tile::TileType::DOOR)


class Rooms
{
public:

	/*
	Tile room1[6][6] = { {w,w,w,w,w,w},
						 {w,n,n,n,n,w},
						 {w,n,n,n,n,d},
						 {w,n,n,n,n,d},
						 {w,n,n,n,n,w},
						 {w,w,w,w,w,w} };
						 */

	std::vector<std::vector<Tile>> room1 = { {w,w,w,w,w,w},
											{w,n,n,n,n,w},
											{w,n,n,n,n,d},
											{w,n,n,n,n,d},
											{w,n,n,n,n,w},
											{w,w,w,w,w,w} };
	/*
	Tile room2[6][6] = { {w,w,w,w,w,w},
						 {w,n,n,n,n,w},
						 {d,n,n,n,n,w},
						 {d,n,n,n,n,w},
						 {w,n,n,n,n,w},
						 {w,w,d,d,w,w} };
						 */
	std::vector<std::vector<Tile>> room2 = { {w,w,w,w,w,w},
											{w,n,n,n,n,w},
											{d,n,n,n,n,w},
											{d,n,n,n,n,w},
											{w,n,n,n,n,w},
											{w,w,d,d,w,w} };

	/*
	Tile room3[6][6] = { {w,w,d,d,w,w},
						 {w,n,n,n,n,w},
						 {w,n,n,n,n,w},
						 {w,n,n,n,n,w},
						 {w,n,n,n,n,w},
						 {w,w,w,w,w,w} };
						 */
	std::vector<std::vector<Tile>> room3 = { {w,w,d,d,w,w},
											 {w,n,n,n,n,w},
											 {w,n,n,n,n,w},
											 {w,n,n,n,n,w},
											 {w,n,n,n,n,w},
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

	std::vector<std::vector<std::vector<Tile>>> vect = {room4};

	void addToList()
	{
		vect.push_back(room5);
	}
};
