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



#include "sprites.h"
#include "parser.h"

SpriteSequences::SpriteSequences(){
  frame = 0;
  current_seq = NULL;
}

SpriteSequences::~SpriteSequences(){
  map<string, vector<struct sprite_info *> *>::iterator i;
  vector<struct sprite_info *> seqptr;
  unsigned int j;

  for(i = sequences.begin(); i != sequences.end(); i++){
    seqptr = *((*i).second);
    for(j = 0; j < seqptr.size(); j++) delete seqptr[j];
    delete (*i).second;
  }
}

void SpriteSequences::Create(char *name){
  vector<struct sprite_info *> *sequence = new vector<struct sprite_info *>;
  string seq_name = name;
  sequences[seq_name] = sequence;
}

int SpriteSequences::AddImageTo(char *name, char *imagefile, int repeat){
  struct sprite_info *new_frame = new struct sprite_info;
  string seq_name = name;
  new_frame->image = SDLscreen::GameSprites.Load(imagefile);
  new_frame->repeat = repeat;
  new_frame->repeated = repeat;
  if(new_frame->image == NULL) return Error;
  if(sequences.find(seq_name) == sequences.end()) return Error;
  sequences[seq_name]->push_back(new_frame);
  return Ok;
}

struct sprite_info *SpriteSequences::GetCurrentFrame(){
  if(current_seq == NULL)return NULL;
  if(current_seq->size() < frame)return NULL;
  return (*current_seq)[frame];
}

void SpriteSequences::Update(){
  if(current_seq == NULL) return;
  struct sprite_info *current_frame = (*current_seq)[frame];
  if(current_frame->repeated > 1) current_frame->repeated--;
  else{
    frame++;
    current_frame->repeated = current_frame->repeat;
  }
  if(frame >= current_seq->size()) frame = 0;
}

void SpriteSequences::ChangeTo(char *name){
  string seq_name = name;
  if(sequences.find(seq_name) == sequences.end()){
    fprintf(stderr, "Warning: animation %s not found\n", name);
    return;
  }
  if(current_seq != sequences[seq_name]){
    current_seq = sequences[seq_name];
    frame = 0;
  }
}

vector<string> SpriteSequences::GetSequenceList(){
  vector<string> seq_list;
  map<string, vector<struct sprite_info *> *>::iterator i;

  for(i = sequences.begin(); i != sequences.end(); i++){
    seq_list.push_back((*i).first);
  }
  return seq_list;
}

vector<struct sprite_info *> SpriteSequences::GetSequence(string name){
  return *(sequences[name]);
}

string SpriteSequences::Print(){
  map <string, string> argument;
  string output_str = "";
  char *filename;
  char repeat[16];

  vector<string> seq_list = GetSequenceList();
  vector<struct sprite_info *> new_seq;

  for(unsigned int i = 0; i < seq_list.size(); i++){
    new_seq = *(sequences[seq_list[i]]);
    argument.clear();
    argument["name"] = seq_list[i];
    output_str = output_str + PackageInXML("animation", &argument, Open_Tag, true);

    for(unsigned int j = 0; j < new_seq.size(); j++){
      filename = SDLscreen::GameSprites.GetFileName(new_seq[j]);
      if(filename == NULL) continue;
      snprintf(repeat, 16, "%i", new_seq[j]->repeat);
      argument.clear();
      argument["repeat"] = repeat;
      output_str = output_str + PackageInXML("frame", &argument, Open_Tag, false)
                   + filename + PackageInXML("frame", NULL, Close_Tag, true);
    }
    output_str = output_str + PackageInXML("animation", NULL, Close_Tag, true);
  }

  return output_str;
}

void SpriteSequences::CopySequences(SpriteSequences *NewSequences){
  vector<string> seq_list = NewSequences->GetSequenceList();
  vector<struct sprite_info *> *tmp;
  vector<struct sprite_info *> new_seq;
  struct sprite_info *new_frame;

  for(unsigned int i = 0; i < seq_list.size(); i++){
    tmp = new vector<struct sprite_info *>;
    new_seq = NewSequences->GetSequence(seq_list[i]);
    for(unsigned int j = 0; j < new_seq.size(); j++){
      new_frame = new struct sprite_info;
      new_frame->image = new_seq[j]->image;
      new_frame->repeat = new_seq[j]->repeat;
      new_frame->repeated = new_seq[j]->repeat;
      tmp->push_back(new_frame);
    }
    sequences[seq_list[i]] = tmp;
  }
}
