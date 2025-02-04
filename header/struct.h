#ifndef STRUCT_H
#define STRUCT_H

#include "SDL_stdinc.h"

#define WINDOW_NAME "File Explorer"
#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720
#define CURSOR_WIDTH 15
#define CURSOR_HEIGHT 30
#define FONT_SIZE 25
#define FONT_SPACING_Y 5
#define FONT_SPACING_X 15
#define LINE_WIDTH 100

#define MAX_VISIBLE_LINE WINDOW_HEIGHT / (FONT_SPACING_Y + FONT_SIZE) 

struct rgb {
    Uint8 red;
    Uint8 green;
    Uint8 blue;
};

struct cursor {
    int x;
    int y;
    float opacity;
    struct rgb color;
    int padding;
};

struct text_segment {
    char *text;
    struct rgb color;
};

struct line {
    struct text_segment *segments;
    int segment_count;
};

struct terminal {
    int current_line;
    int total_line;
    int scroll;
    char *path;
    struct line *lines;
};

struct terminal *create_terminal();
void add_line(struct terminal *term, struct line new_line);
void free_terminal(struct terminal *term);

#endif
