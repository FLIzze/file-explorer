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
        SDL_DestroyTexture(message_texture);

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
        int end_line = app->file_list->count >= MAX_VISIBLE_LINE - LINE_OFFSET 
                ? MAX_VISIBLE_LINE : app->file_list->count;
        for (int i = app->cursor->scroll; i < end_line + app->cursor->scroll; i++) {
                struct file_entry entry = app->file_list->file_entry[i];
                SDL_Color bg_c;
                SDL_Color fg_c;

                if (i == app->cursor->line + app->cursor->scroll) {
                        bg_c = { 255, 255, 255, 255};
                        fg_c = { 255, 0, 0 ,255 };
                        SDL_Texture *texture = create_text_texture(renderer, font, ">", 
                                        fg_c, bg_c);
                        int current_line = (app->cursor->line + LINE_OFFSET) * LINE_HEIGHT;
                        SDL_Rect location = { LINE_WIDTH - 30, current_line, 0, LINE_HEIGHT };
                        SDL_QueryTexture(texture, NULL, NULL, &location.w, &location.h);
                        SDL_RenderCopy(renderer, texture, NULL, &location);
                        SDL_DestroyTexture(texture);
                } else {
                        bg_c = entry.background_color;
                        fg_c = entry.foreground_color;
                }

                /* size_t metadata_len = strlen(entry.owner) + strlen(entry.group) + sizeof(entry.last_edit) + */ 
                /*         sizeof(entry.permissions) + 5; */
                /* char *full_metadata = (char *)malloc(metadata_len); */
                /* snprintf(full_metadata, metadata_len, "%s %s %lu %d", */ 
                /*                 entry.owner, entry.group, entry.last_edit, entry.permissions); */

                /* SDL_Texture *metadata_texture = create_text_texture(renderer, font, full_metadata, */ 
                /*                 SDL_Color{0, 0, 0, 255} , SDL_Color{255, 255, 255, 255}); */
                /* SDL_Rect metadata_location = { 0, position_y, 0, LINE_HEIGHT }; */
                /* SDL_QueryTexture(metadata_texture, NULL, NULL, &metadata_location.w, &metadata_location.h); */
                /* SDL_RenderCopy(renderer, metadata_texture, NULL, &metadata_location); */
                /* SDL_DestroyTexture(metadata_texture); */

                SDL_Texture *texture = create_text_texture(renderer, font, entry.name, fg_c, bg_c);
                SDL_Rect location = { LINE_WIDTH, position_y, 0, LINE_HEIGHT };
                SDL_QueryTexture(texture, NULL, NULL, &location.w, &location.h);
                SDL_RenderCopy(renderer, texture, NULL, &location);
                SDL_DestroyTexture(texture);
                position_y += LINE_HEIGHT;

                /* free(full_metadata); */
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

void draw_user_confirmation(SDL_Renderer *renderer, TTF_Font *font, enum edit_mode edit_mode, struct app *app) {
        char *message;

        if (edit_mode == RENAME) {
                message = strdup("RENAME YES");
        } else if (edit_mode == DELETE) {
                message = strdup("DELETE YES");
        }

        SDL_Color fg_c = { 0, 0, 0, 255 };
        SDL_Color bg_c = { 0, 255, 0, 255 };

        int file_name_width, file_name_height;
        if (TTF_SizeText(font, message, &file_name_width, &file_name_height) == -1) {
                printf("Erorr calculating text size\n");
                return;
        }

        SDL_Texture *file_name_texture = create_text_texture(renderer, font, message, fg_c, bg_c);
        struct file_entry entry = app->file_list->file_entry[app->cursor->line];
        SDL_Rect file_name_area = { 
                entry.x + entry.width,
                LINE_HEIGHT * (app->cursor->line + LINE_OFFSET), 
                file_name_width, 
                file_name_height 
        };

        SDL_RenderCopy(renderer, file_name_texture, NULL, &file_name_area);
        SDL_RenderPresent(renderer);
}

void draw(SDL_Renderer *renderer, TTF_Font *font, struct app *app) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
        SDL_RenderClear(renderer);
        draw_explorer_content(renderer, font, app);
        draw_path(renderer, font, app);
        /* draw_lines(renderer, font, app); */
        /* draw_cursor(renderer, app); */
        /* draw_log(renderer, font, app); */

        /* if (term->log->message) { */
        /*         display_log(renderer, font, term->log->message, term); */
        /* } */
        SDL_RenderPresent(renderer);
}
