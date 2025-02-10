#include "struct.h"
/* #include "display.h" */
/* #include <stdlib.h> */
/* #include <stdio.h> */
/* #include <string.h> */

void free_content(struct app *app) {
        for (int i = 0; i < app->file_list->count; i++) {
                free(app->file_list->file_entry[i].name);
                app->file_list->file_entry[i].name = NULL;
        }

        app->file_list->count = 0;
        app->file_list->capacity = 0;
        app->cursor->scroll = 0;
        app->cursor->line = 0;
        app->cursor->column = 0;
}
