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
#include <math.h>
#include "main.h"
#include "objects.h"
#include "parser.h"


/*==============================================*/

BoundaryMode::~BoundaryMode(){
  map<string, struct boundary *>::iterator i;
  for(i = boundary_modes.begin(); i != boundary_modes.end(); i++){
    delete (*i).second;
  }
}

void BoundaryMode::Add(char *modename, double gx0_, double gy0_, double gx1_, double gy1_){
  string strname = modename; 
  struct boundary *newmode = new struct boundary;
  newmode->gx0 = gx0_;
  newmode->gy0 = gy0_;
  newmode->gx1 = gx1_;
  newmode->gy1 = gy1_;
  boundary_modes[strname] = newmode;
}

void BoundaryMode::Use(char *modename, double *retx0, double *rety0, double *retx1, double *rety1){
  string strname = modename;
  if(boundary_modes.find(strname) == boundary_modes.end()) return;
  struct boundary *retmode = boundary_modes[strname];
  *retx0 = retmode->gx0;
  *rety0 = retmode->gy0;
  *retx1 = retmode->gx1;
  *rety1 = retmode->gy1;
}

void BoundaryMode::GetMapEntry(unsigned int k, string *modename, struct boundary *retbnd){
  map<string, struct boundary *>::iterator i;
  unsigned int j = 0;
  for(i = boundary_modes.begin(); i != boundary_modes.end(); i++){
    if(j == k){
      *modename = (*i).first;
      retbnd->gx0 = (*i).second->gx0;
      retbnd->gy0 = (*i).second->gy0;
      retbnd->gx1 = (*i).second->gx1;
      retbnd->gy1 = (*i).second->gy1;
      return;
    }
    else j++;
  }
}

void BoundaryMode::CopyBoundaries(BoundaryMode *CopyThis){
  string modename;
  struct boundary *newmode;
  for(unsigned k = 0; k < CopyThis->Number(); k++){
    newmode = new struct boundary; 
    CopyThis->GetMapEntry(k, &modename, newmode);
    boundary_modes[modename] = newmode;
  }
}

/*==============================================*/

unsigned int PropertyContainer::Number(){
  return Properties.size();
}

void PropertyContainer::SetProperty(char *property){
  string new_property = property;
  if(!HasProperty(property)) Properties.push_back(new_property);
}

void PropertyContainer::DeleteProperty(char *property){
  vector<string>::iterator i;
  string new_property = property;
  i = find(Properties.begin(), Properties.end(), new_property);
  if(i == Properties.end()) return;
  Properties.erase(i);
}

int PropertyContainer::HasProperty(char *property){ 
  string new_property = property;
  if(find(Properties.begin(), Properties.end(), new_property) == Properties.end()) return False;
  return True;
}

char *PropertyContainer::operator[](const int i){
  return (char *)Properties[i].c_str();
}

string PropertyContainer::Print(){
  string retstr = "";
  for(unsigned int i = 0; i < Properties.size(); i++){
    retstr = retstr + PackageInXML("property", NULL, Open_Tag, false) +
             Properties[i] + PackageInXML("property", NULL, Close_Tag, true);
  }
  return retstr;
}

void PropertyContainer::CopyProperties(PropertyContainer *CopyThis){
  string new_property;
  unsigned int i;
  for(i = 0; i < CopyThis->Number(); i++){
    new_property = (*CopyThis)[i];
    Properties.push_back(new_property);
  }
}

void PropertyContainer::Clear(){
  Properties.clear();
}

/*==============================================*/

BaseObject::BaseObject(){
  x  = 0;
  y  = 0;
  vx = 0;
  vy = 0;
  gx0 = 0;
  gy0 = 0;
  gx1 = 0;
  gy1 = 0;
  mass = 10;
  request_object = false;
}

BaseObject::~BaseObject(){
  ClearCollisionQueue();
  map<string, BoundaryMode *>::iterator i;
}

void BaseObject::SetName(char *setname){
  name = setname;
}

void BaseObject::SetBoundary(double x0, double y0, double x1, double y1){
  gx0 = x0;
  gy0 = y0;
  gx1 = x1;
  gy1 = y1;
}


void BaseObject::Draw(SDLscreen *Screen, int xv, int yv){
  Screen->DrawObject(SpriteAnimation.GetCurrentFrame(), (int)(inverse_scale*((int)x - xv)), (int)(inverse_scale*((int)y - yv)));
}

void BaseObject::SetPosition(double newx, double newy){
  x = newx;
  y = newy;
}

void BaseObject::SetVelocity(double newvx, double newvy){
  vx = newvx;
  vy = newvy;
}

void BaseObject::SetCollisionVelocity(double newdx, double newdy){
 // dx = newdx;
 // dy = newdy;
}

void BaseObject::ChangePosition(double dx, double dy){
  x += dx;
  y += dy;
}

