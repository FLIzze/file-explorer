#include "struct.h"
#include "display.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct terminal *create_terminal() {
        struct terminal *term = (struct terminal *)malloc(sizeof(struct terminal));
        if (!term) {
                fprintf(stderr, "Memory allocation failed!\n");
                return NULL;
        }
        term->current_line = 0;
        term->total_line = 0;
        term->scroll = 0;
        term->path = strdup("/home/abel/Documents/");
        if (!term->path) {
                fprintf(stderr, "Memory allocation failed!\n");
                return NULL;
        }
        term->lines = NULL;
        term->log = (struct log *)malloc(sizeof(struct log));
        if (!term->log) {
                fprintf(stderr, "Memory allocation failed!\n");
                return NULL;
        }
        term->log->message =  NULL;
        term->user_input = (struct user_input *)malloc(sizeof(struct user_input));
        if (!term->user_input) {
                fprintf(stderr, "Memory allocation failed!\n");
                return NULL;
        }

        return term;
}

void add_line(struct terminal *term, struct line new_line) { 
        struct line *new_lines = (struct line *)realloc(term->lines, (term->total_line + 1) * sizeof(struct line));
        if (!new_lines) {
                fprintf(stderr, "Memory allocation failed!\n");
                return;
        }
        term->lines = new_lines;
        term->lines[term->total_line] = new_line;
        term->total_line++;
}

void free_terminal(struct terminal *term) { 
        if (!term) {
                fprintf(stderr, "Couldn't free terminal, does not exist.");
                return;
        }

        for (int i = 0; i < term->total_line; i++) {
                for (int j = 0; j  < term->lines[i].segment_count; j++) {
                        struct text_segment *segment = &term->lines[i].segments[j]; 
                        if (segment->text) {
                                free(segment->text);
                                segment->text = NULL;
                        }

                        if (segment->is_cached) {
                                SDL_DestroyTexture(segment->text_texture);
                                segment->text_texture = NULL;
                        }
                }

                if (term->lines[i].segments) {
                        free(term->lines[i].segments);
                }
        }

        if (term->lines) free(term->lines); term->lines = NULL;
        if (term->path) free(term->path); term->path = NULL;
        if (term->log->message) free(term->log->message); term->log->message = NULL;

        term->total_line = 0;
        term->scroll = 0;
        term->current_line = 0;
}

void update_log(struct terminal *term, Uint32 delay, SDL_Renderer *renderer, TTF_Font *font, struct cursor *cursor) {
    if (term->log->message && SDL_GetTicks() - term->log->timestamp > delay * 1000) {
        free(term->log->message);
        term->log->message = NULL;
        display(renderer, font, term, cursor);
    }
}
