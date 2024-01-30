/**
*@file WczytywaniePliku.cpp
*@brief BudowaInfrastrukturyDrogowej - Main	  
*@author by Adam Skorus
* @date 29.01.2024
* 
*/



#include <iostream>
#include <string>
#include <map>
#include <set>
#include <list>
#include <fstream>
#include <queue>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "funkcje.h"

/**
* @brief Main function where we all needed call functions.
*/
int main(int argc, char* argv[]) {
	if (argc < 5) {
		std::cout << "Usage: program_name -i input_file -o output_file" << std::endl;
		return 1;
	}
	std::pair<std::string, std::string> fileNamesPair = IdentifySwitch(argc, argv);
	std::set<std::string> visited;
	std::vector<Connections> connectionList;
	Graph myGraph = NodesListRead(fileNamesPair.first, connectionList);
	Graph resultGraph = KruskalAlgorithm(connectionList, myGraph);
	saveToFile(fileNamesPair.second,connectionList);
	return 0;
}