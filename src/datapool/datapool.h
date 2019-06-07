#pragma once

#include <stddef.h>
#include <stdbool.h>
struct datapool;

struct datapool *datapool_open(const char *path, size_t size,
    int *fresh, bool prefault);
void datapool_close(struct datapool *pool);

void *datapool_addr(struct datapool *pool);
size_t datapool_size(struct datapool *pool);
