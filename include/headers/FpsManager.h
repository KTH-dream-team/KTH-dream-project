

#define TARGET_FRAME_RATE 60

typedef struct fpsManagerInstance FpsManagerInstance;

typedef struct fpsManager
{
    void (*listen)(void*self);
    float (*getDelta)(void*self);
    void (*destroy)(void *self);

    FpsManagerInstance* instance;
} FpsManager;

FpsManager *getFpsManager();