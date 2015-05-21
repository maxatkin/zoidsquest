#ifndef _GAME_H_
#define _GAME_H_

#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "main.h"
#include "SDLdraw.h"
#include "objects.h"
#include "input.h"
#include "level.h"
#include "text.h"
#include "hud.h"
#include "sound.h"
#include "menu.h"
#include "leveleditor.h"

#define MENU 0
#define GAME 1
#define EDITOR 2

using namespace std;

class GameWrapper;

class GameWrapper{
  public:
    Text Font;
    HUD Hud;
    GameAudio Audio;
    GameMenu Menu;
    LevelEditor Editor;
    ~GameWrapper();
    int Init();
    int InitGame();
    int Update();
    int UpdateMenu();
    int UpdateEditor();
    int AddLevel(char *lvlfile);
    int NewGame();
    int SetLevel(unsigned int level);
    int AdvanceLevel();
    int LoadGlobalTemplates(char *gamefile);
    int Finished(){return finished;};
    int Mode(){ return mode; }
    int SetMode(char *setmode_);
  private:
    SDLscreen Screen;
    Input UserInput;
    vector<string> Levelfiles;
    map<string, BaseObject *> global_templates;
    map<string, unsigned int> controls;
    Level *CurrentLevel;
    unsigned int level_number;
    unsigned int finished;
    unsigned int started_game;
    unsigned int mode;
};

#endif
