/*
    FPSmanager gives important info which is needed to make prame rate indipended game.

    void (*listen)(void*self);
        update FPSmanager, need to be called o every frame;

    instance->elapsedTime = (int)(currentTime - instance->lastTime);
        return elapsed time. the time it took wheen frames in ms;

    float getDelta(void*self)
        return Delta. time it took between frames devices by (1000/TARGET_FRAME_RATE). if game runs on 60 fps the delta value should be 1, if the game run faster then 60 fps delta will be lest then 1.

    void (*destroy)(void *self);
        destroy FpsManager
*/

#define TARGET_FRAME_RATE 60

typedef struct fpsManagerInstance FpsManagerInstance;

typedef struct fpsManager
{
    void (*listen)(void *self);
    float (*getDelta)(void *self);
    int (*getElapsedTime)(void *self);
    void (*destroy)(void *self);

    FpsManagerInstance *instance;
} FpsManager;

FpsManager *getFpsManager();