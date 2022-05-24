#include "Camera.h"
#include "define.h"
#include "define.h"


struct cameraInstance
{
    SDL_Point position;
    SDL_Point cameraOffset;
};

void updateCamera(void *self)
{
    CameraInstance *instance = ((Camera*)self)->instance;
    SDL_Point *pos = &instance->position;

    int camX = -(pos->x - (SCREEN_WIDTH / 2));
    int camY = -(pos->y - (SCREEN_HEIGHT / 2));

    if(camX > 0) camX = 0;
    if(camY > 0) camY = 0;
    if(camX < -SCREEN_WIDTH) camX = -SCREEN_WIDTH;
    if(camY < -(SCREEN_HEIGHT+30)) camY = -(SCREEN_HEIGHT+30);

    int deltaX =  camX - instance->cameraOffset.x;
    int deltaY = camY - instance->cameraOffset.y ;

    instance->cameraOffset.x += deltaX * .03;
    instance->cameraOffset.y += deltaY * .03;
}

void setCameraPosition(void *self, int x, int y)
{
    CameraInstance *instance = ((Camera*)self)->instance;
    instance->position.x = x;
    instance->position.y = y;
}

SDL_Point getCameraOffset(void *self)
{
    return ((Camera*)self)->instance->cameraOffset;
}

Camera *getCamera()
{
    static Camera self;
    if(self.instance != NULL)
        return &self;

    self.instance = malloc(sizeof(CameraInstance));

    self.instance->cameraOffset.x = 0;
    self.instance->cameraOffset.y = 0;


    self.instance->position.x = 0;
    self.instance->position.y = 0;


    self.update = updateCamera;
    self.set = setCameraPosition;
    self.getCameraOffset = getCameraOffset;

    return &self;
}