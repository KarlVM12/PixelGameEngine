#include "Tile.cpp"

// macros
#define n Tile(Tile::TileType::NOTHING)
#define w Tile(Tile::TileType::WALL)
#define d Tile(Tile::TileType::DAMAGE)


class Rooms
{
public:


	Tile room1[6][6] = { {w,w,w,w,w,w},
						 {w,n,n,n,n,w},
						 {w,n,n,n,n,w},
						 {w,n,n,n,n,w},
						 {w,n,n,n,d,w},
						 {w,w,w,w,w,w} };
};
