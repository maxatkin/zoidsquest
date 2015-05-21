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



#include "leveleditor.h"
#include "io.h"

LevelEditor::LevelEditor(){
  xview = 300;
  yview = 0;
  reset = False;
  mx = 0;
  my = 0;
  menu_margin = 0.25;
  current_object = NULL;
}

void LevelEditor::Init(double margin){
  Menu.Create(margin, &templates);
  menu_margin = margin;
}


void LevelEditor::Update(){
  scale = ((double)tilesize)/((double)blocksize);

  Terrain.ClearCurrentBlocks();
  FindObjectsToUpdate();
  Menu.Update();
  current_object = Menu.GetCurrentObject();
  //if(current_object != NULL) fprintf(stderr, "%s\n", current_object->GetName());
  Menu.CurrentPage()->ClearExecuteFlag();

  //Set view window location and make sure it stays within the level
  double scale = ((double)blocksize)/((double)tilesize);
  if(xview < 0) xview = 0;
  if(yview < 0) yview = 0;
  if(xview > width*blocksize  - SCREEN_WIDTH*scale)  xview = (int)(width*blocksize - SCREEN_WIDTH*scale);
  if(yview > height*blocksize - SCREEN_HEIGHT*scale) yview = (int)(height*blocksize - SCREEN_HEIGHT*scale);
}

void LevelEditor::SetFile(char *editfile_){
  editfile = editfile_;
}

void LevelEditor::DrawGrid(SDLscreen *Screen){
  for(unsigned int i = 0; i < (unsigned int)(SCREEN_WIDTH + tilesize); i += tilesize)
    Screen->DrawLine((int)(i - (xview%blocksize)*scale), 0, (int)(i - (xview%blocksize)*scale), SCREEN_HEIGHT, 255, 255, 255);

  for(unsigned int i = 0; i < (unsigned int)(SCREEN_HEIGHT + tilesize); i += tilesize)
    Screen->DrawLine(0, (int)(i - (yview%blocksize)*scale), SCREEN_WIDTH, (int)(i - (yview%blocksize)*scale), 255, 255, 255);
}

void LevelEditor::Draw(SDLscreen *Screen){
  unsigned int size = levelobjects.size();
  Terrain.Draw(Screen, xview, yview, 0);
  for(unsigned int i = 0; i < size; i++){
    levelobjects[size - i - 1]->PreUpdate();    
    levelobjects[size - i - 1]->Draw(Screen, xview, yview);    
  }
  Terrain.Draw(Screen, xview, yview, 1);
  DrawGrid(Screen);

  //Draw red square around selected grid square
  //can't be a baseobject - since scale of level may not be defined yet!

  unsigned int msx, msy;
  msx = mlx - mlx%blocksize;
  msy = mly - mly%blocksize;

  msx = (int)(scale*(msx - xview));
  msy = (int)(scale*(msy - yview));

  if(mx < (unsigned int)(SCREEN_WIDTH*(1 - menu_margin))) Screen->DrawRect(msx, msy, msx + tilesize, msy + tilesize, 255, 0, 0);
  Menu.CurrentPage()->SetEntryWithMouse(mx, my);
  Menu.Draw(Screen);
}

int LevelEditor::LoadToEdit(map<string, BaseObject *> *global_templates_){
  global_templates = global_templates_;
  return Load((char *)editfile.c_str(), global_templates);
}

void LevelEditor::SetMousePosition(unsigned int mx_, unsigned int my_){
  mx = mx_;
  my = my_;
  mlx = (int)(mx_ / scale + xview);
  mly = (int)(my_ / scale + yview);
}

void LevelEditor::ClickMouse(unsigned char button){
  if(mx < (unsigned int)(SCREEN_WIDTH*(1 - menu_margin))){
    current_object = Menu.GetCurrentObject();
    if(current_object == NULL){
      Terrain.ModifyLayout(mlx/blocksize, mly/blocksize, '.');
      return;
    }
    string class_type = current_object->GetClassType();
    if(class_type == "BlockObject") Terrain.ModifyLayout(mlx/blocksize, mly/blocksize, ((BlockObject *)current_object)->GetKey());
    else{
      AddObject(current_object->GetName(), mlx - mlx%blocksize, mly - mly%blocksize, 0, 0);
    }
  }
  else{
    Menu.CurrentPage()->ExecuteEntry();
  }
}

void LevelEditor::ChangeType(){

}

int LevelEditor::Write(char *levelfile){
  File LevelFile;
  string output_str;
  char tmpbuf[32];

  string tmpfile = LEVEL_BASE + editfile;
  if(levelfile == NULL) levelfile = (char *)tmpfile.c_str();
  if(LevelFile.Open(levelfile, "w") == Error) return Error;

  LevelFile.WriteLine("<?xml version=\"1.0\"?>");
  output_str = PackageInXML("level", NULL, Open_Tag, true);
  output_str += PackageInXML("name", NULL, Open_Tag, true) + PackageInXML("name", NULL, Close_Tag, true);
  output_str += PackageInXML("name", NULL, Open_Tag, true) + PackageInXML("name", NULL, Close_Tag, true);

  snprintf(tmpbuf, 32, "%i", width);
  output_str += PackageInXML("width", NULL, Open_Tag, true) + tmpbuf + PackageInXML("width", NULL, Close_Tag, true);
  snprintf(tmpbuf, 32, "%i", height);
  output_str += PackageInXML("height", NULL, Open_Tag, true) + tmpbuf + PackageInXML("height", NULL, Close_Tag, true);
  snprintf(tmpbuf, 32, "%i", blocksize);
  output_str += PackageInXML("blocksize", NULL, Open_Tag, true) + tmpbuf + PackageInXML("blocksize", NULL, Close_Tag, true);
  snprintf(tmpbuf, 32, "%i", tilesize);
  output_str += PackageInXML("tilesize", NULL, Open_Tag, true) + tmpbuf + PackageInXML("tilesize", NULL, Close_Tag, true);
  output_str += PackageInXML("music", NULL, Open_Tag, true) + music + PackageInXML("music", NULL, Close_Tag, true);
  output_str += Terrain.Print();

  map<string, BaseObject *>::iterator i;
  for(i = templates.begin(); i != templates.end(); i++){
    if(global_templates->find((*i).first) == global_templates->end())
      output_str += (*i).second->Print(Template);
  }
  for(unsigned int j = 0; j < objectstore.size(); j++){
    output_str += objectstore[j]->Print(Level_Object);
  }

  output_str += PackageInXML("level", NULL, Close_Tag, true);
  LevelFile.WriteChunk((char *)output_str.c_str());

  return Ok;
}
