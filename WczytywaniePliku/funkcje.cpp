
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <list>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "funkcje.h"
//komentarze z doxygenem /// lub /**
/**@brief
*Identify Switch function  differentiates the paramater with which program was executed 
* @return IdentifySwitch returns pair of inputfile and outputfile
* @param argc: Number of command-line arguments.
* @param argv: Array of command-line arguments.
*/
std::pair<std::string, std::string> IdentifySwitch(int argc, char* argv[]) { 
	std::string inputFile, outputFile;

	for (int i = 1; i < argc; ++i) {
		if (std::string(argv[i]) == "-i" && i + 1 < argc) {  
			inputFile = argv[i + 1];
			i++;
		}
		else if (std::string(argv[i]) == "-o" && i + 1 < argc) {
			outputFile = argv[i + 1];
			i++;
		}
	}

	if (inputFile.empty() || outputFile.empty()) {
		std::cerr << "Usage: program_name -i input_file -o output_file" << std::endl;
	}
	std::cout << "Inputfile: " << inputFile << std::endl << "Outputfile: " << outputFile << std::endl;
	std::pair<std::string, std::string> result{ inputFile, outputFile };
	return result;

}

/**@brief
* 
*NodeListRead reads all data included in input file and converts as a graph and save it sorted by graph's weight to the vector named connectionList
* @param fileName: :Input file name 
* @param connectionList: :Vector which store informations about graph arguments
* @return NodeListRead returns sorted graph
*/

Graph NodesListRead(const std::string fileName, std::vector<Connections>& connectionList){
	Graph graph;
	std::ifstream connections(fileName);
	if (connections) {
		Connections wezel; //struktura po³¹czeñ
		while (connections >> wezel.city1 >> wezel.city2 >> wezel.BuildCost) {
			connectionList.push_back(wezel);
		}

		std::sort(connectionList.begin(), connectionList.end(), //sortowanie po wartoœci kosztu budowy od najwiêkszej do najmniejszej
			[](const Connections& a, const Connections& b) {
				return a.BuildCost > b.BuildCost;
			});

		
		//std::cout << "Connections after sorting:" << std::endl;
		graph = connectionListToGraph(connectionList);
		connections.close();
	}
	return graph;
}
/**@brief
*
*connectionListToGraph converts all data stored in connectionList to graph
* @param connectionList: :Vector which store informations about graph arguments
* @return  connectionListToGraph returns graph from list
*/
Graph connectionListToGraph(std::vector<Connections>& connectionList) { //zmiana listy na graf
	Graph graph;
	for (const auto& wezel : connectionList) {
		//std::cout << connection.city1 << " " << connection.city2 << " " << connection.BuildCost << std::endl;
		graph[wezel.city1].insert({ wezel.city2, wezel.BuildCost });
		graph[wezel.city2].insert({ wezel.city1, wezel.BuildCost });
	}
	//std::cout << std::endl;
	return graph;
} 

/**@brief
*  DFS(Depth-first search) is an algorithm for traversing or searching tree or graph data structures. It is recursive function
* @param graph: :Graph based on connectionList 
* @param city: :City is one of graph's parameter which stores name of city needed to compare visited cities to make DFS works correctly.
* @param nodeCount: :It is a parameter which is used to controll amount of cities in the graph. We need it in later to count nodes.
* @param visited: The set of vistied cities
* */
void DFS(const Graph& graph, const std::string& city, std::set<std::string>& visited, int &nodeCount) { 
	visited.insert(city);	//zapisywanie odwiedzonych miast
	//std::cout << city << " ";
	nodeCount++; //zliczanie kazdego miasta

	for (const auto& neighbor : graph.at(city)) {
		if (visited.find(neighbor.first) == visited.end()) { //sprawdzenie, czy s¹siad nie zosta³ jeszcze odwiedzony, jeœli prawda to wywo³anie funkcji rekurencyjnie i wpisanie tego miasta do vistied
			DFS(graph, neighbor.first, visited, nodeCount);
		}
	}
}

/**@brief
* countNodes is a function which count nodes to make sure the numer of start nodes are equal to nodes after using reverse Kruskal algorithm.
* @param graph: :Graph based on connectionList  
* @return The function returns total nodes number in graph
* */
int countNodes(const Graph& graph) {
	std::set<std::string> visited;
	int totalNodeCount = 0;

	for (const auto& node : graph) {
		if (visited.find(node.first) == visited.end()) {  //dzia³anie takie samo jak dla DFS natomiast zwraca iloœæ wierzcho³ków//prawdopodobnie da sie to zapisac w DFS 
			int nodeCount = 0;
			DFS(graph, node.first, visited, nodeCount);
			totalNodeCount += nodeCount;
		}
	}
	return totalNodeCount;

} 
/**@brief
* isConnected compares number of start nodes with number of nodes after removing one of connections in reverse Kruskal algorithm.
* @param graph: :Graph based on connectionList
* @return The function returns an information in bool if the graph is connected.
* */
bool isConnected(const Graph& graph) {
	std::set<std::string> visited;
	if (graph.empty()) {
		std::cerr << "Graph is empty." << std::endl;
		return true;  
	}

	
	std::string startNode = graph.begin()->first;
	int nodeCount=0;
	DFS(graph, startNode, visited, nodeCount);
	return nodeCount == graph.size();
}

/**@brief
* KruskalAlgorithm in fact is reverse Kruskal algorithm which is made to do minimum spanning tree.
* @note It removes everytime the heaviest vertex from the graph and check if the graph is still connected. If not it adds removed vertex and check the next heaviest vertex.
* @param connectionList: :Vector which store informations about graph arguments.
* @param originalGraph: :graph which was created initially. We need it to make sure the graph is connected
* @return KruskalAlgorithm returns updated vector
* */


Graph KruskalAlgorithm(std::vector<Connections>& connectionList, const Graph& originalGraph) {
	int allNodes = countNodes(originalGraph); //przypisanie iloœci pocz¹tkowej miast w grafie do zmiennej
	Graph graph = connectionListToGraph(connectionList);
	if (!isConnected(originalGraph)) {
		std::cerr << "Error: The original graph is not connected.\n";
		exit(1);
	}
	int i = 0;
	while (i < connectionList.size()) {
		Connections connections = connectionList[i];   //przejscie przez ca³¹ tablice usuwaj¹c najciê¿sze krawêdzie i sprawdzenie czy graf jest nadal spójny.
		connectionList.erase(connectionList.begin() + i);
		graph = connectionListToGraph(connectionList);

		if (!isConnected(graph)) {
			connectionList.insert(connectionList.begin() + i, connections);
		}
		i++;
	}

	return connectionListToGraph(connectionList);
}


/**@brief
* saveToFile saves the answer of program to output file
* @param fileName: :It stores the output file name
* @param connectionList: :Vector which store informations about graph arguments. 
* */

void saveToFile(const std::string fileName, std::vector<Connections>& connectionList) {
	std::ofstream outputFile(fileName);
	if (outputFile.is_open()) {
		for (int i = 0; i < connectionList.size(); i++) {
			Connections connection = connectionList[i];
			outputFile << " = " << connection.city1 << " " << connection.city2 << " " << connection.BuildCost << std::endl;

		}

		outputFile.close();

		if (outputFile.good()) {
			std::cout << "Wynik zostal zapisany do pliku " << fileName << std::endl;
		}
		else {
			std::cerr << "B³¹d podczas zapisu do pliku." << std::endl;
		}
	}
	else {
		std::cerr << "B³¹d podczas otwierania pliku do zapisu." << std::endl;
	}
}