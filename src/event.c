#include "event.h"
#include "display.h"
#include "crud.h"
#include "input.h"

void handle_keyboard(SDL_Event e, struct cursor *cursor, struct terminal *term,
                SDL_Renderer *renderer, TTF_Font *font) {
        switch(e.key.keysym.sym) {
                case SDLK_RETURN:
                        if (is_file(term->path)) return;
                        goto_directory(cursor, term);
                        read_file(term, cursor, renderer, font);
                        break;
                case SDLK_MINUS:
                        if (strcmp(term->path, "/") == 0) return;
                        goback_directory(cursor, term);
                        read_file(term, cursor, renderer, font);
                        break;
                case SDLK_j:
                        scroll_to(term->current_line + 1, cursor, term, 1);
                        break;
                case SDLK_k:
                        scroll_to(term->current_line - 1, cursor, term, 1);
                        break;
                case SDLK_e:
                        scroll_to(term->total_line - 1, cursor, term, 1);
                        break;
                case SDLK_b:
                        scroll_to(0, cursor, term, 1);
                        break;
                case SDLK_a: {
                        if (!user_text_input(term, renderer, font, cursor)) {
                                break;
                        }

                        size_t full_path_len = strlen(term->path) + strlen(term->user_input->text) + 2;
                        char *full_path = (char *)malloc(full_path_len);
                        snprintf(full_path, full_path_len, "%s/%s", term->path, term->user_input->text);
                        if (term->user_input->text[term->user_input->size - 1] == '/') {
                                add_directory(term, cursor, full_path);
                        } else {
                                add_file(term, cursor, full_path);
                        }

                        read_file(term, cursor, renderer, font);
                        break; }
                case SDLK_x:
                        if (delete_content(term, cursor, renderer, font)) {
                                read_file(term, cursor, renderer, font);
                        }
                        break;
                case SDLK_d:
                        scroll_to(term->current_line + MAX_VISIBLE_LINE, cursor, term, 0);
                        break;
                case SDLK_u:
                        scroll_to(term->current_line - MAX_VISIBLE_LINE, cursor, term, 0);
                        break;
                default:
                        break;
        }
        display(renderer, font, term, cursor);
}

void handle_events(
                int *quit, SDL_Event e, struct cursor *cursor, 
                struct terminal *term, SDL_Renderer *renderer, TTF_Font *font) 
{
        while (SDL_PollEvent(&e) != 0) {
                switch(e.type) {
                        case SDL_QUIT:
                                *quit = 1;
                                break;
                        case SDL_KEYDOWN:
                                handle_keyboard(e, cursor, term, renderer, font);
                                break;
                        default:
                                break;
                }
        }
}

void goto_directory(struct cursor *cursor, struct terminal *term) {
        if (is_file(term->path)) return;

        char *current_path = term->path;
        char *selected_file = term->lines[term->current_line].segments[0].text;
        size_t new_path_len = strlen(current_path) + strlen(selected_file) + 2; 
        char *new_path = (char *)malloc(new_path_len);
        if (!new_path) {
                fprintf(stderr, "Memory allocation for new_path failed.");
        }

        snprintf(new_path, new_path_len, "%s/%s", current_path, selected_file);
        free_terminal(term);
        cursor->y = 0;
        term->path = new_path;
}

void goback_directory(struct cursor *cursor, struct terminal *term) {
        char *last_slash = strrchr(term->path, '/');
        if (!last_slash || last_slash == term->path) {
                return;
        }

        *last_slash = '\0';
        char *new_path = strdup(term->path);
        if (!new_path) {
                fprintf(stderr, "Memory allocation failed!\n");
                return;
        }
        *last_slash = '/';

        free_terminal(term);
        term->path = new_path;
        cursor->y = 0;
}

void scroll_to(int line, struct cursor *cursor, struct terminal *term, int move_cursor) { 
        if (line < 0) {
                cursor->y = 0;
                term->scroll = 0;
                term->current_line = 0;
                return;
        } else if (line >= term->total_line) {
                return;
        }

        int previous_line = term->current_line;
        term->current_line = line;

        int new_y = (term->current_line - term->scroll) * (FONT_SPACING_Y + FONT_SIZE);

        if (move_cursor) {
                if (new_y >= LINES_HEIGHT) {
                        term->scroll = term->total_line - MAX_VISIBLE_LINE;
                        cursor->y = (MAX_VISIBLE_LINE - 1) * (FONT_SPACING_Y + FONT_SIZE);
                } else if (new_y < 0) {
                        term->scroll = term->current_line;
                        cursor->y = 0;
                } else {
                        cursor->y = new_y;
                }
        } else {
                term->scroll += term->current_line - previous_line;
        }
}
