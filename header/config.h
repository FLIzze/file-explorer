#ifndef CONFIG_H
#define CONFIG_H

#define WINDOW_NAME "File Explorer"
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define CURSOR_WIDTH 15
#define CURSOR_HEIGHT 30

#define FONT_SIZE 25
#define FONT_SPACING_Y 5
#define FONT_SPACING_X 15

#define LINE_WIDTH 100
#define LINE_OFFSET 1
#define LOG_DELAY 2

#define LINE_HEIGHT (FONT_SPACING_Y + FONT_SIZE)
#define LINES_HEIGHT (WINDOW_HEIGHT - (LINE_HEIGHT * LINE_OFFSET))
#define MAX_VISIBLE_LINE ((WINDOW_HEIGHT - (LINE_HEIGHT * LINE_OFFSET)) / LINE_HEIGHT) - 4

#endif
