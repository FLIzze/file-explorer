#ifndef EVENT_H
#define EVENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "struct.h"

void handle_keyboard(SDL_Event e, struct cursor *cursor, struct terminal *term, SDL_Renderer *renderer, TTF_Font *font);
void handle_events(int *quit, SDL_Event e, struct cursor *cursor, struct terminal *term, SDL_Renderer *renderer, TTF_Font *font);
void goto_directory(struct cursor *cursor, struct terminal *term);
void goback_directory(struct cursor *cursor, struct terminal *term);
void scroll_to(int line, struct cursor *cursor, struct terminal *term, int move_cursor);
void handle_add_file(struct terminal *term, SDL_Renderer *renderer, TTF_Font *font, struct cursor *cursor);
void handle_rename(struct terminal *term, SDL_Renderer *renderer, TTF_Font *font, struct cursor *cursor);

#endif
