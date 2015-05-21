#ifndef _SPRITES_H_
#define _SPRITES_H_

#include <string>
#include <map>
#include <vector>
#include "SDLdraw.h"
#include "main.h"

class SpriteSequences;

class SpriteSequences{
  public:
    SpriteSequences();
    ~SpriteSequences();
    void Create(char *name);
    int AddImageTo(char *name, char *imagefile, int repeat);
    void Update();
    void ChangeTo(char *name);
    struct sprite_info *GetCurrentFrame();
    void CopySequences(SpriteSequences *NewSequences);
    string Print();

    //psudeo-private
    vector<string> GetSequenceList();
    vector<struct sprite_info *> GetSequence(string name);
    
  private:
    map<string, vector<struct sprite_info *> *> sequences;
    vector<struct sprite_info *> *current_seq;
    unsigned int frame;
};

#endif
