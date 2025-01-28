#include "event.h"

void handle_keyboard(SDL_Event e, struct cursor *cursor, struct terminal *term) {
    switch(e.key.keysym.sym) {
        case SDLK_j:
            if (term->current_line < term->total_line - 1) {
                cursor->y += FONT_SIZE + FONT_SPACING_Y; 
                term->current_line += 1;
            }
            break;
        case SDLK_k:
            if (term->current_line > 1) {
                cursor->y -= FONT_SIZE + FONT_SPACING_Y; 
                term->current_line -= 1;
            }
            break;
        case SDLK_RETURN:
            char new_path[1024];
            snprintf(new_path, sizeof(new_path), "%s/%s", term->path, term->files[term->current_line]);
            strcpy(term->path, new_path);ult:
            break;
        case SDLK_MINUS:
            if (strcmp(term->path, "/") != 0) {
                char *last_slash = strrchr(term->path, '/');
                if (last_slash != NULL) {
                    *last_slash = '\0';  
                }
            }
            break;
        default:
            break;
    }
}

void handle_events(int quit, SDL_Event e, struct cursor *cursor, struct terminal *term) {
    while (SDL_PollEvent(&e) != 0) {
        switch(e.type) {
            case SDL_QUIT:
                quit = 1;
                break;
            case SDL_KEYDOWN:
                handle_keyboard(e, cursor, term);
            default:
                break;
        }
    }
}
