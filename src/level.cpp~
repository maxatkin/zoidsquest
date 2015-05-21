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



#include <algorithm>
#include <strings.h>
#include <stdlib.h>
#include <math.h>
#include "level.h"
#include "io.h"


/*=====================================================*/

Level::Level(){
  xview = 300;
  yview = 0;
  reset = False;
}

Level::~Level(){
  vector<BaseObject *>::iterator i;
  for(i = objectstore.begin(); i != objectstore.end(); i++){
    delete *i;
  }
}

char *Level::Music(){
  if(music.size() > 0) return (char *)music.c_str();
  return NULL;
}

int Level::Load(const char *levelfile, map<string, BaseObject *> *global_templates){
  BaseObject *template_ptr;
  string class_type;
  char *level;
  char *element_data;

  char file_path[strlen(LEVEL_BASE) + strlen(levelfile)];
  strcpy(file_path, LEVEL_BASE);
  strcat(file_path, levelfile);
 
  if(LevelParser.LoadData(file_path, "level") == Error) return Error;

  if(LevelParser.FindElement("layout") == Error){
    fprintf(stderr, "Level layout not specified\n");
    return Error;
  }

  char *tmp_level = LevelParser.GetElementData();
  if(tmp_level == NULL){
    fprintf(stderr, "No level layout specified\n");
    return Error;
  }
  fprintf(stderr, "level size: %i, told %i\n", strlen(tmp_level), height*width);

  level = new char[strlen(tmp_level)];
  unsigned int j = 0;
  height = 0;
  width = 0;
  for(unsigned int i = 0; i < strlen(tmp_level); i++){
    level[j] = tmp_level[i];
    if(level[j] == '\n'){
      if(width == 0) width = j;
      height++;
    }
    else j++;
  }
  height++;

  if(width < 1){
    if(LevelParser.FindElement("width") == Error){
      fprintf(stderr, "Level width not specified\n");
      return Error;
    }
    element_data = LevelParser.GetElementData();
    if(element_data == NULL){
      fprintf(stderr, "No level width specified\n");
      return Error;
    }
    width = atoi(element_data);
  }

  if(height < 2){
    if(LevelParser.FindElement("height") == Error){
      fprintf(stderr, "Level height not specified\n");
      return Error;
    }
    element_data = LevelParser.GetElementData();
    if(element_data == NULL){
      fprintf(stderr, "No level hieght specified\n");
      return Error;
    }
    height = atoi(element_data);
  }

  level[height*width] = 0x00;
  fprintf(stderr, "%i, %i\n", height, width);

  char *background = NULL;
  if(LevelParser.FindElement("background") == Error){
    fprintf(stderr, "Warning: Background image not specified\n");
    background = NULL;
  }
  element_data = LevelParser.GetElementData();
  if(element_data == NULL){
    fprintf(stderr, "Warning: background image not specified\n");
    return Error;
  }
  else background = element_data;

  if(LevelParser.FindElement("music") == Error){
    fprintf(stderr, "Warning: Music not specified\n");
    music = "";
  }
  element_data = LevelParser.GetElementData();
  if(element_data == NULL){
    fprintf(stderr, "Warning: Music not specified\n");
  }
  else music = LevelParser.GetElementData();

  fprintf(stderr, "Music loaded: %s\n", music.c_str());
  Terrain.SetProperties(level, background, width, height, blocksize, tilesize);

  if(global_templates != NULL){
    map<string, BaseObject *>::iterator global_object;
    for(global_object = global_templates->begin(); global_object != global_templates->end(); global_object++){
      templates[(*global_object).first] = (*global_object).second;
    }
  }

  while(LevelParser.FindElement("template") == Ok){
    element_data = LevelParser.GetElementAttribute("class");
    if(element_data == NULL) continue;
    class_type = element_data;

    if(class_type == "MovingGroundObject") template_ptr = new MovingGroundObject;
    else if(class_type == "EnemyGroundObject") template_ptr = new EnemyGroundObject;
    else if(class_type == "BulletObject") template_ptr = new BulletObject;
    else if(class_type == "ThrowerObject") template_ptr = new ThrowerObject;
    else if(class_type == "MovingFlyingObject") template_ptr = new MovingFlyingObject;
    else if(class_type == "FallingBlock") template_ptr = new FallingBlock;
    else if(class_type == "BlockObject") template_ptr = new BlockObject;
    else if(class_type == "RuneObject") template_ptr = new RuneObject;
    else if(class_type == "ManaJar") template_ptr = new ManaJar;
    else if(class_type == "Player") template_ptr = new PlayerObject;
    else{
      fprintf(stderr, "None valid class specified for object: %s\n", class_type.c_str());
      return Error;
    }
    template_ptr->SetClassType(class_type.c_str());

    LevelParser.EnterElement();
    if(LevelParser.FindElement("name") == Error){
      fprintf(stderr, "No name for template specified\n");
      return Error;
    }
    element_data = LevelParser.GetElementData();
    if(element_data == NULL){
      fprintf(stderr, "No name for template specified\n");
      return Error;
    }
    string name = element_data;
    templates[name] = template_ptr;
    template_ptr->SetName(element_data);    struct sprite_info *GetBackground();

    if(LevelParser.FindElement("boundary") == Error){
      fprintf(stderr, "No boundary template specified\n");
      return Error;
    }
    double x0 = 0, y0 = 0, x1 = 0, y1 = 0;

    if(LevelParser.GetElementAttribute("x0") != NULL) x0 = atof(LevelParser.GetElementAttribute("x0"));
    if(LevelParser.GetElementAttribute("y0") != NULL) y0 = atof(LevelParser.GetElementAttribute("y0"));
    if(LevelParser.GetElementAttribute("x1") != NULL) x1 = atof(LevelParser.GetElementAttribute("x1"));
    if(LevelParser.GetElementAttribute("y1") != NULL) y1 = atof(LevelParser.GetElementAttribute("y1"));
    template_ptr->SetBoundary(x0, y0, x1, y1);

    while(LevelParser.FindElement("property") == Ok){
      element_data = LevelParser.GetElementData();
      if(element_data == NULL){
        fprintf(stderr, "Warning: <property> tag empty\n");
        continue;
      }
      template_ptr->Properties.SetProperty(element_data);
    }

    while(LevelParser.FindElement("sound") == Ok){
      char *sound_name = LevelParser.GetElementAttribute("action");
      if(sound_name == NULL){
        fprintf(stderr, "Warning: no sound file specified in <sound> tag\n");
        continue;
      }
      element_data = LevelParser.GetElementData();
      if(element_data == NULL){
        fprintf(stderr, "Warning: no sound file specified in <sound> tag\n");
        continue;
      }
      fprintf(stderr, "Adding sound %s\n", sound_name);
      template_ptr->SoundEffects.AddSample(sound_name, element_data);
    }

    while(LevelParser.FindElement("animation") == Ok){
      char *animation_name = LevelParser.GetElementAttribute("name");
      if(animation_name == NULL) continue;
      template_ptr->SpriteAnimation.Create(animation_name);
      LevelParser.EnterElement();
      while(LevelParser.FindElement("frame") == Ok){
        char *repeat_number = LevelParser.GetElementAttribute("repeat");
        int repeat = 1;
        if(repeat_number != NULL) repeat = atoi(repeat_number);
        element_data = LevelParser.GetElementData();
        if(element_data == NULL){
          fprintf(stderr, "Warning: no image specified in <frame> tag\n");
          continue;
        }
        if(template_ptr->SpriteAnimation.AddImageTo(animation_name, element_data, repeat) == Error) return Error;
      }
      LevelParser.ExitElement();
    }

    while(LevelParser.FindElement("magic") == Ok){
      LevelParser.EnterElement();
      while(LevelParser.FindElement("rune") == Ok){
        char *rune_name = LevelParser.GetElementData();
        if(templates.find(rune_name) == templates.end()){
          fprintf(stderr, "Rune '%s' not defined\n", rune_name);
          continue;
        }
        ((MagicObject *)template_ptr)->Magic.AddRune(rune_name, (RuneObject *)templates[rune_name]);
      }
      LevelParser.ExitElement();
    }

    if(class_type == "BlockObject"){
      if(LevelParser.FindElement("blockkey") == Ok){
        element_data = LevelParser.GetElementData();
        if(element_data != NULL){
          char key = LevelParser.GetElementData()[0];
          ((BlockObject *)template_ptr)->SetKey(key);
        }
        else fprintf(stderr, "Warning: no key for block specified\n");
      }
      if(LevelParser.FindElement("layer") == Ok){
        char *layer = LevelParser.GetElementData();
        if(layer != NULL) ((BlockObject *)template_ptr)->SetLayer(atoi(layer));
        fprintf(stderr, "LAYER: %i\n", atoi(layer));
      }
      Terrain.AddBlock((BlockObject *)template_ptr);      
    }
    LevelParser.ExitElement();
  }

  LevelParser.ResetCursor();
  Player = NULL;
  vector<BaseObject *> follow_list;
  while(LevelParser.FindElement("levelobject") == Ok){
    BaseObject *base_ptr;
    string object_type;

    element_data = LevelParser.GetElementAttribute("type");
    if(element_data == NULL){
      fprintf(stderr, "Error: No type specified in object\n");
      return Error;
    }
    object_type = element_data;

    if(templates.find(object_type) == templates.end()){
      fprintf(stderr, "Object type %s doesn't not exist\n", object_type.c_str()); 
      return Error;
    }
    class_type = templates[object_type]->GetClassType();

    double x = 0, y = 0, vx = 0, vy = 0;
    if(LevelParser.GetElementAttribute("x") != NULL) x = atof(LevelParser.GetElementAttribute("x"));
    if(LevelParser.GetElementAttribute("y") != NULL) y = atof(LevelParser.GetElementAttribute("y"));
    if(LevelParser.GetElementAttribute("vx") != NULL) vx = atof(LevelParser.GetElementAttribute("vx"));
    if(LevelParser.GetElementAttribute("vy") != NULL) vy = atof(LevelParser.GetElementAttribute("vy"));

    if(class_type == "MovingGroundObject") base_ptr = new MovingGroundObject;
    else if(class_type == "EnemyGroundObject") base_ptr = new EnemyGroundObject;
    else if(class_type == "ThrowerObject") base_ptr = new ThrowerObject;
    else if(class_type == "MovingFlyingObject") base_ptr = new MovingFlyingObject;
    else if(class_type == "FallingBlock") base_ptr = new FallingBlock;
    else if(class_type == "BlockObject") base_ptr = new BlockObject;
    else if(class_type == "ManaJar") base_ptr = new ManaJar;
    else if(class_type == "Player"){
      base_ptr = new PlayerObject;
      Player = (PlayerObject *)base_ptr;
    }

    if(class_type == "ThrowerObject") follow_list.push_back(base_ptr);
    if(class_type == "MovingGroundObject" || class_type == "MovingFlyingObject" || class_type == "EnemyGroundObject"){
      if(LevelParser.EnterElement() == Ok){
        if(LevelParser.FindElement("route") == Ok){
          LevelParser.EnterElement();
          while(LevelParser.FindElement("point") == Ok){ 
            double speed = 0, direction = 0;
            unsigned int ticks = 0;
            if(LevelParser.GetElementAttribute("speed") != NULL) speed = atof(LevelParser.GetElementAttribute("speed"));
            if(LevelParser.GetElementAttribute("direction") != NULL) direction = atof(LevelParser.GetElementAttribute("direction"));
            if(LevelParser.GetElementAttribute("ticks") != NULL) ticks = atoi(LevelParser.GetElementAttribute("ticks"));
            ((MovingObject *)base_ptr)->AddRoutePoint(speed, direction, ticks);
          }
          LevelParser.ExitElement();
        }
        LevelParser.ExitElement();
      }
    }
    base_ptr->UseTemplate(templates[object_type]);
    base_ptr->SetPosition(x, y);
    base_ptr->SetVelocity(vx, vy);
    base_ptr->SaveState();
    base_ptr->SpriteAnimation.ChangeTo("default");
    base_ptr->SetBoundaryToMode("default");
    objectstore.push_back(base_ptr);
  }
  if(Player == NULL){
    fprintf(stderr, "Error: no player object in level\n");
    return Error;
  }
  for(unsigned int k = 0; k < follow_list.size(); k++){
    ((ThrowerObject *)follow_list[k])->FollowObject(Player);
  }
  return Ok;
}

