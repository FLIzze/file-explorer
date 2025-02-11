#ifndef STRUCT_H
#define STRUCT_H

#include <SDL.h>
#include <SDL_ttf.h>

#define ADD_DA(array, count, capacity, type, value) \
        do { \
                if ((count) >= (capacity)) { \
                        size_t new_capacity = (capacity) == 0 ? 1 : (capacity) * 2; \
                        type *new_array = (type *)realloc((array), new_capacity * sizeof(type)); \
                        if (!new_array) { \
                                fprintf(stderr, "Memory reallocation failed for %s\n", #array); \
                                break; \
                        } \
                        (array) = new_array; \
                        (capacity) = new_capacity; \
                } \
                (array)[(count)] = (value); \
                (count)++; \
        } while (0)

enum mode {
        FILE_EXPLORER,
        TEXT_EDITOR,
};

enum editor_mode {
        NORMAL,
        INSERT,
};

enum log_level {
        INFO,
        WARNING,
        ERROR,
};

struct file_list {
        struct file_entry *file_entry;
        size_t count; 
        size_t capacity;
};

struct file_entry {
        char *name;
        SDL_Color background_color;
        SDL_Color foreground_color;
        short is_file;
        time_t last_edit;
        time_t creation;
};

struct text_editor {
        enum mode editor_mode;
        struct lines *lines;
};

struct app {
        char *path;
        struct cursor *cursor;
        struct user_input *input;
        struct log *log;
        struct file_list *file_list;
        struct text_editor *editor;
        enum mode mode;
};

struct cursor {
        int scroll;
        int column;
        int line;
        SDL_Color color;
};

struct log {
        char *title;
        char *message;
        unsigned int timestamp;
        enum log_level log_level;
};

struct user_input {
        char *text;
};

struct text_segment {
        char *text;
        SDL_Color color;
        size_t count;
        size_t capacity;
};

struct lines {
        struct text_segment *segments;
        size_t count; 
        size_t capacity;
};

void free_content(struct app *app);

#endif
