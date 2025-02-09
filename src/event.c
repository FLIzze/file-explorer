#include"event.h"
#include "draw.h"
/* #include "crud.h" */
/* #include "input.h" */
/* #include "struct.h" */

void handle_keyboard(int *quit, SDL_Event e, struct app *app, SDL_Renderer *renderer, TTF_Font *font) {
        switch(e.key.keysym.sym) {
        case SDLK_RETURN:
                /* if (is_file(term->path)) return; */
                /* goto_directory(cursor, term); */
                /* read_file(term, cursor, renderer, font); */
                break;
        case SDLK_MINUS:
                /* goback_directory(cursor, term); */
                /* read_file(term, cursor, renderer, font); */
                break;
        case SDLK_j:
                scroll_to(1, 0, app, 1);
                break;
        case SDLK_k:
                scroll_to(-1, 0, app, 1);
                break;
        case SDLK_l: 
                scroll_to(0, 1, app, 1);
                break;
        case SDLK_h:
                scroll_to(0, -1, app, 1);
                break;
        case SDLK_e:
                /* scroll_to(term->total_line - 1, cursor, term, 1); */
                break;
        case SDLK_b:
                /* scroll_to(0, cursor, term, 1); */
                break;
        case SDLK_a: 
                /* handle_add_file(term, renderer, font, cursor); */
                break; 
        case SDLK_r: 
                /* handle_rename(term, renderer, font, cursor); */
                break;
        case SDLK_x:
                /* if (delete_content(term, cursor, renderer, font)) { */
                /*         read_file(term, cursor, renderer, font); */
                /* } */
                break;
        case SDLK_d:
                /* scroll_to(term->current_line + MAX_VISIBLE_LINE, cursor, term, 0); */
                break;
        case SDLK_u:
                /* scroll_to(term->current_line - MAX_VISIBLE_LINE, cursor, term, 0); */
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

void scroll_to(int line_increment, int column_increment, struct app *app, int move_cursor) { 
        app->cursor->line += line_increment;
        app->cursor->column += column_increment;
}

/* void goto_directory(struct cursor *cursor, struct terminal *term) { */
/*         if (is_file(term->path)) return; */

/*         char *current_path = term->path; */
/*         char *selected_file = term->lines[term->current_line].segments[0].text; */
/*         size_t new_path_len = strlen(current_path) + strlen(selected_file) + 2; */ 
/*         char *new_path = (char *)malloc(new_path_len); */
/*         if (!new_path) { */
/*                 fprintf(stderr, "Memory allocation for new_path failed\n"); */
/*         } */

/*         snprintf(new_path, new_path_len, "%s/%s", current_path, selected_file); */
/*         free_terminal(term); */
/*         cursor->y = 0; */
/*         term->path = new_path; */
/* } */

/* void goback_directory(struct cursor *cursor, struct terminal *term) { */
/*         for (int i = strlen(term->path) - 1; i > 0; i--) { */
/*                 if (term->path[i] == '/') { */
/*                         char *new_path = (char *)malloc(i + 1); */ 
/*                         if (!new_path) { */
/*                                 fprintf(stderr, "Memory allocation for new_path failed\n"); */
/*                                 return; */
/*                         } */

/*                         strncpy(new_path, term->path, i + 1); */
/*                         new_path[i] = '\0'; */

/*                         free_terminal(term); */
/*                         cursor->y = 0; */
/*                         term->path = new_path; */
/*                         return; */
/*                 } */
/*         } */
/* } */

/* void handle_add_file(struct terminal *term, SDL_Renderer *renderer, TTF_Font *font, struct cursor *cursor) { */
/*         if (!user_text_input(term, renderer, font, cursor)) { */
/*                 return; */
/*         } */

/*         size_t full_path_len = strlen(term->path) + strlen(term->user_input->text) + 2; */
/*         char *full_path = (char *)malloc(full_path_len); */
/*         snprintf(full_path, full_path_len, "%s/%s", term->path, term->user_input->text); */
/*         if (term->user_input->text[term->user_input->size - 1] == '/') { */
/*                 add_directory(term, cursor, full_path); */
/*         } else { */
/*                 add_file(term, cursor, full_path); */
/*         } */

/*         char *current_path = strdup(term->path); */
/*         free_terminal(term); */
/*         term->path = current_path; */
/*         cursor->y = 0; */
/*         term->log->message = full_path; */
/*         read_file(term, cursor, renderer, font); */
/* } */

/* void handle_rename(struct terminal *term, SDL_Renderer *renderer, TTF_Font *font, struct cursor *cursor) { */
/*         if (!user_text_input(term, renderer, font, cursor)) { */
/*                 return; */
/*         } */
/*         char *file_name = term->lines[term->current_line].segments[0].text; */

/*         size_t previous_path_len = strlen(term->path) + strlen(file_name) + 2; */ 
/*         size_t new_path_len = strlen(term->path) + strlen(term->user_input->text) + 2; */ 

/*         char *previous_path = (char *)malloc(previous_path_len); */
/*         char *new_path = (char *)malloc (new_path_len); */

/*         snprintf(previous_path, previous_path_len, "%s/%s", term->path, file_name); */
/*         snprintf(new_path, new_path_len, "%s/%s", term->path, term->user_input->text); */

/*         if (!rename_directory(previous_path, new_path)) { */
/*                 term->log->message = strdup("couldnt rename"); */
/*                 return; */
/*         } */

/*         char *current_path = strdup(term->path); */
/*         free_terminal(term); */
/*         term->path = current_path; */
/*         cursor->y = 0; */
/*         term->log->message = new_path; */
/*         read_file(term, cursor, renderer, font); */
/* } */
