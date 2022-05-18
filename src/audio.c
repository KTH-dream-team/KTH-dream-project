#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL_mixer.h"
#include "audio.h"
#include <string.h>

struct audioInstance
{
    Mix_Music *music;
    Mix_Chunk *jump, *gun, *health, *gunPickup, *hitWarrior;
    // Mix_Chunk *gun;
    // Mix_Chunk *health;
    // Mix_Chunk *gunPickup;
    // Mix_Chunk *hitWarrior;
};


void initAudio(void *self){
    Audio *audio = (Audio*)self;
   //! lower frequency if crash here
    if(Mix_OpenAudio(24100, MIX_DEFAULT_FORMAT, 2, 2048))
    {
        printf("Initialising audio Error, %s\n", Mix_GetError());
    } 
    printf("Audio_init\n");
}



void backgroundMusic(void *self, int volume){
    Audio *audio = (Audio*)self;
    Mix_VolumeMusic(volume);
    audio->instance->music = Mix_LoadMUS("assets/back.wav");
    if(Mix_PlayMusic(audio->instance->music, -1) != 0){
        printf("Cant play background, %s\n", Mix_GetError());
    }
    
}


void playSound(void *self, char *path){
    Audio *audio = (Audio*)self;
    printf("try to play sound: %s\n", path);
    if(strcmp("jump", path) == 0){
        int a = Mix_PlayChannel(3, audio->instance->jump, 0);
        Mix_Volume(3,30);
        printf("play jump on chanel: %d\n", a);
    }
    if(strcmp("gun", path) == 0){
        int a = Mix_PlayChannel(4, audio->instance->gun, 0);
        Mix_Volume(4,30);
        printf("play jump on chanel: %d\n", a);
    }
    if(strcmp("health", path) == 0){
        Mix_PlayChannel(5, audio->instance->health, 0);
        Mix_Volume(5,30);
    }
    if(strcmp("gunPickup", path) == 0){
        Mix_PlayChannel(5, audio->instance->gunPickup, 0);
        Mix_Volume(5,30);
    }
    if(strcmp("hitWarrior", path) == 0){
        Mix_PlayChannel(6, audio->instance->hitWarrior, 0);
        Mix_Volume(6,20);
    }
}


void destroyAudio(void *self){
    Audio *audio = (Audio*)self;
    if(Mix_PlayingMusic()){
        Mix_HaltMusic();
    }
    Mix_FreeMusic(audio->instance->music);
    Mix_FreeChunk(audio->instance->jump);
    Mix_FreeChunk(audio->instance->gun);
    Mix_FreeChunk(audio->instance->health);
    Mix_FreeChunk(audio->instance->gunPickup);
    Mix_FreeChunk(audio->instance->hitWarrior);
    Mix_CloseAudio();
}

Audio *newAudio(){
    static Audio self;

    if(self.instance != NULL)
        return &self;

        if(Mix_OpenAudio(24100, MIX_DEFAULT_FORMAT, 2, 2048))
    {
        printf("Initialising audio Error, %s\n", Mix_GetError());
    } 
    printf("Audio_init\n");

    self.instance = malloc(sizeof(AudioInstance));
    self.instance->jump = malloc(sizeof(Mix_Chunk));
    self.instance->gun = malloc(sizeof(Mix_Chunk));
    self.instance->health = malloc(sizeof(Mix_Chunk));
    self.instance->gunPickup = malloc(sizeof(Mix_Chunk));
    self.instance->hitWarrior = malloc(sizeof(Mix_Chunk));

    self.instance->jump = Mix_LoadWAV("assets/jump.wav");
    self.instance->gun = Mix_LoadWAV("assets/gun.wav");
    self.instance->health = Mix_LoadWAV("assets/health.wav");
    self.instance->gunPickup = Mix_LoadWAV("assets/gunPickup.wav");
    self.instance->hitWarrior = Mix_LoadWAV("assets/hitWarrior.wav");

    self.backgroud = backgroundMusic;
    self.playSound = playSound;
    self.destroy = destroyAudio;
    self.init = initAudio;
    printf("Innit audio\n");

    return &self;
}