void BaseObject::ChangeVelocity(double dvx, double dvy){
  vx += dvx;
  vy += dvy;
}

void BaseObject::SaveOldPosition(){
  //oldx = x;
 // oldy = y;
}

void BaseObject::RestoreOldPosition(){
 // x = oldx;
 // y = oldy;
}

void BaseObject::GetOldPosition(double *ox, double *oy){
 // *ox = oldx;
  //*oy = oldy;
}

void BaseObject::Update(){

}

void BaseObject::FullUpdate(){
  Update();
}

void BaseObject::GetVelocity(double *retvx, double *retvy){
  *retvx = vx;
  *retvy = vy;
}

void BaseObject::GetCollisionVelocity(double *retdx, double *retdy){
  // *retdx = dx;
//   *retdy = dy;
}

void BaseObject::GetPosition(double *retx, double *rety){
  *retx = x;
  *rety = y;
}

int BaseObject::GetState(){
  return state;
}

void BaseObject::GetSize(int *w, int *h){
  *w = width;
  *h = height;
}

void BaseObject::GetBoundary(double *retx0, double *rety0, double *retx1, double *rety1){
  *retx0 = gx0;
  *rety0 = gy0;
  *retx1 = gx1;
  *rety1 = gy1;
}

void BaseObject::GetCollisionQueue(vector<BaseObject *> *retQueue, bool filter, int side){
  int orientation = 2;
  for(unsigned int i = 0; i < collision_queue.size(); i++){
    if(collision_queue[i]->side == LeftSide || collision_queue[i]->side == RightSide) orientation = Check_Horizontal;
    else if(collision_queue[i]->side == TopSide || collision_queue[i]->side == BottomSide) orientation = Check_Vertical;
    if(filter == false || side == orientation) retQueue->push_back(collision_queue[i]->Object);
  }
}

void BaseObject::GetLastCollisions(vector<BaseObject *> *retQueue, bool filter, int side){
  int orientation = 2;
  for(unsigned int i = collision_queue_size; i < collision_queue.size(); i++){
    if(collision_queue[i]->side == LeftSide || collision_queue[i]->side == RightSide) orientation = Check_Horizontal;
    else if(collision_queue[i]->side == TopSide || collision_queue[i]->side == BottomSide) orientation = Check_Vertical;
    if(filter == false || side == orientation) retQueue->push_back(collision_queue[i]->Object);
   // if(side == Check_Horizontal) printf("size: %i\n", retQueue->size());
  }
}

int BaseObject::IsPointInMe(double fx, double fy){
  if((fx >= x+gx0)&&(fy >= y+gy0)&&(fx <= x + gx1)&&(fy <= y + gy1)) return True;
  return False;
}

void BaseObject::SetCollisionInfo(double cx, double cy, char side, char was_i_moving, BaseObject *Object){
  for(unsigned int i = 0; i < collision_queue.size(); i++){
    if(collision_queue[i]->Object == Object) return;
  }

  struct CollisionInfo *collision_info = new struct CollisionInfo;
  collision_info->cx = cx;
  collision_info->cy = cy;
  collision_info->bcx = cx - vx / Detect_Div;
  collision_info->bcy = cy - vy / Detect_Div;
  collision_info->side = side;
  collision_info->was_i_moving = was_i_moving;
  collision_info->Object = Object;
  collision_queue.push_back(collision_info);
}

void BaseObject::SetHitState(int hx, int hy){
  hitx = hx;
  hity = hy;
}

void BaseObject::GetHitState(int *hx, int *hy){
  *hx = hitx;
  *hy = hity;
}


void BaseObject::ClearCollisionQueue(){
  vector<struct CollisionInfo *>::iterator i;
  for(i = collision_queue.begin(); i != collision_queue.end(); i++){
    delete *i;
  }
  collision_queue.clear();
}

void BaseObject::UseTemplate(BaseObject *object_template){
  object_template->GetBoundary(&gx0, &gy0, &gx1, &gy1);
  Properties.CopyProperties(&object_template->Properties);
  SpriteAnimation.CopySequences(&object_template->SpriteAnimation);
  SoundEffects.CopySounds(&object_template->SoundEffects);
  BoundaryModes.CopyBoundaries(&object_template->BoundaryModes);
  name = object_template->GetName();
}

void BaseObject::PreUpdate(){
  oldvx = vx;
  oldvy = vy;
  SpriteAnimation.Update();
}

void BaseObject::PostUpdate(){

}

void BaseObject::SetClassType(const char *type){
  class_type = type;
}

void BaseObject::SetScale(double scale_){
  scale = scale_;
  inverse_scale = 1/scale;
}

char *BaseObject::GetClassType(){
  return (char *)class_type.c_str();
}

void BaseObject::Kill(){
  Properties.SetProperty("dead");
}

void BaseObject::MakeDying(){
  Properties.SetProperty("dying");
  Properties.DeleteProperty("solid");
  SpriteAnimation.ChangeTo("dying");
  vy = -10;
  vx = 0;
}

