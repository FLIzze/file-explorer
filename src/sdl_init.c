#include <SDL_ttf.h>
#include <stdio.h>
#include "config.h"
#include "sdl_init.h"

SDL_Renderer* initialize_SDL(SDL_Window *window) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
                return NULL;
        }

        if (TTF_Init() == -1) {
                fprintf(stderr, "SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
                SDL_Quit();
                return NULL;
        }

        window = SDL_CreateWindow("SDL Window", 0, 0, 800, 600, SDL_WINDOW_SHOWN);
        if (!window) {
                fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
                SDL_Quit();
                return NULL;
        }

        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
                fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
                SDL_DestroyWindow(window);
                SDL_Quit();
                return NULL;
        }

        return renderer;
}

void cleanup_SDL(SDL_Window *window, SDL_Renderer *renderer) {
        if (renderer) {
                SDL_DestroyRenderer(renderer);
        }
        if (window) {
                SDL_DestroyWindow(window);
        }
        TTF_Quit();
        SDL_Quit();
}

TTF_Font* initialize_font() {
        TTF_Font *font = TTF_OpenFont("./roboto.ttf", FONT_SIZE);
        if (!font) {
                fprintf(stderr, "Failed to load font! TTF_Error: %s\n", TTF_GetError());
                return NULL;
        }
        return font;
}
