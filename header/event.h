#ifndef EVENT_H
#define EVENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "struct.h"

void handle_keyboard(SDL_Event e, struct cursor *cursor, struct terminal *term, SDL_Renderer *renderer, TTF_Font *font);
void handle_events(int *quit, SDL_Event e, struct cursor *cursor, struct terminal *term, SDL_Renderer *renderer, TTF_Font *font);

#endif