void Level::Restart(){
  unsigned int i;
  for(i = 0; i < objectstore.size(); i++){
    objectstore[i]->Reset();
  }
  Terrain.Reset();
  reset = True;
}

int Level::Reset(){
  if(reset){
    reset = False;
    return True;
  }
  return False;
}

void Level::SetViewWindow(int vx, int vy){
  xview = vx;
  yview = vy;
}

void Level::MoveViewWindow(int dvx, int dvy){
  xview += dvx;
  yview += dvy;
}

void Level::Draw(SDLscreen *Screen){
  unsigned int size = levelobjects.size();
  Terrain.Draw(Screen, xview, yview, 0);
  for(unsigned int i = 0; i < size; i++){
    levelobjects[size - i - 1]->Draw(Screen, xview, yview);    
  }
  Terrain.Draw(Screen, xview, yview, 1);
}

int Level::AddObject(char *cobject, double cx, double cy, double cvx, double cvy){
  BaseObject *template_ptr, *current_object;
  if(cobject == NULL) return Error;
  string str_object = cobject;
  string class_type;
  if(templates.find(str_object) == templates.end()) return Error;
  current_object = templates[str_object];
  class_type = current_object->GetClassType();
  
  if(class_type == "MovingGroundObject") template_ptr = new MovingGroundObject;
  else if(class_type == "ThrowerObject") template_ptr = new ThrowerObject;
  else if(class_type == "BulletObject") template_ptr = new BulletObject;
  else if(class_type == "EnemyGroundObject") template_ptr = new EnemyGroundObject;
  else if(class_type == "MovingFlyingObject") template_ptr = new MovingFlyingObject;
  else if(class_type == "FallingBlock") template_ptr = new FallingBlock;
  else if(class_type == "BlockObject") template_ptr = new BlockObject;
  else if(class_type == "RuneObject") template_ptr = new RuneObject;
  else if(class_type == "ManaJar") template_ptr = new ManaJar;
  else if(class_type == "Player") template_ptr = new PlayerObject;
  else return Error;
  template_ptr->UseTemplate(current_object);
  template_ptr->SetPosition(cx, cy);
  template_ptr->SetVelocity(cvx, cvy);
  template_ptr->SaveState();
  template_ptr->SpriteAnimation.ChangeTo("default");
  objectstore.push_back((BaseObject *)template_ptr);
  return Ok;
}

