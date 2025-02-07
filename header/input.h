#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "struct.h"

int user_confirmation(SDL_Renderer *renderer, TTF_Font *font, char* message, struct terminal *term);
int user_text_input(struct terminal *term, SDL_Renderer *renderer, TTF_Font *font, struct cursor *cursor);

#endif
