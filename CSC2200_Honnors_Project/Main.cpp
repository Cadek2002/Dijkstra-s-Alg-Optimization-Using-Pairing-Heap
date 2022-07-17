#pragma once
//==========================================================
//
// Title:      Dijkstra's Pairing Heap
// Course:     CSC 2200
// Lab Number: HONN
// Author:     Cayden Koweck
// Date:       4/01/2022
// Description:
//   A variation of Dijkstra's algorithim that uses a pairing heap to optimize the get-min procedure
//
//==========================================================
#include <cstdlib> // For several general-purpose functions
#include <fstream> // For file handling
#include <iomanip> // For formatted output
#include <iostream> // For cin, cout, and system
#include <string> // For string data type
#include <sstream> // For importing a CSV file
#include "priorityQueue.h" //priority queue
#include "pairingHeap.h" //pairing heap
#include <chrono> //le clock :D
using namespace std; // So "std::cout" may be abbreviated to "cout"
using namespace std::chrono;


//function declarations
void printGraph(int**, int);
template <typename minStruct>
void findPath(int, int, int**, int);

int main()
{

	const string FILENAME = "Graph3.csv";

	//header statement
	cout << "Dijkstra's Algorithim Pairing Heap\n====================\n";

	fstream inputFile(FILENAME, ios::in);
	string line, element;
	int s = 0, e, input = 0, n = 0;

	if (inputFile.is_open()) {


		//counting header row to get n
		getline(inputFile, line);
		stringstream eleStream(line);
		//uses the header line of junk data to get the N-Value
		while (getline(eleStream, element, ','))
			n++;


		cout << "Loading graph of size: " << n << endl;

		//dynamicly creating an array of n pointers to an array of n integers and initializing the values to the CSV
		int** graph = new int* [n];
		for (int i = 0; i < n; i++) {

			getline(inputFile, line);
			graph[i] = new int[n];
			stringstream eleStream(line);

			for (int j = 0; j < n; j++) {
				getline(eleStream, element, ',');
				graph[i][j] = stoi(element);
				cout << setw(3) << graph[i][j];
			}
			
			
			cout << endl;

		}

		cout << "\nGraph Loaded. ";

		//Runs the find path algorithim using both a pairing heap, and a priority queue and displays execution time (via system clock)
		while (input != 3) {
			cout << "Select action:\n1) Find Path\n2) Display Graph\n3) Exit\n<< ";
				cin >> input;

			switch (input)
			{
			case 1:
				cout << "Insert Start Node Index: ";
				cin >> s;
				cout << "Insert End Node Index: ";
				cin >> e;
				try {
					if (s < n && s >= 0 && e < n && e >= 0) {
						cout << "\nPairing Heap: ";
						findPath<pairingHeap>(s, e, graph, n);
						cout << "\nPriority Queue:";
						findPath<priorityQueue>(s, e, graph, n);
						cout << "\n\n";
					}
					else cout << "\nError: Chosen node(s) out of range\n";
				}
				catch (underflow_error e) {
					cout << "\nError: Heap Structure Broken\n";
				}
				break;
			case 2:
				printGraph(graph, n);
				break;
			case 3:
				break;
			default:
				cout << "Error: Invalid Action";
				break;
			}

		}
	}
	else
		cout << "Error: File \"" << FILENAME << "\" could not be opened.";
	cout << "\n\nEnd of Application";
	}

//Loops through the file and prints the graph row-by-row
void printGraph(int** graph, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			cout << setw(3) << graph[i][j];
		cout << endl;
	}
}

template <typename minStruct>
void findPath(int startNode, int endNode, int** graph, int n) {
	int minIndex = 0;
	string* path = new string[n];
	minStruct minStructure(n, startNode);
	//for (int i = 0; i < n; i++) cout << minStructure.dequeueMin() << " ";
	//minStructure.traverseTree(minStructure.root);

	for (int i = 0; i < n; i++) {
		path[i] = to_string(i);
	}

	//
	auto start = high_resolution_clock::now();
	while (!minStructure.nodeProcessed(endNode)) {
		minIndex = minStructure.dequeueMin();
		for (int i = 0; i < n; i++) {
 			if (graph[minIndex][i] && (minStructure.getDistance(minIndex) + graph[minIndex][i]) < minStructure.getDistance(i)) {
				minStructure.decreaseKey(i, minStructure.getDistance(minIndex) + graph[minIndex][i]);	//set key
				path[i] = path[minIndex] + "->" + to_string(i);											//track path
			}
		}
		//for (int i = 0; i < n; i++) cout << "Index:" << minStructure.arrayStructure[i]->index << "\tDistance: " << minStructure.getDistance(i) << endl;
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);

	cout << "\nDistance:\t" << minStructure.getDistance(endNode);
	cout << "\nPath:\t\t" << path[endNode];
	cout << "\nRuntime:\t" << duration.count() << " microsecounds";
}
