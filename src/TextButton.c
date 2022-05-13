#include "SDL2/SDL_ttf.h"
#include <stdbool.h>
#include "text.h"
#include "string.h"
#include "GameEngin.h"
#include "TextButton.h"
#include "InputHandler.h"
#include "CollisionManager.h"
#include <stdio.h>

struct buttonInstance{
    SDL_Rect destRect;
    SDL_Color backgroundColor;
    Text *text;
    int btnState;
};



int getStateTextButton(void *self)
{
	return ((TextButton*)self)->instance->btnState;
}

void updateTextButton(void *self)
{
    ButtonInstance *instance = ((TextButton*)self)->instance;
    InputHandler *input = getInputHandler();
    CollisionManager *collision = GetCollisionManager();
    int mouseX, mouseY;
    unsigned int mouseState;
    mouseState = input->getMouseState(&mouseX, &mouseY);

    if(!(collision->PointVsRect(mouseX, mouseY, &instance->destRect)))
    {
        instance->btnState = 0;
        return;
    }
    
    instance->btnState = 1;
    if(mouseState == SDL_BUTTON_LEFT)
    {
        instance->btnState = 2;
    }

}

void SetBtnColor(void *self, SDL_Color bgColor, SDL_Color textColor){
    ButtonInstance *instance = ((TextButton*)self)->instance;
    instance->backgroundColor = bgColor;
    instance->text->setColor(instance->text, textColor);
}


void renderTextButton(void *self)
{
	ButtonInstance *instance = ((TextButton*)self)->instance;
    GameEngin *GE = getGameEngin();
    SDL_Renderer *renderer = GE->getRenderer(GE);
	SDL_Color c = instance->backgroundColor;
	SDL_SetRenderDrawColor(renderer, c.r , c.g , c.b , c.a); 
    SDL_RenderFillRect(renderer,&instance->destRect);
	instance->text->render(instance->text);
}

void destroyTextButton(void *self)
{
    TextButton *button = (TextButton*)self;
    button->instance->text->destroy(button->instance->text);
    free(button->instance);
    free(button);
}

TextButton *newTextButton(char *text, SDL_Color textColor, SDL_Color backgroundColor, int size, SDL_Rect destRect)
{
    TextButton *self = malloc(sizeof(TextButton));
    self->instance = malloc(sizeof(ButtonInstance));
    self->render = renderTextButton;
    self->changeColor = SetBtnColor;
    self->update = updateTextButton;
    self->destroy = destroyTextButton;
    self->getStateTextButton = getStateTextButton;
    self->instance->backgroundColor = backgroundColor;
    self->instance->text = newText(text,destRect.x, destRect.y,size,textColor);
	self->instance->text->centerText(self->instance->text,destRect);
    self->instance->text->render(self->instance->text);
	self->instance->destRect = destRect;
	self->instance->btnState = 0;
	
    return self;
}