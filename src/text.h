#ifndef _TEXT_H_
#define _TEXT_H_

#include <vector>
#include <map>
#include <string>
#include "SDLdraw.h"
#include "main.h"

#define number_of_letters 112

class Text;

struct line{
  string text;
  string font;
  double x;
  double y;
};


class Text{
  public:
    ~Text();
    int Load(char *fontname, char *fontfile);
    static void FontProperties(char *font, unsigned int &w, unsigned int &h);
    static void Print(char *font, char *text_, double x, double y, vector<void *> *format = NULL);
    static void Display(SDLscreen *Screen);
  private:
    static map<string, SDL_Surface *> fonts;
    static vector<struct line> lines;
};

#endif
