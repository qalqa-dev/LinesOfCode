#ifndef FS_WALK_H
#define FS_WALK_H

#include <stddef.h>

typedef void (*file_callback)(const char *path, void *user_data);

int fs_walk_directory(const char *root_path, file_callback cb, void *user_data);

#endif /* FS_WALK_H */

