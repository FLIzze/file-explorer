#include "input.h"
#include "struct.h"
#include "draw.h"

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

int get_user_input(SDL_Renderer *renderer, TTF_Font *font, struct app *app, char *message) {
        SDL_Event event;
        size_t original_size = strlen(message);

        draw_user_confirmation(renderer, font, message);

        while (1) {
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                                free(message);
                                return 0;
                        } else if (event.type == SDL_KEYDOWN) {
                                switch(event.key.keysym.sym) {
                                case SDLK_BACKSPACE:
                                        if (strlen(message) > 0 && strlen(message) > original_size) {
                                                message[strlen(message) - 1] = '\0';
                                        }
                                        draw_user_confirmation(renderer, font, message);
                                        break;
                                case SDLK_RETURN:
                                        app->input->text = strdup(message);
                                        free(message);
                                        return 1;
                                case SDLK_ESCAPE:
                                        free(message);
                                        return 0;
                                default: {
                                         char to_ascii = (char)event.key.keysym.sym;

                                         size_t new_length = strlen(message) + 2; 
                                         char *new_input = (char *)realloc(message, new_length);

                                         if (!new_input) {
                                                 fprintf(stderr, "Memory allocation failed for new_input\n");
                                                 free(message);
                                                 return 0;
                                         }

                                         message = new_input;
                                         message[new_length - 2] = to_ascii; 
                                         message[new_length - 1] = '\0';    

                                         if (app->input->text) {
                                                 free(app->input->text);
                                         }
                                         app->input->text = strdup(message);

                                         if (!app->input->text) {
                                                 fprintf(stderr, "Memory allocation failed for app->input->text\n");
                                                 free(message);
                                                 return 1;
                                         }

                                         draw_user_confirmation(renderer, font, message);
                                         break;
                                         }
                                }
                        }
                }
        }

        free(message);
        return 1;
}
