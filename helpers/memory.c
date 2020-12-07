#include "memory.h"

#include <stdio.h>
#include <string.h>

void* checked_malloc(size_t __size) {
    void* ret = calloc(1, __size); // this also sets the value to 0

    if (ret == NULL) {
        fprintf(stderr, "Could not malloc %ld bytes", __size);
    }

    return ret;
}

void* checked_calloc(size_t __nmemb, size_t __size) {
    void* ret = calloc(__nmemb, __size);

    if (ret == NULL) {
        fprintf(stderr, "Could not calloc %ld times %ld bytes", __nmemb, __size);
    }

    return ret;
}

void checked_free(void* __ptr) {
    if (__ptr == NULL) {
        fprintf(stderr, "Freeing NULL");
    }
    free(__ptr);
}

void* checked_realloc(void* __ptr, size_t __size) {
    void* ret = realloc(__ptr, __size);

    if (ret == NULL) {
        fprintf(stderr, "Could not realloc size %ld", __size);
    }

    return ret;
}

void* checked_reallocarray(void* __ptr, size_t __nmemb, size_t __size) {
    void* ret = reallocarray(__ptr, __nmemb, __size);

    if (ret == NULL) {
        fprintf(stderr, "Could not reallocarray %ld members of size %ld", __nmemb, __size);
    }

    return ret;
}

void* objcopy(void* object, size_t size) {
    void* ret = checked_malloc(size);

    memcpy(ret, object, size);

    return ret;
}
