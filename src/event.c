#include "event.h"
#include "display.h"
#include "crud.h"

void handle_keyboard(SDL_Event e, struct cursor *cursor, struct terminal *term,
        SDL_Renderer *renderer, TTF_Font *font) {
    switch (e.key.keysym.sym) {
        case SDLK_RETURN:
            goto_directory(cursor, term);
            read_file(term, cursor, renderer, font);
            break;
        case SDLK_MINUS:
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

    snprintf(new_path, new_path_len, "%s/%s", current_path, selected_file);
    free_terminal_content(term);
    cursor->y = 0;
    term->path = new_path;
}

void goback_directory(struct cursor *cursor, struct terminal *term) {
    if (strcmp(term->path, "/") != 0 && strlen(term->path) > 1) {
        char *last_slash = strrchr(term->path, '/');
        if (last_slash != NULL && last_slash != term->path) {
            *last_slash = '\0'; 
        }
    }
    free_terminal_content(term);
    cursor->y = 0;
}

void scroll_to(int line, struct cursor *cursor, struct terminal *term, int move_cursor) { 
    if (line < 0 || line >= term->total_line) return;

    int previous_line = term->current_line;
    term->current_line = line;

    int new_y = (term->current_line - term->scroll) * (FONT_SPACING_Y + FONT_SIZE);

    if (move_cursor) {
        if (new_y >= WINDOW_HEIGHT) {
            term->scroll = term->current_line - MAX_VISIBLE_LINE + 1;
            cursor->y = WINDOW_HEIGHT - FONT_SPACING_Y - FONT_SIZE;
        } else if (new_y < 0) {
            term->scroll = term->current_line;
            cursor->y = 0;
        } else {
            cursor->y = new_y;
        }
    } else {
        if (new_y >= WINDOW_HEIGHT) {
            term->scroll = term->current_line - MAX_VISIBLE_LINE + 1;
        } else if (new_y < 0) {
            term->scroll = term->current_line;
        }
    }
}
