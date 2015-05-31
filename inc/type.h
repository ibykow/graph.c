#ifndef TYPE_H
#define TYPE_H

#define type_sort(tname, arr, len) qsort(arr, len, tname->size, tname->diff)

#define type_cast(tname, ...) &(tname##_t){__VA_ARGS__}

#define type_new(tname, ...) (tname##_t *) \
    tname->new(type_cast(tname, __VA_ARGS__))

#define type_free(tname, v) tname->free(v)
#define type_diff(tname, a, b) tname->diff(a, b)
#define type_print(tname, v) tname->print(v)

#define type_struct(name) struct type_##name##_s

#define type_fn(name, ftype) type_##name##_##ftype

#define type_new_fn(name) type_fn(name, new)
#define type_free_fn(name) type_fn(name, free)
#define type_diff_fn(name) type_fn(name, diff)
#define type_to_str_fn(name) type_fn(name, to_str)
#define type_print_fn(name) type_fn(name, print)

#define type_def(tname, type) const Type type_##tname = {   \
    .size = sizeof(type),                                   \
    .new = type_new_fn(tname),                              \
    .free = type_free_fn(tname),                            \
    .diff = type_diff_fn(tname),                            \
    .to_str = type_to_str_fn(tname),                        \
    .print = type_print_fn(tname),                          \
    .name = #tname                                          \
}, *tname = &type_##tname

#define type_define(name) type_def(name, struct type_##name##_s)

#define type_data(tname, type, ...) typedef type __VA_ARGS__ tname##_t; \
    extern const Type *tname

#define type_data_s(tname, ...) type_data(tname, type_struct(tname), { __VA_ARGS__ })

typedef void *(*new_fn)(const void *);
typedef void (*free_fn)(void *);
typedef int (*diff_fn)(const void *, const void *);
typedef void (*to_str_fn)(const void *, char *, size_t);
typedef void (*iter_fn)(void *);
typedef void (*print_fn)(void *);

typedef const struct type_s {
    size_t size;
    new_fn new;
    free_fn free;
    diff_fn diff;
    to_str_fn to_str;
    print_fn print;
    const char name[];
} Type;

#endif
