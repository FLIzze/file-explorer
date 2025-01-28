#ifndef EVENT_H
#define EVENT_H

#include <SDL2/SDL.h>
#include "struct.h"

void handle_keyboard(SDL_Event e, struct cursor *cursor, struct terminal *term);
void handle_events(int quit, SDL_Event e, struct cursor *cursor, struct terminal *term);

#endif