void Level::Update(){
  double px, py;
  double cx, cy, cvx, cvy;
  string cobject;

  Terrain.ClearCurrentBlocks();
  Terrain.Update();
  FindObjectsToUpdate();

  for(int j = 0; j < Detect_Div; j++){
    for(unsigned int i = 0; i < levelobjects.size(); i++){
      if(j == 0){
        levelobjects[i]->PreUpdate();
        //Make things that are "heavy" fall
        if(levelobjects[i]->Properties.HasProperty("heavy")) levelobjects[i]->ChangeVelocity(0, 1);
        if(levelobjects[i]->ObjectRequested()){
          levelobjects[i]->GetRequestedObjectData(cobject, cx, cy, cvx, cvy);
          AddObject((char *)cobject.c_str(), cx, cy, cvx, cvy);
        }
      }

  //  if(levelobjects[i] == Player) printf("player\n");
  //  else printf("object %i\n", i);
      updateobjects.clear();
      FindCollisions(levelobjects[i]); //need this function to move all objects object [i] collides with
  //    printf("********************\n");
      levelobjects[i]->FullUpdate();
      for(unsigned int k = 0; k < updateobjects.size(); k++) updateobjects[k]->Update();
      if(j == Detect_Div - 1) levelobjects[i]->PostUpdate();

      levelobjects[i]->GetPosition(&px, &py);
      if(px < 0) levelobjects[i]->SetPosition(0, py);
      else if(px > width * blocksize) levelobjects[i]->SetPosition(width * blocksize, py);
      if(py > height * blocksize + 150) levelobjects[i]->Kill();

      if(levelobjects[i]->Properties.HasProperty("dead")){
        if(levelobjects[i] != Player) RemoveObject(i);
        else{
          fprintf(stderr, "Lives: %i\n", Player->Lives());
          Restart();
        }
      }
    }
  }

  //Set view window location and make sure it stays within the level
  if(Player->Properties.HasProperty("dying") == False){
    Player->GetPosition(&px, &py);
    xview = (int)(px - SCREEN_WIDTH/2 * scale);
    yview = (int)(py - 0.6 * SCREEN_HEIGHT*scale);
    if(xview < 0) xview = 0;
    if(yview < 0) yview = 0;
    if(xview > width*blocksize  - SCREEN_WIDTH*scale)  xview = (int)(width*blocksize - SCREEN_WIDTH*scale);
    if(yview > height*blocksize - SCREEN_HEIGHT*scale) yview = (int)(height*blocksize - SCREEN_HEIGHT*scale);
  }

}


