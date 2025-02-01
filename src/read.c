#include "read.h"
#include "stdio.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void read_file(struct terminal *term) {
    if (is_file(term->path)) {
        read_file_content(term);
    } else {
        read_directory_content(term);
    }
}

int is_file(char *path) {
    struct stat fileStat;
    if (stat(path, &fileStat) == -1) {
        perror("stat");
        return -1;  
    }
    return S_ISREG(fileStat.st_mode);  
}

void read_file_content(struct terminal *term) {
    FILE *file = fopen(term->path, "r");

    char *line;
    size_t len = 0;
    ssize_t read;
    unsigned int index = 0;

    if (file == NULL) {
        printf("Could not open file %s\n", term->path);
        return;
    }

    free_content(term);

    while ((read = getline(&line, &len, file)) != -1) {
        term->content[index] = strdup(line);
        if (term->content[index] == NULL) {
            printf("failed to allocate memory for line %d\n", index);
            fclose(file);
            return;
        }
        index++;
    }

    term->total_line = index;
    free(line);
    fclose(file);
}

void read_directory_content(struct terminal *term) {
    DIR *d;
    struct dirent *dir;
    d = opendir(term->path);

    free_content(term);
    int index = 0;

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            term->content[index] = strdup(dir->d_name);
            if (term->content[index] == NULL) {
                printf("Failed to allocate memory for directory %d\n", index);
                continue;
            }
            index++;
        }
    }

    term->total_line = index;

    closedir(d);
}

void free_content(struct terminal *term) {
    for (int i = 0; i < term->total_line; i++) {
        free(term->content[i]);
        term->content[i] = NULL;
    }

    term->total_line = 0;
}
