#if 0
########### bt.c ###########

*** traverse ***
printf("%d (%lu), ", *(int *) n->data[BT_VALUE], *(unsigned long *) n->data[BT_COUNT]);

########### test.c ###########

*** matrix ***
unit_test(matrix)
{
    Edge_t **m = graph_to_matrix(g);
    size_t i;
    size_t j;

    printf("\t");
    for(j = 0; j < g->len; j++) {
        printf("%lu%s", j, j > 9 ? " " : "  ");
    }
    printf("\n");

    for(i = 0; i < g->len; i++) {
        printf("%lu\t", i);
        size_t offs = i * g->len;
        for(j = 0; j < g->len; j++) {
            if(m[offs + j])
                printf("%lu%s", m[offs + j]->o, m[offs + j]->o > 9 ? " " : "  ");
            else
                printf("   ");
        }

        printf("\n");
    }

    for(i = 0; i < g->len; i++) {
        Edge_t *e = g->edges[i];
        while(e) {
            pass(edge, m[(i * g->len) + e->i] != 0,
                "should exist between %lu and %lu", i, e->i);

            e = e->next;
        }
    }
    free(m);
}

*** graph ***
graph_print(g);

*** bt ***
static int prev_int = 0;
static bool check_ascend;
static void check_ascend_iter(void *v)
{
    if(*(int *) v < prev_int)
        check_ascend = false;

    printf("%d > %d is %c,", *(int *) v, prev_int, check_ascend ? 'T' : 'F');
    prev_int = *(int *) v;
}

unit_test(traverse)
{
    printf("Traverse: ");
    bt_traverse(bt, BT_IN_ORDER, Int->print);
    printf("\n");

    printf("Extereme left: ");
    type_print(Int, bt_extreme(bt, BT_LEFT)->data[BT_VALUE]);
    printf("\n");

    prev_int = *(int *) bt_extreme(bt, BT_LEFT)->data[BT_VALUE];
    check_ascend = true;
    bt_traverse(bt, BT_IN_ORDER, check_ascend_iter);
    printf("\n");

    pass(ascend, check_ascend, "bst out of order");
}

unit_test(remove)
{
    enum bt_dir_e expect_dir;
    unsigned long count, new_count;
    struct list_s *n = (struct list_s *) bt->data[BT_LEFT], *rep, *expect;
    n = (struct list_s *) n->data[BT_RIGHT];

    expect_dir = n->data[BT_LEFT] ? BT_LEFT : BT_RIGHT;
    expect = (struct list_s *) n->data[expect_dir];

    pass(node, n, "should exist (but, statistically possible that it doesn't)");

    check_ascend_init(*(int *) bt_extreme(bt, BT_LEFT)->data[BT_VALUE]);
    count = bt_traverse(bt, BT_IN_ORDER, check_ascend_iter);

    pass(ascend, check_ascend, "bst out of order");

    printf("n: ");
    type_print(Int, n->data[BT_VALUE]);
    printf("\n");

    printf("expected: ");
    type_print(Int, expect->data[BT_VALUE]);
    printf("\n");

    printf("Traverse: ");
    bt_traverse(n, BT_IN_ORDER, Int->print);
    printf("\n");

    rep = bt_remove(n);
    pass(replacement, rep == expect, "%p should be %p (%s)",
        rep, expect, bt_dir_names[expect_dir]);

    printf("Replaced: ");
    type_print(Int, rep->data[BT_VALUE]);
    printf("\nrep == expect: %c (%s)\n", rep == expect ? 'T' : 'F', bt_dir_names[expect_dir]);

    printf("Traverse: ");
    bt_traverse(n, BT_IN_ORDER, Int->print);
    printf("\n");

    printf("Traverse: ");
    bt_traverse(rep, BT_IN_ORDER, Int->print);
    printf("\n");

    check_ascend_init(*(int *) bt_extreme(bt, BT_LEFT)->data[BT_VALUE]);
    new_count = bt_traverse(bt, BT_IN_ORDER, check_ascend_iter);
    pass(ascend, check_ascend, "bst out of order");
    pass(count, new_count == (count - 1), "%lu should equal %lu",
        new_count, count - 1);
}

#endif
