#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#include "type.h"
#include "type_prim.h"
#include "type_string.h"
#include "random.h"
#include "graph.h"

#define failloc(type, var) type *var = (type *) \
    malloc(sizeof(type)); if(!var) return 0

#endif
