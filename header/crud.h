#ifndef CRUD_H
#define CRUD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <ftw.h>
#include "struct.h"

int is_file(char *path);

void read_file(SDL_Renderer *renderer, struct app *app, TTF_Font *font);
void read_file_content(struct app *app);
void read_directory_content(struct app *app, TTF_Font *font);

int handle_delete(SDL_Renderer *renderer, TTF_Font *font, struct app *app);
int handle_add(SDL_Renderer *renderer, TTF_Font *font, struct app *app);
int handle_rename(SDL_Renderer *renderer, TTF_Font *font, struct app *app);

/* void free_content(struct terminal *term, struct cursor *cursor); */

#endif
