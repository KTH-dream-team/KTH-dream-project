// #include <SDL2/SDL.h>
// #include <stdbool.h>
// #include "GameEngin.h"
// #include "startMenu.h"
// #include "TextButton.h"
// #include "EntityManager.h"
// #include "TextureManager.h"
// #include "SDL2/SDL_image.h"

// struct startmenuinstance
// {
//     bool isRunning;
//     SDL_Surface *surface;
//     TextButton *button;
//     SDL_Renderer *renderer;
//     SDL_Window *window;
// };

// void initStartMenu(void *self)
// {
//     StartMenuInstance *instance = ((StartMenu *)self)->instance;
//     EntityManager *EM = getEntityManager();
//     GameEngin *Engine = getGameEngin();

//     //create startmenu background
//     TextureManager *textureManager = getTextureManager(textureManager);
//     textureManager->load(textureManager, "startmenu", "menu.jpg");
    

//     instance->renderer = Engine->getRenderer(Engine);
//     instance->window = Engine->getWindow(Engine);

//     instance->isRunning = true;
//     // if (!(instance->surface = IMG_Load("src/menu.jpg")))
//     //     printf("Couldn't find the Image\n");

//     // create a button on start menu
//     // SDL_Color txtColor = {255, 255, 255, 255};
//     // SDL_Color bgColor = {0, 0, 0, 255};
//     // SDL_Rect rect = {100, 100, 200, 60};
//     // instance->button = newTextButton("Start Game", txtColor, bgColor, 24, rect);
//     // EM->add(EM, "TextButton-1", instance->button);
    
// }

// void renderStartMenu(void *self)
// {
//     StartMenuInstance *instance = ((StartMenu *)self)->instance;
//     GameEngin *Engine = getGameEngin();
//     TextureManager *textureManager = getTextureManager(textureManager);

//     SDL_Rect srcRect = {0, 0, 1000, 500};
//     SDL_Rect destRect = {0, 0, 1000, 500};
//     textureManager->draw(textureManager, "startmenu", srcRect, destRect, 1); //! draw bakgrundsbild

//     // SDL_Texture *texture = SDL_CreateTextureFromSurface(instance->renderer, instance->surface);
//     // instance->button->render(instance->button);
//     // SDL_RenderCopy(instance->renderer, texture, NULL, NULL);
//     //SDL_RenderPresent(instance->renderer);


//     //printf("startMenu_renderStartMenu_5\n");
//     // SDL_RenderPresent(renderer);
//     // printf("startMenu_renderStartMenu_6\n");

//     // EntityManager *EM = getEntityManager();
//     // TextButton *button = EM->getByID(EM, "TextButton-1");
//     //  button->render(button);
//     //  int btnState = button->getBtnState(button);
//     //  if(btnState == 2){
//     //     printf("Is clicking here\n\n");
//     //     instance->isRunning = 0;
//     //  }
// }

// void destroyStartMenu(void *self)
// {
//     StartMenuInstance *instance = ((StartMenu *)self)->instance;
//     SDL_FreeSurface(instance->surface);
//     free(instance);
//     printf("Menu has been destroyed!\n");
// }

// bool startMenuIsRunning(void *self)
// {
//     return ((StartMenu *)self)->instance->isRunning;
// }

// StartMenu *getStartMenu()
// {
//     static StartMenu self;
//     if (self.instance != NULL)
//         return &self;
//     self.instance = malloc(sizeof(StartMenuInstance));
//     self.init = initStartMenu;
//     self.destroy = destroyStartMenu;
//     self.isRunning = startMenuIsRunning;
//     self.render = renderStartMenu;

//     self.instance->isRunning = false;
//     self.instance->surface = NULL;

//     return &self;
// }