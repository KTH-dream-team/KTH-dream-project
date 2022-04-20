#include "Animation.h"
#include "TextureManager.h"
#include <stdio.h>

struct animationInstance
{
    int row;
    int currentframe;
    int animSpeed;
    int totalFrame;
    int spriteWidth;
    int spriteHeight;
    char *id;
    SDL_RendererFlip flip;
};

void update(void *self)
{
    AnimationInstance *instance = ((Animation *)self)->instance;
    instance->currentframe = (SDL_GetTicks() / instance->animSpeed) % instance->totalFrame;
}
void drawAnimationFrame(void *self, float x, float y)
{
    AnimationInstance *instance = ((Animation *)self)->instance;

    TextureManager *textureManager = getTextureManager();
    textureManager->drawFrame(textureManager, instance->id, x, y, instance->spriteWidth, instance->spriteHeight, instance->row, instance->currentframe, instance->flip);
}
void set(void *self, char *textureId, int spriteWidth, int spriteHeight, int row, int totalFrame, int animSpeed, SDL_RendererFlip flip)
{
    AnimationInstance *instance = ((Animation *)self)->instance;

    instance->row = row;
    instance->totalFrame = totalFrame;
    instance->animSpeed = animSpeed;
    instance->flip = flip;
    instance->id = textureId;
    instance->spriteWidth = spriteWidth;
    instance->spriteHeight = spriteHeight;
}
void destroyAnim(void *self)
{
    free(((Animation *)self)->instance);
    free((Animation *)self);
    printf("Destroyed Animation");
}

Animation *newAnimation()
{
    Animation *self = malloc(sizeof(Animation));

    self->instance = malloc(sizeof(AnimationInstance));
    self->update = update;
    self->draw = drawAnimationFrame;
    self->set = set;
    self->destroy = destroyAnim;

    self->instance->currentframe = 0;

    return self;
}