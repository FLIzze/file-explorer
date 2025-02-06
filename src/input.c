#include "input.h"
#include "display.h"

int user_confirmation(SDL_Renderer *renderer, TTF_Font *font, char* message, struct terminal *term) {
        SDL_Event event;

        char delete_message[] = "confirm deletion of : '";
        char delete_message_choice[] = "' [Y]es [N]o";
        size_t full_message_len = strlen(message) + strlen(delete_message) + strlen(delete_message_choice) + 3;
        char *full_message = (char *)malloc(full_message_len);
        snprintf(full_message, full_message_len, "%s %s %s", delete_message, message, delete_message_choice);

        SDL_Texture *texture = display_log(renderer, font, full_message, term);

        while (1) {
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                                SDL_DestroyTexture(texture);
                                return -1;
                        }
                        if (event.type == SDL_KEYDOWN) {
                                if (event.key.keysym.sym == SDLK_y || event.key.keysym.sym == SDLK_RETURN) {
                                        SDL_DestroyTexture(texture);
                                        return 1;
                                } else if (event.key.keysym.sym == SDLK_n || event.key.keysym.sym == SDLK_ESCAPE) {
                                        SDL_DestroyTexture(texture);
                                        return 0;
                                }
                        }
                }
        }
}

void user_text_input(struct terminal *term, SDL_Renderer *renderer, TTF_Font *font) {
        SDL_Event event;

        term->user_input->text = (char *)malloc(1);
        if (!term->user_input->text) {
                fprintf(stderr, "Memory allocation failed\n");
                free(term);
                return;
        }
        term->user_input->text[0] = '\0';

        while (1) {
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                                return;
                        }
                        if (event.type == SDL_KEYDOWN) {
                                switch(event.key.keysym.sym) {
                                        case SDLK_RETURN:
                                                display_log(renderer, font, term->user_input->text, term);
                                                return;
                                        case SDLK_ESCAPE:
                                                term->log->message = strdup("Aborting...");
                                                return;
                                        default:
                                                char to_ascii = (char)event.key.keysym.sym;
                                                size_t input_len = strlen(term->user_input->text) + 2;
                                                char *new_buffer= (char *)realloc(term->user_input->text, input_len);
                                                if (!new_buffer) {
                                                        fprintf(stderr, "Memory allocation failed\n");
                                                        return;
                                                }
                                                term->user_input->text = new_buffer;
                                                size_t text_len = strlen(term->user_input->text);
                                                term->user_input->text[text_len] = to_ascii;
                                                term->user_input->text[text_len + 1] = '\0';
                                                break;
                                }
                        }
                }
        }
}
