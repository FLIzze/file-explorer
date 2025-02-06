#include "crud.h"
#include "stdio.h"
#include "struct.h"
#include "input.h"
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ftw.h>

void read_file(struct terminal *term, struct cursor *cursor, SDL_Renderer *renderer, TTF_Font *font) {
        if (is_file(term->path) == 1) {
                read_file_content(term);
        } else if (is_file(term->path) == 0) {
                read_directory_content(term);
        } else {
                perror("Error determining if path is file or directory");
        }
}

int is_file(char *path) {
        struct stat file_stat;
        if (stat(path, &file_stat) == -1) {
                perror("is_file: stat");
                return -1;  
        }
        return S_ISREG(file_stat.st_mode);  
}

static void add_line_to_terminal(struct terminal *term, char *text, struct rgb color) {
        struct line new_line;
        new_line.segment_count = 1;
        new_line.segments = (struct text_segment *)malloc(sizeof(struct text_segment));

        if (!new_line.segments) {
                fprintf(stderr, "Memory allocation failed for line segment\n");
                return;
        }

        new_line.segments[0].text = strdup(text); 
        if (!new_line.segments[0].text) {
                fprintf(stderr, "Memory allocation failed for line text\n");
                free(new_line.segments);
                return;
        }

        new_line.segments[0].is_cached = 0;
        new_line.segments[0].color = color;
        add_line(term, new_line); 
}

void read_file_content(struct terminal *term) {
        FILE *file = fopen(term->path, "r");
        if (file == NULL) {
                perror("Could not open file");
                return;
        }

        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        while ((read = getline(&line, &len, file)) != -1) {
                add_line_to_terminal(term, line, (struct rgb){255, 255, 255}); 
        }

        free(line); 
        fclose(file);
}

void read_directory_content(struct terminal *term) {
        DIR *d = opendir(term->path);
        if (!d) {
                perror("opendir");
                return;
        }

        struct dirent *dir;
        while ((dir = readdir(d)) != NULL) {
                if (dir->d_name[0] == '.') continue; 

                size_t full_path_len = strlen(term->path) + strlen(dir->d_name) + 2;
                char *full_path = (char *)malloc(full_path_len);
                snprintf(full_path, full_path_len, "%s/%s", term->path, dir->d_name);

                struct rgb color = is_file(full_path) ? (struct rgb){255, 255, 255} : (struct rgb){0, 0, 255};
                add_line_to_terminal(term, dir->d_name, color);
        }

        closedir(d);
}

void add_file(struct terminal *term, struct cursor *cursor) {
        FILE *new_file = fopen("/home/abel/Documents/test.txt", "w");
        if (new_file == NULL) {
                perror("Error creating file");
                return;
        }

        fclose(new_file);
        free_terminal(term);
        term->path = strdup("/home/abel/Documents");
        cursor->y = 0;
}

void add_directory(struct terminal *term, struct cursor *cursor) {
        if (mkdir("/home/abel/Documents/test", 0700) == -1) {
                perror("Error creating directory");
        }
}

static int remove_callback(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
        int ret = remove(fpath); 
        if (ret) {
                perror(fpath);
        }
        return ret;
}

static void delete_directory(char *path) {
        if (nftw(path, remove_callback, 64, FTW_DEPTH | FTW_PHYS) == -1) {
                perror("Error deleting directory");
        } else {
                printf("Directory deleted successfully.\n");
        }
}

static void delete_file(char *path) {
        if (remove(path) == 0) {
                printf("File deleted\n");
        } else {
                perror("Error deleting the file");
        }
}

int delete_content(struct terminal *term, struct cursor *cursor, SDL_Renderer *renderer, TTF_Font *font) {
        char *file_name = term->lines[term->current_line].segments[0].text;
        size_t new_path_len = strlen(term->path) + strlen(file_name) + 2; 
        char *new_path = (char *)malloc(new_path_len);
        snprintf(new_path, new_path_len, "%s/%s", term->path, file_name);

        if (!user_confirmation(renderer, font, file_name, term)) {
                term->log->message = strdup("Aborting...");
                free(new_path);
                return 0;
        }

        if (is_file(new_path)) {
                delete_file(new_path);
        } else {
                delete_directory(new_path);
        }

        free(new_path);
        int previous_line = term->current_line;
        int previous_scroll = term->scroll;
        int total_line = term->total_line;
        char *previous_path = strdup(term->path);
        free_terminal(term);

        if (previous_line >= total_line - 1) {
                term->current_line = previous_line - 1;
                cursor->y -= (FONT_SIZE + FONT_SPACING_Y);
        } else {
                term->current_line = previous_line;
        }

        term->scroll = previous_scroll;
        term->path = previous_path;
        term->log->message = strdup("Deleted");
        return 1;
}
