#include <stdio.h>
#include <stdlib.h>
#include "textureManager.h"
#include <time.h>
#define row 25
#define col 50
//!skapa 50x25 pixel matris där varje värde representerar en typ av 20x20 pixel block   
int map[row][col] = { 
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


void initMap()
{
    
    TextureManager *textureManager = getTextureManager();//! hämta befintlig textureManger instant obs inte en new
    textureManager->load(textureManager, "dirt", "./assets/dirt.png");//1 //!load texture function sicka id och fil namn 
    textureManager->load(textureManager, "grass", "./assets/grass.png");//2
    textureManager->load(textureManager, "grass.1", "./assets/grass.1.png");//3
    textureManager->load(textureManager, "stone", "./assets/stone.png");//4
    textureManager->load(textureManager, "wood", "./assets/wood.png");//5
    textureManager->load(textureManager, "gun", "./assets/gun.png");//6
    textureManager->load(textureManager, "brick", "./assets/brick.png");//7
    textureManager->load(textureManager, "black", "./assets/black.png");//8
}

void dig(int y, int x){//!gräva rakt ner dely 100 tick
    

    map[y][x] = 0;
}

static int x = 18;//todo remove
static int y = 10;//todo remove
void staticDig(){//!gräva rakt ner tilfäligt implementerad för att testa 

    static unsigned int currentTime;
    static unsigned int lastTime;
    currentTime=SDL_GetTicks();
        if(y<30){
            if (lastTime+100<currentTime)
            {
                y++;
                lastTime=currentTime;
            }
        }
    map[y][x] = 0;


}

void showMap()
{
    // printf("\nin show\n");
    // int random;
    // srand(time(NULL));

    TextureManager *textureManager = getTextureManager();//! hämta befintlig instant av textureManager
    SDL_Rect srcRec = {0, 0, 20, 20};//!skapa 20x20 source rectangel
    printf("\n\n");

    for (int i = 0; i < row; i++)
    {   
        //printf("{");
        for (int j = 0; j < col; j++)
        {
            SDL_Rect destRect = {j * 20, i * 20, 20, 20};//! positionera rectangel enligt for loop

            switch (map[i][j]){
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

void delay(int msec){
    
}


