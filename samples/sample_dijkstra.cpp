#include "Graph.h"
#include "Dijkstra.h"
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>

void printMenu() {
    std::cout << "\n1 - show roads\n";
    std::cout << "2 - run Dijkstra\n";
    std::cout << "3 - show path to point\n";
    std::cout << "4 - change start\n";
    std::cout << "0 - exit\n> ";
}

//считаем минимум и максимум дорог для n точек
void calcRoadRange(int n, int& minM, int& maxM) {
    if (n == 1) {
        minM = 0;
        maxM = 0;
    }
    else {
        minM = n - 1;
        maxM = n * (n - 1) / 2;
    }
}

Graph* readByHand() {
    int n;
    std::cout << "points: ";
    std::cin >> n;
    if (n <= 0) {
        std::cout << "bad number\n";
        return nullptr;
    }

    int minM, maxM;
    calcRoadRange(n, minM, maxM);
    std::cout << "roads must be in [" << minM << ", " << maxM << "]\n";

    int m;
    std::cout << "roads: ";
    std::cin >> m;
    if (m < minM || m > maxM) {
        std::cout << "bad number of roads\n";
        return nullptr;
    }

    Graph* g = new Graph(n);
    std::cout << "enter " << m << " roads in format: a b w (a,b from 0 to " << n - 1 << ", w > 0)\n";
    int added = 0;
    while (added < m) {
        int a, b, w;
        std::cout << added + 1 << ": ";
        std::cin >> a >> b >> w;
        try {
            g->addRoad(a, b, w);
            added = added + 1;
        }
        catch (const std::exception& e) {
            std::cout << "error: " << e.what() << "\n";
        }
    }

    if (!g->isOk()) {
        std::cout << "graph is not connected, try again? (y/n): ";
        char c;
        std::cin >> c;
        if (c == 'y' || c == 'Y') {
            delete g;
            return nullptr;
        }
    }
    return g;
}

Graph* readAuto() {
    int n;
    std::cout << "points: ";
    std::cin >> n;
    if (n <= 0) {
        std::cout << "bad number\n";
        return nullptr;
    }

    int minM, maxM;
    calcRoadRange(n, minM, maxM);
    std::cout << "roads must be in [" << minM << ", " << maxM << "]\n";

    int m;
    std::cout << "roads: ";
    std::cin >> m;

    int minW, maxW;
    std::cout << "weight range (min max): ";
    std::cin >> minW >> maxW;

    try {
        Graph g = Graph::makeGraph(n, m, minW, maxW);
        if (!g.isOk()) {
            std::cout << "bug: generated graph not connected\n";
            return nullptr;
        }
        //копируем в кучу для возврата
        Graph* res = new Graph(g.getN());
        for (int v = 0; v < g.getN(); v++) {
            const Array<Road>& list = g.getRoads(v);
            for (int i = 0; i < list.size(); i++) {
                int to = list[i].to;
                if (v < to) {
                    res->addRoad(v, to, list[i].weight);
                }
            }
        }
        return res;
    }
    catch (const std::exception& e) {
        std::cout << "error: " << e.what() << "\n";
        return nullptr;
    }
}

int main() {
    std::srand((unsigned)std::time(nullptr));//запускаем случайность

    std::cout << "Dijkstra: 3-heap vs binomial heap on AVL\n";

    Graph* g = nullptr;
    while (g == nullptr) {
        std::cout << "\n1 - by hand, 2 - auto: ";
        int mode;
        std::cin >> mode;
        if (mode == 1) {
            g = readByHand();
        }
        else if (mode == 2) {
            g = readAuto();
        }
    }

    int start;
    std::cout << "start point [0, " << g->getN() - 1 << "]: ";
    std::cin >> start;
    if (start < 0 || start >= g->getN()) {
        std::cout << "bad start, using 0\n";
        start = 0;
    }

    Answer* lastAns = nullptr;//последний результат Дейкстры

    while (true) {
        printMenu();
        int choice;
        std::cin >> choice;

        if (choice == 0) { break; }
        else if (choice == 1) {
            g->print();
        }
        else if (choice == 2) {
            std::cout << "running from " << start << "...\n";

            //3-куча
            auto t1 = std::chrono::high_resolution_clock::now();
            Answer a1 = dijkstra3(*g, start);
            auto t2 = std::chrono::high_resolution_clock::now();
            auto dur1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

            //биномиальная куча
            auto t3 = std::chrono::high_resolution_clock::now();
            Answer a2 = dijkstraB(*g, start);
            auto t4 = std::chrono::high_resolution_clock::now();
            auto dur2 = std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();

            std::cout << "3-heap:    " << dur1 << " us\n";
            std::cout << "binomial:  " << dur2 << " us\n";

            //проверяем что результаты одинаковые
            bool same = true;
            for (int i = 0; i < a1.dist.size(); i++) {
                if (a1.dist[i] != a2.dist[i]) {
                    same = false;
                    break;
                }
            }
            if (same) { std::cout << "match: yes\n"; }
            else { std::cout << "match: NO\n"; }

            if (lastAns != nullptr) { delete lastAns; }
            lastAns = new Answer(a1);
        }
        else if (choice == 3) {
            if (lastAns == nullptr) {
                std::cout << "first run Dijkstra (option 2)\n";
            }
            else {
                int v;
                std::cout << "to point: ";
                std::cin >> v;
                lastAns->printPath(v);
            }
        }
        else if (choice == 4) {
            int s;
            std::cout << "new start [0, " << g->getN() - 1 << "]: ";
            std::cin >> s;
            if (s >= 0 && s < g->getN()) {
                start = s;
                if (lastAns != nullptr) {
                    delete lastAns;
                    lastAns = nullptr;
                }
                std::cout << "start = " << start << ", run again\n";
            }
            else {
                std::cout << "bad point\n";
            }
        }
    }

    if (lastAns != nullptr) { delete lastAns; }
    delete g;
    return 0;
}
