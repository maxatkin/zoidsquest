#ifndef _OBJECTS_H_
#define _OBJECTS_H_

#include <vector>
#include <map>
#include <string>

#include "main.h"
#include "SDLdraw.h"
#include "werror.h"
#include "objects.h"
#include "sprites.h"
#include "sound.h"

#define Detect_Div 10

#define DEAD 0
#define ALIVE 1

#define TopSide 0x00
#define LeftSide 0x01
#define RightSide 0xfe
#define BottomSide 0xff
#define Check_Vertical 0
#define Check_Horizontal 1

#define DirRight 0
#define DirLeft 1

#define Rune_Number 
#define Template 0
#define Level_Object 1

using namespace std;

class PropertyContainer;
class BaseObject;
class LevelObject;
class PlayerObject;
class EnemyObject;
class Spells;
class RuneObject;

int CheckObjectCollision(BaseObject *object1, BaseObject *object2, int mode);

struct CollisionInfo{
  double cx;  //Coordinates of collision
  double cy;
  double bcx; //Coordinates just before collision
  double bcy;
  unsigned char side;
  unsigned char was_i_moving;
  BaseObject *Object;
};

struct boundary{
  double gx0;
  double gy0;
  double gx1;
  double gy1;
};

class BoundaryMode{
  public:
    ~BoundaryMode();
    void Add(char *name, double gx0_, double gy0_, double gx1_, double gy1_);
    void Use(char *name, double *retx0, double *rety0, double *retx1, double *rety1);
    void CopyBoundaries(BoundaryMode *CopyThis);
    void GetMapEntry(unsigned int k, string *modename, struct boundary *retbnd);
    unsigned int Number(){ return boundary_modes.size(); }
    string Print();
  private:
    map<string, boundary *> boundary_modes;    
};

class PropertyContainer{
  public:
    void SetProperty(char *property);
    void DeleteProperty(char *property);
    int  HasProperty(char *property);
    void CopyProperties(PropertyContainer *CopyThis);
    void Clear();
    unsigned int Number();
    char *operator[](const int i);
    string Print();
  private:
    vector<string> Properties;  
};

class BaseObject{
  public:
    PropertyContainer Properties;
    SoundContainer SoundEffects;
    SpriteSequences SpriteAnimation;
    BoundaryMode BoundaryModes;

    BaseObject();
    virtual ~BaseObject();
    void SetName(char *setname);
    void SetBoundary(double x0, double y0, double x1, double y1);
    void SetPosition(double newx, double newy);
    void SetVelocity(double newvx, double newvy);
    void SetCollisionVelocity(double newdx, double newdy);
    void SetHitState(int hx, int hy);
    void SetMass(unsigned int mass_){ mass = mass_; }
    void SetClassType(const char *type);
    static void SetScale(double scale_);

    char *GetName(){ return (char *)name.c_str(); }
    void GetVelocity(double *vx, double *vy);
    void GetCollisionVelocity(double *retdx, double *retdy);
    void GetPosition(double *x, double *y);
    void GetOldPosition(double *ox, double *oy);
    void GetBoundary(double *retx0, double *rety0, double *retx1, double *rety1);
    void GetSize(int *w, int *h);
    void GetHitState(int *hx, int *hy);
    void GetCollisionQueue(vector<BaseObject *> *retQueue, bool filter = false, int side = 0); 
    void GetLastCollisions(vector<BaseObject *> *retQueue, bool filter = false, int side = 0); 
    void SaveCollisionQueueSize(){ collision_queue_size = collision_queue.size(); }
    unsigned int Mass(){ return mass; }
    char *GetClassType();

    void SetBoundaryToMode(char *modename);
    void SetCollisionInfo(double cx, double cy, char side, char was_i_moving, BaseObject *Object);
    void SaveOldPosition();
    void RestoreOldPosition();
    void ChangePosition(double dx, double dy);
    void ChangeVelocity(double dvx, double dvy);
    
    bool ObjectRequested(){ return request_object; }
    void GetRequestedObjectData(string& cobject_, double& cx_, double& cy_,
                                double& cvx_, double& cvy_);

    virtual void Draw(SDLscreen *Screen, int xv, int yv);
    virtual void Kill();
    virtual void MakeDying();
    virtual void PreUpdate();
    virtual void Update();
    virtual void FullUpdate();
    virtual void PostUpdate();
    virtual void UseTemplate(BaseObject *newobject);
    virtual void SaveState();
    virtual void Reset();
    virtual string Print(int mode = Template);

    int  GetState();
    int  IsPointInMe(double fx, double fy);

  protected:
    void ClearCollisionQueue();
    void CreateObject(char *cobject_, double cx_, double cy_, double cvx_, double cvy_);
    vector<struct CollisionInfo *> collision_queue;

    double x, y;
    double vx, vy;
    double gx0, gy0, gx1, gy1; //Define collision (boundary) box
    //double oldx, oldy, dx, dy; //Collision detection variables
    double oldvx, oldvy;
    double ivx, ivy;
    static double scale;
    static double inverse_scale;

    double inital_x, inital_y;    //Inital conditions
    double inital_vx, inital_vy;
    PropertyContainer SavedProperties;   

