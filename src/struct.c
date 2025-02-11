#include "struct.h"
#include "struct.h"

void free_app(struct app *app) {
        for (int i = 0; i < app->file_list->count; i++) {
                free(app->file_list->file_entry[i].name);
        }
        free(app->file_list->file_entry);
        free(app->file_list);

        if (app->log) {
                free(app->log->title);
                free(app->log->message);
                free(app->log);
        }

        free(app->cursor);
        free(app->input);
        free(app->path);

        free(app);
}

void free_app_content(struct app *app) {
        for (int i = 0; i < app->file_list->count; i++) {
                free(app->file_list->file_entry[i].name);
        }

        app->file_list->count = 0;
        app->file_list->capacity = 0;

        app->cursor->line = 0;
        app->cursor->scroll = 0;
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
                free(app);
                return NULL;
        }

        app->file_list = (struct file_list *)malloc(sizeof(struct file_list));
        if (!app->file_list) {
                fprintf(stderr, "Memory allocation failed for file_list\n");
                free(app->path);
                free(app);
                return NULL;
        }
        app->file_list->file_entry = NULL;
        app->file_list->capacity = 0;
        app->file_list->count = 0;

        app->cursor = (struct cursor *)malloc(sizeof(struct cursor));
        if (!app->cursor) {
                fprintf(stderr, "Memory allocation failed for file_list\n");
                free(app->path);
                free(app->cursor);
                free(app);
                return NULL;
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
                free(app->cursor);
                free(app->file_list);
                free(app->path);
                free(app);
                return NULL;
        }
        app->log->title = NULL;
        app->log->message = NULL;

        app->input = (struct user_input *)malloc(sizeof(struct user_input));
        if (!app->input) {
                fprintf(stderr, "Memory allocation failed for app->log\n");
                free(app->log);
                free(app->cursor);
                free(app->file_list);
                free(app->path);
                free(app);
                return NULL;
        }

        return app;
}
