#include "../include/adjList.hpp"
#include <algorithm>
#include <iostream>
#include <set>
#include <stack>

using namespace Graphy;
using namespace std;

/*
 * ===========================================================================
 * =                               CONSTRUCTOR                               =
 * ===========================================================================
 */
AdjList::AdjList() : Graph() {}

AdjList::AdjList(const uint &_size, const bool &_directed) : Graph(_size, _directed, false) {
    for (uint i = 0; i < size; ++i) {
        string key(1, static_cast<char>('A' + i));
        adjList[key] = list<tuple<string, int>>();
    }
}

AdjList::AdjList(const uint &_size, const bool &_directed, const vector<string> &names) : Graph(_size, _directed, false) {
    if (names.size() != _size) {
        throw invalid_argument("Number of names must match the size of the graph");
    }

    for (uint i = 0; i < size; ++i) {
        adjList[names[i]] = list<tuple<string, int>>();
    }
}

/*
 * ===========================================================================
 * =                                   DATA                                  =
 * ===========================================================================
 */
vector<string> AdjList::vertexNames() const {
    vector<string> names;
    for (const auto &pair : adjList) {
        names.push_back(pair.first);
    }
    return names;
}

/*
 * ===========================================================================
 * =                                   EDGE                                  =
 * ===========================================================================
 */
inline bool AdjList::viableEdge(const string &from, const string &to) const {
    return hasVertex(from) && hasVertex(to);
}

bool AdjList::hasEdge(const string &from, const string &to) {
    if (!viableEdge(from, to)) {
        return false;
    }

    auto comp = [&to](const auto &t) { return get<0>(t) == to; };
    auto it = find_if(adjList.at(from).begin(), adjList.at(from).end(), comp);

    return it != adjList.at(from).end();
}

bool AdjList::addEdge(const string &from, const string &to, const uint &weight) {
    if (viableEdge(from, to) && !hasEdge(from, to)) {
        adjList[from].push_back(make_tuple(to, weight));
        if (!directed) {
            adjList[to].push_back(make_tuple(from, weight));
        }
        return true;
    }
    return false;
}

bool AdjList::editEdge(const string &from, const string &to, const uint &weight) {
    if (!hasEdge(from, to)) {
        return false;
    }

    // Atualizar apenas o peso da aresta no sentido 'from' -> 'to'
    bool edgeUpdated = false;
    for (auto &edge : adjList[from]) {
        if (get<0>(edge) == to) {
            get<1>(edge) = weight;
            edgeUpdated = true;
            break;
        }
    }

    // Para grafos não direcionados, atualize também a aresta no sentido inverso 'to' -> 'from'
    if (!directed && edgeUpdated) {
        for (auto &edge : adjList[to]) {
            if (get<0>(edge) == from) {
                get<1>(edge) = weight;
                break;
            }
        }
    }

    return edgeUpdated;
}

bool AdjList::removeEdge(const string &from, const string &to) {
    if (hasEdge(from, to)) {
        auto it = remove_if(
            adjList.at(from).begin(),
            adjList.at(from).end(),
            [&to](const auto &t) { return get<0>(t) == to; });

        adjList.at(from).erase(it, adjList.at(from).end());
        return true;
    }
    return false;
}

void AdjList::removeEdgesTo(const string &to) {
    for (auto &pair : adjList) {
        auto &edges = pair.second;
        edges.remove_if([&to](const tuple<string, int> &edge) {
            return get<0>(edge) == to;
        });
    }
}

inline void AdjList::removeEdgesFrom(const string &from) {
    if (auto it = adjList.find(from); it != adjList.end()) it->second.clear();
}

/*
 * ===========================================================================
 * =                                  VERTEX                                 =
 * ===========================================================================
 */
inline bool AdjList::hasVertex(const string &vertexName) const {
    return adjList.find(vertexName) != adjList.end();
}

void AdjList::addVertex(const uint &newVertexCount) {
    for (uint i = size; i < size + newVertexCount; i++) {
        string key(1, static_cast<char>('A' + i));
        adjList[key] = list<tuple<string, int>>();
    }

    size += newVertexCount;
}

