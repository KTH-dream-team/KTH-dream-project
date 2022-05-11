#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL_mixer.h"
#include "audio.h"
#include <string.h>

struct audioInstance
{
    Mix_Music *music;
<<<<<<< Updated upstream
    Mix_Chunk *sound;
};

void initAudio(){
    printf("Audio_init_1\n");
    int a = Mix_OpenAudio(24100, MIX_DEFAULT_FORMAT, 2, 2048);//! lower frequency if crash here
    printf("initAudio: %d\n", a);
=======
    Mix_Chunk *jump;
    Mix_Chunk *gun;
    Mix_Chunk *brick;
};


void initAudio(void *self){
    Audio *audio = (Audio*)self;
    int a = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);//! lower frequency if crash here
>>>>>>> Stashed changes
    if(a != 0)
    {
        printf("Initialising audio Error, %s\n", Mix_GetError());
    } 
    printf("Audio_init\n");
}

void backgroundMusic(void *self, char *path, int volume){
    Audio *audio = (Audio*)self;
<<<<<<< Updated upstream
    initAudio();
    Mix_VolumeMusic(volume);
    audio->instance->music = Mix_LoadMUS(path);
    if(Mix_PlayMusic(audio->instance->music, -1) !=0){
        printf("back.wav cant play, %s\n", Mix_GetError());
=======
    Mix_VolumeMusic(volume);
    strcmp("back", path);
    audio->instance->music = Mix_LoadMUS("assets/back.wav");
    if(Mix_PlayMusic(audio->instance->music, -1) != 0){
        printf("Cant play sound %s, %s\n", path , Mix_GetError());
>>>>>>> Stashed changes
    }
    
}

void playSound(void *self, char *path, int volume){
    Audio *audio = (Audio*)self;
<<<<<<< Updated upstream
    initAudio();
    audio->instance->sound = Mix_LoadWAV(path);
    if(Mix_PlayChannel(3, audio->instance->sound, 0)){
        // printf("Cant play sound %s, %s\n", path , Mix_GetError());//!error messege of
    }
    Mix_Volume(3,volume);
=======
    if(strcmp("jump", path) == 0){
        Mix_PlayChannel(channel, audio->instance->jump, 0);
        Mix_Volume(channel,volume);
    }
    if(strcmp("gun", path) == 0){
        Mix_PlayChannel(channel, audio->instance->gun, 0);
        Mix_Volume(channel,volume);
    }
    if(strcmp("brick", path) == 0){
        Mix_PlayChannel(channel, audio->instance->brick, 0);
        Mix_Volume(channel,volume);
    }
>>>>>>> Stashed changes
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
    static Audio self;
    if(self.instance != NULL)
        return &self;

    self.instance = malloc(sizeof(AudioInstance));
    self.instance->jump = Mix_LoadWAV("assets/jump.wav");
    self.instance->gun = Mix_LoadWAV("assets/gun.wav");
    self.instance->brick = Mix_LoadWAV("assets/break.wav");
    self.backgroud = backgroundMusic;
    self.playSound = playSound;
    self.destroy = destroyAudio;
    self.init = initAudio;
    return &self;
}
