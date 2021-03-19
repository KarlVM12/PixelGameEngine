#include "Tile.cpp"

// macros
#define n Tile(Tile::TileType::NOTHING)
#define w Tile(Tile::TileType::WALL)


class Rooms
{
public:


	Tile room1[6][6] = { {w,w,w,w,w,w},
						 {w,n,n,n,n,w},
						 {w,n,w,n,n,w},
						 {w,n,n,n,n,w},
						 {w,n,n,n,n,w},
						 {w,w,w,w,w,w} };
};
