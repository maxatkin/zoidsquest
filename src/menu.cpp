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



#include "menu.h"

menuEntry::menuEntry(){
  name = "";
}

int menuEntry::SetProperties(char *name_, char *action_){
  if(name_ == NULL || action_ == NULL) return Error;
  name = name_;
  action = action_;
  
  unsigned int w, h;
  Text::FontProperties("white", w, h);
//Convert here
  width = w*name.size();
  height = h;
  return Ok;
}

int menuEntry::SetProperties(BaseObject *object_, char *action_){
  if(object_ == NULL || action_ == NULL) return Error;
  object.UseTemplate(object_);
  action = action_;
//  width = 
//  height =
  return Ok;
}

void menuEntry::SetPosition(unsigned int x_, unsigned int y_){
  px = x_;
  py = y_;
}

void menuEntry::Size(unsigned int &w_, unsigned int &h_){
  w_ = width;
  h_ = height;
}

int menuEntry::AddActionArgument(char *argument_){
  if(argument_ == NULL) return Error;
  string argument = argument_;
  action_arguments.push_back(argument);
  return Ok;
}

void menuEntry::AddPointerToFormat(void *format_arg){
  format.push_back(format_arg);
}

string menuEntry::ActionArgument(unsigned int arg){
  return action_arguments[arg];
}

bool menuEntry::IsPointInMe(unsigned int x, unsigned int y){
  if((x >= px)&&(y >= py)&&(x <= px + width)&&(y <= py + height)) return True;
  return False;
}

int menuEntry::Draw(SDLscreen *Screen, char *font){
  if(font == NULL) return Error;
//convert here
  Text::Print(font, (char *)name.c_str(), px, py, &format);
  Text::Display(Screen);
  return Ok;
}

//__________________________________________________________________

MenuPage::MenuPage(){
  execute_flag = False;
  big_title = False;
  current_selection = 0;
  menu_width = 0;
  menu_height = 0;
  line_spacing = 0;
  mouse_on_entry = false;
}

MenuPage::~MenuPage(){
  for(unsigned int i = 0; i < entries.size(); i++){
    delete entries[i];
  }
}

void MenuPage::Reset(){
  current_selection = 0;
}

void MenuPage::SetPosition(unsigned int px_, unsigned int py_, unsigned int line_spacing_){
  px = px_;
  py = py_;
  line_spacing = line_spacing_;
}

void MenuPage::AddEntry(char *name, char *action, unsigned int ex, unsigned int ey){
  unsigned int w, h;
  menuEntry *newEntry = new menuEntry;
  newEntry->SetProperties(name, action);
  newEntry->SetPosition(px + ex, py + ey);
  newEntry->Size(w, h);
  menu_height += h + line_spacing;
  if(w > menu_width) menu_width = w;
  entries.push_back(newEntry);
}

void MenuPage::AddEntry(BaseObject *object_, char *action, unsigned int ex, unsigned int ey){
/*
  unsigned int w, h;
  menuEntry *newEntry = new menuEntry;
  newEntry->SetProperties(name, action);
  newEntry->SetPosition(px + ex, py + ey);
  newEntry->Size(w, h);
  menu_height += h + line_spacing;
  if(w > menu_width) menu_width = w;
  entries.push_back(newEntry);
*/
}

void MenuPage::AddPointerToFormat(void *format_arg){
  entries.back()->AddPointerToFormat(format_arg);
}

void MenuPage::AddActionArgument(char *argument_){
  entries.back()->AddActionArgument(argument_);
}

bool MenuPage::ExecuteFlag(){
  return execute_flag;
}

void MenuPage::NextEntry(){
  current_selection++;
  if(current_selection > entries.size() - 1) current_selection = 0;
}

void MenuPage::LastEntry(){
  if(current_selection < 1) current_selection = entries.size() - 1;
  else current_selection--;
}

