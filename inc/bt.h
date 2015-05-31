#ifndef BT_H_
#define BT_H_

#include "list.h"

enum bt_e {
    BT_VALUE,
    BT_COUNT = 4,
    BT_NUM_CHILDREN
};

enum bt_dir_e {
    BT_LEFT = 1,
    BT_RIGHT,
    BT_PARENT,
};

enum bt_order_e {
    BT_PRE_ORDER,
    BT_IN_ORDER,
    BT_POST_ORDER
};

extern const char *bt_dir_names[BT_PARENT + 1];
extern const char *bt_order_names[BT_POST_ORDER + 1];

#define bt_count(n) bt_traverse(n, BT_PRE_ORDER, 0)

struct list_s *bt_new();
void bt_free(struct list_s *, free_fn);
bool bt_sinsrt(struct list_s *, struct list_s *, const Type *, const void *);
struct list_s *bt_remove(struct list_s *);
struct list_s *bt_extreme(struct list_s *, enum bt_dir_e);
size_t bt_traverse(struct list_s *n, enum bt_order_e o, iter_fn it);

#endif
