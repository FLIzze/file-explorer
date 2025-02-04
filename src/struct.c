#include "struct.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct terminal *create_terminal() {
    struct terminal *term = (struct terminal *)malloc(sizeof(struct terminal));
    if (!term) {
        fprintf(stderr, "Memory allocation failed!\n");
        free(term);
        return NULL;
    }
    term->current_line = 0;
    term->total_line = 0;
    term->scroll = 0;
    term->path = strdup("/");
    if (!term->path) {
        fprintf(stderr, "Memory allocation failed!\n");
        free(term);
        return NULL;
    }
    term->lines = NULL;
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
    if (term) {
        for (int i = 0; i < term->total_line; i++) {
            for (int j = 0; j  < term->lines[i].segment_count; j++) {
                free(term->lines[i].segments[j].text);
            }
            free(term->lines[i].segments);
        }
        free(term->lines);
        free(term->path);
        term->path = NULL; 
        term->lines = NULL;
        term->total_line = 0;
        term->scroll = 0;
        term->current_line = 0;
    }
}
