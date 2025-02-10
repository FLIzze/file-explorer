#include "input.h"
#include "struct.h"
#include "draw.h"
/* #include "SDL_keycode.h" */
/* #include "display.h" */

int user_confirmation(SDL_Renderer *renderer, TTF_Font *font, struct app *app, char *file_name_confirmation) {
        SDL_Event event;

        draw_user_confirmation(renderer, font, file_name_confirmation);

        while (1) {
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                                return 1;
                        }
                        if (event.type == SDL_KEYDOWN) {
                                if (event.key.keysym.sym == SDLK_y || event.key.keysym.sym == SDLK_RETURN) {
                                        return 1;
                                } else if (event.key.keysym.sym == SDLK_n || event.key.keysym.sym == SDLK_ESCAPE) {
                                        return 0;
                                }
                        }
                }
        }
}

int get_user_input(SDL_Renderer *renderer, TTF_Font *font, struct app *app) {
        SDL_Event event;

        while (1) {
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                                return 0;
                        } else if (event.type == SDL_KEYDOWN) {
                                switch(event.key.keysym.sym) {
                                case SDLK_BACKSPACE: 
                                        /* term->user_input->size--; */
                                        /* term->user_input->text[term->user_input->size] = '\0'; */
                                        /* char *new_buffer = (char *)realloc(term->user_input->text, term->user_input->size); */
                                        /* if (!new_buffer) { */
                                        /*         fprintf(stderr, "Memory allocation failed\n"); */
                                        /*         return 0; */
                                        /* } */
                                        /* term->user_input->text = new_buffer; */
                                        /* display_log(renderer, font, new_buffer, term); */
                                        break; 
                                case SDLK_RETURN:
                                        /* printf("%c\n", app->log->message); */
                                        /* term->log->message = strdup(term->user_input->text); */
                                        return 1;
                                case SDLK_ESCAPE:
                                        /* term->log->message = strdup("Aborting..."); */
                                        return 0;
                                default:
                                        char to_ascii = (char)event.key.keysym.sym;
                                        printf("%c\n", to_ascii);
                                        break;
                                }
                        }
                }
        }
        return 1;
}