void MenuPage::ClearEntries(){
  menu_width = 0;
  menu_height = 0;
  line_spacing = 0;
  entries.clear();
}

void MenuPage::SetEntryWithMouse(unsigned int mx, unsigned int my){
  for(unsigned int i = 0; i < entries.size(); i++){
    if(entries[i]->IsPointInMe(mx, my)){
      current_selection = i;
      mouse_on_entry = true;
      return;
    }
  }
  mouse_on_entry = false;
}

void MenuPage::Draw(SDLscreen *Screen, bool usebg, bool centre, unsigned int w_, unsigned int h_){
  unsigned int x = px, y = py;
  unsigned int w, h;

  if(usebg){
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    if(w_ == 0 && h_ == 0){
//Convert here
      src.w = (3*menu_width)/2;
      src.h = (3*menu_height)/2;
//Convert here
      Screen->DrawObject(&menubg, px - src.w/2, py - src.h/2, &src);
    }
    else{
//Convert here
      src.w = w_;
      src.h = h_;
//Convert here
      Screen->DrawObject(&menubg, px, py, &src);
    }
  }
 
  y = y - menu_height/2;
  for(unsigned int i = 0; i < entries.size(); i++){
    if(centre){
      entries[i]->Size(w, h);
      entries[i]->SetPosition(x - w/2, y);
    }
    if(i != current_selection) entries[i]->Draw(Screen, "white");
    else entries[i]->Draw(Screen, "blue");
    y = y + h + line_spacing;
  }
}

void MenuPage::SetMenuBackGround(char *menubgfile){
  menubg.image = SDLscreen::GameSprites.Load(menubgfile);
}

struct sprite_info MenuPage::menubg;

//_____________________________________________________________

BaseMenu::BaseMenu(){
  current_page = NULL;
}

BaseMenu::~BaseMenu(){
  map<string, MenuPage *>::iterator i;
  for(i = Pages.begin(); i != Pages.end(); i++){
    delete (*i).second;
  }
}


MenuPage *BaseMenu::CurrentPage(){ return current_page; }

void BaseMenu::SetPage(char *page){
  string str_page = page;
  if(Pages.find(str_page) == Pages.end()){
    fprintf(stderr, "Warning: menu page %s not found\n", page);
    return;
  }
  current_page = Pages[str_page];
  Generate();
  PageStack.clear();
}

void BaseMenu::Create(){
}

void BaseMenu::Generate(){
}

void BaseMenu::SetMenuBackGround(char *menubg_){
  MenuPage::SetMenuBackGround(menubg_);
}

void BaseMenu::Update(){
  if(current_page->ExecuteFlag()){
    menuEntry *execute_data = current_page->GetExectueData();
    if(execute_data->Action() == "goto page"){
      current_page->ClearExecuteFlag();
      GotoNextPage((char *)execute_data->ActionArgument(0).c_str());
    }
    else if(execute_data->Action() == "return") GotoLastPage();
  }
}

int BaseMenu::GotoNextPage(char *page){
  string newpage = page;
  if(Pages.find(newpage) == Pages.end()) return Error;
  PageStack.push_back(current_page);
  current_page = Pages[newpage];
  current_page->Reset();
  Generate();
  return Ok;
}

int BaseMenu::GotoLastPage(){
  if(PageStack.size() < 1) return False;
  current_page = PageStack.back();
  PageStack.pop_back();
  current_page->Reset();
  Generate();
  return True;
}

void BaseMenu::Draw(SDLscreen *Screen){
  current_page->Draw(Screen);
}

void BaseMenu::AddPage(char *name, char *title, unsigned int px, unsigned int py){
  MenuPage *newPage = new MenuPage;
  newPage->SetName(name, title);
  newPage->SetPosition(px, py);
  string str_name = name;
  Pages[str_name] = newPage;
  current_page = newPage;
}

//_____________________________________________________________

GameMenu::GameMenu(){
  current_page = NULL;
  change_key = NULL;
}

