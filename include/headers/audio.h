#pragma once

typedef struct audioInstance AudioInstance;

typedef struct audio{
<<<<<<< Updated upstream
    void (*init)();
    void (*backgroud) (void *self, char *path, int volume);
    void (*playSound) (void *self, char *path, int volume);
=======
    void (*init)(void *self);
    void (*backgroud) (void *self, int volume);
    void (*playSound) (void *self, char *path, int volume, int channel);
>>>>>>> Stashed changes
    void (*destroy) (void *self);
    

    AudioInstance *instance;    
}Audio;

Audio *newAudio();