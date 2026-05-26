#ifndef GRAPH_H
#define GRAPH_H

#include "Array.h"

struct Road {
    int to;//куда
    int weight;//вес

    Road() {//конструктор по умолчанию
        to = -1;
        weight = 0;
    }
    Road(int t, int w) {//по точке и весу
        to = t;
        weight = w;
    }
};

class Graph {
private:
    int n;//кол-во точек
    int m;//кол-во путей
    Array<Array<Road>> roads;

public:
    Graph(int n);//граф без дорог

    int getN() const;//кол-во точек
    int getM() const;//кол-во путей
    int minRoads() const;
    int maxRoads() const;
    void addRoad(int a, int b, int w);//добавить путь а - b, вес - w 
    bool hasRoad(int a, int b) const;//проверка, есть ли путь
    const Array<Road>& getRoads(int v) const;//все дороги из v
    bool isOk() const;//проверка на связность
    void print() const;//вывод списком

    //создает случайный связный граф с n точками, m дорогами, веса от minW до maxW
    static Graph makeGraph(int n, int m, int minW, int maxW);
};

#endif
