#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fs_walk.h"
#include "loc_counter.h"

struct loc_accumulator {
    long total;
};

static void count_file_callback(const char *path, void *user_data) {
    struct loc_accumulator *acc = (struct loc_accumulator *)user_data;
    long file_loc = count_loc_in_file(path);
    if (file_loc > 0) {
        acc->total += file_loc;
    }
}

static void print_usage(const char *prog) {
    fprintf(stderr, "Usage: %s <project-path>\n", prog);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        print_usage(argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        print_usage(argv[0]);
        return 0;
    }

    const char *root = argv[1];

    struct loc_accumulator acc;
    acc.total = 0;

    if (fs_walk_directory(root, count_file_callback, &acc) != 0) {
        fprintf(stderr, "Failed to walk directory '%s': %s\\n", root, strerror(errno));
        return 1;
    }

    printf("Your project has %ld lines of code\n", acc.total);
    return 0;
}

