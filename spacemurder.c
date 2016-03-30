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
    //SDL_Surface *Surface = NULL;
    //SDL_Surface *CraftSurface = NULL;
    SDL_Texture *CraftTexture = NULL;
    SDL_Renderer *WindowRenderer = NULL;

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

    WindowRenderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == WindowRenderer) {
        printf("Renderer could not be created: %s\n", SDL_GetError());
        return -1;
    }

    SDL_SetRenderDrawColor(WindowRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

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

    CraftTexture = SDL_CreateTextureFromSurface(WindowRenderer, loadedCraftSurface);
    if(NULL == CraftTexture) {
        printf("Unable to create texture from surface: %s\n", SDL_GetError());
        return -1;
    }
    
    SDL_FreeSurface(loadedCraftSurface);
    }
    
    int keep_going = 1;
    SDL_Event Event;

    while(keep_going) {
        while(SDL_PollEvent(&Event) != 0) {
            if(Event.type == SDL_QUIT) {
                keep_going = 0;
            }
        }
        SDL_RenderClear(WindowRenderer);
        SDL_RenderCopy(WindowRenderer, CraftTexture, NULL, NULL);
        SDL_RenderPresent(WindowRenderer);
    }
    
    return 0;
}
