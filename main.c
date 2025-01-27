#include "SDL_keycode.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <cstdlib>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#define WINDOW_NAME "File Explorer"
#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720
#define CURSOR_WIDTH 15
#define CURSOR_HEIGHT 30
#define FONT_SIZE 25
#define FONT_SPACING_Y 5

struct rgb {
    int red;
    int green;
    int blue;
};

struct cursor {
    int x;
    int y;
    float opacity;
    struct rgb color;
    int padding;
};

struct terminal {
    int current_line;
    int total_line;
    int current_column;
    char path[512];
    char files[512][512];
};

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

void display_cursor(SDL_Renderer *renderer, struct cursor *cursor) {
    int alpha = (int)(cursor->opacity * 255);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, cursor->color.red, cursor->color.green, cursor->color.blue, alpha);

    SDL_Rect rect = {cursor->x, cursor->y - cursor->padding / 2, WINDOW_WIDTH, FONT_SIZE + cursor->padding }; 
    SDL_RenderFillRect(renderer, &rect);
}

void display_path(struct terminal *term, TTF_Font *font, SDL_Renderer *renderer) {
    SDL_Color textColor = { 0, 0, 0 }; 
    SDL_Surface *textSurface = TTF_RenderText_Shaded(font, term->path, textColor, (SDL_Color){ 255, 255, 255 }); 
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    SDL_Rect textLocation = {0, WINDOW_HEIGHT - FONT_SIZE, 0, 0};
    SDL_QueryTexture(textTexture, NULL, NULL, &textLocation.w, &textLocation.h);
    SDL_RenderCopy(renderer, textTexture, NULL, &textLocation);
    SDL_DestroyTexture(textTexture);
}

void display_files(SDL_Renderer *renderer, TTF_Font *font, struct terminal *term, struct cursor *cursor) {
    struct dirent *entry;
    DIR *dp = opendir(term->path);

    if (dp == NULL) {
        perror("opendir");
        return;
    }

    SDL_Color textColor = { 0, 0, 0 }; 
    int position_y = 0;
    int index_y = 1;

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue;
        }

        char text[512];
        snprintf(text, sizeof(text), "%d %s", index_y - term->current_line, entry->d_name);

        strcpy(term->files[index_y], entry->d_name);

        SDL_Surface *textSurface = TTF_RenderText_Shaded(font, text, textColor, (SDL_Color){ 255, 255, 255 }); 
        if (textSurface == NULL) {
            fprintf(stderr, "Failed to create text surface! TTF_Error: %s\n", TTF_GetError());
            closedir(dp);
            return;
        }

        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);

        if (textTexture == NULL) {
            fprintf(stderr, "Failed to create text texture! SDL_Error: %s\n", SDL_GetError());
            closedir(dp);
            return;
        }

        SDL_Rect textLocation = {0, position_y, 0, 0};
        SDL_QueryTexture(textTexture, NULL, NULL, &textLocation.w, &textLocation.h);
        SDL_RenderCopy(renderer, textTexture, NULL, &textLocation);
        SDL_DestroyTexture(textTexture);

        position_y += FONT_SIZE + FONT_SPACING_Y;
        index_y += 1;
    }

    term->total_line = index_y;

    closedir(dp);
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow(WINDOW_NAME, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    TTF_Font *font = TTF_OpenFont("./roboto.ttf", FONT_SIZE);
    if (font == NULL) {
        fprintf(stderr, "Failed to load font! TTF_Error: %s\n", TTF_GetError());
        return -1;
    }

    struct cursor *cursor = (struct cursor *)malloc(sizeof(struct cursor));
    struct terminal *term = (struct terminal *)malloc(sizeof(struct terminal));

    if (cursor == NULL || term == NULL) {
        fprintf(stderr, "Failed to allocate memory for cursor or terminal\n");
        return -1;
    }

    cursor->x = 0;
    cursor->y = 0;
    cursor->color = { 150, 0, 0 };
    cursor->opacity = 0.3f;
    cursor->padding = 5;

    term->current_line = 1;
    term->total_line = 1;
    strcpy(term->path, "/");

    SDL_Event e;
    int quit = 0;
    while (!quit) {
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

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
        SDL_RenderClear(renderer);

        display_files(renderer, font, term, cursor);

        display_cursor(renderer, cursor);

        display_path(term, font, renderer);

        SDL_RenderPresent(renderer);
    }

    free(term);
    free(cursor);

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
