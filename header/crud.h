#ifndef CRUD_H
#define CRUD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <ftw.h>
#include "struct.h"

int is_file(char *path);

void read_file(SDL_Renderer *renderer, struct app *app);
void read_file_content(struct app *app);
void read_directory_content(struct app *app);

static void delete_directory(char *path);
static void delete_file(char *path);
int delete_content(struct app *app, SDL_Renderer *renderer, TTF_Font *font);

/* void add_file(struct terminal *term, struct cursor *cursor, char *path); */
/* void add_directory(struct terminal *term, struct cursor *cursor, char *path); */
/* static void add_line_to_terminal(struct terminal *term, char *text, struct rgb color); */

/* int rename_directory(char *previous_path, char *new_path); */

/* void free_content(struct terminal *term, struct cursor *cursor); */

#endif
