#include <SDL2/SDL.h>
//for unix

#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main() {
    SDL_Window *Window = NULL;
    SDL_Surface *Surface = NULL;

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
    SDL_FillRect(Surface, NULL, SDL_MapRGB(Surface->format, 0xFF, 0xFF, 0xFF));
    SDL_UpdateWindowSurface(Window);
    SDL_Delay(2000);
    
    return 0;
}
