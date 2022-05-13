
typedef struct startmenuinstance StartMenuInstance;
typedef struct startmenu
{
    void(*init)(void *self);
    void(*render)(void *self);
    void(*destroy)(void *self);
    bool (*isRunning)(void *self);
    StartMenuInstance *instance;
}StartMenu;

StartMenu *getStartMenu();