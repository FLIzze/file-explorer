#ifndef STRUCT_H
#define STRUCT_H

#include <SDL.h>
#include <SDL_ttf.h>

#define WINDOW_NAME "File Explorer"
#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720
#define CURSOR_WIDTH 15
#define CURSOR_HEIGHT 30
#define FONT_SIZE 25
#define FONT_SPACING_Y 5
#define FONT_SPACING_X 15
#define LINE_WIDTH 100
#define LINE_OFFSET 2
#define LOG_DELAY 2

#define LINE_HEIGHT (FONT_SPACING_Y + FONT_SIZE)
#define LINES_HEIGHT (WINDOW_HEIGHT - (LINE_HEIGHT * LINE_OFFSET))
#define MAX_VISIBLE_LINE (LINES_HEIGHT / LINE_HEIGHT)

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
        SDL_Texture *text_texture;
        int is_cached;
};

struct line {
        struct text_segment *segments;
        int segment_count;
};

struct log {
        char *message;
        Uint32 timestamp;
};

struct user_input {
        char *text;
        int size;
};

struct terminal {
        int current_line;
        int total_line;
        int scroll;
        char *path;
        struct user_input *user_input;
        struct log *log;
        struct line *lines;
};

struct terminal *create_terminal();
void add_line(struct terminal *term, struct line new_line);
void free_terminal(struct terminal *term);
void update_log(struct terminal *term, Uint32 delay, SDL_Renderer *renderer, TTF_Font *font, struct cursor *cursor);

#endif
