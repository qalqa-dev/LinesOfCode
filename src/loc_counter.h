#ifndef LOC_COUNTER_H
#define LOC_COUNTER_H

#include <stddef.h>

int is_text_file(const char *path);
long count_loc_in_file(const char *path);

#endif /* LOC_COUNTER_H */

