#ifndef GRAPHY_GRAPH_H
#define GRAPHY_GRAPH_H

#include <string>
#include <vector>

typedef unsigned int uint;

namespace Graphy {
    class Graph {
      protected:
        uint size;
        bool directed;
        bool multigraph;

      public:
        Graph() : size(0), directed(true), multigraph(true) {}
        Graph(uint _size_, bool _directed_, bool _multigraph_) : size(_size_), directed(_directed_), multigraph(_multigraph_) {}
        virtual ~Graph() {}

        // Data
        uint getSize() const { return size; }
        bool isDirected() const { return directed; }
        bool isMultigraph() const { return multigraph; }
        virtual std::vector<std::string> vertexNames() const = 0;

        // Edge
        virtual bool viableEdge(const std::string &from, const std::string &to) const = 0;
        virtual bool hasEdge(const std::string &from, const std::string &to) = 0;
        virtual bool addEdge(const std::string &from, const std::string &to, const uint &weight = 1) = 0;
        virtual bool editEdge(const std::string &from, const std::string &to, const uint &weight) = 0;
        virtual bool removeEdge(const std::string &from, const std::string &to) = 0;
        virtual void removeEdgesTo(const std::string &to) = 0;
        virtual void removeEdgesFrom(const std::string &from) = 0;

        // Vertex
        virtual bool hasVertex(const std::string &vertexName) const = 0;
        virtual void addVertex(const uint &newVertexCount) = 0;
        virtual bool addVertex(const std::vector<std::string> &vertexNames) = 0;
        virtual bool removeVertex(const std::string &vertexName) = 0;
        virtual bool removeVertex(const std::vector<std::string> &vertexNames) = 0;

        // Debug
        virtual void print() const = 0;

        // Induced Graph
        virtual Graph *clone() const = 0;
        virtual Graph *transitiveClosure(const std::string &start) = 0;
        virtual Graph *reverse() = 0;
        virtual Graph *minIncomingEdgeGraph() = 0;

        // Minimum spanning tree
        virtual Graph* arborescence(const std::string &start) = 0;
    };
}

#endif // GRAPHY_GRAPH_H