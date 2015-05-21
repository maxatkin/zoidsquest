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



#include <string>
#include "game.h"
#include "main.h"

ErrorLog Errors;

int main(int argc, char *argv[]){
  LevelEditor Editor;
  if(Editor.Init() == Error) exit(Error);

  if(argc != 2) return
  Editor.LoadToEdit(argv[1]);

  unsigned int mx = 0, my = 0;
  while(Editor.Finished() == False){
    UserInput.Check();
    UserInput.GetMouseCoords(mx, my);
    Editor.SetMousePosition(mx, my);
    if(UserInput.IsKeyDown(SDLK_F1) == True) Screen.ToggleFullScreen();
    if(UserInput.IsKeyDown(SDLK_ESCAPE) == True){
      finished = True;
      Editor.Write("test.xml");
      continue;
    }
    if(UserInput.IsKeyHeld(controls["climb up"])   == True) Editor.MoveViewWindow(0,-10);
    if(UserInput.IsKeyHeld(controls["climb down"]) == True) Editor.MoveViewWindow(0,10);
    if(UserInput.IsKeyHeld(controls["left"])       == True) Editor.MoveViewWindow(-10,0);
    if(UserInput.IsKeyHeld(controls["right"])      == True) Editor.MoveViewWindow(10,0);
    if(UserInput.IsKeyDown('.') == True) Editor.ChangeType();
    if(UserInput.IsMouseButtonDown(0)) Editor.ClickMouse(0);
    Editor.Update();
    Editor.Draw(&Screen);
    Screen.Update();
  }
}