bool AdjList::addVertex(const vector<string> &vertexNames) {
    int newVertexCount = vertexNames.size();

    for (auto &name : vertexNames) {
        if (hasVertex(name)) {
            return false;
        }
    }

    for (uint i = size; i < size + newVertexCount; i++) {
        adjList[vertexNames[i - size]] = list<tuple<string, int>>();
    }

    size += newVertexCount;

    return true;
}

inline bool AdjList::removeVertex(const string &vertexName) {
    return hasVertex(vertexName) ? adjList.erase(vertexName), removeEdgesTo(vertexName), size--, true : false;
}

bool AdjList::removeVertex(const vector<string> &vertexNames) {
    int i = 0;
    for (auto &name : vertexNames) {
        i += adjList.erase(name);
    }
    return i == size;
}

/*
 * ===========================================================================
 * =                                   DEBUG                                 =
 * ===========================================================================
 */
void AdjList::print() const {
    for (const auto &[key, edges] : adjList) {
        cout << key << ": ";
        bool isFirst = true;
        for (const auto &[vertex, weight] : edges) {
            if (isFirst) {
                cout << "";
                isFirst = false;
            } else {
                cout << ", ";
            }
            cout << "[" << vertex << " " << weight << "]";
        }
        cout << endl;
    }

    cout << endl;
}

/*
 * ===========================================================================
 * =                              INDUCED GRAPH                              =
 * ===========================================================================
 */
AdjList *AdjList::clone() const {
    // Cria uma nova instância de AdjList
    auto *clone = new AdjList(size, directed);

    // Copia todos os vértices e suas listas de adjacências
    for (const auto &pair : adjList) {
        const auto &vertex = pair.first;
        const auto &edges = pair.second;

        clone->adjList[vertex] = edges; // Copia a lista de adjacências
    }

    return clone;
}

AdjList *AdjList::transitiveClosure(const string &start) {
    AdjList *closure = new AdjList();
    set<string> visited;
    dfs(start, visited, *closure);
    return closure;
}

AdjList *AdjList::reverse() {
    AdjList *reverse = new AdjList();
    reverse->addVertex(vertexNames());

    for (const auto &[from, edges] : this->adjList) {
        for (const auto &[to, weight] : edges) {
            reverse->addEdge(to, from, weight);
        }
    }

    return reverse;
}

AdjList *AdjList::minIncomingEdgeGraph() {
    AdjList *reverse = this->reverse();

    for (auto &[_, edges] : reverse->adjList) {
        if (!edges.empty()) {
            auto minEdge = min_element(edges.begin(), edges.end(),
                                       [](const tuple<string, int> &a, const tuple<string, int> &b) {
                                           return get<1>(a) < get<1>(b);
                                       });

            list<tuple<string, int>> minEdgesList = {*minEdge};
            edges = minEdgesList;
        }
    }

    AdjList *inducedGraph = reverse->reverse();
    delete reverse;
    return inducedGraph;
}

/*
 * ===========================================================================
 * =                          MINIMUM SPANNING TREE                          =
 * ===========================================================================
 */
Graph *AdjList::arborescence(const string &start) {
    if (!hasVertex(start)) {
        return NULL;
    }

    // Preprocessing
    AdjList *preTree = transitiveClosure(start);
    AdjList *backup = preTree->clone();
    preTree->removeEdgesTo(start);

    AdjList *tree;
    bool found = false;

    do {
        tree = preTree->minIncomingEdgeGraph();
        found = tree->countReachableVertices(start) == preTree->getSize();
        if (!found) {
            preTree->adjustEdgeWeightToCycle(tree->findCycle());
            delete tree;
        }
    } while (!found);

    tree->restoreWeights(*backup);
    delete preTree;
    delete backup;
    return tree;
}

/*
 * ===========================================================================
 * =                                 PRIVATE                                 =
 * ===========================================================================
 */
// DFS
void AdjList::dfs(const string &vertex, set<string> &visited, int &count) {
    visited.insert(vertex);
    count++;

    for (const auto &edge : adjList.at(vertex)) {
        string neighbour = get<0>(edge);
        if (visited.find(neighbour) == visited.end()) {
            dfs(neighbour, visited, count);
        }
    }
}

void AdjList::dfs(const string &vertex, set<string> &visited, AdjList &closure) {
    visited.insert(vertex);
    closure.addVertex({vertex});
    for (const auto &[neighbour, weight] : adjList[vertex]) {
        if (visited.find(neighbour) == visited.end()) {
            dfs(neighbour, visited, closure);
        }
        closure.addEdge(vertex, neighbour, weight);
    }
}

