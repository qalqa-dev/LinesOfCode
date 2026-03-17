#include "loc_counter.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "config.h"

static int looks_binary_sample(const unsigned char *buf, size_t len) {
    size_t non_text = 0;
    for (size_t i = 0; i < len; ++i) {
        unsigned char c = buf[i];
        if (c == '\n' || c == '\r' || c == '\t' || (c >= 32 && c < 127)) {
            continue;
        }
        non_text++;
        if (non_text > 8) {
            return 1;
        }
    }
    return 0;
}

int is_text_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "Failed to open file '%s': %s\n", path, strerror(errno));
        return 0;
    }

    unsigned char buf[BINARY_SNIFF_BYTES];
    size_t n = fread(buf, 1, sizeof(buf), f);
    fclose(f);

    if (n == 0) {
        return 1;
    }

    return !looks_binary_sample(buf, n);
}

static int is_line_comment_or_empty(const char *line) {
    const char *p = line;
    while (*p && isspace((unsigned char)*p)) {
        ++p;
    }

    if (*p == '\0') {
        return 1;
    }

    if (p[0] == '/' && p[1] == '/') {
        return 1;
    }

    if (p[0] == '/' && p[1] == '*') {
        const char *end = strstr(p + 2, "*/");
        if (end && *(end + 2) == '\0') {
            return 1;
        }
    }

    return 0;
}

long count_loc_in_file(const char *path) {
    if (!is_text_file(path)) {
        return 0;
    }

    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "Failed to open file '%s' for reading: %s\n", path, strerror(errno));
        return 0;
    }

    char buf[4096];
    long count = 0;

    while (fgets(buf, sizeof(buf), f)) {
        if (!is_line_comment_or_empty(buf)) {
            count++;
        }
    }

    if (ferror(f)) {
        fprintf(stderr, "Error reading file '%s'\n", path);
    }

    fclose(f);
    return count;
}