unsigned int Level::FindCollisions(BaseObject *objectToMove, unsigned int inherited_mass, bool usemode, int mode){
  double dx, dy;
  double ovx, ovy;
  double tvx, tvy;
  bool found_stop_collision = false;
  unsigned int effective_mass, x_mass = 0, y_mass = 0;
  if(inherited_mass == 0) inherited_mass = objectToMove->Mass();
  objectToMove->GetVelocity(&ovx, &ovy);
  dx = ovx / Detect_Div;
  dy = ovy / Detect_Div;
  vector<BaseObject *> objects_collided_with;

  if(usemode && mode == Check_Horizontal) effective_mass = inherited_mass;
  else effective_mass = objectToMove->Mass();

  if(fabs(dx) > 0.0001){
    objectToMove->ChangePosition(dx, 0);
    objectToMove->SaveCollisionQueueSize();
    Terrain.CheckObjectSpace(objectToMove, Check_Horizontal);
    FindObjectCollisions(objectToMove, Check_Horizontal);
    objectToMove->GetLastCollisions(&objects_collided_with, true, Check_Horizontal);

    for(unsigned int i = 0; i < objects_collided_with.size(); i++){
      updateobjects.push_back(objects_collided_with[i]);
      if(!objects_collided_with[i]->Properties.HasProperty("solid") || !objectToMove->Properties.HasProperty("solid")) continue;
      if(effective_mass <= objects_collided_with[i]->Mass()){
        if(x_mass < objects_collided_with[i]->Mass()) x_mass = objects_collided_with[i]->Mass();
        found_stop_collision = true;
      }
    }

    if(found_stop_collision){
      objectToMove->SetVelocity(0, ovy);
      objectToMove->ChangePosition(-dx, 0);
    }

    else{
      for(unsigned int i = 0; i < objects_collided_with.size(); i++){
        if(!objects_collided_with[i]->Properties.HasProperty("solid") || !objectToMove->Properties.HasProperty("solid")) continue;
/*
        for(unsigned int k = 0; k < 4; k++){
          if(levelobjects[k] == objects_collided_with[i]) printf("x recurse %i\n", k);
        }
*/
        objects_collided_with[i]->GetVelocity(&tvx, &tvy);
        objects_collided_with[i]->SetVelocity(ovx, tvy);
        x_mass = FindCollisions(objects_collided_with[i], effective_mass, true, Check_Horizontal);
        if(effective_mass <= x_mass) found_stop_collision = true;
      }
      if(found_stop_collision){
        objectToMove->SetVelocity(0, ovy);
        objectToMove->ChangePosition(-dx, 0);
      }
    }
  }

  objects_collided_with.clear();
  objectToMove->GetVelocity(&ovx, &ovy);
  found_stop_collision = false;
  int checkblock = False;

  if(usemode && mode == Check_Vertical) effective_mass = inherited_mass;
  else effective_mass = objectToMove->Mass();

  if(fabs(dy) > 0.0001){
    objectToMove->ChangePosition(0, dy);
    objectToMove->SaveCollisionQueueSize();
//    printf("dy %f\n", dy);
    checkblock = Terrain.CheckObjectSpace(objectToMove, Check_Vertical);
    FindObjectCollisions(objectToMove, Check_Vertical);
    objectToMove->GetLastCollisions(&objects_collided_with, true, Check_Vertical);

    for(unsigned int i = 0; i < objects_collided_with.size(); i++){
      updateobjects.push_back(objects_collided_with[i]);
/*
      for(unsigned int k = 0; k < 4; k++){
          if(levelobjects[k] == objects_collided_with[i]) printf("collided with %i\n", k);
        }
*/
      if(!objects_collided_with[i]->Properties.HasProperty("solid") || !objectToMove->Properties.HasProperty("solid")) continue;
      if(effective_mass <= objects_collided_with[i]->Mass()){
        if(y_mass < objects_collided_with[i]->Mass()) y_mass = objects_collided_with[i]->Mass();
        found_stop_collision = true;
      }
    }

    if(found_stop_collision){
  //    if(checkblock) printf("moved back\n");
      objectToMove->SetVelocity(ovx, 0);
      objectToMove->ChangePosition(0, -dy);
    }

    else{
      for(unsigned int i = 0; i < objects_collided_with.size(); i++){
        if(!objects_collided_with[i]->Properties.HasProperty("solid") || !objectToMove->Properties.HasProperty("solid")) continue;
/*
        for(unsigned int k = 0; k < 4; k++){
          if(levelobjects[k] == objects_collided_with[i]) printf("recurse %i\n", k);
        }
*/
        objects_collided_with[i]->GetVelocity(&tvx, &tvy);
        objects_collided_with[i]->SetVelocity(tvx, ovy);
        y_mass = FindCollisions(objects_collided_with[i], effective_mass, true, Check_Vertical);
        if(effective_mass <= y_mass) found_stop_collision = true;
      }
      if(found_stop_collision){
  //      if(checkblock) printf("moved back2\n");
        objectToMove->SetVelocity(ovx, 0);
        objectToMove->ChangePosition(0, -dy);
      }
/*
      else if(checkblock){
        printf("didn't moved back\n");
        sleep(1);
      }
*/
    }
  }

  if(objectToMove->Properties.HasProperty("magic")){
    vector<RuneObject *> runes = ((MagicObject *)objectToMove)->Magic.GetUsedRunes();
    for(unsigned int i = 0; i < runes.size(); i++){
      FindObjectCollisions(runes[i]);
    }
  }
  //printf("l_________________________\n");
  if(usemode && mode == Check_Horizontal) return x_mass;
  if(usemode && mode == Check_Vertical) return y_mass;
  //printf("j_________________________\n");
  return effective_mass;
}

