#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Graph.h"
#include "Heap3.h"
#include "BHeap.h"
#include "Array.h"

const long long INF = (long long)1e18;//"бесконечность" - очень большое число

struct Answer {
    int start;//начало
    Array<long long> dist;//наименьшая дистанция до v
    Array<int> from;//откуда пришли в v

    Array<int> getPath(int v) const;//путь от start к v
    void printPath(int v) const;
};

Answer dijkstra3(const Graph& g, int start);
Answer dijkstraB(const Graph& g, int start);

#endif
