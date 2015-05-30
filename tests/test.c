#include "common.h"
#include "unit.h"

#define TEST_NUM_GRAPH_EDGES ((size_t) 100)

static char gstr[] = "Hello, World!";
static struct graph_s *g = 0;

unit_test(unit)
{
    pass(test, true, "%s must pass", "this test");
}

unit_test_pre(graph)
{
    g = graph_new(gstr, sizeof(char), strlen(gstr), Char, Int);
    graph_random_int_edges(g, TEST_NUM_GRAPH_EDGES);
}

unit_test_post(graph)
{
    graph_for_each_edge(g, Int->free, true);
    graph_free(g);
}

unit_test(new)
{
    pass(new, g, "couldn't create new graph");
    pass(length, g->len == strlen(gstr),
        "graph should have %lu verts, has %lu",
        strlen(gstr), g->len);
}

unit_test(matrix)
{
    size_t i;
    Edge_t *m[g->len][g->len], *e;

    graph_to_matrix(g, (Edge_t **) m);

    for(i = 0; i < g->len; i++) {
        e = g->edges[i];
        while(e) {
            pass(edge, m[i][e->i], "should exist between %lu and %lu",
                i, e->i);

            pass(connection, (m[i][e->i]->o == i) &&  (m[i][e->i]->i == e->i),
                "between %lu and %lu should be between %lu and %lu",
                m[i][e->i]->o, m[i][e->i]->i, i, e->i);

            pass(type, m[i][e->i]->t == e->t, "%s should be %s",
                m[i][e->i]->t->name, e->t->name);

            pass(w, m[i][e->i]->w == e->w, "%p should be %p",
                m[i][e->i]->t, e->t);

            e = e->next;
        }
    }
}

unit_test(int)
{
    int n = 38;
    void *v = type_new(Int, 38);

    pass(new, v, "couldn't create n");
    pass(value, *(int *) v == n, "%s v should equal %d, is %d",
        Int->name, n, *(int *) v);

    type_free(Int, v);
}

unit_test(char)
{
    char src[] = "ecdfabg", expected[] = "abcdefg";
    type_sort(Char, src, strlen(src));
    pass(sort, !strncmp(src, expected, strlen(src)), "%s should be %s",
        src, expected);
}

unit_test(index)
{
    char arr[0x100];
    int i;
    for(i = 0; i <= 0xFF; i++)
        arr[i] = '1' + (i % 10);

    for(i = -1; i >= -0xFF; i--)
        pass(bounds, arr[(unsigned char) i] == arr[0x100 + i],
            "%d '%c' should be '%c'", i, arr[(unsigned char) i], arr[0x100 + i]);
}

int main(int argc, char const *argv[])
{
    init_random();

    test_init(UNIT_DEFAULT_NUM_ROUNDS);
    test(unit);

    test(index);
    test(char);
    test(int);

    test_pre(graph);
    test(new);
    test(matrix);
    test_post(graph);

    return 0;
}
