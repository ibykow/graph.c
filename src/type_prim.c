#include "common.h"

#define type_prim_def(name, type, fmt)                                      \
static void *type_new_fn(name)(const void *raw)                             \
{                                                                           \
    if(!raw)                                                                \
        return 0;                                                           \
                                                                            \
    type *n = (type *) malloc(sizeof(type));                                \
    if(!n)                                                                  \
        return 0;                                                           \
                                                                            \
    *n = *(type *) raw;                                                     \
                                                                            \
    return n;                                                               \
}                                                                           \
                                                                            \
static void type_free_fn(name)(void *d)                                     \
{                                                                           \
    free(d);                                                                \
}                                                                           \
                                                                            \
static int type_diff_fn(name)(const void *a, const void *b)                 \
{                                                                           \
    if(a == b)                                                              \
        return 0;                                                           \
                                                                            \
    if(!a || !b)                                                            \
        return -100;                                                        \
                                                                            \
    return (int) *(type *) a - *(type *) b;                                 \
}                                                                           \
                                                                            \
static void type_to_str_fn(name)(const void *d, char *dest, size_t len)     \
{                                                                           \
    if(!d)                                                                  \
        return;                                                             \
                                                                            \
    snprintf(dest, len, #fmt, *(type *) d);                                 \
}                                                                           \
                                                                            \
type_def(name, type)

type_prim_def(Char, char, %c);
type_prim_def(Int, int, %d);
type_prim_def(Long, long, %ld);
type_prim_def(LongDouble, long double, %Lg);
type_prim_def(Float, float, %f);
type_prim_def(Double, double, %f);
type_prim_def(UnsignedChar, unsigned char, %u);
type_prim_def(Unsigned, unsigned, %u);
type_prim_def(UnsignedLong, unsigned long, %lu);
