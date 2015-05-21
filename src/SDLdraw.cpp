//  ZoidsQuest
//  Copyright (C) 2004 Max Atkin <g_8pit@users.sourceforge.net>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA.



#include <math.h>
#include "SDLdraw.h"
#include "werror.h"
#include "main.h"
#include <string>
#include <vector>
//#include "SDL_rotozoom.h"

////////////////////////////////////////////////////////////////////////////////

int SDLscreen::Init( ){
    fprintf(stderr,"Initialising SDL video...\n");

    if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0){
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        return Error;
    }

    video_info=SDL_GetVideoInfo( );
    if(!video_info){
        fprintf(stderr,"Video query failed...\n");
        return Error;
    }
    
    videoFlags = SDL_DOUBLEBUF;
    videoFlags = SDL_HWPALETTE;       /* Store the palette in hardware */
    
    if(video_info->hw_available)videoFlags|=SDL_HWSURFACE;
    else videoFlags|=SDL_SWSURFACE;

    //if(video_info->blit_hw)videoFlags|=SDL_HWACCEL;

    fprintf(stderr,"Setting SDL video mode...\n");

    screen = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,0,videoFlags);
    SDL_WM_SetCaption("Platform Game", "Platform Game");

    if(screen == NULL){
        fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
        return Error;
    }
    return Ok;
}

void SDLscreen::ToggleFullScreen(){
  fprintf(stderr, "Toggle full screen\n");
  SDL_WM_ToggleFullScreen(screen);
}

void SDLscreen::DrawObject(struct sprite_info *sprite, int x, int y, SDL_Rect *src,
                           unsigned int Red, unsigned int Green, unsigned int Blue){
    SDL_Rect dest;
    if(sprite == NULL) return;
    SDL_Surface *image = sprite->image;
    if(image == NULL){
      fprintf(stderr, "bad image\n");
      return;
    }
    dest.x = x;
    dest.y = y;
    dest.w = image->w;
    dest.h = image->h;

    if(Red < 0xff || Green < 0xff || Blue < 0xff){
      SDL_Surface *RGBimage = SDL_CreateRGBSurface(SDL_HWSURFACE, image->w, image->h, 16, Red, Green, Blue, 0xff);
      if(RGBimage != NULL){
        SDL_BlitSurface(image, src, RGBimage, NULL);
        SDL_BlitSurface(RGBimage, NULL, screen, &dest);
        SDL_FreeSurface(RGBimage);
      }
    }
    else SDL_BlitSurface(image, src, screen, &dest);
}

void SDLscreen::Update( ){
  int oldtime, frame_time;
  oldtime = SDL_GetTicks();
  SDL_Flip(screen);
  frame_time = SDL_GetTicks() - oldtime;
  int delay_time = (int)(1000 / FRAME_RATE - frame_time);
  if(delay_time > 10) SDL_Delay(delay_time);
}

void SDLscreen::DrawPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B){
    Uint32 color = SDL_MapRGB(screen->format, R, G, B);
    Uint8 *bufp;
    if((x < 0)||(x > SCREEN_WIDTH)||(y < 0)||(y > SCREEN_HEIGHT)) return;

    if(SDL_MUSTLOCK(screen)){
        if (SDL_LockSurface(screen) < 0){
            return;
        }
    }
    bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
    *bufp = color;

    if(SDL_MUSTLOCK(screen)){
        SDL_UnlockSurface(screen);
    }
}

void SDLscreen::DrawLine(int x0, int y0, int x1, int y1, Uint8 R, Uint8 G, Uint8 B){
  lineRGBA(screen, x0, y0, x1, y1, R, G, B, 255);
}

void SDLscreen::DrawRect(int x0, int y0, int x1, int y1, Uint8 R, Uint8 G, Uint8 B){
  rectangleRGBA(screen, x0, y0, x1, y1, 255, 0, 0, 255);
}

SDLscreen::~SDLscreen( ){
  SDL_Quit();
}

Sprites SDLscreen::GameSprites;

Sprites::~Sprites(){
}

SDL_Surface *Sprites::Load(char *file, int save_image){
  SDL_Surface *image;
  string filename = file;
  if(file == NULL){
    fprintf(stderr, "File name is NULL\n");
    return NULL;
  }
  if(SpriteMap.find(filename) == SpriteMap.end()){
    char file_path[strlen(IMAGE_BASE)  + strlen(file)];
    strcpy(file_path, IMAGE_BASE);
    strcat(file_path, file);

    image = IMG_Load(file_path); 
    if(image == NULL){
      fprintf(stderr, "Couldn't load %s: %s\n", file_path, SDL_GetError());
      return NULL;
    }
    fprintf(stderr, "Loaded image. Converting... %s\n", file);
    image = SDL_DisplayFormatAlpha(IMG_Load(file_path));
    if(save_image){
      SpriteMap[filename] = image;
      InverseSpriteMap[image] = filename;
    }
    else return image;
  }
  return SpriteMap[filename];
}

char *Sprites::GetFileName(struct sprite_info *sprite){
  if(InverseSpriteMap.find(sprite->image) == InverseSpriteMap.end()) return NULL;
  return (char *)InverseSpriteMap[sprite->image].c_str();    
}

void LockFrameRate(){
  static int oldtime = 0;
  int frame_time;
  frame_time = SDL_GetTicks() - oldtime;
  oldtime = SDL_GetTicks();
  int delay_time = (int)(1000 / FRAME_RATE - frame_time);
  fprintf(stderr, "%i, %i, %i\n", SDL_GetTicks(), frame_time, delay_time);
  if(delay_time > 0) SDL_Delay(delay_time + 10);
}
