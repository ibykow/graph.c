#if 0
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

graph_print(g);

#endif
