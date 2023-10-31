#include "projection.h"

Player::Player()
{
	x = 200;
	y = 50;
	turndirection = 0;
	walkDirection = 0;
	rotationangle = 0;
	walkspeed = 100;
	turnspeed = 45 * (3.14159f / 180.0f);
	strafedirection = 0;
	strafeleft = true;
	straferight = true;
	fPlayerH = 0.5f;

}

void Player::input(olc::PixelGameEngine* pge)
{
}

void Player::movement(olc::PixelGameEngine* pge, float deltatime)
{
}
