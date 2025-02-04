#include "crud.h"
#include "stdio.h"
#include "struct.h"
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void read_file(struct terminal *term, struct cursor *cursor, SDL_Renderer *renderer, TTF_Font *font) {
    if (is_file(term->path)) {
        read_file_content(term);
    } else {
        read_directory_content(term);
    }
}

int is_file(char *path) {
    struct stat fileStat;
    if (stat(path, &fileStat) == -1) {
        perror("is file : stat");
        return -1;  
    }
    return S_ISREG(fileStat.st_mode);  
}

void read_file_content(struct terminal *term) {
    FILE *file = fopen(term->path, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", term->path);
        free(file);
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1) {
        struct line new_line;
        new_line.segment_count = 1;
        new_line.segments = (struct text_segment *)malloc(sizeof(struct text_segment));

        if (!new_line.segments) {
            fprintf(stderr, "Memory allocation failed!\n");
            free(file);
            fclose(file);
            return;
        }

        new_line.segments[0].text = strdup(line); 
        if (!new_line.segments[0].text) {
            fprintf(stderr, "Memory allocation failed!\n");
            free(file);
            fclose(file);
            return;
        }

        new_line.segments[0].color = (struct rgb){0, 0, 0}; 
        add_line(term, new_line);
    }

    free(line);
    fclose(file);
}


void read_directory_content(struct terminal *term) {
    DIR *d;
    struct dirent *dir;
    d = opendir(term->path);

    if (!d) {
        perror("opendir");
        return;
    }

    while ((dir = readdir(d)) != NULL) {
        if (dir->d_name[0] == '.') continue;

        struct line new_line;
        new_line.segment_count = 1;
        new_line.segments = (struct text_segment *)malloc(sizeof(struct text_segment));

        if (!new_line.segments) {
            fprintf(stderr, "Memory allocation failed!\n");
            free(new_line.segments);
            closedir(d);
            return;
        }

        new_line.segments[0].text = strdup(dir->d_name); 
        if (!new_line.segments[0].text) {
            fprintf(stderr, "Memory allocation failed!\n");
            free(new_line.segments);
            closedir(d);
            return;
        }

        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/%s", term->path, dir->d_name);
        if (is_file(full_path)) {
            new_line.segments[0].color = (struct rgb){0, 0, 0}; 
        } else {
            new_line.segments[0].color = (struct rgb){0, 0, 255}; 
        }

        add_line(term, new_line);
    }

    closedir(d);
}

void add_file(struct terminal *term, struct cursor *cursor) {
    FILE *new_file;
    new_file = fopen("/home/abel/Documents/test.txt", "w");
    fclose(new_file);
    free_terminal(term);
    term->path = strdup("/home/abel/Documents");
    cursor->y = 0;
}

void delete_file(struct terminal *term, struct cursor *cursor) {
    if (remove("/home/abel/Documents/test.txt") == 0 ) {
        printf("File deleted\n");
    } else {
        perror("Error deleting the file\n");
    }
    free_terminal(term);
    term->path = strdup("/home/abel/Documents");
    cursor->y = 0;
}
