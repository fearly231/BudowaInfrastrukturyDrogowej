#ifndef FUNKCJE_H
#define FUNKCJE_H
#include <vector>
#include <string>
/**
 * @brief Represents a graph data structure where nodes are identified by strings,
 * and edges are represented as pairs of connected nodes with associated weights.
 *
 * The Graph typedef defines a mapping from node names (strings) to sets of
 * neighboring nodes, each associated with a weight (double).
 */
typedef std::map<std::string, std::set<std::pair<std::string, double>>> Graph;


/**
 * @brief Represents a connection between two cities along with the associated build cost.
 */
struct Connections {
    std::string city1; /**< Name of the first city. */
    std::string city2; /**< Name of the second city. */
    double BuildCost;  /**< Cost of building a connection between the cities. */
};
Graph NodesListRead(const std::string fileName, std::vector<Connections>& connectionList);
void DFS(const Graph& graph, const std::string& city, std::set<std::string>& visited, int& nodeCount);
std::pair<std::string, std::string> IdentifySwitch(int argc, char* argv[]);
bool isConnected(const Graph& graph);
Graph KruskalAlgorithm(std::vector<Connections>& connectionList, const Graph& graph);
int countNodes(const Graph& graph);
Graph connectionListToGraph(std::vector<Connections>& connectionList);
void saveToFile(const std::string fileName, std::vector<Connections>& connectionList);











#endif
