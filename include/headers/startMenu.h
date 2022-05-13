
typedef struct startmenuinstance StartMenuInstance;
typedef struct startmenu
{
    void(*render)(void *self);
    void(*update)(void *self);
    void(*destroy)(void *self);
    bool (*isRunning)(void *self);
    StartMenuInstance *instance;
}StartMenu;

StartMenu *getStartMenu();