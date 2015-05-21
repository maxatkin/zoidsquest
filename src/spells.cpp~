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
#include "objects.h"  /*Spell class definitions in objects.h*/

void RuneObject::Draw(SDLscreen *Screen, int xv, int yv){
  double nx = x - abs((int)((gx0 - gx1)/2));
  double ny = y - abs((int)((gy0 - gy1)/2));
  Screen->DrawObject(SpriteAnimation.GetCurrentFrame(), (int)(inverse_scale*((int)nx - xv)), (int)(inverse_scale*((int)ny - yv)));
}


Spells::Spells(){
  spell_angle = 0;
  spell_radius = 100;
  current_rune = NULL;
  last_selected_rune = NULL;
  mana = 0;
  attack = False;
  mana_charge = 0.0015;
}

Spells::~Spells(){
  vector<RuneObject *>::iterator i;
  for(i = runestore.begin(); i != runestore.end(); i++){
    delete *i;
  }
}

void Spells::AddRune(char *name, RuneObject *template_rune){
  RuneObject *NewRune;
  string rune_name = name;
  NewRune = new RuneObject;
  NewRune->UseTemplate(template_rune);
  rune_key[rune_name] = NewRune;
  runestore.push_back(NewRune);
}

void Spells::Copy(Spells *CopyMagic){
  vector<RuneObject *> new_runes = CopyMagic->GetRunes();
  RuneObject *tmpRune;
  for(unsigned int i = 0; i < new_runes.size(); i++){
    fprintf(stderr, "Copying Rune\n");
    tmpRune = new RuneObject;
    tmpRune->UseTemplate(new_runes[i]);
    tmpRune->SpriteAnimation.ChangeTo("default");
    runestore.push_back(tmpRune);
  }
  max_mana = runestore.size() * 10;
  mana = 0;
}

void Spells::SetCaster(BaseObject *caster){
  Caster = caster;
}

void Spells::ScrollRunesLeft(){
  spell_angle -= 0.1;
}

void Spells::ScrollRunesRight(){
  spell_angle += 0.1;
  attack = True;
  mana -= 0.25;
}

void Spells::SelectRune(){
}

void Spells::DoSpell(){
}


void Spells::Draw(SDLscreen *Screen, int xv, int yv){
  for(unsigned int i = 0; i < runes.size(); i++){
    runes[i]->Draw(Screen, xv, yv);
  }
}

void Spells::Update(){
  double dx, dy;
  double cx, cy;
  double centre_x, centre_y;
  double gx0, gx1, gy0, gy1;

  mana += mana_charge;
  if(mana > max_mana) mana = max_mana;
  if(mana < 0) mana = 0;
  if(runes.size() < 1) return;

  for(unsigned int i = 0; i < runes.size(); i++){
    if(runes[i]->Properties.HasProperty("killer")) runes[i]->SpriteAnimation.ChangeTo("killer");
    else runes[i]->SpriteAnimation.ChangeTo("default");
  }

  spell_angle += 0.05;

  Caster->GetPosition(&cx, &cy);
  Caster->GetBoundary(&gx0, &gy0, &gx1, &gy1);

  centre_x = cx + (int)((gx0 + gx1)/2 + 5);
  centre_y = cy + (int)((gy0 + gy1)/2);

  for(unsigned int i = 0; i < runes.size(); i++){
    dx = spell_radius * sin(-spell_angle + i*dangle);
    dy = spell_radius * cos(-spell_angle + i*dangle);
    runes[i]->SetPosition(centre_x + (int)dx, centre_y - (int)dy);
  }
}

vector<RuneObject *> Spells::GetUsedRunes(){
  return runes;
}

vector<RuneObject *> Spells::GetRunes(){
  return runestore;
}

void Spells::SetMana(double m){
  mana = m;
  if(mana > max_mana) mana = max_mana;
  if(mana < 0) mana = 0;
}

double Spells::GetMana(){
  return mana;
}

double Spells::GetManaPercentage(){
  return mana/max_mana;
}

unsigned int Spells::GetNumberOfRunes(){
  return (unsigned int)(mana/10 + 0.5);
}

void Spells::FindUseableRunes(){
  unsigned int number_of_runes = (unsigned int)(mana/10 + 0.5);
  runes.clear();
  max_mana = runestore.size() * 10;
  if(number_of_runes > runestore.size()) number_of_runes = runestore.size();
  for(unsigned int i = 0; i < number_of_runes; i++){
    runes.push_back(runestore[i]);
    if(runes[i]->Properties.HasProperty("killer")) runes[i]->Properties.DeleteProperty("killer"); 
    if(attack == True) runes[i]->Properties.SetProperty("killer");
  }
  attack = False;
  dangle = 2*M_PI / (double)runes.size();
}