void BaseObject::SaveState(){
  inital_x = x;
  inital_y = y;
  inital_vx = vx;
  inital_vy = vy;
  SavedProperties.CopyProperties(&Properties);
}

void BaseObject::Reset(){
  x = inital_x;
  y = inital_y;
  vx = inital_vx;
  vy = inital_vy;
  Properties.Clear();
  Properties.CopyProperties(&SavedProperties);
}

void BaseObject::GetRequestedObjectData(string& cobject_, double& cx_, double& cy_,
                                        double& cvx_, double& cvy_){
  cobject_ = cobject;
  cx_ = cx;
  cy_ = cy;
  cvx_ = cvx;
  cvy_ = cvy;
  request_object = false;
}

void BaseObject::SetBoundaryToMode(char *modename){
  BoundaryModes.Use(modename, &gx0, &gy0, &gx1, &gy1);
}

void BaseObject::CreateObject(char *cobject_, double cx_, double cy_, double cvx_, double cvy_){
  cobject = cobject_;
  cx = cx_;
  cy = cy_;
  cvx = cvx_;
  cvy = cvy_;
  request_object = true;
}

string BaseObject::Print(int mode){
  map<string, string> argument;
  argument["class"] = class_type;
  char tmpbuf[32];
  string output_str;

  if(mode == Template){
    output_str = PackageInXML("template", &argument, Open_Tag, true);
    output_str = output_str + PackageInXML("name", NULL, Open_Tag, false) + name + PackageInXML("name", NULL, Close_Tag, true);

    argument.clear();
    snprintf(tmpbuf, 32, "%f", gx0);
    argument["x0"] = tmpbuf;
    snprintf(tmpbuf, 32, "%f", gy0);
    argument["y0"] = tmpbuf;
    snprintf(tmpbuf, 32, "%f", gx1);
    argument["x1"] = tmpbuf;
    snprintf(tmpbuf, 32, "%f", gy1);
    argument["y1"] = tmpbuf;

    output_str = output_str + PackageInXML("boundary", &argument, Single_Tag, true);
    output_str = output_str + SoundEffects.Print() + Properties.Print() + SpriteAnimation.Print();
    output_str = output_str + PackageInXML("template", NULL, Close_Tag, true);
    return output_str;
  }
  else{
    argument.clear();
    argument["type"] = name;
    snprintf(tmpbuf, 32, "%f", inital_x);
    argument["x"] = tmpbuf;
    snprintf(tmpbuf, 32, "%f", inital_y);
    argument["y"] = tmpbuf;
    snprintf(tmpbuf, 32, "%f", inital_vx);
    argument["vx"] = tmpbuf;
    snprintf(tmpbuf, 32, "%f", inital_vy);
    argument["vy"] = tmpbuf;
    output_str = PackageInXML("levelobject", &argument, Single_Tag, true);
    return output_str;
  }
}

double BaseObject::scale;
double BaseObject::inverse_scale;

/*================================================*/

BlockObject::BlockObject(){
  request_object = false;
  layer = 0;
}

void BlockObject::SetMapPosition(int mbx, int mby){
  bx = mbx;
  by = mby;
}

int BlockObject::IsAtMapPosition(int mbx, int mby){
  if(bx == mbx && by == mby) return True;
  return False;
}

void BlockObject::SetKey(char c){
  key = c;
};

char BlockObject::GetKey(){
  return key;
}

void BlockObject::UseTemplate(BaseObject *object_temp){
  BlockObject *object_template = (BlockObject *)object_temp;
  object_template->GetBoundary(&gx0, &gy0, &gx1, &gy1);
  Properties.CopyProperties(&object_template->Properties);
  SpriteAnimation.CopySequences(&object_template->SpriteAnimation);
  BoundaryModes.CopyBoundaries(&object_template->BoundaryModes);
  key = object_template->GetKey();
  name = object_template->GetName();
  layer = object_template->GetLayer();
}

