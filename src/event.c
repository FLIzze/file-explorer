#include"event.h"
#include "draw.h"
#include "config.h"
#include "crud.h"

void handle_keyboard(int *quit, SDL_Event e, struct app *app, SDL_Renderer *renderer, TTF_Font *font) {
        switch(e.key.keysym.sym) {
        case SDLK_RETURN:
                if (is_file(app->path)) return;
                goto_directory(app);
                read_file(renderer, app);
                break;
        case SDLK_MINUS:
                goback_directory(app);
                read_file(renderer, app);
                break;
        case SDLK_j:
                scroll_to(1, 0, app);
                break;
        case SDLK_k:
                scroll_to(-1, 0, app);
                break;
        case SDLK_l: 
                scroll_to(0, 1, app);
                break;
        case SDLK_h:
                scroll_to(0, -1, app);
                break;
        case SDLK_e: 
                if (app->file_list->count <= MAX_VISIBLE_LINE) {
                        app->cursor->line = app->file_list->count - 1;
                        app->cursor->scroll = 0; 
                } else {
                        app->cursor->scroll = app->file_list->count - MAX_VISIBLE_LINE;
                        app->cursor->line = app->file_list->count - app->cursor->scroll;
                }
                break;
        case SDLK_b:
                app->cursor->line = 0;
                app->cursor->scroll = 0;
                break;
        case SDLK_a: 
                if (handle_add(renderer, font, app)) {
                        read_file(renderer, app);
                }
                break; 
        case SDLK_r: 
                if (handle_rename(renderer, font, app)) {
                        read_file(renderer, app);
                }
                break;
        case SDLK_x:
                if (handle_delete(renderer, font, app)) {
                        read_file(renderer, app);
                }
                break;
        case SDLK_d:
                scroll_to(MAX_VISIBLE_LINE, 0, app);
                break;
        case SDLK_u:
                scroll_to(-MAX_VISIBLE_LINE, 0, app);
                break;
        default:
                break;
        }
        draw(renderer, font, app);
}

void handle_events(int *quit, SDL_Event e, struct app *app, SDL_Renderer *renderer, TTF_Font *font)
{
        while (SDL_PollEvent(&e) != 0) {
                switch(e.type) {
                case SDL_QUIT:
                        *quit = 1;
                        break;
                case SDL_KEYDOWN:
                        handle_keyboard(quit, e, app, renderer, font);
                        break;
                default:
                        break;
                }
        }
}

void scroll_to(int line_increment, int column_increment, struct app *app) { 
        int end_line = app->file_list->count <= MAX_VISIBLE_LINE ?
                app->file_list->count : MAX_VISIBLE_LINE;
        int current_line = app->cursor->line + line_increment;

        if (app->mode == FILE_EXPLORER) {
                if (current_line >= end_line) {
                        if (current_line + app->cursor->scroll >= app->file_list->count - 1) {
                                return;
                        }
                        app->cursor->scroll += line_increment;
                        return;
                } else if (current_line < 0) {
                        if (app->cursor->scroll > 0) {
                                app->cursor->scroll += line_increment;
                        }
                        return;
                }
        }

        app->cursor->line += line_increment;
        app->cursor->column += column_increment;
}

void goto_directory(struct app *app) {
        if (is_file(app->path)) return;

        char *current_path = app->path;
        char *selected_file = app->file_list->file_entry[app->cursor->line + app->cursor->scroll].name;
        size_t new_path_len = strlen(current_path) + strlen(selected_file) + 2; 
        char *new_path = (char *)malloc(new_path_len);
        if (!new_path) {
                fprintf(stderr, "Memory allocation for new_path failed\n");
                return;
        }

        snprintf(new_path, new_path_len, "%s/%s", current_path, selected_file);
        free(app->path);
        app->path = strdup(new_path);
        free(new_path);
}

void goback_directory(struct app *app) {
        for (int i = strlen(app->path) - 1; i > 0; i--) {
                if (app->path[i] == '/') {
                        char *new_path = (char *)malloc(i + 1); 
                        if (!new_path) {
                                fprintf(stderr, "Memory allocation for new_path failed\n");
                                return;
                        }

                        strncpy(new_path, app->path, i);
                        new_path[i] = '\0';
                        free(app->path);
                        app->path = strdup(new_path);
                        free(new_path);
                        return;
                }
        }
}