int Level::FindObjectCollisions(BaseObject *object_to_check, int mode){
  unsigned int j;
  int hit_something = False;
  for(unsigned int i = 0; i < levelobjects.size(); i++){
    if(levelobjects[i] == object_to_check) continue;
    if(CheckObjectCollision(object_to_check, levelobjects[i], mode) && levelobjects[i]->Properties.HasProperty("solid")){
      if(mode == Check_Horizontal) hit_something = True;
    }
  }
  return hit_something;
}

void Level::RemoveObject(int offset){
  vector<BaseObject *>::iterator i = levelobjects.begin();
  levelobjects.erase(i + offset);
}

void Level::FindObjectsToUpdate(){
  double px, py;
  int object_is_on_screen = False;

  //Update_boundary
  double upb_x0 = (double)xview - 1500;
  double upb_y0 = (double)yview - 1500;
  double upb_x1 = (double)xview + 1500;
  double upb_y1 = (double)yview + 1500;

  vector<BaseObject *>::iterator object_check;
  vector<BaseObject *>::iterator stored_object;
  vector<BaseObject *>::iterator start = objectstore.begin();
  vector<BaseObject *>::iterator end   = objectstore.end();

  for(stored_object = start; stored_object != end; stored_object++){
    (*stored_object)->GetPosition(&px, &py);

    if(px > upb_x0 && px < upb_x1 && py > upb_y0 && py < upb_y1) object_is_on_screen = True;
    object_check = find(levelobjects.begin(), levelobjects.end(), *stored_object);
   
    if(object_check == levelobjects.end()){
      if(object_is_on_screen && !(*stored_object)->Properties.HasProperty("dead")) levelobjects.push_back(*stored_object);
      else if((*stored_object)->Properties.HasProperty("moving block") && !(*stored_object)->Properties.HasProperty("dead"))
        levelobjects.push_back(*stored_object);
    }
    else if(object_check != levelobjects.end() && !object_is_on_screen && !(*stored_object)->Properties.HasProperty("moving block")){
      objectstore.erase(stored_object);
      objectstore.push_back(*object_check);
      levelobjects.erase(object_check);
    }
    object_is_on_screen = False;
  }
}

