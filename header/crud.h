#ifndef CRUD_H
#define CRUD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <ftw.h>
#include "struct.h"

void read_file(struct terminal *term, struct cursor *cursor, SDL_Renderer *renderer, TTF_Font *font);
int is_file(char *path);
void read_file_content(struct terminal *term);
void read_directory_content(struct terminal *term);
void free_content(struct terminal *term, struct cursor *cursor);
void add_file(struct terminal *term, struct cursor *cursor);
void add_directory(struct terminal *term, struct cursor *cursor);
void delete_content(struct terminal *term, struct cursor *cursor);

#endif
