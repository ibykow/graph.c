#include "common.h"

struct list_s *list_new(size_t len)
{
    failloc(struct list_s, n);

    list_init(n, 0, 0, len);

    if(len && !(n->data = calloc(len, sizeof(void *)))) {
        free(n);
        return 0;
    }

    return n;
}

void list_free(struct list_s *n)
{
    if(!n)
        return;

    free(n->data);
    free(n);
}

void list_for_each(struct list_s *n, iter_fn it)
{
    if(!n)
        return;

    size_t i;
    for(i = 0; i < n->num; i++)
        it(n->data[i]);
}

bool list_resize(struct list_s *n, size_t len)
{
    if(!n)
        return false;

    void **data = realloc(n->data, len * sizeof(void *));
    if(len && !data)
        return false;

    n->data = data;
    n->len = len;
    if(len < n->num)
        n->num = len;

    return true;
}
