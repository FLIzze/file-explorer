#ifndef DRAW_H
#define DRAW_H

#include <SDL2/SDL.h>
#include "struct.h"

/* #include <SDL2/SDL_ttf.h> */
/* #include <sys/stat.h> */

/* void display_file_content(SDL_Renderer *renderer, TTF_Font *font, struct terminal *term); */
/* SDL_Texture* display_log(SDL_Renderer *renderer, TTF_Font *font, char* message, struct terminal *term); */
void display_cursor(SDL_Renderer *renderer, struct app *app);
void draw_path(SDL_Renderer *renderer, TTF_Font *font, struct app *app);
void draw_lines(SDL_Renderer *renderer, TTF_Font *font, struct app *app);
void draw(SDL_Renderer *renderer, TTF_Font *font, struct app *app);
void draw_explorer_content(SDL_Renderer *renderer, TTF_Font *font, struct app *app);
SDL_Texture* create_text_texture(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg);

#endif
