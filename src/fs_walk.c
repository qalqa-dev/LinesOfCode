#include "fs_walk.h"

#define _XOPEN_SOURCE 700

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "config.h"

static int is_ignored_dir(const char *name) {
    for (int i = 0; i < IGNORED_DIRS_COUNT; ++i) {
        if (strcmp(name, IGNORED_DIRS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

static int walk_internal(const char *path, file_callback cb, void *user_data) {
    DIR *dir = opendir(path);
    if (!dir) {
        fprintf(stderr, "Failed to open directory '%s': %s\n", path, strerror(errno));
        return -1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        const char *name = entry->d_name;
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
            continue;
        }

        char child_path[4096];
        int n = snprintf(child_path, sizeof(child_path), "%s/%s", path, name);
        if (n < 0 || (size_t)n >= sizeof(child_path)) {
            fprintf(stderr, "Path too long, skipping: '%s/%s'\n", path, name);
            continue;
        }

        struct stat st;
        if (lstat(child_path, &st) != 0) {
            fprintf(stderr, "Failed to stat '%s': %s\n", child_path, strerror(errno));
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            if (is_ignored_dir(name)) {
                continue;
            }
            if (walk_internal(child_path, cb, user_data) != 0) {
                /* keep walking, but propagate error at the end */
            }
        } else if (S_ISREG(st.st_mode)) {
            cb(child_path, user_data);
        }
    }

    closedir(dir);
    return 0;
}

int fs_walk_directory(const char *root_path, file_callback cb, void *user_data) {
    if (!root_path || !cb) {
        return -1;
    }
    return walk_internal(root_path, cb, user_data);
}

