#ifndef SDL_INIT_H
#define SDL_INIT_H

#include <SDL_ttf.h>
#include <SDL.h>

SDL_Renderer* initialize_SDL(SDL_Window *window);
void cleanup_SDL(SDL_Window *window, SDL_Renderer *renderer);
TTF_Font* initialize_font();

#endif