void GameMenu::Create(map<string, unsigned int> *controls){
  xv = 0;
  yv = 0;
  control_ptr["climb up"]   = &(*controls)["climb up"];
  control_ptr["climb down"] = &(*controls)["climb down"];
  control_ptr["left"]   = &(*controls)["left"];
  control_ptr["right"]  = &(*controls)["right"];
  control_ptr["jump"]   = &(*controls)["jump"];
  control_ptr["attack"] = &(*controls)["attack"];
  control_ptr["scroll"] = &(*controls)["scroll"];

//Convert here
  AddPage("start page", "Start Page", (unsigned int)(SCREEN_WIDTH/2), (unsigned int)((SCREEN_HEIGHT/2)*1.2));
    current_page->AddEntry("New Game", "start");
    current_page->AddEntry("Options", "goto page");
    current_page->AddActionArgument("options");
    current_page->AddEntry("Quit", "quit");

//Convert here
  AddPage("pause", "Pause Page", (unsigned int)(SCREEN_WIDTH/2), (unsigned int)((SCREEN_HEIGHT/2)*1.2));
    current_page->AddEntry("Continue", "continue");
    current_page->AddEntry("New Game", "start");
    current_page->AddEntry("Options", "goto page");
    current_page->AddActionArgument("options");
    current_page->AddEntry("Quit", "quit");

//Convert here
  AddPage("options", "Options", (unsigned int)(SCREEN_WIDTH/2), (unsigned int)(SCREEN_HEIGHT/2*1.2));
    current_page->AddEntry("Set Volume", "goto page");
    current_page->AddActionArgument("blah");
    current_page->AddEntry("Controls", "goto page");
    current_page->AddActionArgument("controls");
    current_page->AddEntry("Return", "return");

//Convert here
  AddPage("controls", "Controls", (unsigned int)(SCREEN_WIDTH/2), (unsigned int)(SCREEN_HEIGHT/2*1.3));
  SetPage("start page");
}

void GameMenu::Generate(){
  string name = current_page->GetName();
  if(name == "controls"){
    current_page->ClearEntries();

    CurrentPage()->AddEntry("Climb up:   %e", "change key");
    CurrentPage()->AddActionArgument("climb up");
    if(control_ptr.find("climb up")   != control_ptr.end()) CurrentPage()->AddPointerToFormat((void *)control_ptr["climb up"]);

    current_page->AddEntry("Climb down: %e", "change key");
    current_page->AddActionArgument("climb down");
    if(control_ptr.find("climb down") != control_ptr.end()) current_page->AddPointerToFormat((void *)control_ptr["climb down"]);

    CurrentPage()->AddEntry("Walk left:  %e", "change key");
    CurrentPage()->AddActionArgument("left");
    if(control_ptr.find("left")   != control_ptr.end()) CurrentPage()->AddPointerToFormat((void *)control_ptr["left"]);

    CurrentPage()->AddEntry("Walk right: %e", "change key");
    CurrentPage()->AddActionArgument("right");
    if(control_ptr.find("right")  != control_ptr.end()) CurrentPage()->AddPointerToFormat((void *)control_ptr["right"]);

    CurrentPage()->AddEntry("Jump:       %e", "change key");
    CurrentPage()->AddActionArgument("jump");
    if(control_ptr.find("jump")   != control_ptr.end()) CurrentPage()->AddPointerToFormat((void *)control_ptr["jump"]);

    CurrentPage()->AddEntry("Scroll runes: %e", "change key");
    CurrentPage()->AddActionArgument("scroll");
    if(control_ptr.find("scroll") != control_ptr.end()) CurrentPage()->AddPointerToFormat((void *)control_ptr["scroll"]);

    CurrentPage()->AddEntry("Rune Attack:  %e", "change key");
    CurrentPage()->AddActionArgument("attack");
    if(control_ptr.find("attack") != control_ptr.end()) CurrentPage()->AddPointerToFormat((void *)control_ptr["attack"]);

    current_page->AddEntry("Return", "return");
  }
}

