#include "display.h"
#include "stdio.h"

void display_cursor(SDL_Renderer *renderer, struct cursor *cursor) {
    int alpha = (int)(cursor->opacity * 255);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, cursor->color.red, cursor->color.green, cursor->color.blue, alpha);
    SDL_Rect rect = { cursor->x, cursor->y - cursor->padding / 2, WINDOW_WIDTH, FONT_SIZE + cursor->padding }; 
    SDL_RenderFillRect(renderer, &rect);
}

void display_path(struct terminal *term, TTF_Font *font, SDL_Renderer *renderer) {
    SDL_Color textColor = { 120, 120, 120 }; 
    SDL_Surface *textSurface = TTF_RenderText_Shaded(font, term->path, textColor, (SDL_Color){ 255, 255, 255 }); 
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    SDL_Rect textLocation = {0, WINDOW_HEIGHT - FONT_SIZE, 0, 0};
    SDL_QueryTexture(textTexture, NULL, NULL, &textLocation.w, &textLocation.h);
    SDL_RenderCopy(renderer, textTexture, NULL, &textLocation);
    SDL_DestroyTexture(textTexture);
}

void display_lines(SDL_Renderer *renderer, TTF_Font *font, struct terminal *term) {
    SDL_Color textColor = { 0, 255, 255 };
    int position_y = 0;
    for (int i = 1; position_y < WINDOW_HEIGHT; i++) {
        char line_number[20];
        snprintf(line_number, sizeof(line_number), "%d", i + term->scroll);
        SDL_Surface *textSurface = TTF_RenderText_Shaded(font, line_number, textColor, (SDL_Color){ 0, 0, 0 }); 
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textLocation = { 0, position_y, 0, 0 };
        SDL_QueryTexture(textTexture, NULL, NULL, &textLocation.w, &textLocation.h);
        SDL_RenderCopy(renderer, textTexture, NULL, &textLocation);
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
        position_y += FONT_SIZE + FONT_SPACING_Y;
    }
}

void display_file_content(SDL_Renderer *renderer, TTF_Font *font, struct terminal *term) {
    SDL_Color textColor = { 255, 0, 255 };
    int position_y = 0;

    for (int i = term->scroll; position_y < WINDOW_HEIGHT && i < 2000; i++) {
        char *line = term->content[i];
        if (line == NULL) break;

        char line_copy[256]; 
        strncpy(line_copy, line, sizeof(line_copy) - 1);
        line_copy[sizeof(line_copy) - 1] = '\0'; 

        char *newline_pos = strchr(line_copy, '\n');
        if (newline_pos != NULL) {
            *newline_pos = '\0';
        }

        SDL_Surface *textSurface = TTF_RenderText_Shaded(font, line_copy, textColor, (SDL_Color){ 255, 255, 255 });
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textLocation = { LINE_WIDTH, position_y, 0, 0 };
        SDL_QueryTexture(textTexture, NULL, NULL, &textLocation.w, &textLocation.h);
        SDL_RenderCopy(renderer, textTexture, NULL, &textLocation);

        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);

        position_y += FONT_SIZE + FONT_SPACING_Y;
    }
}

void display(SDL_Renderer *renderer, TTF_Font *font, struct terminal *term, struct cursor *cursor) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
    SDL_RenderClear(renderer);
    display_file_content(renderer, font, term);
    display_cursor(renderer, cursor);
    display_lines(renderer, font, term);
    /* display_path(term, font, renderer); */
    SDL_RenderPresent(renderer);
}
