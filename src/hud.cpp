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



#include "hud.h"
#include "SDLdraw.h"

void HUDObject::Draw(SDLscreen *Screen, int x, int y){
  Screen->DrawObject(SpriteAnimation.GetCurrentFrame(), (int)x, (int)y);
}

PowerBar::PowerBar(){
  percentage = 0;
}

void PowerBar::Draw(SDLscreen *Screen, int x, int y){
  struct sprite_info *bar = SpriteAnimation.GetCurrentFrame();
  if(bar == NULL) return;
  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.h = bar->image->h;
  src.w = (int)(bar->image->w * percentage);
  Screen->DrawObject(bar, (int)x, (int)y, &src);
}

void PowerBar::SetPercentage(double new_percent){
  percentage = new_percent;
}

void HUD::Draw(SDLscreen *Screen){
  Text::Display(Screen);
  ManaBar.Draw(Screen, 70, 10); 
}

int HUD::SetPowerBar(PowerBar *setpowerbar){
  if(setpowerbar == NULL) return Error;
  ManaBar.UseTemplate(setpowerbar);
  ManaBar.SpriteAnimation.ChangeTo("default");
  return Ok;
}
