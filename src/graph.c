#include "common.h"

#define edge_new(gr, ty, or, in, we) \
    type_new(Edge, .g = gr, .t = ty, .o = or, .i = in, 0, .w = we)

void edge_print_i(void *v)
{
    Edge_t *e = (Edge_t *) v;

    char s[101];
    e->t->to_str(e->w, s, 101);

    printf("->%lu(%s) ", e->i, s);
}

static void *type_new_fn(Edge)(const void *raw)
{
    if(!raw)
        return 0;

    failloc(Edge_t, e);

    *e = *(Edge_t *) raw;

    return (void *) e;
}

static void type_free_fn(Edge)(void *d)
{
    if(!d)
        return;

    Edge_t *e = (Edge_t *) d, *next;
    while(e) {
        next = e->next;
        free(e);
        e = next;
    }
}

static int type_diff_fn(Edge)(const void *a, const void *b)
{
    if(a == b)
        return 0;

    if(!a || !b)
        return -1;

    Edge_t *ea = (Edge_t *) a, *eb = (Edge_t *) b;

    if(ea->t != eb->t)
        return -1;

    return ea->t->diff(ea->w, eb->w);
}

static void type_to_str_fn(Edge)(const void *d, char *s, size_t len)
{
    if(!d)
        return;

    Edge_t *e = (Edge_t *) d;
    e->t->to_str(e->w, s, len);
}

static void edge_for_each(Edge_t *e, iter_fn iter, bool value)
{
    while(e) {
        Edge_t *next = e->next;
        iter(value ? e->w : e);
        e = next;
    }
}

static bool graph_resize(struct graph_s *g, size_t len)
{
    Edge_t **edges = (Edge_t **)
        realloc(g->edges, sizeof(Edge_t *) * len);

    if(len && !edges)
        return false;

    void *verts = (void *) malloc(sizeof(void *) * len);

    if(len && !verts) {
        free(edges);
        return false;
    }

    g->edges = edges;
    g->verts = verts;

    if(len > g->len) {
        memset(g->edges + g->len, 0, len * sizeof(Edge_t *));
        memset(g->verts + g->len, 0, len * sizeof(void *));
    }

    g->len = len;

    return true;
}

struct graph_s *graph_new(void *data, size_t size, size_t len,
    const Type *vert_type, const Type *edge_type)
{
    failloc(struct graph_s, g);

    g->edges = 0;
    g->verts = 0;
    g->len = 0;
    g->vert_type = vert_type;
    g->edge_type = edge_type;

    if(!graph_resize(g, len)) {
        free(g);
        return 0;
    }

    memcpy(g->verts, data, size * len);

    return g;
}

void graph_free(struct graph_s *g)
{
    if(!g)
        return;

    graph_for_each_edge(g, free, false);

    free(g->edges);
    free(g->verts);

    free(g);
}

bool graph_connect(struct graph_s *g, size_t i, size_t d, void *w)
{
    if(!g || (i >= g->len))
        return false;

    Edge_t *new = edge_new(g, g->edge_type, i, d, w);

    if(!g->edges[i]) {
        g->edges[i] = new;
        return true;
    }

    if(g->edges[i]->i > d) {
        new->next = g->edges[i];
        g->edges[i] = new;
        return true;
    }

    Edge_t *e = g->edges[i];
    while(e) {
        if(e->i == d) {
            free(new);
            return false;
        }

        if(!e->next || (e->next->i > d))
            break;

        e = e->next;
    }

    if(e->next)
        new->next = e->next;

    e->next = new;

    return true;
}

bool graph_has_edge(struct graph_s *g, size_t o, size_t i)
{
    if(!g || (g->len <= o) || !g->edges[o])
        return false;

    Edge_t *e = g->edges[o];
    while(e) {
        if(e->i == i)
            return true;

        e = e->next;
    }

    return false;
}

void graph_for_each_edge(struct graph_s *g, iter_fn iter, bool value)
{
    if(!g || !iter)
        return;

    unsigned i;
    for(i = 0; i < g->len; i++)
        edge_for_each(g->edges[i], iter, value);
}

void graph_random_int_edges(struct graph_s *g, size_t max)
{
    if(!g)
        return;

    while(max--) {
        void *n = type_new(Int, (int) ur(g->len) - 10);

        if(!graph_connect(g, ur(g->len), ur(g->len), n))
            type_free(Int, n);
    }
}

void graph_print(struct graph_s *g)
{
    size_t i;

    for(i = 0; i < g->len; i++) {
        char vert_str[101];
        g->vert_type->to_str(g->verts + i, vert_str, 101);

        printf("[%lu]\t%s ", i, vert_str);
        edge_for_each(g->edges[i], edge_print_i, false);
        printf("\n");
    }

    printf("\n");
}

void graph_to_matrix(struct graph_s *g, Edge_t **m)
{
    if(!g || !g->len || !m)
        return;

    size_t i;
    memset(m, 0, sizeof(*m) * g->len * g->len);

    for(i = 0; i < g->len; i++) {
        Edge_t *e = g->edges[i];
        size_t offs = i * g->len;
        while(e) {
            m[offs + e->i] = e;
            e = e->next;
        }
    }
}

type_define(Edge);
