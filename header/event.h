#ifndef EVENT_H
#define EVENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "struct.h"

void handle_keyboard(int *quit, SDL_Event e, struct app *app, SDL_Renderer *renderer, TTF_Font *font);
void handle_events(int *quit, SDL_Event e, struct app *app, SDL_Renderer *renderer, TTF_Font *font);
void scroll_to(int line_increment, int column_increment, struct app *app, int move_cursor);
/* void goto_directory(struct cursor *cursor, struct terminal *term); */
/* void goback_directory(struct cursor *cursor, struct terminal *term); */
/* void handle_add_file(struct terminal *term, SDL_Renderer *renderer, TTF_Font *font, struct cursor *cursor); */
/* void handle_rename(struct terminal *term, SDL_Renderer *renderer, TTF_Font *font, struct cursor *cursor); */

#endif
