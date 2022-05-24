#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "TextureManager.h"
#include "map.h"
#include "CollisionManager.h"
#include "networkClient.h"
#include "data.h"
#include "audio.h"
#include "EntityManager.h"
#include "Warrior.h"
#include "Camera.h"
#define ROW 25
#define COL 50
#define PRIVET static 
#define PUBLIC

struct mapManagerInstance {
    int map[ROW][COL];
};


bool chekBlockContact(void *self,int blockRow, int blockCol);

void initMap(void *self)
{    
    printf("in initMap\n");
    TextureManager *textureManager = getTextureManager();//! hämta befintlig textureManger instant obs inte en new
    textureManager->load(textureManager, "dirt", "./assets/dirt.png");//1 //!load texture function sicka id och fil namn 
    textureManager->load(textureManager, "grass", "./assets/grass.png");//2
    textureManager->load(textureManager, "grass.1", "./assets/grass.1.png");//3
    textureManager->load(textureManager, "stone", "./assets/stone.png");//4
    textureManager->load(textureManager, "wood", "./assets/wood.png");//5
    textureManager->load(textureManager, "gun", "./assets/gun.png");//6
    textureManager->load(textureManager, "brick", "./assets/brick.png");//7
    textureManager->load(textureManager, "black", "./assets/black.png");//8
    textureManager->load(textureManager, "healthPack", "./assets/healthPack.png");//9

    MapManager *mapmanager =(MapManager*)self;
    int map1[ROW][COL] ={ 
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,7,7,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,0,9,0,0,5,0,0,0,2,0,0,0,5,8,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,2,2,2,0,0,0,2,2,2,0,0,0,0,0,0,2,2,0,5,0,0,0,2,2,0,0,5,8,5,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,},
    {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,},
    {3,3,3,3,3,3,3,9,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
    {1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
    {1,1,0,6,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
    {1,4,1,4,4,1,4,1,4,4,1,1,4,1,4,4,1,4,1,0,0,0,0,0,9,4,1,1,1,4,1,1,1,4,1,4,4,4,4,9,1,4,1,1,1,1,4,4,1,4,},
    {1,4,4,1,4,1,4,4,1,4,1,4,4,1,1,1,4,4,4,1,1,1,1,1,1,1,1,4,1,4,1,1,4,4,1,4,4,4,1,4,1,4,4,4,4,4,4,1,4,4,},
    {4,1,1,1,4,4,1,4,1,4,4,4,1,1,4,1,1,1,4,1,4,4,1,4,4,1,4,4,4,1,1,1,4,1,0,0,6,0,4,1,4,4,4,1,4,4,1,1,1,1,},
    {1,4,1,4,4,4,1,1,4,4,1,1,1,4,1,4,1,1,4,1,1,4,4,1,1,1,1,4,1,4,1,1,4,0,0,0,1,1,4,4,4,1,4,4,4,1,1,1,1,4,},
    {4,4,4,1,1,1,4,4,4,4,1,4,1,1,4,4,1,4,1,4,4,1,4,1,1,4,4,4,4,1,4,4,1,0,4,4,1,4,4,4,1,4,4,1,4,4,1,4,4,1,},
    {1,1,1,1,4,4,4,4,1,4,1,1,1,4,1,1,4,1,1,1,1,1,4,4,4,9,4,1,1,1,4,4,1,1,1,1,4,4,9,1,1,4,4,4,1,1,4,4,4,1,},
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,},
    {4,4,4,9,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,},
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,},
    }; 

          for (int i = 0; i < ROW; i++){
            for (int j = 0; j < COL; j++){   
                mapmanager->instance->map[i][j]=map1[i][j];
            }
    }

}
//!function can bradcast data with udp
void build(void *self, int x,int y, int blockType){//!builds when holding E
    MapManager *mapmanager = (MapManager *)self;
    blockType = 1; 
    int blockCol = x/20;
    int blockRow = y/20;
    
    if (mapmanager->instance->map[blockRow][blockCol] == 0 && chekBlockContact(mapmanager,blockRow,blockCol))
    {
        mapmanager->instance->map[blockRow][blockCol] = blockType;
        NetworkClient *network = getNetworkClient();
        BlockBuild dataToSend = {
            network->getTCPID(network),
            blockCol,
            blockRow,
            blockType
    };
    network->TCPbroadCast(network, &dataToSend, sizeof(BlockBuild), 6);
    }
}

void buildNoSend(void *self, int x,int y, int blockType){
    MapManager *mapmanager = (MapManager *)self;
    mapmanager->instance->map[y][x] = blockType; 
}

//!function can bradcast data with udp
void dig(void *self,int x, int y){//!dig when holding Q
    MapManager *mapmanager = (MapManager *)self;
    int intBlockCol = x/20;
    int intBlockRow = y/20;
    //float blockCol = intBlockCol;
    //float blockRow = intBlockRow;
    mapmanager->instance->map[intBlockRow][intBlockCol] = 0;
    NetworkClient *network = getNetworkClient();
    BlockDestroy dataToSend = {
        network->getTCPID(network),
        intBlockCol,
        intBlockRow
    };
    // Audio *audio = newAudio();
    //audio->playSound(audio, "brick");
    network->TCPbroadCast(network, &dataToSend, sizeof(BlockDestroy), 5);
    
}
void digNoSend(void *self,int x, int y){
    MapManager *mapmanager = (MapManager *)self;
    mapmanager->instance->map[y][x] = 0;
}

