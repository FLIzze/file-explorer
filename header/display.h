#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "struct.h"
#include <sys/stat.h>

void display_cursor(SDL_Renderer *renderer, struct cursor *cursor);
void display_path(struct terminal *term, TTF_Font *font, SDL_Renderer *renderer);
void display_lines(SDL_Renderer *renderer, TTF_Font *font, struct terminal *term);
void display_file_content(SDL_Renderer *renderer, TTF_Font *font, struct terminal *term);
int display_confirm(SDL_Renderer *renderer);
void display(SDL_Renderer *renderer, TTF_Font *font, struct terminal *term, struct cursor *cursor);

#endif
