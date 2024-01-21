#ifndef GRAPHY_ADJLIST_H
#define GRAPHY_ADJLIST_H

#include "graph.hpp"
#include <list>
#include <map>
#include <set>
#include <stack>

namespace Graphy {
    class AdjList : public Graph {
      private:
        std::map<std::string, std::list<std::tuple<std::string, int>>> adjList;

        // DFS
        void dfs(const std::string &vertex, std::set<std::string> &visited, int &count);
        void dfs(const std::string &vertex, std::set<std::string> &visited, AdjList &closure);
        void dfs(const std::string &vertex, std::map<std::string, int> &vertexState, std::stack<std::string> &pathStack, std::set<std::string> &cycle);

        // Arborescence
        uint countReachableVertices(const std::string &start);
        std::set<std::string> findCycle();
        void adjustEdgeWeightToCycle(const std::set<std::string> &cycle);
        void restoreWeights(const AdjList &original);

      public:
        AdjList();
        AdjList(const uint &_size, const bool &_directed);
        AdjList(const uint &_size, const bool &_directed, const std::vector<std::string> &names);

        // Data
        std::vector<std::string> vertexNames() const override;

        // Edge
        bool viableEdge(const std::string &from, const std::string &to) const override;
        bool hasEdge(const std::string &from, const std::string &to) override;
        bool addEdge(const std::string &from, const std::string &to, const uint &weight) override;
        bool editEdge(const std::string &from, const std::string &to, const uint &weight) override;
        bool removeEdge(const std::string &from, const std::string &to) override;
        void removeEdgesTo(const std::string &to) override;
        void removeEdgesFrom(const std::string &from) override;

        // Vertex
        bool hasVertex(const std::string &vertexName) const override;
        void addVertex(const uint &newVertexCount) override;
        bool addVertex(const std::vector<std::string> &vertexNames) override;
        bool removeVertex(const std::string &vertexName) override;
        bool removeVertex(const std::vector<std::string> &vertexNames) override;

        // Debug
        void print() const override;

        // Induced Graph
        AdjList *clone() const override;
        AdjList *transitiveClosure(const std::string &start) override;
        AdjList *reverse() override;
        AdjList *minIncomingEdgeGraph() override;

        // Minimum spanning tree
        Graph *arborescence(const std::string &start) override;
    };

    enum Color { WHITE,
                 GRAY,
                 BLACK };
} // namespace Graphy

#endif // GRAPHY_ADJLIST_H
