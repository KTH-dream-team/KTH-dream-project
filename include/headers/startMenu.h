
typedef struct startmenuinstance StartMenuInstance;
typedef struct startmenu
{
    void(*render)(void *self);
    void(*mouseInput)(void *self);
    void(*destroy)(void *self);
    void(*input)(void *self);
    char *(*getIP)(void *self);
    bool (*isRunning)(void *self);
    StartMenuInstance *instance;
}StartMenu;

StartMenu *getStartMenu();