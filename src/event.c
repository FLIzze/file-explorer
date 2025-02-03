#include "event.h"
#include "display.h"
#include "read.h"

void handle_keyboard(SDL_Event e, struct cursor *cursor, struct terminal *term,
        SDL_Renderer *renderer, TTF_Font *font) {
    switch (e.key.keysym.sym) {
        case SDLK_j:
            if (cursor->y < WINDOW_HEIGHT - FONT_SIZE - FONT_SPACING_Y && term->current_line < term->total_line - 1) {
                term->current_line++;
                cursor->y += FONT_SIZE + FONT_SPACING_Y;
            } else if (term->current_line < term->total_line - 1) {
                term->current_line++;
                term->scroll++;
            }
            display(renderer, font, term, cursor);
            break;
        case SDLK_k:
            if (cursor->y > 0) {
                term->current_line--;
                cursor->y -= FONT_SIZE + FONT_SPACING_Y;
            } else if (term->current_line > 0) {
                term->scroll--;
                term->current_line--;
            }
            display(renderer, font, term, cursor);
            break;
        case SDLK_RETURN:
            if (is_file(term->path)) return;

            {
                char *current_path = term->path;
                char *selected_file = term->lines[term->current_line].segments[0].text;
                size_t new_path_len = strlen(current_path) + strlen(selected_file) + 2; 
                char *new_path = (char *)malloc(new_path_len);

                snprintf(new_path, new_path_len, "%s/%s", current_path, selected_file);
                free_terminal_content(term);
                cursor->y = 0;
                term->path = new_path;

                read_file(term, cursor, renderer, font);
            }
            break;
        case SDLK_MINUS:
            if (strcmp(term->path, "/") != 0 && strlen(term->path) > 1) {
                char *last_slash = strrchr(term->path, '/');
                if (last_slash != NULL && last_slash != term->path) {
                    *last_slash = '\0'; 
                }
            }
            free_terminal_content(term);
            cursor->y = 0;
            read_file(term, cursor, renderer, font);
            break;
        default:
            break;
    }
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
