#include "display.h"
#include "struct.h"
#include "event.h"
#include "crud.h"

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
    struct terminal *term = create_terminal();

    if (cursor == NULL) {
        fprintf(stderr, "Failed to allocate memory for cursor\n");
        return -1;
    }

    cursor->x = 0;
    cursor->y = 0;
    cursor->color = { 0, 255, 0 };
    cursor->opacity = 0.4f;
    cursor->padding = 5;

    read_file(term, cursor, renderer, font);
    display(renderer, font, term, cursor);

    SDL_Event e;
    int quit = 0;
    while (!quit) {
        handle_events(&quit, e, cursor, term, renderer, font);
    }

    free_terminal(term);
    free(term);
    free(cursor);

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
