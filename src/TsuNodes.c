#include <stdio.h>
#include "TsuNodes.h"

#define MAX_NODES 1
static int tsu_nodes_count = 0;
static TsuNodes tsu_nodes = {0};

#define MAX_NODES_POINTS 53

static Point nodes_points[MAX_NODES_POINTS] = {0};

TsuNodes* newNodes(int capacity) {
    if (++tsu_nodes_count > MAX_NODES) {
        fprintf(stderr, "only %d TsuNodes scan be created\n", MAX_NODES);
        return NULL;
    }
    if (capacity > MAX_NODES) {
        return NULL;
    }

    tsu_nodes = (TsuNodes) {
        .sz = 0,
        .ps = nodes_points
    };

    return &tsu_nodes;
}


// int nodes_push_back(TsuNodes* ns, Point p) {
// 
// }
