#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL_mixer.h"
#include "audio.h"
#include <string.h>

struct audioInstance
{
    Mix_Music *music;
    Mix_Chunk *jump;
    Mix_Chunk *gun;
    Mix_Chunk *brick;
};


void initAudio(void *self){
    Audio *audio = (Audio*)self;
   //! lower frequency if crash here
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048))
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
    if(strcmp("jump", path) == 0){
        Mix_PlayChannel(3, audio->instance->jump, 0);
        Mix_Volume(3,30);
    }
    if(strcmp("gun", path) == 0){
        Mix_PlayChannel(4, audio->instance->gun, 0);
        Mix_Volume(4,30);
    }
    if(strcmp("brick", path) == 0){
        Mix_PlayChannel(5, audio->instance->brick, 0);
        Mix_Volume(5,30);
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
    Mix_FreeChunk(audio->instance->brick);
    Mix_CloseAudio();
}

Audio *newAudio(){
    Audio *self = malloc(sizeof(Audio));
    self->instance = malloc(sizeof(AudioInstance));
    self->instance->jump = Mix_LoadWAV("assets/jump.wav");
    self->instance->gun = Mix_LoadWAV("assets/gun.wav");
    self->instance->brick = Mix_LoadWAV("assets/brick.wav");

    self->backgroud = backgroundMusic;
    self->playSound = playSound;
    self->destroy = destroyAudio;
    self->init = initAudio;

    return self;
}