void GameMenu::SetLogo(char *logo_){
  logo.image = SDLscreen::GameSprites.Load(logo_);
}

void GameMenu::SetMusic(char *music_){
  music = music_;
}

char *GameMenu::GetMusic(){
  if(music.size() > 0) return (char *)music.c_str();
  return NULL;
}

void GameMenu::SetOpeningBackGround(char *openingbg_){
  BackgroundLand.SetProperties(NULL, openingbg_, 1, 1, 1, 1);
}

void GameMenu::Update(){
  xv+=3;
  if(current_page->ExecuteFlag()){
    menuEntry *execute_data = current_page->GetExectueData();
    if(execute_data->Action() == "goto page"){
      current_page->ClearExecuteFlag();
      GotoNextPage((char *)execute_data->ActionArgument(0).c_str());
    }
    else if(execute_data->Action() == "return") GotoLastPage();

    else if(execute_data->Action() == "change key"){
      if(control_ptr.find(execute_data->ActionArgument(0)) != control_ptr.end()){
        change_key = control_ptr[execute_data->ActionArgument(0)];
        *change_key = 0;
      }
    }
  }
}

void GameMenu::Draw(SDLscreen *Screen, bool draw_background, bool draw_logo){
  if(draw_background) BackgroundLand.Draw(Screen, xv, yv, 0);
  if(current_page != NULL) current_page->Draw(Screen);
//Convert here
  Screen->DrawObject(&logo, (SCREEN_WIDTH - logo.image->w)/2, SCREEN_HEIGHT/4 - logo.image->h/2);
}

//_____________________________________________________________

EditorMenu::EditorMenu(){
  current_page = NULL;
  current_object = NULL;
}

void EditorMenu::Create(double margin_, map<string, BaseObject *> *templates_){
  margin = margin_;
  map<string, BaseObject *>::iterator i;
  string classtype;
  template_map = templates_;
  for(i = templates_->begin(); i != templates_->end(); i++){
    classtype = (*i).second->GetClassType();
    if(classtype == "RuneObject") continue;
    templates.push_back((*i).second);
  }
  AddPage("object page", "Object Page", (unsigned int)(SCREEN_WIDTH*(1 - margin)), 0);
  SetPage("object page");
}

void EditorMenu::Generate(){
  string name = current_page->GetName();
  string objname;
  if(name == "object page"){
    current_page->ClearEntries();
    current_page->AddEntry("None", "change object", 10, 10);
    current_page->AddActionArgument("null");

    unsigned int j = 0;
    for(unsigned int i = 0; i != templates.size() && j < 22; i++){
      objname = templates[i]->GetClassType();
      if(objname == "BulletObject" || objname == "PowerBar") continue;
      current_page->AddEntry(templates[i]->GetName(), "change object", 10, 30 + j*20);
      current_page->AddActionArgument(templates[i]->GetName());
      j++;
    }
  }
}

void EditorMenu::Update(){
  if(current_page->ExecuteFlag()){
    menuEntry *execute_data = current_page->GetExectueData();
    if(execute_data->Action() == "goto page"){
      current_page->ClearExecuteFlag();
      GotoNextPage((char *)execute_data->ActionArgument(0).c_str());
    }
    else if(execute_data->Action() == "return") GotoLastPage();
    else if(execute_data->Action() == "change object"){
      if(execute_data->ActionArgument(0) == "null") current_object = NULL;
      else current_object = (*template_map)[execute_data->ActionArgument(0)];
    }
  }
}

void EditorMenu::Draw(SDLscreen *Screen){
  if(current_page != NULL) current_page->Draw(Screen, true, false, (unsigned int)(SCREEN_WIDTH*margin), SCREEN_HEIGHT);
}