string BlockObject::Print(int mode){
  map<string, string> argument;
  argument["class"] = class_type;
  char tmpbuf[32];
  string output_str;

  if(mode == Template){
    output_str = PackageInXML("template", &argument, Open_Tag, true);
    output_str = output_str + PackageInXML("name", NULL, Open_Tag, false) + name + PackageInXML("name", NULL, Close_Tag, true);
    snprintf(tmpbuf, 32, "%i", layer);
    output_str = output_str + PackageInXML("layer", NULL, Open_Tag, false) + tmpbuf + PackageInXML("layer", NULL, Close_Tag, true);

    argument.clear();
    snprintf(tmpbuf, 32, "%f", gx0);
    argument["x0"] = tmpbuf;
    snprintf(tmpbuf, 32, "%f", gy0);
    argument["y0"] = tmpbuf;
    snprintf(tmpbuf, 32, "%f", gx1);
    argument["x1"] = tmpbuf;
    snprintf(tmpbuf, 32, "%f", gy1);
    argument["y1"] = tmpbuf;

    output_str = output_str + PackageInXML("boundary", &argument, Single_Tag, true);
    output_str = output_str + SoundEffects.Print() + Properties.Print() + SpriteAnimation.Print();
    output_str = output_str + PackageInXML("blockkey", NULL, Open_Tag, false) + key + PackageInXML("blockkey", NULL, Close_Tag, true);
    output_str = output_str + PackageInXML("template", NULL, Close_Tag, true);
    return output_str;
  }
  else{
    argument.clear();
    argument["type"] = name;
    snprintf(tmpbuf, 32, "%f", inital_x);
    argument["x"] = tmpbuf;
    snprintf(tmpbuf, 32, "%f", inital_y);
    argument["y"] = tmpbuf;
    snprintf(tmpbuf, 32, "%f", inital_vx);
    argument["vx"] = tmpbuf;
    snprintf(tmpbuf, 32, "%f", inital_vy);
    argument["vy"] = tmpbuf;
    output_str = PackageInXML("levelobject", &argument, Single_Tag, true);
    return output_str;
  }
}

/*================================================*/

MagicObject::MagicObject(){
  Magic.SetCaster((BaseObject *)this);
  request_object = false;
}

void MagicObject::MagicAttack(){
  Magic.ScrollRunesRight();
}

void MagicObject::Draw(SDLscreen *Screen, int xv, int yv){
  if(Properties.HasProperty("dying") == False) Magic.Draw(Screen, xv, yv);
  Screen->DrawObject(SpriteAnimation.GetCurrentFrame(), (int)(inverse_scale*((int)x - xv)), (int)(inverse_scale*((int)y - yv)));
}

void MagicObject::UseTemplate(BaseObject *object_temp){
  MagicObject *object_template = (MagicObject *)object_temp;
  object_template->GetBoundary(&gx0, &gy0, &gx1, &gy1);
  Properties.CopyProperties(&object_template->Properties);
  SpriteAnimation.CopySequences(&object_template->SpriteAnimation);
  SoundEffects.CopySounds(&object_template->SoundEffects);
  BoundaryModes.CopyBoundaries(&object_template->BoundaryModes);
  Magic.Copy(&object_template->Magic);
  name = object_template->GetName();
}


/*================================================*/

PlayerObject::PlayerObject(){
  Magic.SetCaster((BaseObject *)this);
  i_can_climb = False;
  i_am_climbing = False;
  i_am_attacking = False;
  i_can_jump = False;
  i_am_in_water = False;
  direction = DirRight;
  walk = False;
  lives = 3;
  request_object = false;
  time_to_jump = 0;
  mass = 9;
}

void PlayerObject::MakeDying(){
  if(Properties.HasProperty("invulnerable")) return;
  GameAudio::AddToSampleQueue(SoundEffects["die"]);
  Properties.SetProperty("dying");
  Properties.DeleteProperty("solid");
  SpriteAnimation.ChangeTo("dying");
  vy = -20;
}

void PlayerObject::TakeHit(){
  if(Properties.HasProperty("invulnerable")) return;
  double mana = Magic.GetMana();
  GameAudio::AddToSampleQueue(SoundEffects["hurt"]);
  if(mana < 5) MakeDying();
  Magic.SetMana(mana - 10);
  vy = -15;
  vx = -vx;
  invulnerable = 50;
  Properties.SetProperty("invulnerable");
}

void PlayerObject::Kill(){
  lives--;
  Properties.SetProperty("dead");
}

void PlayerObject::GetMagic(){
  double mana = Magic.GetMana();
  Magic.SetMana(mana + 1);
  GameAudio::AddToSampleQueue(SoundEffects["getmagic"]);
}

void PlayerObject::MagicAttack(){
  if(Magic.GetNumberOfRunes() < 1) return;
  i_am_attacking = True;
  Magic.ScrollRunesRight();
}

int PlayerObject::Lives(){
  return lives;
}

int PlayerObject::Score(){
  return score;
}

void PlayerObject::SetLives(int lives_){
  lives = lives_;
}

void PlayerObject::SetScore(int score_){
  score = score_;
}

void PlayerObject::ClimbUp(){
 if(i_can_climb){
   vy = -4;
   i_am_climbing = True;
 }
}

void PlayerObject::ClimbDown(){
  if(i_can_climb){
    vy = 4;
    i_am_climbing = True;
  }
  else{
    i_am_crouching = True;
  }
}

void PlayerObject::Left(){
  if(i_am_crouching && i_can_jump) return;
  vx -= 1;
  walk = True;
  direction = DirLeft;
}

void PlayerObject::Right(){
  if(i_am_crouching && i_can_jump) return;
  vx += 1;
  walk = True;
  direction = DirRight;
}

