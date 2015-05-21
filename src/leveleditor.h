#ifndef _LEVELEDITOR_H_
#define _LEVELEDITOR_H_

#include <vector>
#include <map>
#include <string>
#include "main.h"
#include "level.h"
#include "SDLdraw.h"
#include "menu.h"

using namespace std;

class LevelEditor;

class LevelEditor: public Level{
  public:
    EditorMenu Menu;
    LevelEditor();
    void Init(double margin);
    void Update();
    int LoadToEdit(map<string, BaseObject *> *global_templates_);
    void Draw(SDLscreen *Screen);
    void SetMousePosition(unsigned int mx_, unsigned int my_);
    void SetFile(char *editfile_);
    void ClickMouse(unsigned char button);
    void ChangeType();
    int Write(char *levelfile = NULL);
  private:
    void DrawGrid(SDLscreen *Screen);
    unsigned int mx, my; //mouse position
    unsigned int mlx, mly; //grid square mouse is in
    map<string, BaseObject *> *global_templates;
    BaseObject *current_object;
    double scale;
    double menu_margin;
    int type;
    string editfile;
};


#endif
