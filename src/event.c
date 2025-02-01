#include "event.h"
#include "display.h"
#include "read.h"

void handle_keyboard(SDL_Event e, struct cursor *cursor, struct terminal *term,
                     SDL_Renderer *renderer, TTF_Font *font) {
    switch (e.key.keysym.sym) {
        case SDLK_j:
            if (term->current_line < term->total_line) {
                term->current_line += 1;

                if (cursor->y >= WINDOW_HEIGHT - FONT_SIZE - FONT_SPACING_Y) {
                    term->scroll += 1;
                } else {
                    cursor->y += FONT_SIZE + FONT_SPACING_Y;
                }
            }

            display(renderer, font, term, cursor);
            break;

        case SDLK_k:
            if (term->current_line > 1) {
                term->current_line -= 1;
                cursor->y -= FONT_SIZE + FONT_SPACING_Y;

                if (cursor->y <= FONT_SPACING_Y - FONT_SIZE) {
                    term->scroll -= 1;
                    cursor->y = 0;
                }
            }

            display(renderer, font, term, cursor);
            break;

        case SDLK_RETURN: {
            if (is_file(term->path)) {
                return;
            }

            char *selected_entry = term->content[term->current_line - 1];
            if (selected_entry == NULL) {
                fprintf(stderr, "Selected entry is NULL\n");
                break;
            }

            size_t new_path_length = strlen(term->path) + strlen(selected_entry) + 2; 
            char *new_path = (char *)malloc(new_path_length);
            if (new_path == NULL) {
                fprintf(stderr, "Failed to allocate memory for new path\n");
                break;
            }
            snprintf(new_path, new_path_length, "%s/%s", term->path, selected_entry);

            free(term->path);
            term->path = new_path;

            read_file(term, cursor);
            display(renderer, font, term, cursor);
            break;
        }

        case SDLK_MINUS:
            if (strcmp(term->path, "/") != 0) {
                char *last_slash = strrchr(term->path, '/');
                if (last_slash != NULL) {
                    *last_slash = '\0'; 
                }
            }
            read_file(term, cursor);
            display(renderer, font, term, cursor);
            printf("displaying");
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
