#ifndef _INPUT_H_
#define _INPUT_H_

#include "SDL.h"
#include "main.h"

#define Escape 27
#define Space  0x20

using namespace std;

class Input;

struct state_of_key{
  int held;
  int down;
  int up;
};

class Input{
  public:
    Input();
    void Check();
    unsigned char IsKeyHeld(unsigned int key);
    unsigned char IsKeyDown(unsigned int key);
    unsigned char IsKeyUp(unsigned int key);
    unsigned int GetAnyInput(){ return last_key; }
    void GetMouseCoords(unsigned int& mx_, unsigned int& my_);
    unsigned char IsMouseButtonDown(int button);
    unsigned char IsMouseButtonUp(int button);
    unsigned char IsMouseButtonHeld(int button);
    unsigned char HasMouseMoved(){ return mouse_moved; }

  private:
    SDL_Event event;
    struct state_of_key keystate[SDLK_LAST];
    struct state_of_key mousestate[3];
    int mdx, mdy;
    int mx, my;
    unsigned int last_key;
    unsigned char mouse_moved;

    void UpdateKeys();
    void UpdateMouse();
};



#endif
