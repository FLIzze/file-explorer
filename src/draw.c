#include "draw.h"
#include "config.h"
/* #include "stdio.h" */

/* void display_file_content(SDL_Renderer *renderer, TTF_Font *font, struct terminal *term) { */
/*         if (!renderer || !font || !term || !term->lines) { */
/*                 fprintf(stderr, "Invalid renderer, font, or terminal!\n"); */
/*                 return; */
/*         } */

/*         int y = 0; */

/*         int start_line = term->scroll; */
/*         int end_line = term->scroll + MAX_VISIBLE_LINE; */
/*         end_line = (end_line > term->total_line) ? term->total_line : end_line; */

/*         for (int i = start_line; i < end_line; i++) { */
/*                 for (int j = 0; j < term->lines[i].segment_count; j++) { */
/*                         struct text_segment *segment = &term->lines[i].segments[j]; */
/*                         if (!segment->text) continue; */
/*                         if (strcmp(segment->text, "") == 0) continue; */
/*                         if (!segment->is_cached) { */
/*                                 SDL_Color background_color = { 0, 0, 0, 255 }; */
/*                                 SDL_Color text_color = { segment->color.red, segment->color.green, segment->color.blue, 255 }; */
/*                                 segment->text_texture = create_text_texture(renderer, font, */ 
/*                                                 segment->text, text_color, background_color); */
/*                                 segment->is_cached = 1; */
/*                         } */

/*                         SDL_Rect text_location = { LINE_WIDTH, y, WINDOW_WIDTH, FONT_SPACING_Y + FONT_SIZE }; */
/*                         SDL_QueryTexture(segment->text_texture, NULL, NULL, &text_location.w, &text_location.h); */
/*                         SDL_RenderCopy(renderer, segment->text_texture, NULL, &text_location); */
/*                 } */
/*                 y += FONT_SIZE + FONT_SPACING_Y; */
/*         } */
/* } */

/* SDL_Texture* display_log(SDL_Renderer *renderer, TTF_Font *font, char* message, struct terminal *term) { */
/*     SDL_Color text_color = { 0, 255, 0 }; */
/*     SDL_Color background_color = { 0, 0, 0 }; */

/*     SDL_Rect log_area = { */ 
/*         0, WINDOW_HEIGHT - 2 * (FONT_SPACING_Y + FONT_SIZE), */ 
/*         WINDOW_WIDTH, FONT_SPACING_Y + FONT_SIZE */ 
/*     }; */

/*     SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, 255); */
/*     SDL_RenderFillRect(renderer, &log_area); */

/*     SDL_Texture *text_texture = create_text_texture(renderer, font, message, text_color, background_color); */
/*     SDL_QueryTexture(text_texture, NULL, NULL, &log_area.w, &log_area.h); */
/*     SDL_RenderCopy(renderer, text_texture, NULL, &log_area); */
/*     SDL_RenderPresent(renderer); */

/*     if (!term->log->message) { */
/*         term->log->message = strdup(message); */
/*         term->log->timestamp = SDL_GetTicks(); */
/*     } else if (strcmp(term->log->message, message) != 0) { */
/*         free(term->log->message); */  
/*         term->log->message = strdup(message); */
/*         term->log->timestamp = SDL_GetTicks(); */
/*     } */

/*     return text_texture; */
/* } */

void draw_lines(SDL_Renderer *renderer, TTF_Font *font, struct app *app) {
        SDL_Color text_color = { 0, 0, 0 };
        SDL_Color background_color = { 255, 255, 255 };
        int position_y = LINE_HEIGHT;

        int start_line = 0;
        int end_line = MAX_VISIBLE_LINE;

        if (app->mode == FILE_EXPLORER) {
                end_line = (end_line > app->file_list->count) ? app->file_list->count : end_line;
        }

        for (int i = start_line; i < end_line; i++) {
                char line_number[12];
                snprintf(line_number, sizeof(line_number), "%d", i + 1);
                SDL_Texture *texture = create_text_texture(renderer, font, line_number, text_color, background_color);
                SDL_Rect location = { 0, position_y, LINE_WIDTH, LINE_HEIGHT };

                SDL_QueryTexture(texture, NULL, NULL, &location.w, &location.h);
                SDL_RenderCopy(renderer, texture, NULL, &location);
                SDL_DestroyTexture(texture);

                position_y += LINE_HEIGHT;
        }
}

SDL_Texture* create_text_texture(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg) {
        SDL_Surface *surface = TTF_RenderText_Shaded(font, text, fg, bg);
        if (!surface) {
                fprintf(stderr, "Could not create surface\n");
                return NULL;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
                fprintf(stderr, "Could not create texture\n");
                return NULL;
        }

        SDL_FreeSurface(surface);
        return texture;
}

void draw_explorer_content(SDL_Renderer *renderer, TTF_Font *font, struct app *app) {
        int position_y = LINE_HEIGHT;
        for (int i = 0; i < app->file_list->count; i++) {
                struct file_entry entry = app->file_list->file_entry[i];
                SDL_Texture *texture = create_text_texture(renderer, font, entry.name, entry.foreground_color, entry.background_color);
                SDL_Rect location = { LINE_WIDTH, position_y, 0, LINE_HEIGHT };
                SDL_QueryTexture(texture, NULL, NULL, &location.w, &location.h);
                SDL_RenderCopy(renderer, texture, NULL, &location);
                SDL_DestroyTexture(texture);
                position_y += LINE_HEIGHT;
        }
}

void draw_path(SDL_Renderer *renderer, TTF_Font *font, struct app *app) {
        SDL_Color text_color = { 0, 0, 255 }; 
        SDL_Color background_color = { 255, 255, 255 }; 
        SDL_Texture *text_texture = create_text_texture(renderer, font, app->path, text_color, background_color);
        SDL_Rect text_location = { 0, 0, WINDOW_WIDTH, LINE_HEIGHT };

        SDL_QueryTexture(text_texture, NULL, NULL, &text_location.w, &text_location.h);
        SDL_RenderCopy(renderer, text_texture, NULL, &text_location);
        SDL_DestroyTexture(text_texture);
}

void display_cursor(SDL_Renderer *renderer, struct app *app) {
        struct cursor *cursor = app->cursor;

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, cursor->color.r, cursor->color.g, cursor->color.b, cursor->color.a);
        SDL_Rect rect = { LINE_WIDTH + cursor->column * FONT_SIZE, cursor->line * LINE_HEIGHT, 15, LINE_HEIGHT }; 
        SDL_RenderFillRect(renderer, &rect);
}

void draw(SDL_Renderer *renderer, TTF_Font *font, struct app *app) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
        SDL_RenderClear(renderer);
        draw_explorer_content(renderer, font, app);
        draw_lines(renderer, font, app);
        draw_path(renderer, font, app);
        display_cursor(renderer, app);

        /* if (term->log->message) { */
        /*         display_log(renderer, font, term->log->message, term); */
        /* } */
        SDL_RenderPresent(renderer);
}
