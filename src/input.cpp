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



#include "input.h"

Input::Input(){
  for(int i = 0; i < SDLK_LAST; i++){
    keystate[i].held = False;
    keystate[i].down = False;
    keystate[i].up = False;
  }
  last_key = 0;
  mouse_moved = False;
}

void Input::Check(){
  for(int i = 0; i < SDLK_LAST; i++){
    keystate[i].up = False;
    keystate[i].down = False;
  }
  last_key = 0;
  mousestate[0].up = False;
  mousestate[0].down = False;
  mouse_moved = False;

  while(SDL_PollEvent(&event)){
    switch(event.type){
      case SDL_KEYDOWN:
        Input::UpdateKeys();
        break;
      case SDL_KEYUP:
        Input::UpdateKeys();
        break;
      case SDL_MOUSEMOTION:
        Input::UpdateMouse();
        break;
      case SDL_MOUSEBUTTONDOWN:
        Input::UpdateMouse();
        break;
      case SDL_MOUSEBUTTONUP:
        Input::UpdateMouse();
        break;
      default:
        break;
    }
  }
}

void Input::UpdateKeys(){
  int c = event.key.keysym.sym;
  if(event.type == SDL_KEYDOWN){
    keystate[c].held = True;
    keystate[c].down = True;
    last_key = c;
  }
  else if(event.type == SDL_KEYUP){
    keystate[c].held = False;
    keystate[c].up = True;
  }
}

unsigned char Input::IsKeyHeld(unsigned int key){
  return keystate[key].held;
}

unsigned char Input::IsKeyDown(unsigned int key){
  return keystate[key].down;
}

unsigned char Input::IsKeyUp(unsigned int key){
  return keystate[key].up;
}

void Input::UpdateMouse(){
  switch(event.type){
  case SDL_MOUSEMOTION:
    mouse_moved = True;
    mdx = event.motion.xrel;
    mdy = event.motion.yrel;
    mx = event.motion.x;
    my = event.motion.y;
    break;
  case SDL_MOUSEBUTTONDOWN:
    mousestate[0].held = True;
    mousestate[0].down = True;
    mx = event.motion.x;
    my = event.motion.y;
    break;
  case SDL_MOUSEBUTTONUP:
    mousestate[0].held = False;
    mousestate[0].up = True;
    mx = event.motion.x;
    my = event.motion.y;
    break;
  }
}

unsigned char Input::IsMouseButtonHeld(int button){
  return mousestate[button].held;
}

unsigned char Input::IsMouseButtonDown(int button){
  return mousestate[button].down;
}

unsigned char Input::IsMouseButtonUp(int button){
  return mousestate[button].up;
}

void Input::GetMouseCoords(unsigned int& mx_, unsigned int& my_){
  mx_ = mx;
  my_ = my;
}


