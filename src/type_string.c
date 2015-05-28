#include "common.h"

static void *type_new_fn(String)(const void *raw)
{
    if(!raw)
        return 0;

    String_t *s = (String_t *) malloc(String->size);

    if(!s)
        return 0;

    s->len = strlen((char *) raw);
    if(!s->len) {
        free(s);
        return 0;
    }

    s->buf = (char *) malloc(sizeof(char) * (s->len + 1));
    if(!s->buf) {
        free(s);
        return 0;
    }

    snprintf(s->buf, s->len + 1, "%s", (char *) raw);

    return s;
}

static void type_free_fn(String)(void *data)
{
    if(!data)
        return;

    String_t *s = (String_t *) data;
    free(s->buf);
    free(s);
}

static int type_diff_fn(String)(const void *a, const void *b)
{
    if(a == b)
        return 0;

    if(!a || !b)
        return -1;

    String_t *s = (String_t *) a;
    String_t *s1 = (String_t *) b;

    if(s->len != s1->len)
        return (int) s->len - (int) s1->len;

    return strncmp(s->buf, s1->buf, s->len);
}

static void type_to_str_fn(String)(const void *data, char *dest, size_t len)
{
    if(!data || !dest || !len)
        return;

    String_t *s = (String_t *) data;
    snprintf(dest, len, "%s", s->buf);
}

type_define(String);
