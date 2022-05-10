#include "Animation.h"
#include "TextureManager.h"
#include <stdio.h>
#include <stdbool.h>

struct animationInstance
{
    int row;
    int currentframe;
    int animSpeed;
    int totalFrame;
    int spriteWidth;
    int spriteHeight;
    char *id;
    bool infinity;
    SDL_RendererFlip flip;
};

void update(void *self)
{
    AnimationInstance *instance = ((Animation *)self)->instance;
    instance->currentframe = (SDL_GetTicks() / instance->animSpeed) % instance->totalFrame;

    if (instance->infinity==false){
        instance->row = 7;
        instance->totalFrame =7;
        instance->currentframe = instance->totalFrame-1;
    }

}
void drawAnimationFrame(void *self, float x, float y, float scale)
{
    AnimationInstance *instance = ((Animation *)self)->instance;
    TextureManager *textureManager = getTextureManager();
    textureManager->drawFrame(textureManager, instance->id, x, y, instance->spriteWidth, instance->spriteHeight, instance->row, instance->currentframe, scale, instance->flip);

}
void set(void *self, char *textureId, int spriteWidth, int spriteHeight, int row, int totalFrame, int animSpeed, SDL_RendererFlip flip,bool infinity)
{
    AnimationInstance *instance = ((Animation *)self)->instance;
    instance->row = row;
    instance->totalFrame = totalFrame;
    instance->animSpeed = animSpeed;
    instance->flip = flip;
    instance->id = textureId;
    instance->spriteWidth = spriteWidth;
    instance->spriteHeight = spriteHeight;
    instance->infinity = infinity;
    
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