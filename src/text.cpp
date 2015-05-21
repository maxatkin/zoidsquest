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



#include "text.h"

Text::~Text(){

}

int Text::Load(char *fontname, char *fontfile){
  SDL_Surface *newfont = SDLscreen::GameSprites.Load(fontfile, False);
  if(newfont == NULL || fontname == NULL) return Error;
  string name = fontname;
  fonts[name] = newfont;
  return Ok;
}

void Text::FontProperties(char *font, unsigned int &w, unsigned int &h){
  string strfont = font;
  if(fonts.find(strfont) == fonts.end()){
    fprintf(stderr, "Warning: font %s not found\n", font);
    return;
  }
  w = 12;
  h = 17;
}

void Text::Print(char *font, char *text_, double x, double y, vector<void *> *format){
  int line_number;
  string text = text_;
  lines.resize(lines.size() + 1);
  line_number = lines.size() - 1;

  unsigned int i = 0, j = 0, f = 0; 
  if(format != NULL && format->size() > 0){
    for(i = 0; i < text.size(); i++){
      if(text[i] == '%' && (*format)[f] != NULL){
        lines[line_number].text += text.substr(j, i);
        j = i + 2;
        i++;
        if(text[i] == 's') lines[line_number].text += (char *)(*format)[f];
        else if(text[i] == 'c'){
          char charbuf[16];
          char c = *((char *)(*format)[f]);
          sprintf(charbuf, "%c", c);
          lines[line_number].text += charbuf;
        }

        else if(text[i] == 'e'){
          char charbuf[16];
          unsigned int c = *((unsigned int *)(*format)[f]);
          if(c == SDLK_SPACE) lines[line_number].text += "Space";
          else if(c == SDLK_UP) lines[line_number].text += "Up";
          else if(c == SDLK_DOWN) lines[line_number].text += "Down";
          else if(c == SDLK_LEFT) lines[line_number].text += "Left";
          else if(c == SDLK_RIGHT) lines[line_number].text += "Right";
          else{
            sprintf(charbuf, "%c", c);
            lines[line_number].text += charbuf;
          }
        }
        else if(text[i] == 'i'){
          char numbuf[32];
          sprintf(numbuf, "%i", *((int *)(*format)[f]));
          lines[line_number].text += numbuf;
        }
        f++;
      }
    }
    lines[line_number].text += text.substr(j, i);
  }
  else lines[line_number].text = text_;
  lines[line_number].font = font;
  lines[line_number].x = x;
  lines[line_number].y = y;
}

void Text::Display(SDLscreen *Screen){
  struct sprite_info letters;
  SDL_Rect src;
  double x, y;
  char chr;
  int cx, cy;

  for(unsigned int j = 0; j < lines.size(); j++){
    x = lines[j].x;
    y = lines[j].y;

    for(unsigned int i = 0; i < lines[j].text.size(); i++){
      chr = lines[j].text[i];
      cx = (chr - 32) % 16;
      cy = ((chr - 32) - cx)/16;

      src.w = 12;
      src.h = 17;
      src.x = (int)(cx*((double)src.w + 0.55));
      src.y = (int)(cy*((double)src.h + 10));

      if(fonts.find(lines[j].font) == fonts.end()){
        fprintf(stderr, "Warning: font %s not found\n", lines[j].font.c_str());
        return;
      }
      letters.image = fonts[lines[j].font];
      Screen->DrawObject(&letters, (int)x + i*src.w, (int)y, &src);
    }
  }
  lines.clear();
}    

vector<struct line> Text::lines;
map<string, SDL_Surface *> Text::fonts;
