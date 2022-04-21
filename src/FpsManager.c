#include "FpsManager.h"
#include "SDL2/SDL.h"
#include <stdio.h>
#define FPSARRAY_SIZE 100
//#include <SDL_TTF.h>

struct fpsManagerInstance
{
    unsigned int lastTime;
    float deltaTime;
    int elapsedTime;
    float fps[FPSARRAY_SIZE];
    int fpsCurrentIndex;
};

void fpsManagerListen(void *self)
{
    unsigned int currentTime = SDL_GetTicks();
    FpsManagerInstance *instance = ((FpsManager *)self)->instance;
    float dt = (currentTime - instance->lastTime) * (TARGET_FRAME_RATE / 1000.0);
    instance->deltaTime = dt;
    instance->elapsedTime = (int)(currentTime - instance->lastTime);
    instance->lastTime = currentTime;
}

void frameRateListen(void *self){
    FpsManagerInstance *instance = ((FpsManager *)self)->instance;
    float fps;
    if(instance->elapsedTime != 0)
        fps = 1000.0/instance->elapsedTime;
    else{
        fps=1000;
    }
    
    instance->fps[instance->fpsCurrentIndex]=fps;    
    // printf("test %f\n",fps);
    // printf("elapsed %d\n",instance->elapsedTime);

    instance->fpsCurrentIndex = instance->fpsCurrentIndex+1;

    if (instance->fpsCurrentIndex>=FPSARRAY_SIZE)
    {
        instance->fpsCurrentIndex=0;
    }
}

int getFrameRate(void *self){
    FpsManagerInstance *instance = ((FpsManager *)self)->instance;
    int fps=0;
    for (int i = 0; i < FPSARRAY_SIZE; i++)
    {
        fps += instance->fps[i];
    }
    fps=fps/FPSARRAY_SIZE;
    return (int)fps;
}

float fpsGetDelta(void *self)
{
    return ((FpsManager *)self)->instance->deltaTime;
}
int fpsGetElapsedTime(void *self)
{
    return ((FpsManager *)self)->instance->elapsedTime;
}
void destroyFpsManager(void *self)
{
    FpsManagerInstance *instance = ((FpsManager *)self)->instance;
    free(instance);
}
FpsManager *getFpsManager()
{
    static FpsManager self;

    if (self.instance != NULL)
        return &self;

    self.instance = malloc(sizeof(FpsManagerInstance));
    self.listen = fpsManagerListen;
    self.frameRateListen = frameRateListen;
    self.getDelta = fpsGetDelta;
    self.getElapsedTime = fpsGetElapsedTime;
    self.getFrameRate = getFrameRate;
    self.destroy = destroyFpsManager;
    

    self.instance->lastTime = 1;
    self.instance->deltaTime = 16.0;

    self.instance->fpsCurrentIndex = 0;


    return &self;
}
