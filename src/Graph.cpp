#include "Graph.h"
#include "Queue.h"
#include <stdexcept>
#include <iostream>
#include <cstdlib>

Graph::Graph(int n) {//создаем граф с n точками без дорог
    if (n <= 0) {
        throw std::invalid_argument("need positive number of points");
    }
    (*this).n = n;
    (*this).m = 0;
    roads.resize(n);//n пустых списков дорог
}

int Graph::getN() const {
    return n;
}

int Graph::getM() const {
    return m;
}

int Graph::minRoads() const {//минимум дорог для связности = n-1 (дерево)
    return n - 1;
}

int Graph::maxRoads() const {//максимум дорог в простом графе
    return n * (n - 1) / 2;
}

bool Graph::hasRoad(int a, int b) const {//есть ли уже дорога между a и b
    if (a < 0 || a >= n || b < 0 || b >= n) {
        return false;
    }
    const Array<Road>& list = roads[a];
    for (int i = 0; i < list.size(); i++) {
        if (list[i].to == b) {
            return true;
        }
    }
    return false;
}

void Graph::addRoad(int a, int b, int w) {
    if (a < 0 || a >= n || b < 0 || b >= n) {
        throw std::invalid_argument("bad point number");
    }
    if (a == b) {
        throw std::invalid_argument("no loops allowed");
    }
    if (w <= 0) {
        throw std::invalid_argument("weight must be positive");
    }
    if (hasRoad(a, b)) {
        throw std::invalid_argument("road already exists");
    }
    //дорога двусторонняя, поэтому добавляем в оба списка
    roads[a].add(Road(b, w));
    roads[b].add(Road(a, w));
    m = m + 1;
}

const Array<Road>& Graph::getRoads(int v) const {
    if (v < 0 || v >= n) {
        throw std::invalid_argument("bad point number");
    }
    return roads[v];
}

bool Graph::isOk() const {//проверка связности обходом в ширину из точки 0
    if (n <= 1) {
        return true;
    }
    Array<bool> seen(n);//массив "посетили ли точку"
    for (int i = 0; i < n; i++) {
        seen[i] = false;
    }
    Queue<int> q;
    q.add(0);
    seen[0] = true;
    int count = 1;//сколько уже посетили
    while (!q.isEmpty()) {
        int v = q.take();
        const Array<Road>& list = roads[v];
        for (int i = 0; i < list.size(); i++) {
            int next = list[i].to;
            if (!seen[next]) {
                seen[next] = true;
                count = count + 1;
                q.add(next);
            }
        }
    }
    return count == n;//если посетили всех - граф связный
}

void Graph::print() const {//вывод всех дорог
    std::cout << "Graph: " << n << " points, " << m << " roads\n";
    for (int a = 0; a < n; a++) {
        const Array<Road>& list = roads[a];
        for (int i = 0; i < list.size(); i++) {
            if (a < list[i].to) {//печатаем каждую дорогу только один раз
                std::cout << a << " - " << list[i].to << " : " << list[i].weight << "\n";
            }
        }
    }
}

Graph Graph::makeGraph(int n, int m, int minW, int maxW) {
    if (n <= 0) {
        throw std::invalid_argument("n must be positive");
    }
    if (minW <= 0 || maxW < minW) {
        throw std::invalid_argument("bad weight range");
    }
    //считаем минимум и максимум дорог
    int minM, maxM;
    if (n == 1) {
        minM = 0;
        maxM = 0;
    } else {
        minM = n - 1;
        maxM = n * (n - 1) / 2;
    }
    if (m < minM || m > maxM) {
        throw std::invalid_argument("bad number of roads");
    }

    Graph g(n);
    if (n == 1) {
        return g;
    }

    //перемешиваем точки случайно (тасование как карты)
    Array<int> order(n);
    for (int i = 0; i < n; i++) {
        order[i] = i;
    }
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = order[i];
        order[i] = order[j];
        order[j] = tmp;
    }

    //строим дерево из (n-1) дорог:
    //каждую новую точку соединяем со случайной из уже добавленных
    //(так гарантированно получим связный граф)
    int wRange = maxW - minW + 1;
    for (int i = 1; i < n; i++) {
        int parentIdx = rand() % i;
        int a = order[i];
        int b = order[parentIdx];
        int w = minW + rand() % wRange;
        g.addRoad(a, b, w);
    }

    //добавляем остальные дороги случайно
    int extraNeeded = m - (n - 1);
    int extraAdded = 0;
    long long tries = 0;
    long long maxTries = (long long)maxM * 20 + 100;
    while (extraAdded < extraNeeded && tries < maxTries) {
        tries = tries + 1;
        int a = rand() % n;
        int b = rand() % n;
        if (a == b) { continue; }//нельзя петлю
        if (g.hasRoad(a, b)) { continue; }//уже есть
        int w = minW + rand() % wRange;
        g.addRoad(a, b, w);
        extraAdded = extraAdded + 1;
    }

    //если случайно не получилось (граф очень плотный) - перебираем все пары
    if (extraAdded < extraNeeded) {
        for (int a = 0; a < n; a++) {
            if (extraAdded >= extraNeeded) { break; }
            for (int b = a + 1; b < n; b++) {
                if (extraAdded >= extraNeeded) { break; }
                if (!g.hasRoad(a, b)) {
                    int w = minW + rand() % wRange;
                    g.addRoad(a, b, w);
                    extraAdded = extraAdded + 1;
                }
            }
        }
    }

    return g;
}
