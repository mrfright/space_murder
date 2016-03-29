#include <SDL2/SDL.h>
//for unix

#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main() {
    SDL_Window *Window = NULL;
    SDL_Surface *Surface = NULL;
    SDL_Surface *CraftSurface = NULL;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize: %s\n", SDL_GetError());
        return -1;
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

    Surface = SDL_GetWindowSurface(Window);

    CraftSurface = SDL_LoadBMP("spacemurdercraft.bmp");
    if(NULL == CraftSurface) {
        printf("Could not laod spacemurdercraft.bmp: %s\n", SDL_GetError());
        return -1;
    }
    
    
    SDL_BlitSurface(CraftSurface, NULL, Surface, NULL);
    SDL_UpdateWindowSurface(Window);
    SDL_Delay(2000);
    
    return 0;
}
