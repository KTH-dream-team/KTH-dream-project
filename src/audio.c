#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL_mixer.h"
#include "audio.h"

struct audioInstance
{
    Mix_Music *music;
    Mix_Chunk *sound;

};


void initAudio(){
    printf("Audio_init_1\n");
    int a = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);//! lower frequency if crash here
    printf("initAudio: %d\n", a);
    if(a != 0)
    {
        printf("Audio_init_2\n");
        printf("Initialising audio Error, %s\n", Mix_GetError());
    } 
    else
    {
        printf("Audio_init_3\n");
        printf("Audio initialised\n");
    }
    printf("Audio_init_4\n");

}


void backgroundMusic(void *self, char *path, int volume){
    Audio *audio = (Audio*)self;
    //initAudio();
    Mix_VolumeMusic(volume);
    audio->instance->music = Mix_LoadMUS(path);
    if(Mix_PlayMusic(audio->instance->music, -1) !=0){
        printf("Cant play sound %s, %s\n", path , Mix_GetError());
    }
}

void playSound(void *self, char *path, int volume, int channel){
    Audio *audio = (Audio*)self;
    //initAudio();
    audio->instance->sound = Mix_LoadWAV(path);
    if(Mix_PlayChannel(channel, audio->instance->sound, 0)){
        printf("Cant play sound %s, %s\n", path , Mix_GetError());//!error messege of
    }
    Mix_Volume(channel,volume);
}

void destroyAudio(void *self){
    Audio *audio = (Audio*)self;
    if(Mix_PlayingMusic()){
        Mix_HaltMusic();
    }
    Mix_FreeMusic(audio->instance->music);
    Mix_FreeChunk(audio->instance->sound);
    Mix_CloseAudio();
}

Audio *newAudio(){
    Audio *self = malloc(sizeof(Audio));
    self->instance = malloc(sizeof(AudioInstance));
    self->backgroud = backgroundMusic;
    self->playSound = playSound;
    self->destroy = destroyAudio;
    self->init = initAudio;

    return self;
}
