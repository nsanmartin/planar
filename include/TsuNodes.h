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

int nodes_push_back(TsuNodes* ns, Point p);
TsuNodes* newNodes();
void freeNodes(TsuNodes* ns);

bool node_contains_point(Point* n, int sz, int x, int y);
int nodes_find_touched(const TsuNodes* ns, int x, int y, TsuNode** res);
#endif

