#include "crud.h"
#include "struct.h"
#include "stdio.h"
#include "input.h"
#include <dirent.h>

void read_file(SDL_Renderer *renderer, struct app *app) {
        free_app_content(app);
        if (is_file(app->path) == 1) {
                app->mode = TEXT_EDITOR;
                read_file_content(app);
        } else if (is_file(app->path) == 0) {
                app->mode = FILE_EXPLORER;
                read_directory_content(app);
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

void read_file_content(struct app *app) {
        /* FILE *file = fopen(term->path, "r"); */
        /* if (file == NULL) { */
        /*         perror("Could not open file"); */
        /*         return; */
        /* } */

        /* char *line = NULL; */
        /* size_t len = 0; */
        /* ssize_t read; */

        /* while ((read = getline(&line, &len, file)) != -1) { */
        /*         add_line_to_terminal(term, line, (struct rgb){255, 255, 255}); */ 
        /* } */

        /* free(line); */ 
        /* fclose(file); */
}

void read_directory_content(struct app *app) {
        DIR *d = opendir(app->path);
        if (!d) {
                perror("opendir");
                return;
        }

        struct dirent *dir;
        while ((dir = readdir(d)) != NULL) {
                if (dir->d_name[0] == '.') continue;

                size_t full_path_len = strlen(app->path) + strlen(dir->d_name) + 2;
                char *full_path = (char *)malloc(full_path_len);
                if (!full_path) {
                        fprintf(stderr, "Memory allocation failed for full_path\n");
                        continue;
                }
                snprintf(full_path, full_path_len, "%s/%s", app->path, dir->d_name);

                struct file_entry entry;
                int is_file_result = is_file(full_path);
                if (is_file_result == -1) {
                        free(full_path);
                        continue;
                }

                entry.is_file = is_file_result;
                entry.foreground_color = entry.is_file ? (SDL_Color){0, 0, 0, 255} : (SDL_Color){0, 0, 255, 255};
                entry.background_color = SDL_Color{255, 255, 255, 255};
                entry.name = strdup(dir->d_name);
                if (!entry.name) {
                        fprintf(stderr, "Memory allocation failed for entry name\n");
                        free(full_path);
                        continue;
                }

                struct file_list *file_list = app->file_list;
                ADD_DA(file_list->file_entry, file_list->count, file_list->capacity, struct file_entry, entry);

                free(full_path);
        }

        closedir(d);
}

static void add_file(char *path) {
        FILE *new_file = fopen(path, "w");
        if (new_file == NULL) {
                perror("Error creating file");
        }
        fclose(new_file);
}

static void add_directory(char *path) {
        if (mkdir(path, 0700) == -1) {
                perror("Error creating file directory");
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

int handle_delete(SDL_Renderer *renderer, TTF_Font *font, struct app *app) {
        char *file_name = app->file_list->file_entry[app->cursor->line + app->cursor->scroll].name;
        size_t new_path_len = strlen(app->path) + strlen(file_name) + 2; 
        char *new_path = (char *)malloc(new_path_len);
        snprintf(new_path, new_path_len, "%s/%s", app->path, file_name);

        char delete_str[] = "DELETE";
        size_t file_name_confirmation_len = strlen(file_name) + strlen(delete_str) + 2;
        char *file_name_confirmation = (char *)malloc(file_name_confirmation_len);
        snprintf(file_name_confirmation, file_name_confirmation_len, "%s %s ?", delete_str, file_name);

        if (!user_confirmation(renderer, font, app, file_name_confirmation)) {
                return 0;
        }

        if (is_file(new_path)) {
                delete_file(new_path);
        } else {
                delete_directory(new_path);
        }

        return 1;
}

static int rename_directory(char *previous_path, char *new_path) {
        printf("%s - %s\n", previous_path, new_path);
        if (rename(previous_path, new_path) != 0) {
                fprintf(stderr, "couldnt rename\n");
                return 0;
        }

        return 1;
}

int handle_rename(SDL_Renderer *renderer, TTF_Font *font, struct app *app) {
        char message[] = "RENAME";
        char *file_name = app->file_list->file_entry[app->cursor->line].name;
        size_t message_len = strlen(message) + strlen(file_name) + 6;
        char *full_message = (char *)malloc(message_len);
        snprintf(full_message, message_len, "%s %s -> ", message, file_name);

        if (!get_user_input(renderer, font, app, full_message)) {
                return 0;
        }

        size_t previous_path_len = strlen(app->path) + strlen(file_name) + 2; 
        size_t new_path_len = strlen(app->path) + strlen(app->input->text) + 2; 

        char *previous_path = (char *)malloc(previous_path_len); 
        char *new_path = (char *)malloc (new_path_len); 

        snprintf(previous_path, previous_path_len, "%s/%s", app->path, file_name);
        snprintf(new_path, new_path_len, "%s/%s", app->path, app->input->text);

        if (!rename_directory(previous_path, new_path)) {
                return 0;
        }

        return 1;
}

int handle_add(SDL_Renderer *renderer, TTF_Font *font, struct app *app) {
        char buffer[] = "ADD ";
        size_t message_len = strlen(buffer) + 1;
        char *message = (char *)malloc(message_len);
        strcpy(message, buffer);

        if (!get_user_input(renderer, font, app, message)) {
                return 0;
        }

        printf("%s %s\n", app->path, app->input->text);
        /* size_t full_path_len_yes = strlen(app->path) + strlen(app->input->text) + 2; */
        /* char *full_path = (char *)malloc(1); */
        /* snprintf(full_path, full_path_len, "%s%s", app->path, app->input->text); */
        /* printf("%s\n", full_path); */

        /* add_file(full_path); */
        /* if (is_file(app->path)) { */
        /* } else { */
        /*         add_directory(app->path); */
        /* } */

        return 1;
}
