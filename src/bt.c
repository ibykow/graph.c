#include "common.h"

const char *bt_dir_names[BT_PARENT + 1] = { "", "left", "right", "parent" };
const char *bt_order_names[BT_POST_ORDER + 1] = {
    "pre-order", "in-order", "post-order"
};

struct list_s *bt_new()
{
    struct list_s *n = list_new(BT_NUM_CHILDREN);
    if(!n)
        return 0;

    if(!(n->data[BT_COUNT] = type_new(UnsignedLong, 1))) {
        list_free(n);
        return 0;
    }

    n->num = BT_NUM_CHILDREN;
    return n;
}

void bt_free(struct list_s *n, free_fn freev)
{
    if(!n)
        return;

    freev(n->data[BT_VALUE]);
    type_free(UnsignedLong, n->data[BT_COUNT]);

    bt_free(n->data[BT_LEFT], freev);
    bt_free(n->data[BT_RIGHT], freev);

    list_free(n);
}

bool bt_sinsrt(struct list_s *n, struct list_s *p, const Type *t, const void *v)
{
    if(!n || !v || !t || (n->num < BT_NUM_CHILDREN))
        return false;

    if(!n->data[BT_VALUE]) {
        n->data[BT_VALUE] = t->new(v);
        n->data[BT_PARENT] = p;
        return true;
    }

    int delta = t->diff(n->data[BT_VALUE], v);

    if(!delta) {
        (*(unsigned long *) n->data[BT_COUNT])++;
        return true;
    }

    if(delta > 0) {
        if(!n->data[BT_LEFT] && !(n->data[BT_LEFT] = bt_new()))
            return false;

        return bt_sinsrt(n->data[BT_LEFT], n, t, v);
    }

    if(!n->data[BT_RIGHT] && !(n->data[BT_RIGHT] = bt_new()))
        return false;

    return bt_sinsrt(n->data[BT_RIGHT], n, t, v);
}

/*
 * struct list_s *bt_remove(struct list_s *n)
 * Remove a node from a binary tree.
 * <-
 *  . n, the node to remove
 * -> the node which replaces n in the tree
 * se:
 *  . the existing tree structure has n removed from it
 *  . if n has both a left and a right child, the right child is
 *      attached to a free slot in the left subtree
 */
struct list_s *bt_remove(struct list_s *n)
{
    /* sanitize the node */
    if(!n || (n->len < 5))
        return 0;

    /* next, keeps track of the replacement node */
    struct list_s *next;

    if(n->data[BT_LEFT]) {

        /*
         * if a right subtree exists, attach it to an available slot
         * on the left subtree
         */
        if(n->data[BT_RIGHT]) {
            /* find the right-most node in the left subtree */
            next = bt_extreme(n->data[BT_LEFT], BT_RIGHT);

            /* attach the right subtree to the available slot */
            next->data[BT_RIGHT] = n->data[BT_RIGHT];

            /* update the right subtree's parent */
            ((struct list_s *) n->data[BT_RIGHT])->data[BT_PARENT] = next->data[BT_RIGHT];
        }

        /* the root of the left subtree becomes the replacement node */
        next = n->data[BT_LEFT];

        /* forget about the left subtree */
        n->data[BT_LEFT] = 0;

    } else {
        /*
         * no left subtree, means that the root of the right subtree
         * becomes the replacement node
         */
        next = n->data[BT_RIGHT];
    }

    /* forget about the right subtree */
    n->data[BT_RIGHT] = 0;

    /* update the parent node if one exists */
    if(n->data[BT_PARENT]) {

        /* determine if n is the left or right child */
        enum bt_dir_e dir =
            ((struct list_s *) n->data[BT_PARENT])->data[BT_LEFT] == n
                ? BT_LEFT : BT_RIGHT;

        /* attach the replacement node to the parent */
        ((struct list_s *) n->data[BT_PARENT])->data[dir] = next;

        if(next)
            /* update the replacement node's parent */
            next->data[BT_PARENT] = n->data[BT_PARENT];

        /* forget about the parent */
        n->data[BT_PARENT] = 0;
    }

    return next;
}

struct list_s *bt_extreme(struct list_s *n, enum bt_dir_e dir)
{
    if(!n)
        return 0;

    while(n->data[dir])
        n = n->data[dir];

    return n;
}

/*
 * size_t bt_traverse(struct list_s *n, enum bt_order_e o, iter_fn it);
 * Traverses the binary tree rooted at n, calls 'it' in the order specified,
 * and returns the count of the number of visited vertices.
 * <-
 *  . n, the root of the binary tree to be traversed
 *  . o, the binary tree order in which to call the iterator function
 *  . it, the iterator function which takes the vertex value as an argument
 * -> count of the number of visited vertices
 * se: none
 */
size_t bt_traverse(struct list_s *n, enum bt_order_e o, iter_fn it)
{
    if(!n || (n->len < 5))
        return 0;

    if(o == BT_PRE_ORDER)
        it ? it(n->data[BT_VALUE]) : 0;

    size_t count = bt_traverse(n->data[BT_LEFT], o, it) +
        *(unsigned long *) n->data[BT_COUNT];

    if(o == BT_IN_ORDER)
        it ? it(n->data[BT_VALUE]) : 0;

    count += bt_traverse(n->data[BT_RIGHT], o, it);

    if(o == BT_POST_ORDER)
        it ? it(n->data[BT_VALUE]) : 0;

    return count;
}
