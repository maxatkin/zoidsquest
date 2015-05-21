#ifndef _MENU_H_
#define _MENU_H_

#include <vector>
#include <string>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "main.h"
#include "SDLdraw.h"
#include "input.h"
#include "level.h"
#include "text.h"
#include "sound.h"
#include "objects.h"

using namespace std;

class menuEntry;
class GameMenu;
class MenuPage;


class menuEntry{
  public:
    menuEntry();
    int SetProperties(char *name_, char *action_);
    int SetProperties(BaseObject *object_, char *action);
    void SetPosition(unsigned int x_, unsigned int y_);
    int AddActionArgument(char *argument_);
    void AddPointerToFormat(void *format_arg);

    string Name(){ return name; }
    string Action(){ return action; }
    vector<void *> FormatList(){ return format; }
    string ActionArgument(unsigned int arg);
    void Size(unsigned int &w_, unsigned int &h_);

    bool IsPointInMe(unsigned int x, unsigned int y);
    int Draw(SDLscreen *Screen, char *font);

  private:
    unsigned int px, py;
    unsigned int width, height; //in pixels
    string name;
    string action;
    BaseObject object;
    vector<void *> format;
    vector<string> action_arguments;
};

class MenuPage{
  public:
    MenuPage();
    virtual ~MenuPage();
    void SetName(char *name_, char *title_){
      name = name_;
      title = title_;
    }
    char *GetName(){ return (char*)name.c_str(); }
    void Reset();
    void SetPosition(unsigned int px_, unsigned int py_, unsigned int line_spacing_ = 3);
    void AddEntry(char *name, char *action, unsigned int ex = 0, unsigned int ey = 0);
    void AddEntry(BaseObject *object, char *action, unsigned int ex = 0, unsigned int ey = 0);

    void ClearEntries();
    void AddActionArgument(char *argument_);
    void AddPointerToFormat(void *format_arg);
    void Size(unsigned int& w, unsigned int& h);
    void Position(unsigned int& x, unsigned int& y);
    void NextEntry();
    void LastEntry();
    void ExecuteEntry(){ execute_flag = True; }
    bool ExecuteFlag();
    void ClearExecuteFlag(){ execute_flag = False; } 
    bool MouseOnEntry(){ return mouse_on_entry; }
    void SetEntryWithMouse(unsigned int mx, unsigned int my);
    menuEntry *GetExectueData(){ return entries[current_selection]; }
    static void SetMenuBackGround(char *menubgfile);
    void Draw(SDLscreen *Screen, bool usebg = true, bool centre = true, unsigned int w_ = 0, unsigned int h_ = 0);

  private:
    bool big_title;
    bool execute_flag;
    bool mouse_on_entry;
    unsigned int current_selection;
    unsigned int px, py, line_spacing;
    unsigned int menu_width, menu_height;
    string title;
    string name;
    vector<menuEntry *> entries;
    static struct sprite_info menubg;
};

class BaseMenu{
  public:
    BaseMenu();
    virtual ~BaseMenu();
    virtual void Update();
    virtual void Create();
    virtual void Draw(SDLscreen *Screen);
    void AddPage(char *name, char *title, unsigned int px, unsigned int py);
    void SetPage(char *page);
    void SetMenuBackGround(char *menubg_);
    int GotoLastPage();
    int GotoNextPage(char *page);
    MenuPage *CurrentPage();
    virtual void Generate();

  protected:
    map<string, MenuPage *> Pages;
    vector<MenuPage *> PageStack;
    MenuPage *current_page;
};

class EditorMenu: public BaseMenu{
  public:
    EditorMenu();
    void Update();
    void Create(double margin_, map<string, BaseObject *> *templates_);
    void Draw(SDLscreen *Screen);
    void Generate();
    BaseObject *GetCurrentObject(){ return current_object; }
  private:
    vector<BaseObject *> templates;
    map<string, BaseObject *> *template_map;
    double margin;
    BaseObject *current_object;
};

class GameMenu: public BaseMenu{
  public:
    GameMenu();
    void Create(map<string, unsigned int> *controls);
    void Update();
    void Draw(SDLscreen *Screen, bool draw_background, bool draw_logo);
    void SetLogo(char *logo_);
    void SetMusic(char *music_);
    void SetOpeningBackGround(char *openingbg_);
    char *GetMusic();
    unsigned int *ChangeKey(){ return change_key; }
    void ClearChangeKey(){ change_key = NULL; }
    MenuPage *CurrentPage(){ return current_page; }
    void Generate();

  private:

    map<string, unsigned int *> control_ptr;
    unsigned int *change_key;
    string music;
    Land BackgroundLand;
    struct sprite_info menubg;
    struct sprite_info logo;
    int xv, yv;
};

#endif
