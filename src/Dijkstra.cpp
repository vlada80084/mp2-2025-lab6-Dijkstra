#include "Dijkstra.h"
#include <iostream>
#include <stdexcept>

Array<int> Answer::getPath(int v) const {
    //идем от v к start по массиву from, потом переворачиваем
    Array<int> result;
    if (v < 0 || v >= dist.size()) { return result; }
    if (dist[v] >= INF) { return result; }//не дойти

    //собираем путь задом наперед
    Array<int> back;
    int cur = v;
    while (cur != -1) {
        back.add(cur);
        if (cur == start) { break; }
        cur = from[cur];
    }
    //переворачиваем
    for (int i = back.size() - 1; i >= 0; i--) {
        result.add(back[i]);
    }
    return result;
}

void Answer::printPath(int v) const {
    if (v < 0 || v >= dist.size()) {
        std::cout << "bad point\n";
        return;
    }
    if (dist[v] >= INF) {
        std::cout << "no path from " << start << " to " << v << "\n";
        return;
    }
    Array<int> path = getPath(v);
    std::cout << start << " -> " << v << " | length " << dist[v] << " | ";
    for (int i = 0; i < path.size(); i++) {
        if (i > 0) { std::cout << " "; }
        std::cout << path[i];
    }
    std::cout << "\n";
}

Answer dijkstra3(const Graph& g, int start) {
    int n = g.getN();
    if (start < 0 || start >= n) {
        throw std::invalid_argument("bad start point");
    }

    Answer ans;
    ans.start = start;
    ans.dist.resize(n);
    ans.from.resize(n);
    for (int i = 0; i < n; i++) {//в начале расстояния "бесконечны"
        ans.dist[i] = INF;
        ans.from[i] = -1;
    }
    ans.dist[start] = 0;

    Heap3 pq;
    pq.add(Item(start, 0));

    while (!pq.isEmpty()) {
        Item cur = pq.takeMin();
        int v = cur.point;
        long long d = cur.dist;

        //если в куче лежит устаревшая запись - пропускаем
        if (d > ans.dist[v]) { continue; }

        //смотрим всех соседей и пробуем улучшить путь
        const Array<Road>& list = g.getRoads(v);
        for (int i = 0; i < list.size(); i++) {
            int to = list[i].to;
            long long newDist = d + list[i].weight;
            if (newDist < ans.dist[to]) {
                ans.dist[to] = newDist;
                ans.from[to] = v;
                pq.add(Item(to, newDist));
            }
        }
    }

    return ans;
}

Answer dijkstraB(const Graph& g, int start) {//то же самое, только pq - BHeap
    int n = g.getN();
    if (start < 0 || start >= n) {
        throw std::invalid_argument("bad start point");
    }

    Answer ans;
    ans.start = start;
    ans.dist.resize(n);
    ans.from.resize(n);
    for (int i = 0; i < n; i++) {
        ans.dist[i] = INF;
        ans.from[i] = -1;
    }
    ans.dist[start] = 0;

    BHeap pq;
    pq.add(Item(start, 0));

    while (!pq.isEmpty()) {
        Item cur = pq.takeMin();
        int v = cur.point;
        long long d = cur.dist;

        if (d > ans.dist[v]) { continue; }

        const Array<Road>& list = g.getRoads(v);
        for (int i = 0; i < list.size(); i++) {
            int to = list[i].to;
            long long newDist = d + list[i].weight;
            if (newDist < ans.dist[to]) {
                ans.dist[to] = newDist;
                ans.from[to] = v;
                pq.add(Item(to, newDist));
            }
        }
    }

    return ans;
}
