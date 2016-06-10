#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>
//for unix

#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main() {
    SDL_Window *Window = NULL;
    SDL_Texture *CraftTexture = NULL;
    SDL_Renderer *WindowRenderer = NULL;

    int CraftTextureWidth;
    int CraftTextureHeight;
    int CraftTextureX = 0;
    int CraftTextureY = 0;
    int CraftTextureXvel = 0;
    int CraftTextureYvel = 0;

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

    {
        SDL_Surface *loadedCraftSurface = IMG_Load("spacemurdercraft.png");
        if(NULL == loadedCraftSurface) {
            printf("Could not laod spacemurdercraft.png: %s\n", SDL_GetError());
            return -1;
        }

        SDL_SetColorKey(loadedCraftSurface, SDL_TRUE, SDL_MapRGB(loadedCraftSurface->format, 0, 0xFF, 0xFF));

        CraftTexture = SDL_CreateTextureFromSurface(WindowRenderer, loadedCraftSurface);
        if(NULL == CraftTexture) {
            printf("Unable to create texture from surface: %s\n", SDL_GetError());
            return -1;
        }

        CraftTextureWidth = loadedCraftSurface->w;
        CraftTextureHeight = loadedCraftSurface->h;
    
        SDL_FreeSurface(loadedCraftSurface);
    }
    
    int keep_going = 1;
    SDL_Event Event;

    while(keep_going) {
        while(SDL_PollEvent(&Event) != 0) {
            if(Event.type == SDL_QUIT) {
                keep_going = 0;
            }
            else if(Event.type == SDL_KEYDOWN && Event.key.repeat == 0) {
                switch(Event.key.keysym.sym) {
                    case SDLK_UP: CraftTextureYvel -= 4; break;
                    case SDLK_DOWN: CraftTextureYvel += 4; break;
                    case SDLK_LEFT: CraftTextureXvel -= 4; break;
                    case SDLK_RIGHT: CraftTextureXvel += 4; break;
                }
            }
            //If a key was released
            else if(Event.type == SDL_KEYUP && Event.key.repeat == 0) {
                //Adjust the velocity
                switch(Event.key.keysym.sym) {
                    case SDLK_UP: CraftTextureYvel += 4; break;
                    case SDLK_DOWN: CraftTextureYvel -= 4; break;
                    case SDLK_LEFT: CraftTextureXvel += 4; break;
                    case SDLK_RIGHT: CraftTextureXvel -= 4; break;
                }
            }
        }
        
        SDL_RenderClear(WindowRenderer);

        if(CraftTextureX + CraftTextureXvel >= 0 &&
           CraftTextureX + CraftTextureXvel + CraftTextureWidth <= SCREEN_WIDTH) {
            CraftTextureX += CraftTextureXvel;
        }

        if(CraftTextureY + CraftTextureYvel >= 0 &&
           CraftTextureY + CraftTextureYvel + CraftTextureHeight <= SCREEN_HEIGHT) {
            CraftTextureY += CraftTextureYvel;
        }

        SDL_Rect renderQuad = {
                                  CraftTextureX,
                                  CraftTextureY,
                                  CraftTextureWidth,
                                  CraftTextureHeight
                              };
        
        SDL_RenderCopy(WindowRenderer, CraftTexture, NULL, &renderQuad);
        SDL_RenderPresent(WindowRenderer);
    }
    
    return 0;
}
