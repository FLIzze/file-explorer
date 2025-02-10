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
SDL_Texture* display_log(SDL_Renderer *renderer, TTF_Font *font, char* message, struct terminal *term);
void display(SDL_Renderer *renderer, TTF_Font *font, struct terminal *term, struct cursor *cursor);
SDL_Texture* create_text_texture(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg);

#endif
