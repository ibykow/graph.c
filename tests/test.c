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
    graph_random_edges(g, TEST_NUM_GRAPH_EDGES);
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

unit_test(new_int)
{
    int n = 38;
    void *v = type_new(Int, 38);

    pass(new, v, "couldn't create n");
    pass(value, *(int *) v == n, "%s v should equal %d, is %d",
        Int->name, n, *(int *) v);

    type_free(Int, v);
}

int main(int argc, char const *argv[])
{
    init_random();

    test_init(UNIT_DEFAULT_NUM_ROUNDS);
    test(unit);

    test_pre(graph);
    test(new);
    // graph_print(g);
    test_post(graph);

    test(new_int);

    return 0;
}
