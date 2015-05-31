#include "common.h"
#include "unit.h"

#define TEST_NUM_GRAPH_EDGES ((size_t) 100)

static char gstr[] = "Hello, World!";
static struct graph_s *g = 0;
static struct list_s *bt = 0;

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

unit_test_pre(bt)
{
    bt = bt_new();
}

unit_test(bt_new)
{
    pass(new, bt, "should exist");
}

static unsigned long bt_nodes_count = 0;
unit_test(insert)
{
    int n = 100, i;

    if(bt_sinsrt(bt, 0, Int, &n))
        bt_nodes_count++;

    pass(insert, bt->data[0], "should exist");
    pass(type, !type_diff(Int, bt->data[0], &n), "%d should equal %d",
        *(Int_t *) bt->data[0], n);

    for(i = 0; i < (n << 10); i++)
        if(bt_sinsrt(bt, 0, Int, type_cast(Int, ur(n << 3) - (n << 2))))
            bt_nodes_count++;
}

static int prev_int = 0;
static bool check_ascend;

static void check_ascend_init(int n)
{
    prev_int = n;
    check_ascend = true;
}

static void check_ascend_iter(void *v)
{
    if(*(int *) v < prev_int)
        check_ascend = false;

    prev_int = *(int *) v;
}

unit_test(traverse)
{
    check_ascend_init(*(int *) bt_extreme(bt, BT_LEFT)->data[BT_VALUE]);
    unsigned long count = bt_traverse(bt, BT_IN_ORDER, check_ascend_iter);

    pass(ascend, check_ascend, "bst out of order");
    pass(count, count == bt_nodes_count, "%lu should be %lu (off by %ld)",
        count, bt_nodes_count, (long) count - bt_nodes_count);
}

unit_test(remove)
{
    enum bt_dir_e expect_dir;
    unsigned long n_count, count, new_count;
    struct list_s *n = (struct list_s *) bt->data[BT_LEFT], *rep, *expect;
    n = (struct list_s *) n->data[BT_RIGHT];
    pass(node, n, "should exist (but, statistically possible that it doesn't)");

    if(!n) {
        printf("Please run the tests again.");
        return;
    }

    n_count = *(unsigned long *) n->data[BT_COUNT];

    expect_dir = n->data[BT_LEFT] ? BT_LEFT : BT_RIGHT;

    expect = (struct list_s *) n->data[expect_dir];

    check_ascend_init(*(int *) bt_extreme(bt, BT_LEFT)->data[BT_VALUE]);

    count = bt_traverse(n, BT_IN_ORDER, check_ascend_iter);

    pass(ascend, check_ascend, "bst out of order");

    rep = bt_remove(n);

    pass(replacement, rep == expect, "%p should be %p (%s)",
        rep, expect, bt_dir_names[expect_dir]);

    check_ascend_init(*(int *) bt_extreme(bt, BT_LEFT)->data[BT_VALUE]);
    new_count = bt_traverse(rep, BT_IN_ORDER, check_ascend_iter);
    pass(ascend, check_ascend, "bst out of order");
    pass(count, new_count == (count - n_count), "%lu should be %lu (off by %ld)",
        new_count, count - n_count, (long) new_count - count - n_count);

    new_count = bt_count(n);
    pass(n_count, new_count == n_count, "%lu should be %lu", new_count, n_count);

    bt_free(n, Int->free);
}

unit_test_post(bt)
{
    bt_free(bt, Int->free);
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

    test_pre(bt);
    test(bt_new);
    test(insert);
    test(traverse);
    test(remove);
    test_post(bt);

    return 0;
}
