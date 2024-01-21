#include "include/graphy.hpp"

int main() {
    Graphy::Graph *g = new Graphy::AdjList(9, true);
    g->addEdge("A", "D", 2);
    g->addEdge("B", "D", 3);
    g->addEdge("B", "C", 2);
    g->addEdge("C", "A", 4);
    g->addEdge("D", "E", 5);
    g->addEdge("E", "H", 3);
    g->addEdge("E", "F", 4);
    g->addEdge("F", "G", 3);
    g->addEdge("F", "I", 2);
    g->addEdge("F", "H", 3);
    g->addEdge("G", "D", 6);
    g->addEdge("G", "C", 1);
    g->addEdge("H", "I", 3);
    g->addEdge("I", "F", 1);
    
    g->print();

    Graphy::Graph *a = g->arborescence("B");
    a->print();

    return 0;
}
