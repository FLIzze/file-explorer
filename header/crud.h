#ifndef CRUD_H
#define CRUD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <ftw.h>
#include "struct.h"

int is_file(char *path);

void read_file(struct terminal *term, struct cursor *cursor, SDL_Renderer *renderer, TTF_Font *font);
void read_file_content(struct terminal *term);
void read_directory_content(struct terminal *term);

static void delete_directory(char *path, struct terminal *term);
static void delete_file(char *path, struct terminal *term);
int delete_content(struct terminal *term, struct cursor *cursor, SDL_Renderer *renderer, TTF_Font *font);

void add_file(struct terminal *term, struct cursor *cursor, char *path);
void add_directory(struct terminal *term, struct cursor *cursor, char *path);
static void add_line_to_terminal(struct terminal *term, char *text, struct rgb color);

void rename_file(struct terminal *term, char *path);
void rename_directory(char *previous_path, char *new_path);

void free_content(struct terminal *term, struct cursor *cursor);

#endif
