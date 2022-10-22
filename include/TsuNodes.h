#ifndef _H_TSU_NODES_H_
#define _H_TSU_NODES_H_

#include "TsuPoint.h"

typedef struct TsuNode {
    Point p;
    bool touched;
} TsuNode;

typedef struct {
    int sz;
    int capacity;
    TsuNode* ps;
    int node_size;
} TsuNodes;

#endif

