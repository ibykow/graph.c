#ifndef LIST_H_
#define LIST_H_

#include "type.h"

#define list_init(np, d, n, l) *(np) = (struct list_s) \
                        { .data = d, .num = n, .len = l }

struct list_s {
    size_t num, len;
    void **data;
};

struct list_s *list_new(size_t);
void list_free(struct list_s *);
void list_for_each(struct list_s *, iter_fn);

#endif
