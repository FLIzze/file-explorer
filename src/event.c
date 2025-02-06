#include "event.h"
#include "display.h"
#include "crud.h"

void handle_keyboard(SDL_Event e, struct cursor *cursor, struct terminal *term,
                SDL_Renderer *renderer, TTF_Font *font) {
        switch (e.key.keysym.sym) {
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
                case SDLK_a:
                        add_file(term, cursor);
                        add_directory(term, cursor);
                        read_file(term, cursor, renderer, font);
                        break;
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

int user_confirmation(SDL_Renderer *renderer, TTF_Font *font, char* message, struct terminal *term) {
        SDL_Event event;

        char full_message[50];
        snprintf(full_message, sizeof(full_message), "Delete %s         [Y]es [N]o", message);
        SDL_Texture *texture = display_log(renderer, font, full_message, term);

        while (1) {
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                                SDL_DestroyTexture(texture);
                                return 0; 
                        }
                        if (event.type == SDL_KEYDOWN) {
                                if (event.key.keysym.sym == SDLK_y || event.key.keysym.sym == SDLK_RETURN) {
                                        SDL_DestroyTexture(texture);
                                        return 1;
                                } else if (event.key.keysym.sym == SDLK_n || event.key.keysym.sym == SDLK_ESCAPE) {
                                        SDL_DestroyTexture(texture);
                                        return 0;
                                }
                        }
                }
                SDL_Delay(100); 
        }
}
