#include <string> //for string functions
#include <cstdlib> // For several general-purpose functions
#include <iostream>
using namespace std; // I know this is bad but this project isn't of high enough scope for it to matter

//technicly not exactly what a priority queue is supposed to be, but the amortized time complexity of an actual priority queue and whatever this is are of equivilent value for this application (swap out complexity of insert for complexity of deQueue min)

class priorityQueue {

	struct node {
	int distance;
	bool processed;
	};
	node* arrayStructure;
	int size;

public:

	//default constructor, unused of course O(1)
	priorityQueue() : size(0), arrayStructure(nullptr) {}

	//actual constructor O(n)
	priorityQueue(int n, int src) : arrayStructure(new node[n]), size(n) {
		for (int i = 0; i < n; i++) {
			arrayStructure[i].distance = INT_MAX;
			arrayStructure[i].processed = false;
		}
		arrayStructure[src].distance = 0;
	}

	//modifies a key O(1)
	void decreaseKey(int index, int cost) {
		arrayStructure[index].distance = cost;
	}


	//returns the index of the highest priority (lowest cost) element and marks it as processed O(n)
	int dequeueMin() {
		int minCost = INT_MAX;
		int minIndex = 0;
		for (int i = 0; i < size; i++) {
			if (arrayStructure[i].distance <= minCost && !arrayStructure[i].processed) {
				minCost = arrayStructure[i].distance;
				minIndex = i;
				//cout << arrayStructure[minIndex].distance;
			}
		}
		arrayStructure[minIndex].processed = 1;

		return minIndex;
	}

	int getDistance(int index) {
		return arrayStructure[index].distance;
	}

	//checks if a node has been processed O(1)
	bool nodeProcessed(int index) {
		return arrayStructure[index].processed;
	}
};