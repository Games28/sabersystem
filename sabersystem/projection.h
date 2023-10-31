#include "olcPixelGameEngine.h"

class Player
{
public:
	Player();
	void input(olc::PixelGameEngine* pge);
	void movement(olc::PixelGameEngine* pge, float deltatime);
public:
	float x, y;
	int walkDirection;
	float walkspeed;
	bool strafeleft;
	bool straferight;
	int strafedirection;
	float fPlayerH;
	float rotationangle;
	float turndirection;
	float turnspeed;

};

class Projection
{
public:
	Projection();


public:

};