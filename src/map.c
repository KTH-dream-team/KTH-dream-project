#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "TextureManager.h"
#include "map.h"
#include "CollisionManager.h"
#include "data.h"
#include "networkClient.h"
#include <stdbool.h>
#define ROW 25
#define COL 50
#define PRIVET static 
#define PUBLIC

struct mapManagerInstance {
    int map[ROW][COL];
};

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

    MapManager *mapmanager =(MapManager*)self;
    int map1[ROW][COL] ={ 
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,7,7,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,2,0,0,0,5,8,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,2,2,2,0,0,0,2,2,2,0,0,0,0,0,0,2,2,0,5,0,0,0,2,2,0,0,5,8,5,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,},
    {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
    {1,1,1,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
    {1,4,1,4,4,1,4,1,4,4,1,1,4,1,4,4,1,4,1,1,4,1,1,4,4,4,1,1,1,4,1,1,1,4,1,4,4,4,4,1,1,4,1,1,1,1,4,4,1,4,},
    {1,4,4,1,4,1,4,4,1,4,1,4,4,1,1,1,4,4,4,1,1,1,1,1,1,1,1,4,1,4,1,1,4,4,1,4,4,4,1,4,1,4,4,4,4,4,4,1,4,4,},
    {4,1,1,1,4,4,1,4,1,4,4,4,1,1,4,1,1,1,4,1,4,4,1,4,4,1,4,4,4,1,1,1,4,1,4,1,6,4,4,1,4,4,4,1,4,4,1,1,1,1,},
    {1,4,1,4,4,4,1,1,4,4,1,1,1,4,1,4,1,1,4,1,1,4,4,1,1,1,1,4,1,4,1,1,4,1,4,1,1,1,4,4,4,1,4,4,4,1,1,1,1,4,},
    {4,4,4,1,1,1,4,4,4,4,1,4,1,1,4,4,1,4,1,4,4,1,4,1,1,4,4,4,4,1,4,4,1,1,4,4,1,4,4,4,1,4,4,1,4,4,1,4,4,1,},
    {1,1,1,1,4,4,4,4,1,4,1,1,1,4,1,1,4,1,1,1,1,1,4,4,4,1,4,1,1,1,4,4,1,1,1,1,4,4,4,1,1,4,4,4,1,1,4,4,4,1,},
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,},
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,},
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,},
    }; 

          for (int i = 0; i < ROW; i++){
            for (int j = 0; j < COL; j++){   
                //printf("%d ",mapmanager->instance->map[i][j]);
                mapmanager->instance->map[i][j]=map1[i][j];
            
            }
    }

}


void dig(void *self,int x, int y){//!dig when holding Q
    MapManager *mapmanager = (MapManager *)self;
    int blockCol = x/20;
    int blockRow = y/20;
    mapmanager->instance->map[blockRow][blockCol] = 0;
    printf("after remove block x%d y%d\n",blockCol,blockRow);
    NetworkClient *network = getNetworkClient();
    
    BlockPos blockSendPos ={blockCol, blockRow,1,0};//!
    //network->TCPbroadCast(network,&blockSendPos,sizeof(BlockPos));
    // printf("removed block x%d, y%d\n",blockCol,blockRow);
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

bool checkColision(void *self,SDL_Rect dRect, SDL_FPoint *dir, float dt,int collisionType){//

    CollisionManager *colisionManager = GetCollisionManager();
    MapManagerInstance *mapManagerInstance = ((MapManager *)self)->instance; 
    MapManager *map = ((MapManager *)self);
    int lowerBounce = ((dRect.y+dRect.h)/20)+2;
    int upperBounce = (dRect.y/20)-1;    
    int rightBounce = ((dRect.x+dRect.w)/20)+2;
    int leftBounce = (dRect.x/20)-1;
   // int rightBounce = (dRect.x+dRect.w/20)+2;//!optimize collision detection
    for (int i = 0; i < ROW; i++)
        {   
            for (int j = 0; j < COL; j++)
            {
                if(i<0 && i>ROW)continue;
                if(j<0 && j>COL)continue;

                SDL_Rect mapBlock = {j * 20, i * 20, 20, 20};
                int blockType = mapManagerInstance->map[i][j];
                if (blockType==0)continue;
                
                switch (collisionType)
                {
                case 1://! 1 warrior collison
                    colisionManager->ResolveDynamicRectVsRect(dRect,dir,mapBlock,dt);
                    // return true;
                    break;
                case 2://! 2 bullet collison
                    if (colisionManager->ResolveBulletVSRect(dRect,dir,mapBlock,dt))
                    {
                        map->dig(map,mapBlock.x, mapBlock.y);
                        printf("Bullet collision\n");
                        return true;
                    }  
                    break;
                default:printf("den gjorde defult\n");
                    break;
                }
            }
        }        
        return false;
}
void build(void *self, int x,int y, int blockType){//!builds when holding E
    MapManager *mapmanager = (MapManager *)self;

    blockType = 1; //defult dirt
    int blockCol = x/20;
    int blockRow = y/20;
    printf("x %d y%d",x,y);
    if (mapmanager->instance->map[blockRow][blockCol] == 0 && chekBlockContact(mapmanager,blockRow,blockCol))
    {
        mapmanager->instance->map[blockRow][blockCol] = blockType;
    }else{
        printf("\ncant build on exiisting block\n");
    }
       printf("build on x %d y %d\n",blockCol,blockRow);
}


void showMap(void *self)
{
    TextureManager *textureManager = getTextureManager();//! hämta befintlig instant av textureManager
    MapManager *mapmanager = (MapManager *)self;
    SDL_Rect srcRec = {0, 0, 20, 20};//!skapa 20x20 source rectangel
    for (int i = 0; i < ROW; i++)
    {   
        for (int j = 0; j < COL; j++)
        {
            SDL_Rect destRect = {j * 20, i * 20, 20, 20};//! positionera rectangel enligt for loop
            switch (mapmanager->instance->map[i][j]){
                case 0: break;
                case 1: textureManager->draw(textureManager, "dirt", srcRec, destRect, 1); break;//!swicha textures eligt map values
                case 2: textureManager->draw(textureManager, "grass", srcRec, destRect, 1); break;
                case 3: textureManager->draw(textureManager, "grass.1", srcRec, destRect, 1); break;
                case 4: textureManager->draw(textureManager, "stone", srcRec, destRect, 1); break;
                case 5: textureManager->draw(textureManager, "wood", srcRec, destRect, 1); break;
                case 6: textureManager->draw(textureManager, "gun", srcRec, destRect, 1); break;
                case 7: textureManager->draw(textureManager, "brick", srcRec, destRect, 1); break;
                case 8: textureManager->draw(textureManager, "black", srcRec, destRect, 1); break;

            }
        
        }
    }
}


void destroyMap(void *self){
    MapManagerInstance *instance = ((MapManager *)self)->instance;
    free(instance);
    printf("map destroyed\n");
}

void show(void *self){ //todo remove :debug
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
    self.build = build;
    self.destroyMap = destroyMap;
    self.checkColision = checkColision;

    return &self;
}