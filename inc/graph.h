#ifndef GRAPH_H
#define GRAPH_H

typedef void (*iter_fn)(void *);

#define graph_at(g, i) (i < g->num ? g->verts[i] : 0)
#define graph_print_matrix(gp, m) graph_fprint_matrix(gp, m, stdout)

struct graph_s {
    struct edge_s {
        size_t i;
        void *w;
        struct edge_s *next;
    } **edges;
    void **verts;
    const Type *vert_type, *edge_type;
    size_t len;
};

struct graph_s *graph_new(void *data, size_t size, size_t len,
    const Type *, const Type *);

void graph_free(struct graph_s *g);
bool graph_connect(struct graph_s *g, size_t i, size_t d, void *w);
void graph_random_edges(struct graph_s *g, size_t max);
void graph_for_each_edge(struct graph_s *, iter_fn, bool);
void graph_print(struct graph_s *g);

#endif
