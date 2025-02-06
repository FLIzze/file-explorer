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
        SDL_Color text_color = { 0, 255, 0 }; 
        SDL_Color background_color = { 0, 0, 0 }; 
        SDL_Texture *text_texture = create_text_texture(renderer, font, term->path, text_color, background_color);
        SDL_Rect text_location = { 0, WINDOW_HEIGHT - ( FONT_SPACING_Y + FONT_SIZE), 
                WINDOW_WIDTH, 0 };

        SDL_QueryTexture(text_texture, NULL, NULL, &text_location.w, &text_location.h);
        SDL_RenderCopy(renderer, text_texture, NULL, &text_location);
        SDL_DestroyTexture(text_texture);
}

void display_lines(SDL_Renderer *renderer, TTF_Font *font, struct terminal *term) {
        SDL_Color text_color = { 255, 0, 0 };
        SDL_Color background_color = { 0, 0, 0 };
        int position_y = 0;

        int start_line = term->scroll;
        int end_line = term->scroll + MAX_VISIBLE_LINE;
        end_line = (end_line > term->total_line) ? term->total_line : end_line;

        for (int i = start_line; i < end_line; i++) {
                char line_number[20];
                int relative_line_number = (i == term->current_line) ? (i + 1) : abs(i - term->current_line);
                snprintf(line_number, sizeof(line_number), "%d", relative_line_number);

                SDL_Texture *text_texture = create_text_texture(renderer, font, line_number, text_color, background_color);
                SDL_Rect text_location = { 0, position_y, LINE_WIDTH, LINE_HEIGHT };

                SDL_QueryTexture(text_texture, NULL, NULL, &text_location.w, &text_location.h);
                SDL_RenderCopy(renderer, text_texture, NULL, &text_location);
                SDL_DestroyTexture(text_texture);

                position_y += FONT_SIZE + FONT_SPACING_Y;
        }
}

void display_file_content(SDL_Renderer *renderer, TTF_Font *font, struct terminal *term) {
        if (!renderer || !font || !term || !term->lines) {
                fprintf(stderr, "Invalid renderer, font, or terminal!\n");
                return;
        }

        int y = 0;

        int start_line = term->scroll;
        int end_line = term->scroll + MAX_VISIBLE_LINE;
        end_line = (end_line > term->total_line) ? term->total_line : end_line;

        for (int i = start_line; i < end_line; i++) {
                for (int j = 0; j < term->lines[i].segment_count; j++) {
                        struct text_segment *segment = &term->lines[i].segments[j];
                        if (!segment->text) continue;
                        if (strcmp(segment->text, "") == 0) continue;
                        if (!segment->is_cached) {
                                SDL_Color background_color = { 0, 0, 0, 255 };
                                SDL_Color text_color = { segment->color.red, segment->color.green, segment->color.blue, 255 };
                                segment->text_texture = create_text_texture(renderer, font, 
                                                segment->text, text_color, background_color);
                                segment->is_cached = 1;
                        }

                        SDL_Rect text_location = { LINE_WIDTH, y, WINDOW_WIDTH, FONT_SPACING_Y + FONT_SIZE };
                        SDL_QueryTexture(segment->text_texture, NULL, NULL, &text_location.w, &text_location.h);
                        SDL_RenderCopy(renderer, segment->text_texture, NULL, &text_location);
                }
                y += FONT_SIZE + FONT_SPACING_Y;
        }
}

SDL_Texture* display_log(SDL_Renderer *renderer, TTF_Font *font, char* message, struct terminal *term) {
        SDL_Color text_color = { 0, 255, 0 };
        SDL_Color background_color = { 0, 0, 0 };

        SDL_Texture *text_texture = create_text_texture(renderer, font, message, text_color, background_color);
        SDL_Rect text_location = { 0, WINDOW_HEIGHT - 2 * ( FONT_SPACING_Y + FONT_SIZE), 
                WINDOW_WIDTH, FONT_SPACING_Y + FONT_SIZE };

        SDL_QueryTexture(text_texture, NULL, NULL, &text_location.w, &text_location.h);
        SDL_RenderCopy(renderer, text_texture, NULL, &text_location);
        SDL_RenderPresent(renderer);
        term->log.timestamp = SDL_GetTicks();
        return text_texture;
}

SDL_Texture* create_text_texture(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg) {
        SDL_Surface *surface = TTF_RenderText_Shaded(font, text, fg, bg);
        if (!surface) {
                fprintf(stderr, "Could not create surface");
                return NULL;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
                fprintf(stderr, "Could not create texture");
                return NULL;
        }

        SDL_FreeSurface(surface);
        return texture;
}

void display(SDL_Renderer *renderer, TTF_Font *font, struct terminal *term, struct cursor *cursor) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderClear(renderer);

        display_file_content(renderer, font, term);
        display_lines(renderer, font, term);
        display_cursor(renderer, cursor);
        display_path(term, font, renderer);
        if (term->log.message) {
                display_log(renderer, font, term->log.message, term);
        }
        SDL_RenderPresent(renderer);
}