bool chekBlockContact(void *self,int blockRow, int blockCol){//klass hjälp funktion kolla om block gör kontakt
    MapManager *mapmanager = (MapManager *)self;
    
    if (mapmanager->instance->map[blockRow][blockCol+1] != 0||
    mapmanager->instance->map[blockRow][blockCol-1] != 0||
    mapmanager->instance->map[blockRow+1][blockCol] != 0||
    mapmanager->instance->map[blockRow-1][blockCol] != 0)
    {
        return true;
    }
    return false;
}

int checkColision(void *self,SDL_Rect dRect, SDL_FPoint *dir, float dt,int collisionType,int *blockTypePtr){//

    CollisionManager *colisionManager = GetCollisionManager();
    MapManagerInstance *mapManagerInstance = ((MapManager *)self)->instance; 
    MapManager *map = ((MapManager *)self);
    
    EntityManager *EM = getEntityManager();
    Warrior *warrior000 = EM->getByID(EM,"Warrior-000");
    Audio *audio = newAudio();
    
    // WarriorInstance *warriorInstance = ((Warrior *)warrior000)->instance;
    int lowerBounce = ((dRect.y+dRect.h)/20)+2;
    int upperBounce = (dRect.y/20)-1;    
    int rightBounce = ((dRect.x+dRect.w)/20)+2;
    int leftBounce = (dRect.x/20)-1;
    int blockType=0;
    int returnBlock=0;
   // int rightBounce = (dRect.x+dRect.w/20)+2;//!optimize collision detection
    for (int i = 0; i < ROW; i++)
    {   
        for (int j = 0; j < COL; j++)
        {
            if(i<0 && i>ROW)continue;
            if(j<0 && j>COL)continue;
            SDL_Rect mapBlock = {j * 20, i * 20, 20, 20};
            blockType = mapManagerInstance->map[i][j];
            if (blockType==0)continue;
            
            switch (collisionType){
                case 1://! 1 warrior collison
                    if(colisionManager->ResolveDynamicRectVsRect(dRect,dir,mapBlock,dt)){
                        if (blockType==6){
                            audio->playSound(audio, "gunPickup");
                            map->dig(map,mapBlock.x,mapBlock.y);
                            *blockTypePtr= blockType; 
                            returnBlock= blockType;
                        }
                        if (blockType==9){
                            audio->playSound(audio, "health");
                            map->dig(map,mapBlock.x,mapBlock.y);
                            *blockTypePtr= blockType;
                            returnBlock= blockType;
                        }else{
                            returnBlock=1;
                        }
                    }break;
                case 2://! 2 bullet collison
                    if (colisionManager->ResolveBulletVSRect(dRect,dir,mapBlock,dt)){
                        if (blockType==9||blockType==6){
                            return 0;
                        }
                        map->dig(map,mapBlock.x, mapBlock.y);
                        return 1;
                        
                    }break;
            }
        }
    }        
    return returnBlock;
}


void showMap(void *self)
{
    Camera* camera = getCamera();
    SDL_Point offset =  camera->getCameraOffset(camera);

    TextureManager *textureManager = getTextureManager();//! hämta befintlig instant av textureManager
    MapManager *mapmanager = (MapManager *)self;
    SDL_Rect srcRec = {0, 0, 20, 20};//!skapa 20x20 source rectangel
    for (int i = 0; i < ROW; i++)
    {   
        for (int j = 0; j < COL; j++)
        {
            SDL_Rect destRect = {j * 20 + offset.x, i * 20 + offset.y, 20, 20};//! positionera rectangel enligt for loop
            switch (mapmanager->instance->map[i][j]){
                case 0: break;
                case 1: textureManager->draw(textureManager, "dirt", &srcRec, &destRect, 1); break;//!swicha textures eligt map values
                case 2: textureManager->draw(textureManager, "grass", &srcRec, &destRect, 1); break;
                case 3: textureManager->draw(textureManager, "grass.1", &srcRec, &destRect, 1); break;
                case 4: textureManager->draw(textureManager, "stone", &srcRec, &destRect, 1); break;
                case 5: textureManager->draw(textureManager, "wood", &srcRec, &destRect, 1); break;
                case 6: textureManager->draw(textureManager, "gun", &srcRec, &destRect, 0); break;
                case 7: textureManager->draw(textureManager, "brick", &srcRec, &destRect, 1); break;
                case 8: textureManager->draw(textureManager, "black", &srcRec, &destRect, 1); break;
                case 9: textureManager->draw(textureManager, "healthPack", &srcRec, &destRect, 1); break;

            }
        
        }
    }
}


void destroyMap(void *self){
    MapManagerInstance *instance = ((MapManager *)self)->instance;
    free(instance);
    printf("map destroyed\n");
}

void show(void *self){ //map maker prints activ map to terminal in array fromat. use to make a new map
    printf("in show\n");

    MapManager *mapmanager = (MapManager *)self;
        for (int i = 0; i < ROW; i++){
            printf("\n");   
            for (int j = 0; j < COL; j++){   
                printf("%d ",mapmanager->instance->map[i][j]);
            }
        }
}

MapManager *getMapManager(){
    PRIVET MapManager self;
    if (self.instance!=NULL)
    {
        return &self;
    }

    self.instance = malloc(sizeof(MapManagerInstance));
    self.show = show;
    self.showMap = showMap;
    self.initMap = initMap;
    self.dig = dig;
    self.digNoSend = digNoSend;
    self.buildNoSend = buildNoSend;
    self.build = build;
    self.destroyMap = destroyMap;
    self.checkColision = checkColision;

    return &self;
}