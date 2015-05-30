#ifndef GRAPH_H
#define GRAPH_H

typedef void (*iter_fn)(void *);

#define graph_at(g, i) (i < g->num ? g->verts[i] : 0)
#define graph_print_matrix(gp, m) graph_fprint_matrix(gp, m, stdout)

type_data_s(Edge,
    struct graph_s *g;
    Type *t;
    size_t o, i;
    type_struct(Edge) *next;
    void *w;
);

struct graph_s {
    Edge_t **edges;
    void *verts;
    const Type *vert_type, *edge_type;
    size_t len;
};

struct graph_s *graph_new(void *data, size_t size, size_t len,
    const Type *, const Type *);

void edge_print_i(void *v);

void graph_free(struct graph_s *g);
bool graph_connect(struct graph_s *g, size_t i, size_t d, void *w);
bool graph_has_edge(struct graph_s *g, size_t o, size_t i);
void graph_for_each_edge(struct graph_s *, iter_fn, bool);
void graph_random_int_edges(struct graph_s *g, size_t max);
void graph_print(struct graph_s *g);
void graph_to_matrix(struct graph_s *g, Edge_t **m);

#endif
