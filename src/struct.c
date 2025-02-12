#include "struct.h"
#include "struct.h"

void free_app(struct app *app) {
        for (int i = 0; i < app->file_list->count; i++) {
                struct file_entry entry = app->file_list->file_entry[i];
                app->file_list->file_entry[i].width = 0;
                free(entry.name);
                free(entry.owner);
                free(entry.group);
        }
        free(app->file_list->file_entry);
        free(app->file_list);

        if (app->log) {
                free(app->log->title);
                free(app->log->message);
                free(app->log);
        }

        if (app->input) {
                free(app->input->text);
                free(app->input);
        }

        free(app->cursor);
        free(app->path);

        free(app);
}

void free_app_content(struct app *app) {
        for (int i = 0; i < app->file_list->count; i++) {
                struct file_entry entry = app->file_list->file_entry[i];
                free(entry.name);
                free(entry.owner);
                free(entry.group);
        }

        app->cursor->line = 0;
        app->cursor->scroll = 0;

        app->file_list->count = 0;
        app->file_list->capacity = 0;
}

struct app* ini_app() {
        struct app *app = (struct app *)malloc(sizeof(struct app));
        if (!app) {
                fprintf(stderr, "Memory allocation failed for app\n");
                return NULL;
        }

        app->path = strdup("/");
        if (!app->path) {
                fprintf(stderr, "Memory allocation failed for app->path\n");
                goto error_app;
        }

        app->file_list = (struct file_list *)malloc(sizeof(struct file_list));
        if (!app->file_list) {
                fprintf(stderr, "Memory allocation failed for app->file_list\n");
                goto error_path;
        }
        app->file_list->file_entry = NULL;
        app->file_list->capacity = 0;
        app->file_list->count = 0;

        app->cursor = (struct cursor *)malloc(sizeof(struct cursor));
        if (!app->cursor) {
                fprintf(stderr, "Memory allocation failed for app->cursor\n");
                goto error_file_list;
        }
        app->cursor->column = 0;
        app->cursor->line = 1;
        app->cursor->scroll = 0;
        app->cursor->color.a = 100;
        app->cursor->color.r = 255;
        app->cursor->color.g = 0;
        app->cursor->color.b = 0;

        app->log = (struct log *)malloc(sizeof(struct log));
        if (!app->log) {
                fprintf(stderr, "Memory allocation failed for app->log\n");
                goto error_cursor;
        }
        app->log->title = NULL;
        app->log->message = NULL;

        app->input = (struct user_input *)malloc(sizeof(struct user_input));
        if (!app->input) {
                fprintf(stderr, "Memory allocation failed for app->input\n");
                goto error_log;
        }

        app->input->text = strdup("");
        if (!app->input->text) {
                fprintf(stderr, "Memory allocation failed for app->input->text\n");
                goto error_input;
        }

        return app;

error_input:
        free(app->input);
error_log:
        free(app->log);
error_cursor:
        free(app->cursor);
error_file_list:
        free(app->file_list);
error_path:
        free(app->path);
error_app:
        free(app);
        return NULL;
}
