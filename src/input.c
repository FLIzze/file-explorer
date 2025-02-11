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
        
        free(app->input->text);
        app->input->text = strdup("");
        draw_user_confirmation(renderer, font, message);

        while (1) {
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                                free(message);
                                return 0;
                        } else if (event.type == SDL_KEYDOWN) {
                                switch(event.key.keysym.sym) {
                                case SDLK_BACKSPACE: {
                                        char *user_input = app->input->text;
                                        if (strlen(user_input) <= 0) {
                                                break;
                                        }

                                        user_input[strlen(user_input) - 1] = '\0';
                                        size_t full_message_length = strlen(user_input) + strlen(message) + 1;
                                        char *full_message = (char *)malloc(full_message_length);

                                        snprintf(full_message, full_message_length, "%s%s", message, user_input);
                                        draw_user_confirmation(renderer, font, full_message);
                                        break; }
                                case SDLK_RETURN:
                                        free(message);
                                        return 1;
                                case SDLK_ESCAPE:
                                        free(message);
                                        return 0;
                                default: {
                                         char *user_input = app->input->text;
                                         char to_ascii = (char)event.key.keysym.sym;
                                         size_t new_length = strlen(user_input) + 2;
                                         user_input = (char *)realloc(user_input, new_length);
                                         user_input[new_length - 2] = to_ascii;
                                         user_input[new_length - 1] = '\0';

                                         size_t full_message_length = strlen(user_input) + strlen(message) + 1;
                                         char *full_message; 
                                         snprintf(full_message, full_message_length, "%s%s", message, user_input);
                                         draw_user_confirmation(renderer, font, full_message);
                                         break;
                                         }
                                }
                        }
                }
        }
        return 1;
}
