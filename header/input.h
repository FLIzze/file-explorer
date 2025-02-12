#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "struct.h"

/* int user_text_input(struct terminal *term, SDL_Renderer *renderer, TTF_Font *font, struct cursor *cursor); */
int user_confirmation(SDL_Renderer *renderer, TTF_Font *font, struct app *app, enum edit_mode edit_mode);
int get_user_input(SDL_Renderer *renderer, TTF_Font *font, enum edit_mode edit_mode, struct app *app);

#endif
