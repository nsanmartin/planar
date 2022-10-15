#ifndef _H_TSU_NODES_H_
#define _H_TSU_NODES_H_

#include "TsuPoint.h"

typedef struct {
    int sz;
    int capacity;
    Point* ps;
} TsuNodes;

int nodes_push_back(TsuNodes* ns, Point p);
TsuNodes* newNodes();

#endif