void PlayerObject::Jump(){
  if(i_am_in_water && time_to_jump > 22){
    vy -= 14;
    time_to_jump = 0;
  }

  else if(i_can_jump){
    vy = -12.5;
    GameAudio::AddToSampleQueue(SoundEffects["jump"]);
  }
}

void PlayerObject::FullUpdate(){
  //Set max horizontal speed
  if(vx < -7 + ivx) vx = -7 + ivx;
  else if(vx > 7 + ivx) vx = 7 + ivx;

  //Do horizontal deceleration
  if(fabs(vx - ivx) > 0){
    if(vx - ivx > 0.001) vx -= 0.5 / Detect_Div;
    if(vx - ivx < -0.001) vx += 0.5 / Detect_Div;
  }

  if(i_am_in_water){
    if(vx < -3.5 + ivx) vx = -3.5 + ivx;
    else if(vx > 3.5 + ivx) vx = 3.5 + ivx;
    if(vy > 2) vy -= 2.0 / Detect_Div;
    else if(vy < -2) vy += 0.5 / Detect_Div;
  }
  Update();
}

void PlayerObject::Update(){
  for(unsigned int i = 0; i < collision_queue.size(); i++){
    if(collision_queue[i]->Object->Properties.HasProperty("climbable")) i_can_climb = True;
    if(!Properties.HasProperty("dying")){
      if(collision_queue[i]->Object->Properties.HasProperty("finish")) Properties.SetProperty("Finished level");
      if(collision_queue[i]->Object->Properties.HasProperty("solid") && Properties.HasProperty("solid") && !Properties.HasProperty("dying")) SolidHandler(collision_queue[i]);
      if(collision_queue[i]->Object->Properties.HasProperty("water")) i_am_in_water = True;
      if(collision_queue[i]->Object->Properties.HasProperty("givemagic")) GetMagic();
      if(collision_queue[i]->Object->Properties.HasProperty("bullet")) TakeHit();
    }
  }
  ClearCollisionQueue();
  //printf("%f, %f\n", vx, vy);
}

void PlayerObject::PreUpdate(){
  if(!Properties.HasProperty("dying")){
    if(i_am_climbing == True){
      SpriteAnimation.ChangeTo("climbing");
    }
    else if(i_can_jump == False){
      if(direction == DirRight) SpriteAnimation.ChangeTo("jump_right");
      else SpriteAnimation.ChangeTo("jump_left"); 
    }
    else if(i_am_crouching == True){
      if(direction == DirRight) SpriteAnimation.ChangeTo("crouching_right");
      else SpriteAnimation.ChangeTo("crouching_left");
    }
    else if(walk == False && i_am_attacking == False){
      if(direction == DirRight) SpriteAnimation.ChangeTo("standing_right");
      else SpriteAnimation.ChangeTo("standing_left");
    }
    else if(walk == False && i_am_attacking == True){
      if(direction == DirRight) SpriteAnimation.ChangeTo("standing_attack_right");
      else SpriteAnimation.ChangeTo("standing_attack_left"); 
    }
    else if(i_am_attacking == True){
      if(direction == DirRight) SpriteAnimation.ChangeTo("attack_right");
      else SpriteAnimation.ChangeTo("attack_left"); 
    }
    else if(i_am_attacking == False){
      if(direction == DirRight) SpriteAnimation.ChangeTo("right");
      else SpriteAnimation.ChangeTo("left"); 
    }
  }

  if(invulnerable > 0) invulnerable--;
  if(invulnerable == 0) Properties.DeleteProperty("invulnerable");
  if(i_am_crouching) SetBoundaryToMode("crouching");
  else SetBoundaryToMode("default");

  SpriteAnimation.Update();
  Magic.FindUseableRunes();
  Magic.Update();
  time_to_jump++;

  i_can_jump = False;
  i_can_climb = False;
  i_am_attacking = False;
  i_am_in_water = False;
  i_am_crouching = False;
  ivx = 0;
  walk = False;
  i_am_climbing = False;

  oldvx = vx;
  oldvy = vy;
}

void PlayerObject::PostUpdate(){

}

void PlayerObject::SolidHandler(struct CollisionInfo *collision_info){
  double xtmp, ytmp;

  if(collision_info->side == BottomSide){
    i_can_jump = True;
    if(collision_info->Object->Properties.HasProperty("spikes")){
      MakeDying();
      return;
    }
    if(collision_info->Object->Properties.HasProperty("sticky")){
      collision_info->Object->GetVelocity(&xtmp, &ytmp);
      ivx = xtmp;
    }
  }
  else if(collision_info->side == TopSide){
    
  }
  else if(collision_info->side == RightSide || collision_info->side == LeftSide){
    if(collision_info->Object->Properties.HasProperty("enemy")){
      TakeHit();
      return;
    }
  }
}

/*===============================================*/

