#include "input.h"
#include "struct.h"
#include "draw.h"

int user_confirmation(SDL_Renderer *renderer, TTF_Font *font, struct app *app, enum edit_mode edit_mode) {
        SDL_Event event;
        char message[] = "";

        draw_user_confirmation(renderer, font, edit_mode, app);

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

static void handle_backspace(SDL_Renderer *renderer, TTF_Font *font, struct app *app, enum edit_mode edit_mode) {
    char *user_input = app->input->text;
    size_t len = strlen(user_input);

    if (len > 0) {
        user_input[len - 1] = '\0';  
        
        app->input->text = (char *)realloc(app->input->text, len);  
        app->input->size = len - 1;
        
        draw_user_confirmation(renderer, font, edit_mode, app);
    }
}

static void handle_default_key(SDL_Renderer *renderer, TTF_Font *font, 
                                struct app *app, SDL_Event event, enum edit_mode edit_mode) {
        char to_ascii = (char)event.key.keysym.sym;

        size_t new_length = strlen(app->input->text) + 2;
        app->input->text = (char *)realloc(app->input->text, new_length);
        if (!app->input->text) {
                fprintf(stderr, "Memory reallocation failed for user_input\n");
                return;
        }

        app->input->text[new_length - 2] = to_ascii;
        app->input->text[new_length - 1] = '\0';
        app->input->size = new_length;

        draw_user_confirmation(renderer, font, edit_mode, app);
}

int get_user_input(SDL_Renderer *renderer, TTF_Font *font, enum edit_mode edit_mode, struct app *app) {
        SDL_Event event;

        app->input->text = (char *)realloc(app->input->text, 1);
        if (!app->input->text) {
                fprintf(stderr, "Memory allocation failed for app->input->text\n");
                return -1;
        }
        app->input->text[0] = '\0';

        draw_user_confirmation(renderer, font, edit_mode, app);

        while (1) {
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                                free(app->input->text);
                                return 0;
                        } else if (event.type == SDL_KEYDOWN) {
                                switch (event.key.keysym.sym) {
                                case SDLK_BACKSPACE: 
                                        handle_backspace(renderer, font, app, edit_mode);  
                                        break; 
                                case SDLK_RETURN:
                                        return 1;
                                case SDLK_ESCAPE:
                                        return 0; 
                                default: 
                                        handle_default_key(renderer, font, app, event, edit_mode);
                                        break; 
                                }
                        }
                }
        }

        return 1;
}
