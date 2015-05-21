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
  GameWrapper Game;
  if(Game.Init() == Error) exit(Error);

  bool start_editor = false;
  if(argc == 3){
    string strargv = argv[1];
    if(strargv == "--lvledit") start_editor = true;
  }
  int retval;
  if(start_editor){
    Game.Editor.SetFile(argv[2]);
    retval = Game.SetMode("editor");
  }
  else retval = Game.SetMode("intro");
  if(retval == Error) return Error;

  while(Game.Finished() == False){
    if(Game.Mode() == GAME) retval = Game.Update();
    else if(Game.Mode() == EDITOR) retval = Game.UpdateEditor();
    else retval = Game.UpdateMenu();
    if(retval == Error) break;
  }
}

