#ifndef HELPERS_MEMORY_H
#define HELPERS_MEMORY_H

#include <stdlib.h>

void* checked_malloc(size_t __size);
void* checked_calloc(size_t __nmemb, size_t __size);
void* checked_realloc(void* __ptr, size_t __size);
void checked_free(void* __ptr);

void* objcopy(void* object, size_t size);

#endif //HELPERS_MEMORY_H
