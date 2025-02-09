#include "sdl_init.h"
#include "struct.h"
#include "draw.h"
#include "crud.h"
#include "event.h"

int main(int argc, char *argv[]) {
        SDL_Window *window = NULL;
        SDL_Renderer *renderer = initialize_SDL(window);
        TTF_Font *font = initialize_font();

        struct app *app = (struct app *)malloc(sizeof(struct app));
        if (!app) {
                fprintf(stderr, "Memory allocation failed for app\n");
                return -1;
        }
        app->path = strdup("/");
        if (!app->path) {
                fprintf(stderr, "Memory allocation failed for app->path\n");
                free(app);
                return -1;
        }

        app->file_list = (struct file_list *)malloc(sizeof(struct file_list));
        if (!app->file_list) {
                fprintf(stderr, "Memory allocation failed for file_list\n");
                free(app->path);
                free(app);
                return -1;
        }
        app->file_list->file_entry = NULL;
        app->file_list->capacity = 0;
        app->file_list->count = 0;

        app->cursor = (struct cursor *)malloc(sizeof(struct cursor));
        app->cursor->column = 0;
        app->cursor->line = 2;
        app->cursor->scroll = 0;
        app->cursor->color.a = 100;
        app->cursor->color.r = 255;
        app->cursor->color.g = 0;
        app->cursor->color.b = 0;

        read_file(renderer, app);
        draw(renderer, font, app);

        SDL_Event e;
        int quit = 0;
        while (!quit) {
                handle_events(&quit, e, app, renderer, font);
                /* update_log(term, LOG_DELAY, renderer, font, cursor); */
        }

        cleanup_SDL(window, renderer);

        return 0;
}
