typedef struct mapManagerInstance MapManagerInstance;

typedef struct mapManager{
    void(*showMap)(void *self);//draws the map on screen using drw from texture manager
    void(*initMap)(void *self);//initialize the map load textuers creat 2d array
    void(*dig)(void *self,int x,int y);//remove blocks from the map when clicking on mouse and holding Q
    void(*build)(void *self,int x,int y,int blockType);//place blocks on mp when clicking on mouse and holding E
    void(*destroyMap)(void *self);//destroy the map
    void(*show)(void *self);//debug

    MapManagerInstance *instance;
}MapManager;


MapManager *getMapManager();
