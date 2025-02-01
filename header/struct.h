#ifndef STRUCT_H
#define STRUCT_H

#define WINDOW_NAME "File Explorer"
#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720
#define CURSOR_WIDTH 15
#define CURSOR_HEIGHT 30
#define FONT_SIZE 25
#define FONT_SPACING_Y 5
#define LINE_WIDTH 100

struct rgb {
    int red;
    int green;
    int blue;
};

struct cursor {
    int x;
    int y;
    float opacity;
    struct rgb color;
    int padding;
};

struct terminal {
    int current_line;
    int total_line;
    int current_column;
    int scroll;
    int offset;
    char *path;
    char *content[2000];
};

struct array {
    unsigned int len;
};

#endif
