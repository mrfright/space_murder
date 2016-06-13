
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>
//for unix


//for windows
//#include "SDL_win\SDL.h"
//#include "SDL_win\SDL_image.h"

#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MAX_BULLETS 100

struct Sprite {
    SDL_Texture *Texture;
    int TextureWidth;
    int TextureHeight;
    int TextureX;
    int TextureY;
    int TextureXvel;
    int TextureYvel;
    int TextureMinX;
    int TextureMaxX;
    int TextureMinY;
    int TextureMaxY;
};

struct Sprite CreateSprite(char *image_file,
                           int r_key,
                           int g_key,
                           int b_key,
                           SDL_Renderer *Renderer,
                           int minx,
                           int maxx,
                           int miny,
                           int maxy) {
    struct Sprite sprite;
    
    SDL_Surface *loadedCraftSurface = IMG_Load(image_file);
    if(NULL == loadedCraftSurface) {
        printf("Could not laod %s: %s\n", image_file, SDL_GetError());
        exit(-1);
    }

    SDL_SetColorKey(loadedCraftSurface,
                    SDL_TRUE,
                    SDL_MapRGB(loadedCraftSurface->format, r_key, g_key, b_key));

    sprite.Texture = SDL_CreateTextureFromSurface(Renderer, loadedCraftSurface);
    if(NULL == sprite.Texture) {
        printf("Unable to create texture from surface: %s\n", SDL_GetError());
        exit(-1);
    }

    sprite.TextureWidth = loadedCraftSurface->w;
    sprite.TextureHeight = loadedCraftSurface->h;
    sprite.TextureX      = 0;
    sprite.TextureY      = 0;
    sprite.TextureXvel   = 0;
    sprite.TextureYvel   = 0;
    sprite.TextureMinX   = minx;
    sprite.TextureMaxX   = maxx;
    sprite.TextureMinY   = miny;
    sprite.TextureMaxY   = maxy;
    
    SDL_FreeSurface(loadedCraftSurface);

    return sprite;
}

struct BulletSprite {
    int EnemyBullet;
    struct Sprite sprite;
};

struct BulletSprite CreatePlayerBulletSprite(struct Sprite *player,
                                             SDL_Renderer *Renderer) {
    struct BulletSprite bullet;
    bullet.EnemyBullet = 0;
    char bulletfilename[] = "spacemurderbullet.png";
    bullet.sprite = CreateSprite(bulletfilename, 0, 0, 0,
                                 Renderer, 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);

    bullet.sprite.TextureX = player->TextureX;
    bullet.sprite.TextureY = player->TextureY;

    return bullet;
}

void RenderSprite(struct Sprite *sprite, SDL_Renderer *Renderer) {
    if(sprite->TextureX + sprite->TextureXvel >= sprite->TextureMinX &&
       sprite->TextureX + sprite->TextureXvel + sprite->TextureWidth <= sprite->TextureMaxX) {
        sprite->TextureX += sprite->TextureXvel;
    }

    if(sprite->TextureY + sprite->TextureYvel >= sprite->TextureMinY &&
       sprite->TextureY + sprite->TextureYvel + sprite->TextureHeight <= sprite->TextureMaxY) {
        sprite->TextureY += sprite->TextureYvel;
    }

    SDL_Rect renderQuad = {
        sprite->TextureX,
        sprite->TextureY,
        sprite->TextureWidth,
        sprite->TextureHeight
    };
    
    SDL_RenderCopy(Renderer, sprite->Texture, NULL, &renderQuad);
}

//needed for Windows
#ifdef main
#undef main
#endif 

int main() {
    SDL_Window *Window = NULL;

    SDL_Renderer *WindowRenderer = NULL;
    struct Sprite CraftSprite;
    struct BulletSprite Bullets[MAX_BULLETS];
    int BulletCount = 0;    
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize: %s\n", SDL_GetError());
        return -1;
    }
    
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        printf("Linear tecture filtering not enabled.\n");
    }

    Window = SDL_CreateWindow("Space Murder",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH,
                              SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);

    if(NULL == Window) {
        printf("Window could not be created: %s\n", SDL_GetError());
        return -1;
    }

    WindowRenderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(NULL == WindowRenderer) {
        printf("Renderer could not be created: %s\n", SDL_GetError());
        return -1;
    }

    SDL_SetRenderDrawColor(WindowRenderer, 0, 0, 0, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image cuold not initialize: %s\n", IMG_GetError());
        return -1;
    }

    char craftspritefile[] = "spacemurdercraft.png";
    CraftSprite = CreateSprite(craftspritefile, 0x00, 0xFF, 0xFF,
                               WindowRenderer, 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);

    int keep_going = 1;
    SDL_Event Event;

    while(keep_going) {
        while(SDL_PollEvent(&Event) != 0) {
            if(Event.type == SDL_QUIT) {
                keep_going = 0;
            }
            else if(Event.type == SDL_KEYDOWN && Event.key.repeat == 0) {
                switch(Event.key.keysym.sym) {
                    case SDLK_UP: CraftSprite.TextureYvel -= 4; break;
                    case SDLK_DOWN: CraftSprite.TextureYvel += 4; break;
                    case SDLK_LEFT: CraftSprite.TextureXvel -= 4; break;
                    case SDLK_RIGHT: CraftSprite.TextureXvel += 4; break;
                    case SDLK_SPACE:
                        if(BulletCount < MAX_BULLETS) {
                            Bullets[BulletCount++] =
                                CreatePlayerBulletSprite(&CraftSprite,
                                                         WindowRenderer);
                        }
                }
            }
            //If a key was released
            else if(Event.type == SDL_KEYUP && Event.key.repeat == 0) {
                //Adjust the velocity
                switch(Event.key.keysym.sym) {
                    case SDLK_UP: CraftSprite.TextureYvel += 4; break;
                    case SDLK_DOWN: CraftSprite.TextureYvel -= 4; break;
                    case SDLK_LEFT: CraftSprite.TextureXvel += 4; break;
                    case SDLK_RIGHT: CraftSprite.TextureXvel -= 4; break;
                }
            }
        }
        
        SDL_RenderClear(WindowRenderer);

        RenderSprite(&CraftSprite, WindowRenderer);

        int currentbullet;
        for(currentbullet = 0; currentbullet < BulletCount; ++currentbullet) {
            RenderSprite(&(Bullets[currentbullet].sprite), WindowRenderer);
        }

        SDL_RenderPresent(WindowRenderer);
    }
    
    return 0;
}