MovingObject::MovingObject(){
  ticks = 0;
  next_route_point = 0;
  request_object = false;
}

MovingObject::~MovingObject(){
  vector<struct RouteCoordinate *>::iterator i;
  ClearCollisionQueue();
  for(i = route.begin(); i != route.end(); i++) delete *i;
}

void MovingObject::Reset(){
  x = inital_x;
  y = inital_y;
  vx = inital_vx;
  vy = inital_vy;
  Properties.Clear();
  Properties.CopyProperties(&SavedProperties);
  ticks = 0;
  next_route_point = 0;
}

void MovingObject::PreUpdate(){
  FollowRoute();
  oldvx = vx;
  oldvy = vy;
}

void MovingObject::Update(){
  for(unsigned int i = 0; i < collision_queue.size(); i++){
    if(collision_queue[i]->Object->Properties.HasProperty("solid") && Properties.HasProperty("solid")) SolidHandler(collision_queue[i]);
  }
  ClearCollisionQueue();
}

void MovingObject::FollowRoute(){
  if(route.size() < 1) return;
  if(ticks == 0){
    vx = route[next_route_point]->speed * cos(route[next_route_point]->direction);
    vy = route[next_route_point]->speed * sin(route[next_route_point]->direction);
    ticks = route[next_route_point]->ticks;
    next_route_point++;
    if(next_route_point >= route.size()) next_route_point = 0;
  }
  ticks--;
};

void MovingObject::AddRoutePoint(double speed, double direction, unsigned int ticks){
  struct RouteCoordinate *new_coord = new struct RouteCoordinate;
  new_coord->speed = speed;
  new_coord->direction = M_PI * (direction / 180);
  new_coord->ticks = ticks;
  route.push_back(new_coord);
  fprintf(stderr, "Added route coord\n");
};

string MovingObject::Print(int mode){
  map<string, string> argument;
  argument["class"] = class_type;
  char tmpbuf[32];
  string output_str;

  if(mode == Template){
    output_str = PackageInXML("template", &argument, Open_Tag, true);
    output_str = output_str + PackageInXML("name", NULL, Open_Tag, false) + name + PackageInXML("name", NULL, Close_Tag, true);

    argument.clear();
    snprintf(tmpbuf, 32, "%f", gx0);
    argument["x0"] = tmpbuf;
    snprintf(tmpbuf, 32, "%f", gy0);
    argument["y0"] = tmpbuf;
    snprintf(tmpbuf, 32, "%f", gx1);
    argument["x1"] = tmpbuf;
    snprintf(tmpbuf, 32, "%f", gy1);
    argument["y1"] = tmpbuf;

    output_str = output_str + PackageInXML("boundary", &argument, Single_Tag, true);
    output_str = output_str + SoundEffects.Print() + Properties.Print() + SpriteAnimation.Print();
    output_str = output_str + PackageInXML("template", NULL, Close_Tag, true);
    return output_str;
  }
  else{
    argument.clear();
    argument["type"] = name;
    snprintf(tmpbuf, 32, "%f", inital_x);
    argument["x"] = tmpbuf;
    snprintf(tmpbuf, 32, "%f", inital_y);
    argument["y"] = tmpbuf;
    snprintf(tmpbuf, 32, "%f", inital_vx);
    argument["vx"] = tmpbuf;
    snprintf(tmpbuf, 32, "%f", inital_vy);
    argument["vy"] = tmpbuf;
    output_str = PackageInXML("levelobject", &argument, Open_Tag, true);
    output_str = output_str + PackageInXML("route", NULL, Open_Tag, true);
    for(unsigned int i = 0; i < route.size(); i++){
      argument.clear();
      snprintf(tmpbuf, 32, "%f", route[i]->speed);
      argument["speed"] = tmpbuf;
      snprintf(tmpbuf, 32, "%f", 180*route[i]->direction/M_PI);
      argument["direction"] = tmpbuf;
      snprintf(tmpbuf, 32, "%i", route[i]->ticks);
      argument["ticks"] = tmpbuf;
      output_str = output_str + PackageInXML("point", &argument, Single_Tag, true);
    }
    output_str = output_str + PackageInXML("route", NULL, Close_Tag, true);
    output_str = output_str + PackageInXML("levelobject", NULL, Close_Tag, true);

    return output_str;
  }
}

/*===============================================*/

void MovingGroundObject::SolidHandler(struct CollisionInfo *collision_info){
  if(collision_info->side == BottomSide){
    hit_bottom = True;
  }
  if(collision_info->side == TopSide){
    hit_top = True;
  }
  if(collision_info->side == RightSide){
    hit_right = True;
    if(oldvx > 0 && (mass <= collision_info->Object->Mass() || vx == 0)) vx = -oldvx;
  }
  if(collision_info->side == LeftSide){
    hit_left = True;
    if(oldvx < 0 && (mass <= collision_info->Object->Mass() || vx == 0)) vx = -oldvx;
  }
}

