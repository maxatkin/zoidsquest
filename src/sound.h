#ifndef _SOUND_H_
#define _SOUND_H_

#include "SDL_mixer.h"
#include <string>
#include <map>

#define MUTE 0

using namespace std;

class GameAudio;
int OpenAudio(unsigned int audio_rate, unsigned int audio_format, unsigned int audio_channels, unsigned int audio_buffers);
void CloseAudio();

struct sound_info{
  Mix_Chunk *sound;
  unsigned int volume;
};

class Sounds{
  public:
    Mix_Chunk *LoadSample(char *file);
    map<string, Mix_Chunk *> SoundMap;
  private:
};

class SoundContainer{
  public:
    ~SoundContainer();
    void AddSample(char *name, char *file);
    void CopySounds(SoundContainer *newSounds);
    string Print();
    sound_info *operator[](string i);
    map<string, sound_info *> *GetSounds(){ return &sounds; };
    void GetMapEntry(unsigned int k, string *soundname, struct sound_info *retsnd);
    unsigned int Number(){return sounds.size();}
  private:
    map<string, sound_info *> sounds;  
    map<string, string> InverseSoundMap;
};

class GameAudio{
  public:
    static Sounds GameSounds;

    GameAudio();
    ~GameAudio();
    static void AddToSampleQueue(struct sound_info *snd_info);
    void PlayQueue();
    void SetMusic(char *music_file);
    void PlayMusic();
    void SetVolume(unsigned int new_volume);
  private:
    Mix_Music *game_song;
    static vector<sound_info *> sample_queue;
    unsigned int volume;
};

#endif
