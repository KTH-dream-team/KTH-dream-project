#include "Warrior.h"
#include "Animation.h"
#include "EventHandler.h"
#include "TextureManager.h"


struct warriorInstance
{
    Animation *animation;
};

void updateWarrior(void*self)
{
    Animation *anim = ((Warrior*)self)->instance->animation;
    anim->update(anim);
}
void renderWarrior(void*self)
{
    Animation *anim = ((Warrior*)self)->instance->animation;
    anim->draw(anim, 55, 55);
}
void warriorEventHandle(void*self)
{
    EventHandler *eventHandler = getEventHandler();
    eventHandler->listen(eventHandler);

    Animation *anim = ((Warrior*)self)->instance->animation;

    if(eventHandler->getKeyPress(eventHandler,SDL_SCANCODE_A))
    {
        anim->set(anim, "warrior", 32, 32, 0, 13, 90, 0);
    }
    if(eventHandler->getKeyPress(eventHandler,SDL_SCANCODE_S))
    {
        anim->set(anim, "warrior", 32, 32, 7, 7, 90, 0);
    }
    if(eventHandler->getKeyPress(eventHandler,SDL_SCANCODE_D))
    {
        anim->set(anim, "warrior", 32, 32, 3, 10, 90, 0);
    }
}
void destroyWarrior(void *self)
{
    Warrior *warrior = ((Warrior*)self);

    warrior->instance->animation->destroy(warrior->instance->animation);

    free(warrior->instance);
    free(warrior);

    printf("Warrior destroyed\n");
}

Warrior *createWarrior()
{
    Warrior *self = malloc(sizeof(Warrior));
    self->instance = malloc(sizeof(WarriorInstance));
    
    self->instance->animation = newAnimation();
    self->instance->animation->set(self->instance->animation, "warrior", 32, 32, 0, 13, 90, 0);

    self->update = updateWarrior;
    self->eventHandler = warriorEventHandle;
    self->destroy = destroyWarrior;
    self->render = renderWarrior;
    
    TextureManager *texterManager = getTextureManager();
    texterManager->load(texterManager, "warrior", "./assets/WariorAnim.png");

    return self;
}