/*===============================================*/

void MovingFlyingObject::SolidHandler(struct CollisionInfo *collision_info){
  if(collision_info->side == BottomSide){
    hit_bottom = True;
    if(oldvy > 0 && (mass <= collision_info->Object->Mass() || vy == 0)) vy = -oldvy;
  }
  if(collision_info->side == TopSide){
    hit_top = True;
    if(oldvy < 0 && (mass <= collision_info->Object->Mass() || vy == 0)) vy = -oldvy;
  }
  if(collision_info->side == RightSide){
    hit_right = True;
    if(oldvx > 0 && (mass <= collision_info->Object->Mass() || vx == 0)) vx = -oldvx;
  }
  if(collision_info->side == LeftSide){
    hit_left = True;
    if(oldvx < 0 && (mass <= collision_info->Object->Mass() || vx == 0)) vx = -oldvx;
  }
}

/*===============================================*/

void EnemyGroundObject::MakeDying(){
  if(Properties.HasProperty("invulnerable")) return;
  Properties.SetProperty("dying");
  Properties.SetProperty("invulnerable");
  Properties.DeleteProperty("solid");
  SpriteAnimation.ChangeTo("dying");
  vy = -10;
  vx = 0;
  GameAudio::AddToSampleQueue(SoundEffects["die"]);
}


void EnemyGroundObject::PreUpdate(){
  if(Properties.HasProperty("dying") == False){
    if(vx < 0) SpriteAnimation.ChangeTo("right");
    if(vx >= 0) SpriteAnimation.ChangeTo("left");
  }
  SpriteAnimation.Update();
  FollowRoute();
}

void EnemyGroundObject::Update(){
  hit_top = False;
  hit_bottom = False;
  for(unsigned int i = 0; i < collision_queue.size(); i++){
    if(collision_queue[i]->Object->Properties.HasProperty("solid") && Properties.HasProperty("solid")) SolidHandler(collision_queue[i]);
    if(collision_queue[i]->Object->Properties.HasProperty("rune") && collision_queue[i]->Object->Properties.HasProperty("killer")) MakeDying();
  }
  ClearCollisionQueue();
  if(hit_top == True) MakeDying();
  oldvx = vx;
  oldvy = vy;
}

//_____________________________________________________________


FollowEnemyGroundObject::FollowEnemyGroundObject(){
  ticks = 0;
  next_route_point = 0;
  object = NULL;
  request_object = false;
}

void FollowEnemyGroundObject::MakeDying(){
  if(Properties.HasProperty("invulnerable")) return;
  Properties.SetProperty("dying");
  Properties.SetProperty("invulnerable");
  Properties.DeleteProperty("solid");
  //SpriteAnimation.ChangeTo("dying");
  vy = -10;
  vx = 0;
  GameAudio::AddToSampleQueue(SoundEffects["die"]);
}

void FollowEnemyGroundObject::PreUpdate(){
  double objx = 0, objy = 0;
  if(Properties.HasProperty("dying") == False){
    if(object != NULL) object->GetPosition(&objx, &objy);
    if(objx > x) SpriteAnimation.ChangeTo("right");
    if(objx <= x) SpriteAnimation.ChangeTo("left");
  }
  SpriteAnimation.Update();
}

void FollowEnemyGroundObject::Update(){
  hit_top = False;
  hit_bottom = False;

  for(unsigned int i = 0; i < collision_queue.size(); i++){
    if(collision_queue[i]->Object->Properties.HasProperty("solid") && Properties.HasProperty("solid")) SolidHandler(collision_queue[i]);
    if(collision_queue[i]->Object->Properties.HasProperty("rune") && collision_queue[i]->Object->Properties.HasProperty("killer")) MakeDying();
  }
  ClearCollisionQueue();
  if(hit_top == True) MakeDying();

  oldvx = vx;
  oldvy = vy;
}

void FollowEnemyGroundObject::FollowObject(BaseObject *follow){
  object = follow;
}

//__________________________________________________________

ThrowerObject::ThrowerObject(){
  ticks = 0;
  next_route_point = 0;
  object = NULL;
  request_object = false;
  time_to_throw = 100;
}

void ThrowerObject::PreUpdate(){
  double objx = 0, objy = 0;
  if(Properties.HasProperty("dying") == False){
    if(object != NULL) object->GetPosition(&objx, &objy);
    if(objx > x) SpriteAnimation.ChangeTo("right");
    if(objx <= x) SpriteAnimation.ChangeTo("left");
  }
  SpriteAnimation.Update();
  time_to_throw--;
  if(time_to_throw == 0 && !Properties.HasProperty("dying")) ThrowObject();

  oldvx = vx;
  oldvy = vy;
}

