#ifndef _SDLDRAW_H_
#define _SDLDRAW_H_

#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_gfxPrimitives.h"
#include <vector>
#include <map>
#include <string>
#include "main.h"

#define SCREEN_WIDTH 640	
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 16
#define FRAME_RATE 40

using namespace std;

class SDLscreen;
class Sprites;
void LockFrameRate();

struct sprite_info{
  SDL_Surface *image;
  int repeat;
  int repeated;
};

class Sprites{
  public:
    ~Sprites();
    SDL_Surface *Load(char *file, int save_image = True);
    char *GetFileName(struct sprite_info *sprite);
    map<string, SDL_Surface *> SpriteMap;
    map<SDL_Surface *, string> InverseSpriteMap;
  private:
};

class SDLscreen{
  public:
    static Sprites GameSprites;
    Sprites LevelSprites;
    int Init( );
    void Update( );
    void ToggleFullScreen();
    void DrawObject(struct sprite_info *sprite, int x, int y, SDL_Rect *src = NULL,
                    unsigned int Red = 0xff, unsigned int Green = 0xff, unsigned int Blue = 0xff);
    void DrawPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
    void DrawLine(int x0, int y0, int x1, int y1, Uint8 R, Uint8 G, Uint8 B);
    void DrawRect(int x0, int y0, int x1, int y1, Uint8 R, Uint8 G, Uint8 B);
    ~SDLscreen( );

  private:
    const SDL_VideoInfo *video_info;
    SDL_Surface *screen;
    int videoFlags;
};

#endif

