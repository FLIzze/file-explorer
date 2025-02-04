#include "display.h"
#include "stdio.h"

void display_cursor(SDL_Renderer *renderer, struct cursor *cursor) {
    int alpha = (int)(cursor->opacity * 255);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, cursor->color.red, cursor->color.green, cursor->color.blue, alpha);
    SDL_Rect rect = { cursor->x, cursor->y, WINDOW_WIDTH, FONT_SIZE + cursor->padding }; 
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
    for (int i = 1; i < term->total_line - term->scroll + 1; i++) {
        char line_number[20];
        snprintf(line_number, sizeof(line_number), "%d", i + term->scroll);
        SDL_Surface *textSurface = TTF_RenderText_Shaded(font, line_number, textColor, (SDL_Color){ 0, 0, 0 }); 
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textLocation = { 0, position_y, LINE_WIDTH, 0 };
        SDL_QueryTexture(textTexture, NULL, NULL, &textLocation.w, &textLocation.h);
        SDL_RenderCopy(renderer, textTexture, NULL, &textLocation);
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
        position_y += FONT_SIZE + FONT_SPACING_Y;
    }
}

void display_file_content(SDL_Renderer *renderer, TTF_Font *font, struct terminal *term) {
    if (!renderer || !font || !term || !term->lines) {
        fprintf(stderr, "Invalid renderer, font, or terminal!\n");
        return;
    }

    int x = LINE_WIDTH;
    int y = 0;

    for (int i = term->scroll; i < term->total_line; i++) {
        if (!term->lines[i].segments) continue;

        for (int j = 0; j < term->lines[i].segment_count; j++) {
            if (!term->lines[i].segments[j].text) continue;

            char *segment_text = term->lines[i].segments[j].text;
            for (int k = 0; segment_text[k] != '\0'; k++) {
                if (segment_text[k] == '\n') {
                    segment_text[k] = '\0';  
                }
            }

            SDL_Color text_color = {
                term->lines[i].segments[j].color.red,
                term->lines[i].segments[j].color.green,
                term->lines[i].segments[j].color.blue,
                255
            };

            if (strcmp(term->lines[i].segments[j].text, "") == 0) continue;
            SDL_Surface *text_surface = TTF_RenderText_Solid(font, term->lines[i].segments[j].text, text_color);
            if (!text_surface) {
                fprintf(stderr, "Failed to render text: %s\n", TTF_GetError());
                continue;
            }

            SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
            if (!text_texture) {
                fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
                SDL_FreeSurface(text_surface);
                continue;
            }

            SDL_Rect dest_rect = { x, y, text_surface->w, text_surface->h };
            SDL_RenderCopy(renderer, text_texture, NULL, &dest_rect);

            x += text_surface->w + FONT_SPACING_X;

            SDL_FreeSurface(text_surface);
            SDL_DestroyTexture(text_texture);
        }

        x = LINE_WIDTH;
        y += FONT_SIZE + FONT_SPACING_Y;
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