void Level::SetScale(int blocksize_, int tilesize_){
  blocksize = blocksize_;
  tilesize = tilesize_;
  scale = ((double)blocksize)/((double)tilesize);
}

int Level::blocksize;
int Level::tilesize;
double Level::scale;

//__________________________________________________________

Land::Land(){
  level = NULL;
}

Land::~Land(){
  map<char, BlockObject *>::iterator i;
  for(i = Blockkey.begin(); i != Blockkey.end(); i++){
    delete (*i).second;
  }
  ClearCurrentBlocks();
  if(level != NULL) delete[] level;
}

void Land::AddBlock(BlockObject *template_block){
  BlockObject *NewBlock = new BlockObject;
  NewBlock->UseTemplate(template_block);
  NewBlock->SpriteAnimation.ChangeTo("default");
  Blockkey[NewBlock->GetKey()] = NewBlock;
}

void Land::AddBlock2(char *name, char c){
  BlockObject *NewBlock = new BlockObject;
  string newname = name;

  NewBlock->SetName(name);
  Block[newname] = NewBlock;
  Blockkey[c] = NewBlock;
}

void Land::SetProperties(char *levelscape, char *bg, int w, int h, int bs, int ts){
  level = levelscape;
  width = w;
  height = h;
  blocksize = bs;
  tilesize = ts;
  background_offset = 0;
  background.image = SDLscreen::GameSprites.Load(bg);
  scale = ((double)blocksize)/((double)tilesize); 
}

