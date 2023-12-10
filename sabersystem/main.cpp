#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class SplitSprite : public olc::PixelGameEngine {

public:
    SplitSprite() {
        sAppName = "Split sprite demo";
    }

private:
    // put your class variables here
    olc::vi2d split1, split2;     // defines cut line

    // little struct to animate the sprites
    struct sAnimStruct {
        olc::vi2d pos;
        olc::Sprite* pSprite = nullptr;
    };
    std::vector<sAnimStruct> vObjects;
    std::vector<olc::vf2d> cut_line;
    std::vector<olc::vi2d> cut_sprite_line;

public:
    bool OnUserCreate() override {

        // initialize your assets here
        split1 = { 50, 150 };
        split2 = { 350, 250 };

        sAnimStruct aux = { olc::vi2d(200, 200), new olc::Sprite("trooper.png") };
        sAnimStruct aux2 = { olc::vi2d(800, 200), new olc::Sprite("trooper.png") };
        sAnimStruct saber = { olc::vi2d(200, 400), new olc::Sprite("saber.png") };
        //vObjects.push_back(saber);
        vObjects.push_back(aux);
        vObjects.push_back(aux2);
        
        return true;
    }

    // from https://stackoverflow.com/questions/1560492/how-to-tell-whether-a-point-is-to-the-right-or-left-side-of-a-line
    // Returns true if "point" is to the left of "line". Note that the line has a direction, that is determined by
    // the order of line1 and line2
    bool IsLeft(olc::vi2d line1, olc::vi2d line2, olc::vi2d point, float& pos) {

        
        return (line2.x - line1.x) * (point.y - line1.y) - (line2.y - line1.y) * (point.x - line1.x) < 0;
    }

    olc::Sprite* TrimToMinimal(olc::Sprite* inputSpritePtr, olc::vi2d& vOffset) {
        olc::Sprite* resultPtr = nullptr;
        if (inputSpritePtr == nullptr) {
            std::cout << "ERROR: TrimToMinimal() --> cannot trim nullptr input!" << std::endl;
        }
        else {
            int lowX = inputSpritePtr->width, hghX = -1, lowY = inputSpritePtr->height, hghY = -1;
            for (int y = 0; y < inputSpritePtr->height; y++) {
                for (int x = 0; x < inputSpritePtr->width; x++) {
                    // find the extreme values that are non blank
                    if (inputSpritePtr->GetPixel(x, y) != olc::BLANK) {
                        lowX = std::min(x, lowX);
                        lowY = std::min(y, lowY);
                        hghX = std::max(x, hghX);
                        hghY = std::max(y, hghY);
                    }
                }
            }
            vOffset = { lowX,lowY };
            // having found the bounding box, create a new sprite with new size
            resultPtr = new olc::Sprite(hghX - lowX + 1, hghY - lowY + 1);
            for (int y = lowY; y <= hghY; y++) {
                for (int x = lowX; x <= hghX; x++) {
                    // copy the pixels in the new smaller sprite
                    olc::Pixel aux = inputSpritePtr->GetPixel(x, y);
                    resultPtr->SetPixel(x - lowX, y - lowY, aux);
                }
            }
        }
        return resultPtr;
    }

    // splits sprite pointed to by org (and at screen posiiton spriteOrigin) into two sprites along
    // cut line specified by split1 and split 2
    // NOTE: if the cut line doesn't actually cut the sprite, this will result one empty (i.e. all blank)
    // sub sprite and one copy of the original
    void SpriteSplit(
        olc::Sprite* org,        // ptr to original sprite
        olc::vi2d spriteOrigin,  // origin of original sprite in screen space (needed to transform cut line from screen space to sprite space)
        olc::vi2d split1,        // def. of cut line
        olc::vi2d split2,
        olc::Sprite** newHalf1,   // ptr to new parts of original sprite
        olc::Sprite** newHalf2,
        olc::vi2d &OffsetHalf1,
        olc::vi2d &OffsetHalf2
    ) {
        // transform cut line coordinates into sprite space
        olc::vi2d localSplit1 = split1 - spriteOrigin;
        olc::vi2d localSplit2 = split2 - spriteOrigin;

        // create new sprites
        *newHalf1 = new olc::Sprite(org->width, org->height);
        *newHalf2 = new olc::Sprite(org->width, org->height);

        // iterate all pixels of the original sprite
        for (int y = 0; y < org->height; y++) {
            for (int x = 0; x < org->width; x++) {
                // get current pixel
                olc::vi2d pixPos = olc::vi2d(x, y);
                olc::Pixel curPix = org->GetPixel(x, y);
                // depending on left or right of cut line, put the pixel in the right sub sprite
                // and set the corresponding pixel in the other subsprite to olc::BLANK
               
                float pos;
                if (IsLeft(localSplit1, localSplit2, pixPos, pos)) {
                    (*newHalf1)->SetPixel(x, y, curPix);
                    (*newHalf2)->SetPixel(x, y, olc::BLANK);
                    
                }
                else {
                    (*newHalf1)->SetPixel(x, y, olc::BLANK);
                    (*newHalf2)->SetPixel(x, y, curPix);
                }
            }
        }

      
        olc::Sprite* pTmpPtr1 = TrimToMinimal(*newHalf1, OffsetHalf1);
        delete* newHalf1;
        *newHalf1 = pTmpPtr1;
        olc::Sprite* pTmpPtr2 = TrimToMinimal(*newHalf2, OffsetHalf2);
        delete* newHalf2;
        *newHalf2 = pTmpPtr2;

       
    }

    void sabermarks(olc::Sprite* sprite)
    {
        olc::Pixel p1 = olc::DARK_RED;
        olc::Pixel p = olc::DARK_YELLOW;


        for (int y = 0; y < sprite->height; y++)
        {
            for (int x = 0; x < sprite->width; x++)
            {
               
                    if (sprite->GetPixel(x, y) == olc::BLANK)
                    {
                        if (sprite->GetPixel(x - 1, y) != olc::BLANK && sprite->GetPixel(x - 1, y) != olc::MAGENTA)
                        {
                            int i = 0;
                            sprite->SetPixel(x - 1, y, p);
                            sprite->SetPixel(x - 2, y, p1);
                        }
                        if (sprite->GetPixel(x, y - 1) != olc::BLANK && sprite->GetPixel(x, y - 1) != olc::MAGENTA)
                        {
                            int  i = 0;
                            sprite->SetPixel(x, y - 1, p);
                            sprite->SetPixel(x, y - 2, p1);
                        }
                        if (sprite->GetPixel(x, y + 1) != olc::BLANK && sprite->GetPixel(x, y + 1) != olc::MAGENTA)
                        {
                            int  i = 0;
                            sprite->SetPixel(x, y + 1, p);
                            sprite->SetPixel(x, y + 2, p1);
                        }
                        if (sprite->GetPixel(x + 1, y) != olc::BLANK && sprite->GetPixel(x + 1, y) != olc::MAGENTA)
                        {
                            int  i = 0;
                            sprite->SetPixel(x + 1, y, p);
                            sprite->SetPixel(x + 2, y, p1);
                        }
                    }
                
            }
        }
    }


    bool OnUserUpdate(float fElapsedTime) override {

        // your update code per frame here
        Clear(olc::VERY_DARK_GREY);
        
        DrawLine(split1, split2, olc::MAGENTA);
        DrawCircle(split1, 4, olc::YELLOW);
        DrawCircle(split2, 4, olc::YELLOW);
       

        // cut the sprites on user request (key T)
        std::vector<sAnimStruct> vObjectsCopy;
        for (int i = 0; i < (int)vObjects.size(); i++) {
            // testing SpriteSplit() function when pressing T


            if (GetKey(olc::Key::SPACE).bPressed) {

                // grab pointer to current sprite
                olc::Sprite* curSprite = vObjects[i].pSprite;
                olc::Sprite* newSprite1 = nullptr;
                olc::Sprite* newSprite2 = nullptr;

                olc::vi2d vOffset1, vOffset2;
                SpriteSplit(curSprite, vObjects[i].pos, split1, split2, &newSprite1, &newSprite2, vOffset1, vOffset2);
                
                sAnimStruct aux1 = { vObjects[i].pos + vOffset1,newSprite1}; //bottom
                sAnimStruct aux2 = {vObjects[i].pos + vOffset2, newSprite2 }; // top
               
                if (aux1.pSprite->width > 0 && aux1.pSprite->height > 0)
                {
                    vObjectsCopy.push_back(aux1);
                }
                else
                {
                    delete aux1.pSprite;
                }
                if (aux2.pSprite->width > 0 && aux2.pSprite->height > 0)
                {
                    vObjectsCopy.push_back(aux2);
                }
                else
                {
                    delete aux2.pSprite;
                }
                delete curSprite;
                vObjects[i].pSprite = nullptr;
            }
            else {
                vObjectsCopy.push_back(vObjects[i]);
            }
        }
        vObjects.clear();
        vObjects = vObjectsCopy;

       // animate sprite movement
       for (int i = 0; i < (int)vObjects.size(); i++) {
           auto rand_range3 = [=]() {
               switch (rand() % 50) {   // don't move too often
               case 0: return -1;
               case 1: return +1;
               }
               return 0;
               };
       
           vObjects[i].pos += olc::vi2d(rand_range3(), rand_range3());
       }

        for (auto& obj : vObjects)
        {
            sabermarks(obj.pSprite);
        }
        // alter cut line points
        //if (GetMouse(0).bPressed) { split1 = GetMousePos(); }
        split1 = { ScreenWidth() / 2, ScreenHeight() / 2 };
        split2 = { GetMouseX(),GetMouseY() };
        // render sprite object(s)
        for (int i = 0; i < (int)vObjects.size(); i++) {
            SetPixelMode(olc::Pixel::MASK);

            DrawSprite(vObjects[i].pos, vObjects[i].pSprite);
            SetPixelMode(olc::Pixel::NORMAL);

            //            DrawRect( vObjects[i].pos, olc::vi2d( vObjects[i].pSprite->width, vObjects[i].pSprite->height ), olc::WHITE );
        }
        // render cut line
       
        // render # of objects
        DrawString(0, 0, "#objects = " + std::to_string(vObjects.size()));

        return true;
    }

    bool OnUserDestroy() override {

        // your clean up code here
        for (int i = 0; i < (int)vObjects.size(); i++) {
            delete vObjects[i].pSprite;
        }
        vObjects.clear();

        return true;
    }
};

// keep the screen dimensions constant and vary the resolution by adapting the pixel size
#define SCREEN_X    1200
#define SCREEN_Y    800
#define PIXEL_X       1
#define PIXEL_Y       1

int main()
{
    SplitSprite demo;
    if (demo.Construct(SCREEN_X / PIXEL_X, SCREEN_Y / PIXEL_Y, PIXEL_X, PIXEL_Y))
        demo.Start();

    return 0;
}

