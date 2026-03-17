#ifndef CONFIG_H
#define CONFIG_H

static const char *const IGNORED_DIRS[] = {
    ".git",
    "node_modules",
    "dist",
    "build",
    "out",
    ".idea",
    ".vscode",
    "target",
    "venv",
    ".venv",
    "__pycache__",
};

static const int IGNORED_DIRS_COUNT = (int)(sizeof(IGNORED_DIRS) / sizeof(IGNORED_DIRS[0]));

/* Number of bytes to inspect for binary detection */
static const int BINARY_SNIFF_BYTES = 4096;

#endif /* CONFIG_H */

