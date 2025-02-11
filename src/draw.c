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

SDL_Texture* draw_log(SDL_Renderer *renderer, TTF_Font *font, struct app *app) {
        SDL_Color text_color = { 0, 255, 0 }; 
        SDL_Color background_color = { 255, 0, 0 }; 

        SDL_Rect square_area = { (WINDOW_WIDTH - 400) / 2, (WINDOW_HEIGHT - 400) / 2, 400, 400 };

        SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, 255);
        SDL_RenderFillRect(renderer, &square_area); 

        int title_width, title_height;
        if (TTF_SizeText(font, app->log->message, &title_width, &title_height) == -1) {
                printf("Error calculating text size: %s\n", TTF_GetError());
                return NULL;
        }

        SDL_Texture *title_texture = create_text_texture(renderer, font, app->log->message, text_color, background_color);

        SDL_Rect title_area = { 
                square_area.x + (square_area.w - title_width) / 2, 
                square_area.y, 
                title_width, title_height 
        };

        SDL_RenderCopy(renderer, title_texture, NULL, &title_area);

        int message_width, message_height;
        if (TTF_SizeText(font, app->log->message, &message_width, &message_height) == -1) {
                printf("Error calculating message size: %s\n", TTF_GetError());
                return NULL;
        }

        SDL_Texture *message_texture = create_text_texture(renderer, font, app->log->title, text_color, background_color);

        SDL_Rect message_area = { 
                square_area.x, 
                square_area.y + square_area.h - message_height, 
                message_width, message_height 
        };

        SDL_RenderCopy(renderer, message_texture, NULL, &message_area);

        SDL_RenderPresent(renderer);

        return title_texture;
}

void draw_lines(SDL_Renderer *renderer, TTF_Font *font, struct app *app) {
        SDL_Color text_color = { 0, 0, 0 };
        SDL_Color background_color = { 255, 255, 255 };
        int position_y = LINE_HEIGHT;

        int end_line = app->file_list->count >= MAX_VISIBLE_LINE - LINE_OFFSET 
                ? MAX_VISIBLE_LINE : app->file_list->count;
        SDL_Texture *texture;

        for (int i = app->cursor->scroll; i < end_line + app->cursor->scroll; i++) {
                char line_number[12];
                snprintf(line_number, sizeof(line_number), "%d", i + 1);
                SDL_Texture *texture = create_text_texture(renderer, font, line_number, text_color, background_color);
                SDL_Rect location = { 0, position_y, LINE_WIDTH, LINE_HEIGHT };

                SDL_QueryTexture(texture, NULL, NULL, &location.w, &location.h);
                SDL_RenderCopy(renderer, texture, NULL, &location);

                position_y += LINE_HEIGHT;
        }

        SDL_DestroyTexture(texture);
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
        int end_line = app->file_list->count >= MAX_VISIBLE_LINE - LINE_OFFSET 
                ? MAX_VISIBLE_LINE : app->file_list->count;
        for (int i = app->cursor->scroll; i < end_line + app->cursor->scroll; i++) {
                struct file_entry entry = app->file_list->file_entry[i];
                SDL_Color background_color;
                SDL_Color foreground_color;

                if (i == app->cursor->line + app->cursor->scroll) {
                        background_color = { 255, 255, 255, 255};
                        foreground_color = { 255, 0, 0 ,255 };
                        SDL_Texture *texture = create_text_texture(renderer, font, ">", 
                                        foreground_color, background_color);
                        int current_line = (app->cursor->line + LINE_OFFSET) * LINE_HEIGHT;
                        SDL_Rect location = { 5, current_line, 0, LINE_HEIGHT };
                        SDL_QueryTexture(texture, NULL, NULL, &location.w, &location.h);
                        SDL_RenderCopy(renderer, texture, NULL, &location);
                        SDL_DestroyTexture(texture);
                } else {
                        background_color = entry.background_color;
                        foreground_color = entry.foreground_color;
                }

                SDL_Texture *texture = create_text_texture(renderer, font, entry.name, 
                                foreground_color, background_color);
                SDL_Rect location = { 50, position_y, 0, LINE_HEIGHT };
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

void draw_cursor(SDL_Renderer *renderer, struct app *app) {
        /* struct cursor *cursor = app->cursor; */

        /* SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); */
        /* SDL_SetRenderDrawColor(renderer, cursor->color.r, cursor->color.g, cursor->color.b, cursor->color.a); */
        /* SDL_Rect rect = { 0 + cursor->column * FONT_SIZE, */ 
        /*         (cursor->line + LINE_OFFSET) * LINE_HEIGHT, */ 
        /*         15, */ 
        /*         LINE_HEIGHT }; */ 
        /* SDL_RenderFillRect(renderer, &rect); */
}

void draw_user_confirmation(SDL_Renderer *renderer, TTF_Font *font, char *file_name_confirmation) {
        char confirmation[] = "[Y]es [N]o";

        SDL_Rect square_area = { (WINDOW_WIDTH - 500) / 2, (WINDOW_HEIGHT - 500) / 2, 500, 500 };

        int confirm_width, confirm_height;
        if (TTF_SizeText(font, confirmation, &confirm_width, &confirm_height) == -1) {
                printf("Erorr calculating text size\n");
                return;
        }

        SDL_Color background = { 0, 0, 0, 255 };
        SDL_Color foreground = { 255, 255, 255, 255 };
        SDL_Texture *confirm_texture = create_text_texture(renderer, font, confirmation, foreground, background);
        SDL_Rect confirm_area = { 
                square_area.x + (square_area.w - confirm_width) / 2, 
                square_area.y + square_area.h - confirm_height,       
                confirm_width, 
                confirm_height 
        };

        int file_name_width, file_name_height;
        if (TTF_SizeText(font, file_name_confirmation, &file_name_width, &file_name_height) == -1) {
                printf("Erorr calculating text size\n");
                return;
        }

        SDL_Texture *file_name_texture = create_text_texture(renderer, font, file_name_confirmation, foreground, background);
        SDL_Rect file_name_area = { square_area.x, square_area.y, file_name_width, file_name_height };

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &square_area); 
        SDL_RenderCopy(renderer, confirm_texture, NULL, &confirm_area);
        SDL_RenderCopy(renderer, file_name_texture, NULL, &file_name_area);

        SDL_RenderPresent(renderer);
}

void draw(SDL_Renderer *renderer, TTF_Font *font, struct app *app) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
        SDL_RenderClear(renderer);
        draw_explorer_content(renderer, font, app);
        /* draw_lines(renderer, font, app); */
        draw_path(renderer, font, app);
        /* draw_cursor(renderer, app); */
        /* draw_log(renderer, font, app); */

        /* if (term->log->message) { */
        /*         display_log(renderer, font, term->log->message, term); */
        /* } */
        SDL_RenderPresent(renderer);
}