void AdjList::dfs(const string &vertex, map<string, int> &vertexState, stack<string> &pathStack, set<string> &cycle) {
    vertexState[vertex] = GRAY;
    pathStack.push(vertex);

    for (const auto &edge : adjList[vertex]) {
        string neighbour = get<0>(edge);
        if (vertexState[neighbour] == GRAY) {
            // Ciclo detectado
            while (!pathStack.empty() && pathStack.top() != neighbour) {
                cycle.insert(pathStack.top());
                pathStack.pop();
            }
            cycle.insert(neighbour);
            return; // Retorna, já que encontrou um ciclo
        } else if (vertexState[neighbour] == WHITE) {
            dfs(neighbour, vertexState, pathStack, cycle);
            if (!cycle.empty()) return; // Ciclo já foi encontrado em um dos ramos
        }
    }

    pathStack.pop();
    vertexState[vertex] = BLACK; // Marcando como visitado
}

// Arborescence
uint AdjList::countReachableVertices(const string &start) {
    set<string> visited;
    int count = 0;

    if (adjList.find(start) != adjList.end()) {
        dfs(start, visited, count);
    }

    return count;
}

set<string> AdjList::findCycle() {
    map<string, int> vertexState;
    for (const auto &pair : adjList) {
        vertexState[pair.first] = WHITE;
    }

    stack<string> pathStack;
    set<string> cycle;

    for (const auto &pair : adjList) {
        if (vertexState[pair.first] == WHITE) {
            dfs(pair.first, vertexState, pathStack, cycle);
            if (!cycle.empty()) {
                break; // Um ciclo foi encontrado
            }
        }
    }

    return cycle;
}

void AdjList::adjustEdgeWeightToCycle(const set<string> &cycle) {
    if (cycle.size() == 0) {
        cout << "Erro, ciclo vazio enviado" << endl;
        return;
    }

    // Armazenar ponteiros para as arestas que apontam para o ciclo
    vector<tuple<string, list<tuple<string, int>>::iterator>> edgesPointingToCycle;

    // Encontrar arestas que apontam para o ciclo
    for (auto &pair : adjList) {
        const auto &from = pair.first;
        if (cycle.find(from) == cycle.end()) { // Se 'from' não faz parte do ciclo
            for (auto it = pair.second.begin(); it != pair.second.end(); ++it) {
                const auto &to = get<0>(*it);
                if (cycle.find(to) != cycle.end()) { // Se 'to' faz parte do ciclo
                    edgesPointingToCycle.push_back(make_tuple(from, it));
                }
            }
        }
    }

    // Encontrar o menor peso entre essas arestas
    int minWeight = INT_MAX;
    for (const auto &edgeInfo : edgesPointingToCycle) {
        auto edgeIt = get<1>(edgeInfo);
        minWeight = min(minWeight, get<1>(*edgeIt));
    }

    // Verifica se encontrou uma aresta válida
    if (minWeight == INT_MAX) {
        return; // Não há arestas apontando para o ciclo
    }

    // Ajustar o peso das arestas que apontam para o ciclo
    for (const auto &edgeInfo : edgesPointingToCycle) {
        const auto &from = get<0>(edgeInfo);
        auto edgeIt = get<1>(edgeInfo);
        get<1>(*edgeIt) -= minWeight; // Ajustar o peso
    }
}

void AdjList::restoreWeights(const AdjList &original) {
    for (auto &pair : adjList) {
        const string &from = pair.first;
        auto &neighbours = pair.second;

        for (auto &edge : neighbours) {
            string &to = std::get<0>(edge);
            int &weight = std::get<1>(edge);

            // Encontrar a aresta correspondente no grafo original e restaurar o peso
            const auto &originalNeighbours = original.adjList.at(from);
            auto it = std::find_if(originalNeighbours.begin(), originalNeighbours.end(),
                                   [&to](const std::tuple<std::string, int> &origEdge) {
                                       return std::get<0>(origEdge) == to;
                                   });
            if (it != originalNeighbours.end()) {
                weight = std::get<1>(*it); // Restaura o peso original
            }
        }
    }
}