void Land::Reset(){
  background_offset = 0;
}

string Land::Print(){
  string output_str = "";
  char *filename = SDLscreen::GameSprites.GetFileName(&background);
  if(filename != NULL){
    output_str = PackageInXML("background", NULL, Open_Tag, false) + filename
                 + PackageInXML("background", NULL, Close_Tag, false);
  }
  output_str += PackageInXML("layout", NULL, Open_Tag, false);
  char tmpchar[2];
  tmpchar[1] = 0x00;
  for(unsigned int j = 0; j < (unsigned int)height; j++){
    for(unsigned int i = 0; i < (unsigned int)width; i++){
      tmpchar[0] = level[j*width + i];
      output_str += tmpchar;
    }
    output_str += '\n';
  }
  output_str += PackageInXML("layout", NULL, Close_Tag, false);
  
  return output_str;
}

int Land::CheckObjectSpace(BaseObject *object, int mode){
  BlockObject *newblock;
  double x, y;
  unsigned int i;
  int bx, by, btype;
  double x0, x1, y0, y1;
  int collided_with_solid = False;
  int add_block = True;

  object->GetPosition(&x, &y);
  object->GetBoundary(&x0, &y0, &x1, &y1);

  int bxs = (int)((x + x0)/blocksize);
  int bxf = (int)((x + x1)/blocksize);
  int bys = (int)((y + y0)/blocksize);
  int byf = (int)((y + y1)/blocksize);
  
  for(by = byf; by >= bys; by--){
    for(bx = bxs; bx <= bxf; bx++){
      if(by*width + bx > width*height - 1) continue;
      btype = level[by*width + bx];
      if(Blockkey.find(btype) == Blockkey.end()) continue;      

      //Check whether we have already made a BlockObject for this position
      for(i = 0; i < current_blocks.size(); i++){
        
        if(current_blocks[i]->IsAtMapPosition(bx, by)){
          add_block = False;
          newblock = current_blocks[i];
          break;
        }
      }

      //If no BlockObject exists, make one

      if(add_block){
        double x0, y0, x1, y1;
        newblock = new BlockObject;
        Blockkey[btype]->GetBoundary(&x0, &y0, &x1, &y1);
        newblock->SetBoundary(x0, y0, x1, y1);
        newblock->SetPosition((double)bx*blocksize, (double)by*blocksize);
        newblock->SetMapPosition(bx, by);
        newblock->Properties.CopyProperties(&Blockkey[btype]->Properties);
        current_blocks.push_back(newblock);
        //printf("NEWBLOCK: %i, %p\n", newblock->Properties.Number(), newblock);
      }

      if(CheckObjectCollision(object, newblock, mode) && newblock->Properties.HasProperty("solid")){
        collided_with_solid = True;
  //      printf("block collision\n");
      }
      add_block = True;
    }
  }
  return collided_with_solid;
}