    string class_type;
    unsigned char state;
    unsigned int mass; // zero mass signifies infinite mas
    unsigned int collision_queue_size;
    string name;
    int type;
    int width;
    int height;
    int hitx;
    int hity;

    bool request_object;
    string cobject;
    double cx, cy, cvx, cvy;
  private:
};

/**** YUCK! I had to put the spell header in here to avoid circular dependencies :( ***/

class RuneObject: public BaseObject{
  public:
    void Draw(SDLscreen *Screen, int xv, int yv);
  private:
};

class Spells{
  public:
    Spells();
    ~Spells();
    void SetCaster(BaseObject *caster);
    void SetMana(double mana);
    double GetMana();
    void AddRune(char *name, RuneObject *template_rune);
    void Copy(Spells *CopyMagic);
    void FindUseableRunes();
    void ScrollRunesLeft();
    void ScrollRunesRight();
    void SelectRune();
    void DoSpell();
    unsigned int GetNumberOfRunes();
    void Update();
    void Draw(SDLscreen *Screen, int xv, int yv);
    double GetManaPercentage();

    vector<RuneObject *> GetRunes();
    vector<RuneObject *> GetUsedRunes();   
    struct sprite_info *GetManabar();
    map<string, RuneObject*> rune_key;

  private:
    BaseObject *Caster;
    RuneObject *current_rune;
    RuneObject *last_selected_rune;
    vector<RuneObject *> runes;
    vector<RuneObject *> runestore;
    vector<RuneObject *> spell;

    double dangle;
    double spell_angle;
    double spell_radius;
    double mana;
    double mana_charge;
    double max_mana;

    int attack;
};

/*************************************************/

class BlockObject: public BaseObject{
  public:
    BlockObject();
    void SetMapPosition(int mbx, int mby);
    int IsAtMapPosition(int mbx, int mby);
    void SetKey(char c);
    char GetKey();
    void UseTemplate(BaseObject *newobject);
    void SetLayer(unsigned int layer_){ layer = layer_; }
    unsigned int  GetLayer(){ return layer; }
    virtual string Print(int mode = Template); 
  private:
    int bx;
    int by;
    char key;
    unsigned int layer;
};

class MagicObject: public BaseObject{
  public:
    Spells Magic;
    MagicObject();
    void UseTemplate(BaseObject *newobject);
    void Draw(SDLscreen *Screen, int xv, int yv);
    virtual void MagicAttack();

  private:
};

class PlayerObject: public MagicObject{
  public:
    PlayerObject();
    void ClimbUp();
    void ClimbDown();
    void Left();
    void Right();
    void Jump();
    void PreUpdate();
    void Update();
    void FullUpdate();
    void PostUpdate();
    void Kill();
    void MakeDying();
    void TakeHit();
    void GetMagic();
    void MagicAttack();
    void SetLives(int lives_);
    void SetScore(int score_);
    int Lives();
    int Score();

  private:
    void SolidHandler(struct CollisionInfo *collision_info);
    int i_can_jump;
    int i_can_climb;
    int i_am_climbing;
    int i_am_attacking;
    int i_am_in_water;
    int i_am_crouching;
    int time_to_jump;

    int invulnerable;
    int direction;
    int walk;
    int lives;
    int score;
};

class MovingObject: public BaseObject{
  public:
    MovingObject();
    ~MovingObject();
    void PreUpdate();
    void Reset();
    void Update();
    void AddRoutePoint(double speed, double direction, unsigned int ticks);
    virtual string Print(int mode = Template); 
  protected:
    struct RouteCoordinate{
      unsigned int ticks;  //number of times to increment position
      double speed;
      double direction;    //angle to horizontal
    };

    void FollowRoute();
    virtual void SolidHandler(struct CollisionInfo *collision_info) = 0;
    vector<struct RouteCoordinate *> route;
    unsigned int ticks;
    unsigned int next_route_point;
    int hit_top, hit_bottom;
    int hit_left, hit_right;
};

class MovingGroundObject: public MovingObject{
  public:
  protected:
    void SolidHandler(struct CollisionInfo *collision_info);
};

class MovingFlyingObject: public MovingObject{
  public:
  protected:
    void SolidHandler(struct CollisionInfo *collision_info);
};

class EnemyGroundObject: public MovingGroundObject{
  public:
    void MakeDying();
    void PreUpdate();
    void Update();
};

class FollowEnemyGroundObject: public MovingGroundObject{
  public:
    FollowEnemyGroundObject();
    void MakeDying();
    void PreUpdate();
    void Update();
    void FollowObject(BaseObject *follow);
  protected:
    BaseObject *object;
};

class ThrowerObject: public FollowEnemyGroundObject{
  public:
    ThrowerObject();
    void PreUpdate();
    void ThrowObject();
  private:
    unsigned int time_to_throw;
};

class BulletObject: public MovingGroundObject{
  public:
    BulletObject();
    void PreUpdate();
    void Update();
  private:
    unsigned int time_to_die;
};

class ManaJar: public MovingFlyingObject{
  public:
    void Kill();
    void Update();
  protected:
};

class FallingBlock: public BaseObject{
  public:
    FallingBlock();
    void PreUpdate();
    void Update();
    void FullUpdate();
  private:
    void SolidHandler(struct CollisionInfo *collision_info);
    int i_have_been_stepped_on;
    int count_down;
};

#endif