void ThrowerObject::ThrowObject(){
  double objx = 0, objy = 0;
  double tx = 0, ty = 0, tvx = 0, tvy = 0;
  if(object != NULL) object->GetPosition(&objx, &objy);
  if(objx > x){
    tx = gx1 + x;
    tvx = 7;
  }
  if(objx <= x){
    tx = gx0 + x;
    tvx = -7;
  }
  ty = y + (gy0 + gy1)/2;
  tvy = -10;
  
  CreateObject("badfruit", tx, ty, tvx, tvy);
  time_to_throw = 100;
}

//_________________________________________________________

BulletObject::BulletObject(){
  ticks = 0;
  next_route_point = 0;
  request_object = false;
  time_to_die = 30;
}

void BulletObject::PreUpdate(){
  time_to_die--;
  if(time_to_die == 0) Properties.SetProperty("dead");
  oldvx = vx;
  oldvy = vy;
}

void BulletObject::Update(){
  for(unsigned int i = 0; i < collision_queue.size(); i++){
    if(collision_queue[i]->Object->Properties.HasProperty("solid") && !collision_queue[i]->Object->Properties.HasProperty("enemy")) Properties.SetProperty("dead");
  }
  ClearCollisionQueue();
}

//__________________________________________________________

void ManaJar::Kill(){
  Properties.SetProperty("dead");
  //GameAudio::AddToSampleQueue(SoundEffects["collected"]);
}

void ManaJar::Update(){
  hit_top = False;
  hit_bottom = False;
  hit_left = False;
  hit_right = False;

  for(unsigned int i = 0; i < collision_queue.size(); i++){
    if(collision_queue[i]->Object->Properties.HasProperty("solid") && collision_queue[i]->Object->Properties.HasProperty("player")){
      SolidHandler(collision_queue[i]);
    }
  }
  ClearCollisionQueue();
  if(hit_top||hit_left||hit_bottom||hit_right) Kill();
}

//_________________________________________________________

FallingBlock::FallingBlock(){
  i_have_been_stepped_on = False;
  count_down = 2;
  request_object = false;
  mass = 1;
}

void FallingBlock::PreUpdate(){
  SpriteAnimation.Update();
  if(Properties.HasProperty("I have been stepped on")) count_down--;
  if(count_down < 0){
    Properties.SetProperty("fall");
    count_down = 2;
  }
}

void FallingBlock::FullUpdate(){
  if(Properties.HasProperty("fall")) vy += 1.5 / Detect_Div;  

  if(fabs(vx) > 0){
    if(vx > 0.001) vx -= 0.5 / Detect_Div;
    if(vx < -0.001) vx += 0.5 / Detect_Div;
  }
  Update();
}

void FallingBlock::Update(){
  for(unsigned int i = 0; i < collision_queue.size(); i++){
    if(collision_queue[i]->Object->Properties.HasProperty("solid")) SolidHandler(collision_queue[i]);
  }
  ClearCollisionQueue();
}

void FallingBlock::SolidHandler(struct CollisionInfo *collision_info){
  if(collision_info->side == TopSide) Properties.SetProperty("I have been stepped on");
}

/*===============================================*/

int CheckObjectCollision(BaseObject *object1, BaseObject *object2, int mode){
  double x0_1, y0_1, x1_1, y1_1;  //Defines collision boundary for object 1
  double x_1, y_1;
  double x0_2, y0_2, x1_2, y1_2;  //Defines collision boundary for object 2
  double x_2, y_2;
  double dx, dy;
  int xflag = LeftSide, yflag = TopSide;
  char side;

  object1->GetBoundary(&x0_1, &y0_1, &x1_1, &y1_1);
  object1->GetPosition(&x_1, &y_1);

  object2->GetBoundary(&x0_2, &y0_2, &x1_2, &y1_2);
  object2->GetPosition(&x_2, &y_2);

  dx = (x_2 + x0_2) - (x_1 + x0_1);
  dy = (y_2 + y0_2) - (y_1 + y0_1);

  double width1 = x1_1 - x0_1, height1 = y1_1 - y0_1;
  double width2 = x1_2 - x0_2, height2 = y1_2 - y0_2;
  
  //Check if boundaries overlap
  if(dx <= width1  && dx >= -width2
  && dy <= height1 && dy >= -height2){

    //All this stuff checks what side of the boundary box we collided with
    double overlap_x = width1 - dx;
    double overlap_y = height1 - dy;
    
    if(overlap_x > (width1 + width2) / 2){
      xflag = RightSide;
      overlap_x = width2 + width1 - overlap_x;
    }
    if(overlap_y > (height1 + height2)/ 2){
      yflag = BottomSide;
      overlap_y = height2 + height1 - overlap_y;
    }
    if(mode == Check_Horizontal) side = xflag;
    else side = yflag;
    object1->SetCollisionInfo(x_1, y_1, ~side, True, object2);
    object2->SetCollisionInfo(x_2, y_2, side, False, object1);
    return True;
  }
  return False;
}
