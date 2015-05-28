#include "common.h"

static void edge_print_i(void *v)
{
    printf("->%lu", ((struct edge_s *) v)->i);
}

static struct edge_s *edge_new(size_t i, void *w)
{
    failloc(struct edge_s, e);

    e->i = i;
    e->w = w;
    e->next = 0;

    return e;
}

static void edge_for_each(struct edge_s *e, iter_fn iter, bool value)
{
    while(e) {
        struct edge_s *next = e->next;
        iter(value ? e->w : e);
        e = next;
    }
}

static bool graph_resize(struct graph_s *g, size_t len)
{
    struct edge_s **edges = (struct edge_s **)
        realloc(g->edges, sizeof(struct edge_s *) * len);

    if(len && !edges)
        return false;

    void **verts = (void **) malloc(sizeof(void *) * len);

    if(len && !verts) {
        free(edges);
        return false;
    }

    g->edges = edges;
    g->verts = verts;

    if(len > g->len) {
        memset(g->edges + g->len, 0, len * sizeof(struct edge_s *));
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

    unsigned i, max = len * size;
    for(i = 0; i < max; i += size)
        g->verts[i] = (char *) data + i;

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

    struct edge_s *new = edge_new(d, w);
    if(!g->edges[i]) {
        g->edges[i] = new;
        return true;
    }

    if(g->edges[i]->i > d) {
        new->next = g->edges[i];
        g->edges[i] = new;
        return true;
    }

    struct edge_s *e = g->edges[i];
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

void graph_for_each_edge(struct graph_s *g, iter_fn iter, bool value)
{
    if(!g || !iter)
        return;

    unsigned i;
    for(i = 0; i < g->len; i++)
        edge_for_each(g->edges[i], iter, value);
}

void graph_random_edges(struct graph_s *g, size_t max)
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
        g->vert_type->to_str(g->verts[i], vert_str, 101);

        printf("[%lu]\t%s ", i, vert_str);
        edge_for_each(g->edges[i], edge_print_i, false);
        printf("\n");
    }

    printf("\n");
}