void Land::Update(){
  map<char, BlockObject *>::iterator i;
  for(i = Blockkey.begin(); i != Blockkey.end(); i++){
    (*i).second->PreUpdate();
  }
}

void Land::ModifyLayout(unsigned int x, unsigned int y, char c){
  if(y*width + x > (unsigned int)width*height - 1) return;
  level[y*width + x] = c;
}

void Land::ClearCurrentBlocks(){
  for(unsigned i = 0; i < current_blocks.size(); i++){
    delete current_blocks[i];
  }
  current_blocks.clear();
}

void Land::Draw(SDLscreen *Screen, int xv, int yv, unsigned int layer){
    int bgx, bx, by, btype, bxv, byv;

    if(blocksize < 1) return;
    bxv = xv/blocksize;
    byv = yv/blocksize;

    if(background.image != NULL && layer == 0){
      background_offset = (int)(xv / (2*background.image->w)) * background.image->w;
      for(bgx = 0; bgx < SCREEN_WIDTH + 1; bgx = bgx + background.image->w){
        Screen->DrawObject(&background, (bgx - xv/2) + background_offset, 0);
      }
    }
    for(by = byv; by < byv + SCREEN_HEIGHT/tilesize + 2; by++){
      for(bx = bxv; bx < bxv + SCREEN_WIDTH/tilesize + 2; bx++){
        if(by*width + bx > width*height - 1) continue;
        btype = level[by*width + bx];
        if(Blockkey.find(btype) == Blockkey.end()) continue;
        if(Blockkey[btype]->GetLayer() != layer) continue; 
        Blockkey[btype]->SetPosition(bx*blocksize, by*blocksize);
        Blockkey[btype]->Draw(Screen, xv, yv);
      }
    }
}
