#include "display.h"
#include "stdio.h"
#include <dirent.h>

void display_cursor(SDL_Renderer *renderer, struct cursor *cursor) {
    int alpha = (int)(cursor->opacity * 255);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, cursor->color.red, cursor->color.green, cursor->color.blue, alpha);

    SDL_Rect rect = {cursor->x, cursor->y - cursor->padding / 2, WINDOW_WIDTH, FONT_SIZE + cursor->padding }; 
    SDL_RenderFillRect(renderer, &rect);
}

void display_path(struct terminal *term, TTF_Font *font, SDL_Renderer *renderer) {
    SDL_Color textColor = { 0, 0, 0 }; 
    SDL_Surface *textSurface = TTF_RenderText_Shaded(font, term->path, textColor, (SDL_Color){ 255, 255, 255 }); 
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    SDL_Rect textLocation = {0, WINDOW_HEIGHT - FONT_SIZE, 0, 0};
    SDL_QueryTexture(textTexture, NULL, NULL, &textLocation.w, &textLocation.h);
    SDL_RenderCopy(renderer, textTexture, NULL, &textLocation);
    SDL_DestroyTexture(textTexture);
}

void display_files(SDL_Renderer *renderer, TTF_Font *font, struct terminal *term, struct cursor *cursor) {
    struct dirent *entry;
    DIR *dp = opendir(term->path);

    if (dp == NULL) {
        perror("opendir");
        return;
    }

    int position_y = 0;
    int index_y = 1;

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue;  
        }

        char text[512];
        strcpy(text, entry->d_name);

        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", term->path, entry->d_name);  

        struct stat fileStat;
        if (stat(fullpath, &fileStat) == -1) {
            perror("stat");
            continue;  
        }

        const char *type = S_ISDIR(fileStat.st_mode) ? "DIR" : "FILE";

        strcpy(term->files[index_y], entry->d_name);

        SDL_Surface *textSurface;
        SDL_Color textColor;

        if (strcmp(type, "DIR") == 0) {
            textColor = { 0, 0, 255 };
        } else {
            textColor = { 0, 0, 0 };
        }

        textSurface = TTF_RenderText_Shaded(font, text, textColor, (SDL_Color){ 255, 255, 255 }); 

        if (textSurface == NULL) {
            fprintf(stderr, "Failed to create text surface! TTF_Error: %s\n", TTF_GetError());
            closedir(dp);
            return;
        }

        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);

        if (textTexture == NULL) {
            fprintf(stderr, "Failed to create text texture! SDL_Error: %s\n", SDL_GetError());
            closedir(dp);
            return;
        }

        SDL_Rect textLocation = {LINE_WIDTH, position_y, 0, 0};
        SDL_QueryTexture(textTexture, NULL, NULL, &textLocation.w, &textLocation.h);
        SDL_RenderCopy(renderer, textTexture, NULL, &textLocation);
        SDL_DestroyTexture(textTexture);

        position_y += FONT_SIZE + FONT_SPACING_Y;
        index_y += 1;
    }

    term->total_line = index_y;

    closedir(dp);
}

void display_lines(SDL_Renderer *renderer, TTF_Font *font, struct terminal *term) {
    SDL_Color textColor = { 0, 0, 0 };

    for (int i = 0; i < sizeof(term->files) / sizeof(term->files[0]); i++) {
        char line_number[5];
        sprintf(line_number, "%d", i);
        SDL_Surface *textSurface = TTF_RenderText_Shaded(font, line_number, textColor, (SDL_Color){ 255, 255, 255 }); 
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textLocation = { 0, FONT_SIZE * i, 0, 0 };
        SDL_QueryTexture(textTexture, NULL, NULL, &textLocation.w, &textLocation.h);
        SDL_RenderCopy(renderer, textTexture, NULL, &textLocation);
        SDL_DestroyTexture(textTexture);
    }
}

void display_file_content(SDL_Renderer *renderer, TTF_Font *font, struct terminal *term) {
    FILE *file = fopen(term->path, "r");

    if (file == NULL) {
        printf("Could not open directory %s\n", term->path);
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *file_content = (char *)malloc(file_size + 1);
    if (file_content == NULL) {
        printf("Memory allocation failed\n");
        fclose(file);
        return;
    }

    printf("%d", *file_content);
}
