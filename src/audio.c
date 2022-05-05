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
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0){
        printf("Initialising audio Error, %s\n", Mix_GetError());
    } else{
        printf("Audio initialised\n");
    }
}

void backgroundMusic(void *self, char *path, int volume){
    Audio *audio = (Audio*)self;
    //initAudio();
    Mix_VolumeMusic(volume);
    audio->instance->music = Mix_LoadMUS(path);
    if(Mix_PlayMusic(audio->instance->music, -1) !=0){
        printf("back.wav cant play, %s\n", Mix_GetError());
    }
    
}

void playSound(void *self, char *path, int volume){
    Audio *audio = (Audio*)self;
    //initAudio();
    audio->instance->sound = Mix_LoadWAV(path);
    if(Mix_PlayChannel(3, audio->instance->sound, 0)){
        // printf("Cant play sound %s, %s\n", path , Mix_GetError());//!error messege of
    }
    Mix_Volume(3,volume);
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
