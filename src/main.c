#include "sdl_init.h"
#include "struct.h"
#include "draw.h"
#include "crud.h"
#include "event.h"

int main(int argc, char *argv[]) {
        SDL_Window *window = NULL;
        SDL_Renderer *renderer = initialize_SDL(&window);
        TTF_Font *font = initialize_font();
        struct app *app = ini_app();

        /* read_file(renderer, app); */
        /* draw(renderer, font, app); */

        /* SDL_Event e; */
        /* int quit = 0; */
        /* while (!quit) { */
        /*         handle_events(&quit, e, app, renderer, font); */
        /* } */

        cleanup_SDL(window, renderer, font);
        free_app(app);

        return 0;
}
