#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class SaberSystem : public olc::PixelGameEngine
{
public:
	SaberSystem()
	{
		sAppName = "saber system";
	}

public:
	olc::Sprite* trooper;

	std::vector<olc::vf2d> cut_line;
	std::vector<olc::vi2d> cut_sprite_line;
	int x = 100;
	int y = 100;
	bool iscut = false;
	bool completecoords = false;
	
	void newspr(int32_t x, int32_t y, olc::Sprite* sprite, uint32_t scale = 1, uint8_t flip = 0)
	{
		
		if (sprite == nullptr)
			return;
		cut_sprite_line.clear();
		int32_t fxs = 0, fxm = 1, fx = 0;
		int32_t fys = 0, fym = 1, fy = 0;
		if (flip & olc::Sprite::Flip::HORIZ) { fxs = sprite->width - 1; fxm = -1; }
		if (flip & olc::Sprite::Flip::VERT) { fys = sprite->height - 1; fym = -1; }

		if (scale > 1)
		{
			fx = fxs;
			for (int32_t i = 0; i < sprite->width; i++, fx += fxm)
			{
				fy = fys;
				for (int32_t j = 0; j < sprite->height; j++, fy += fym)
					for (uint32_t is = 0; is < scale; is++)
						for (uint32_t js = 0; js < scale; js++)
						{
							int32_t x1 = x + (i * scale) + is;
							int32_t y1 = y + (j * scale) + js;
							olc::Pixel p = sprite->GetPixel(fx, fy);
							for (int i = 0; i < cut_line.size(); i++)
							{
								int32_t cutx = (int32_t)cut_line[i].x;
								int32_t cuty = (int32_t)cut_line[i].y;
								if (cutx == x1 && cuty == y1)
								{
									
									p = olc::GREEN;
								}
							}
							
							Draw(x1,y1, p);
						}
			}
		}
		else
		{
			fx = fxs;
			for (int32_t i = 0; i < sprite->width; i++, fx += fxm)
			{

				fy = fys;
				for (int32_t j = 0; j < sprite->height; j++, fy += fym)
				{
					if (sprite->GetPixel(fx, fy) != olc::MAGENTA)
					{
						int32_t x1 = x + i;
						int32_t y1 = y + j;
						olc::Pixel p = sprite->GetPixel(fx, fy);
						for (int i = 0; i < cut_line.size(); i++)
						{
							int32_t cutx = (int32_t)cut_line[i].x;
							int32_t cuty = (int32_t)cut_line[i].y;
							if (cutx == x1 && cuty == y1)
							{
								olc::vi2d coord = { x1,y1 };
								cut_sprite_line.push_back(coord);
								p = olc::GREEN;
							}
						}
						
						
						
						Draw(x1, y1,p);
					}
				}
			}
			completecoords = true;
		}
	}

	void get_line_coord(int x0, int y0, int x1, int y1)
	{
		cut_line.clear();
		olc::vf2d currentcoord;
		int delta_x(x1 - x0);
		int delta_y(y1 - y0);

		int longest_side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

		float x_inc = delta_x / float(longest_side_length);
		float y_inc = delta_y / float(longest_side_length);

		currentcoord = { (float)x0,(float)y0 };
		

		for (int i = 0; i <= longest_side_length; i++)
		{
			cut_line.push_back(currentcoord);
			
			currentcoord.x += x_inc;
			currentcoord.y += y_inc;
			

		}
	}

	void draw_line(int x0, int y0, int x1, int y1, olc::Pixel color)
	{
		
		int delta_x(x1 - x0);
		int delta_y(y1 - y0);

		int longest_side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);



		for (int i = 0; i <= longest_side_length; i++)
		{
			
			Draw(round(cut_line[i].x), round(cut_line[i].y), color);


		}
	}

	void newDraw(int32_t x, int32_t y, olc::Sprite* sprite)
	{
		int32_t fxs = 0, fxm = 1, fx = 0;
		int32_t fys = 0, fym = 1, fy = 0;

		fx = fxs;
		for (int32_t i = 0; i < sprite->width; i++, fx += fxm)
		{
			
			
			fy = fys;
			for (int32_t j = 0; j < sprite->height; j++, fy += fym)
			{
				
				
				if (sprite->GetPixel(fx, fy) != olc::MAGENTA)
				{
					int32_t y1 = y + j;
					int32_t x1 = x + i;
					olc::Pixel p;
					for (int i = 0; i < cut_sprite_line.size(); i++)
					{
						int32_t cutx = (int32_t)cut_sprite_line[i].x;
						int32_t cuty = (int32_t)cut_sprite_line[i].y;
						
						if (x1 <= cutx && y1 <= cuty)
						{
							
							p = sprite->GetPixel(fx, fy);
						}
					
						Draw(x1, y1, p);
					}
					
					
					


					
				}
			}
		}
	}

public:

	bool OnUserCreate() override
	{
		trooper = new olc::Sprite("trooper.png");

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetKey(olc::UP).bHeld) y += -100 * fElapsedTime;
		if (GetKey(olc::DOWN).bHeld) y += 100 * fElapsedTime;
		if (GetKey(olc::LEFT).bHeld) x += -100 * fElapsedTime;
		if (GetKey(olc::RIGHT).bHeld) x += 100 * fElapsedTime;
		if (GetKey(olc::SPACE).bHeld) iscut = true;
		if (GetKey(olc::UP).bReleased) y += 0;
		if (GetKey(olc::DOWN).bReleased) y += 0;
		if (GetKey(olc::LEFT).bReleased) x += 0;
		if (GetKey(olc::RIGHT).bReleased) x += 0;
		if (GetKey(olc::SPACE).bReleased) iscut = false;


		Clear(olc::BLACK);

		
		

		get_line_coord(ScreenWidth() / 2, ScreenHeight() / 2, GetMouseX(), GetMouseY());

		draw_line(ScreenWidth() / 2, ScreenHeight() / 2, GetMouseX(), GetMouseY(), olc::WHITE);
		if (iscut && cut_sprite_line.size() != 0)
		{
			newDraw(x, y, trooper);
		}
		else
		{
			newspr(x, y, trooper);
		}

		return true;
	}

};


int main()
{
	SaberSystem demo;

	if (demo.Construct(1200, 800, 1, 1))
	{
		demo.Start();
	}

	return 0;
}