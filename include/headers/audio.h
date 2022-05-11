#pragma once

typedef struct audioInstance AudioInstance;

typedef struct audio{
    void (*init)(void *self);
    void (*backgroud) (void *self, int volume);
    void (*playSound) (void *self, char *path, int volume, int channel);
    void (*destroy) (void *self);
    

    AudioInstance *instance;    
}Audio;

Audio *newAudio();