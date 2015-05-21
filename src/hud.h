#ifndef _HUD_H_
#define _HUD_H_

#include "objects.h"
#include "text.h"

class HUDObject: public BaseObject{
  public:
    void Draw(SDLscreen *Screen, int x, int y);
  private:
};

class PowerBar: public HUDObject{
  public:
    PowerBar();
    void Draw(SDLscreen *Screen, int x, int y);    
    void SetPercentage(double new_percent);
  private:
    double percentage;
};

class HUD{
  public:
    void Draw(SDLscreen *Screen);
    int SetPowerBar(PowerBar *setpowerbar);
    PowerBar ManaBar;

  private:
};

#endif
