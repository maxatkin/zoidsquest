#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <string>
#include <map>
#include <vector>
#include <unistd.h>
#include "main.h"
#include "SDLdraw.h"
#include "werror.h"
#include "objects.h"
#include "parser.h"

#define Background 0
#define Foreground 1

using namespace std;

class Level;
class Land;

class Land{
  public:
    map<string, BlockObject *> Block;

    Land();
    ~Land();
    void SetProperties(char *level, char *bg, int w, int h, int bs, int ts);
    void AddBlock(BlockObject *template_block); //, int w, int h, char c, int solid);
    void AddBlock2(char *name, char c);
    void Draw(SDLscreen *Screen, int xv, int yv, unsigned int layer);
    void ClearCurrentBlocks();
    void Reset();
    void ModifyLayout(unsigned int x, unsigned int y, char c);
    void Update();
    string Print();
    int  CheckObjectSpace(BaseObject *object, int mode);

  private:
    vector<BlockObject *> current_blocks;
    map<char, BlockObject *> Blockkey;
    struct sprite_info background;
    char *level;
    int blocksize;
    int tilesize;
    int width;
    int height;
    int background_offset;
    double scale;
};

class Level{
  public:
    Level();
    virtual ~Level();
    int Load(const char *levelfile, map<string, BaseObject *> *global_templates);
    void Restart();
    int  Reset();
    void SetViewWindow(int vx, int vy);
    void MoveViewWindow(int dvx, int dvy);
    int AddObject(char *cobject, double cx, double cy, double cxv, double cvy);
    char *Music();
    static void SetScale(int blocksize, int tilesize);
    virtual void Draw(SDLscreen *Screen);
    virtual void Update();

    Land Terrain;
    PlayerObject *Player;

  protected:
    void PlaceObjects();
    unsigned int FindCollisions(BaseObject *objectToMove, unsigned int inherited_mass = 0, bool usemode = false, int mode = 0);
    void FindObjectsToUpdate();
    void RemoveObject(int offset);
    int FindObjectCollisions(BaseObject *object_to_check, int mode = 0);

    Parser LevelParser;
    map<string, BaseObject *> templates;
    vector<BaseObject *> levelobjects;
    vector<BaseObject *> objectstore;
    vector<BaseObject *> updateobjects;
    string music;
    int xview;
    int yview;
    int width;
    int height;
    int reset;
    static int blocksize;
    static int tilesize;
    static double scale;
};


#endif
