#include "FpsManager.h"
#include "SDL2/SDL.h"

struct fpsManagerInstance
{
    unsigned int lastTime;
    float deltaTime;
    int elapsedTime;
};

void fpsListen(void *self)
{
    unsigned int currentTime = SDL_GetTicks();

    FpsManagerInstance *instance = ((FpsManager *)self)->instance;

    float dt = (currentTime - instance->lastTime) * (TARGET_FRAME_RATE / 1000.0);
    instance->deltaTime = dt;
    instance->elapsedTime = (int)(currentTime - instance->lastTime);
    instance->lastTime = currentTime;
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
    self.listen = fpsListen;
    self.getDelta = fpsGetDelta;
    self.getElapsedTime = fpsGetElapsedTime;
    self.destroy = destroyFpsManager;

    self.instance->lastTime = 1;
    self.instance->deltaTime = 16.0;

    return &self;
}
