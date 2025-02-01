#ifndef READ_H
#define READ_H

#include "struct.h"

void read_file(struct terminal *term, struct cursor *cursor);
int is_file(char *path);
void read_file_content(struct terminal *term);
void read_directory_content(struct terminal *term);
void free_content(struct terminal *term, struct cursor *cursor);

#endif
