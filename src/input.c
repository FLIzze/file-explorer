#include "input.h"
#include "struct.h"
#include "draw.h"

int user_confirmation(SDL_Renderer *renderer, TTF_Font *font, 
                        struct app *app, char *file_name_confirmation) {
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

static void handle_backspace(SDL_Renderer *renderer, TTF_Font *font, struct app *app, char *message) {
        char *user_input = app->input->text;
        size_t input_length = strlen(user_input);
        if (input_length > 0) {
                user_input[input_length - 1] = '\0'; 

                size_t full_message_length = strlen(user_input) + strlen(message) + 1;
                char *full_message = (char *)malloc(full_message_length);
                if (!full_message) {
                        fprintf(stderr, "Memory allocation failed for full_message\n");
                        return;
                }

                snprintf(full_message, full_message_length, "%s%s", message, user_input);
                draw_user_confirmation(renderer, font, full_message);
                free(full_message);
        }
}

static void handle_default_key(SDL_Renderer *renderer, TTF_Font *font, 
                                struct app *app, char *message, SDL_Event event) {
        char to_ascii = (char)event.key.keysym.sym;

        size_t new_length = strlen(app->input->text) + 2;
        char *new_user_input = (char *)realloc(app->input->text, new_length);
        if (!new_user_input) {
                fprintf(stderr, "Memory reallocation failed for user_input\n");
                return;
        }

        app->input->text = new_user_input;
        app->input->text[new_length - 2] = to_ascii;
        app->input->text[new_length - 1] = '\0';

        size_t full_message_length = strlen(app->input->text) + strlen(message) + 1;
        char *full_message = (char *)malloc(full_message_length);
        if (!full_message) {
                fprintf(stderr, "Memory allocation failed for full_message\n");
                return;
        }

        snprintf(full_message, full_message_length, "%s%s", message, app->input->text);
        draw_user_confirmation(renderer, font, full_message);
        free(full_message);
}

int get_user_input(SDL_Renderer *renderer, TTF_Font *font, struct app *app, char *message) {
        SDL_Event event;

        app->input->text = (char *)malloc(1);
        if (!app->input->text) {
                fprintf(stderr, "Memory allocation failed for app->input->text\n");
                return -1;
        }
        app->input->text[0] = '\0';

        draw_user_confirmation(renderer, font, message);

        while (1) {
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                                free(app->input->text);
                                return 0;
                        } else if (event.type == SDL_KEYDOWN) {
                                switch (event.key.keysym.sym) {
                                case SDLK_BACKSPACE: 
                                        handle_backspace(renderer, font, app, message);  
                                        break; 
                                case SDLK_RETURN:
                                        return 1;
                                case SDLK_ESCAPE:
                                        return 0; 
                                default: 
                                        handle_default_key(renderer, font, app, message, event);
                                        break; 
                                }
                        }
                }
        }

        return 1;
}
