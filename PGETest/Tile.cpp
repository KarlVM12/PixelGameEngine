#include "olcPixelGameEngine.h"

class Tile
{

public:
	float x, y;
	float tileSize = 100;
	// Tile Type
	enum TileType { NOTHING, WALL } type;
	
	olc::vf2d topLeftCollisionBox, botRightCollisionBox;

	Tile(TileType type_)
	{
		x = 0;
		y = 0;
		type = type_;

		topLeftCollisionBox = { x, y };
		botRightCollisionBox = { x + tileSize, y + tileSize};
	}
	
	Tile(float x_, float y_)
	{
		x = x_;
		y = y_;
		type = NOTHING;

		topLeftCollisionBox = { x, y };
		botRightCollisionBox = { x + tileSize, y + tileSize };
	}

	Tile(float x_, float y_, TileType type_)
	{
		x = x_;
		y = y_;
		type = type_;

		topLeftCollisionBox = { x, y };
		botRightCollisionBox = { x + tileSize, y + tileSize };
	}

	void setX(float x_)
	{
		x = x_;

		topLeftCollisionBox = { x, y };
		botRightCollisionBox = { x + tileSize, y + tileSize };
	}

	void setY(float y_)
	{
		y = y_;

		topLeftCollisionBox = { x, y };
		botRightCollisionBox = { x + tileSize, y + tileSize };
	}

};