#pragma once

#include <string> //for string functions
#include <cstdlib> // For several general-purpose functions
//#include <iostream> //for debugging

class pairingHeap {

	struct HeapNode {
		int index;
		int distance;
		bool processed;
		HeapNode* parent;
		HeapNode* leftChild;
		HeapNode* nextSibling;

		//default constructor so the compiler is happy, something has gone horribly wrong if this is relevant just remembered you can do assignment statements in the same line as a constructor declaration
		HeapNode() : distance(0), index(0),processed(false), parent(nullptr), leftChild(nullptr), nextSibling(nullptr) {}

		//creates a new node O(1)
		HeapNode(int k, int i, HeapNode* l, HeapNode* n) :
			distance(k), index(i), processed(false), parent(nullptr), leftChild(l), nextSibling(n) {
		}

		// Adds a child and sibling to the node
		void addChild(HeapNode* node) {
			if (leftChild == nullptr) {
				//place node as the child of this HeapNode
				leftChild = node;
				node->parent = this;
			}
				
			else {
				//emplace node before leftChild in list
				node->nextSibling = leftChild;
				node->parent = this;
				leftChild->parent = node;
				leftChild = node;
			}
		}

	};

private:
	//tests if a node actually has anything in it
	bool empty(HeapNode* node) {
		return (node == nullptr);
	}

	//the kinda scary functino that actually wasnt too bad, merging
	HeapNode* merge(HeapNode* A, HeapNode* B) {
		//first test the edge case of either sub-heap being empty
		if (A == nullptr) return B;
		if (B == nullptr) return A;

		//compare the two heads to find out who becomes the parent, and who becomes the child (smaller key = parent) return the new head of the heap
		if (A->distance < B->distance) {
			A->addChild(B);
			return A;
		}
		else {
			B->addChild(A);
			return B;
		}
	}

	//the scarier function which was much more of a pain to figure out, used for deleting the head (very common for a min heap ofc)
	HeapNode* TwoPassMerge(HeapNode* node) {

		//if the heap is empty or hass no children the head can be deleted without any other implementation
		if (node == nullptr || node->nextSibling == nullptr)
			return node;
		//otherwise initiate three pointers, one to keep track of the subject of deletion, and the other two to keep track of the next two nodes in the head's linked list
		else {
			HeapNode* A = node;
			HeapNode* B = node->nextSibling;
			HeapNode* newNode = node->nextSibling->nextSibling;

			//break the heap into three sub-heaps
				//set all parents to nullptr;
			if (A->nextSibling->parent == root->leftChild) A->nextSibling == nullptr;


			A->nextSibling = nullptr;
			B->nextSibling = nullptr;

			//merge A and B, and recursivley start the process over until you get to the end of the head's link list when this happens the recursive function will collapse and the tree's will be re-merged
			//recursion is so fun :D also a pain to understand
			return merge(merge(A, B), TwoPassMerge(newNode));
		}
		return nullptr;
	}

	//Inserting a new node into the heap is pretty simple once merge is defined, just make a new node and merge it the array structure is updated as well
	HeapNode* insert(HeapNode *node, int k, int i) {
		arrayStructure[i] = new HeapNode(k, i, nullptr, nullptr);
		return merge(node, arrayStructure[i]);
	}

	//returns the value of a node, only used on the head of the tree hence the title "top"
	int top(HeapNode* node) {
		return node->distance;
	}

	//Deleting a node (always deleting the root) by starting the double merge from the child
	HeapNode* deleteNode(HeapNode* node) {
		return TwoPassMerge(node->leftChild);
	}

public:
	HeapNode* root;
	HeapNode** arrayStructure;
	//int size;

	//2nd-level funcitons that generify the 3rd level functions

	bool empty() {
		return empty(root);
	}

	void insert(int key, int index) {
		root = insert(root, key, index);
	}

	void deleteNode() {
		root = deleteNode(root);
	}

	//the functions directly used by the algorithim

	int dequeueMin() {
		if (!empty()) {
			int ind = root->index;
			arrayStructure[ind]->processed = true;
			deleteNode();
			if (!empty())
				root->parent = nullptr;
			return ind;
		}
		else throw underflow_error("No elements left");
  	}

	//returns the processed field from the array structure
	bool nodeProcessed(int index) {
		return (arrayStructure[index]->processed);
	}

	//returns distance from array structure
	int getDistance(int index) {
		return arrayStructure[index]->distance;
	}

	//The big one, disconnects a node from the heap, decreases its key, and remerges it
	void decreaseKey(int index, int newKey) {

		HeapNode* target = arrayStructure[index];
		arrayStructure[index]->distance = newKey;

		if (target != root && target->parent->distance > target->distance) {
			//break the link by going to the parent of the target node figuring out if its parent is its prevSibling or actual parent and setting that to nullptr
			//cout << "\nNewKey for index " << index << ": " << arrayStructure[index]->distance << "\nDecreaseKey target Parent's sibling == target: " << (target->parent->nextSibling == target) << endl;

			if (target->nextSibling != nullptr) {
				target->nextSibling->parent = nullptr;
				merge(target, target->nextSibling);
				target->nextSibling = nullptr;
			}

			(target->parent->leftChild == target ? target->parent->leftChild : target->parent->nextSibling) = nullptr;
			target->parent = nullptr;



			root = merge(root, target);
			//traverseTree();
		}
	} 

	//empty constructor (SNC - should not comeup)
	pairingHeap() :
		root(nullptr), arrayStructure(nullptr) {}

	//Testing constructor (SNC)
	pairingHeap(int n) : root(nullptr) {
		arrayStructure = new HeapNode * [n];
		for (int i = 0; i < n; i++) {
			//quadratic function so key structure isnt bad
			insert(i*i-2*i+3, i);
		}
	}

	//constructor
	pairingHeap(int n, int src) {
		arrayStructure = new HeapNode * [n];
		for (int i = 0; i < n; i++) {
			insert(INT_MAX, i);
		}
		decreaseKey(src, 0);
	}

	//DEBUGGING because I love choose your own adventure books: Have a choose your own adventure heap:
	HeapNode* traverseTree(HeapNode* current) {

		int input;
		if (!empty()) {
			cout << "\nCurrent index: " << current->index << "\t\tCurrent Distance: " << current->distance;
			cout << "\n1) Parent:\tIndex: " << (current->parent == nullptr ? "NULL" : to_string(current->parent->index)) << "\tDistance: " << (current->parent == nullptr ? "NULL" : to_string(current->parent->distance));
			cout << "\n2) Left Child:\tIndex: " << (current->leftChild == nullptr ? "NULL" : to_string(current->leftChild->index)) << "\tDistance: " << (current->leftChild == nullptr ? "NULL" : to_string(current->leftChild->distance));
			cout << "\n3) Next Sibling: Index: " << (current->nextSibling == nullptr ? "NULL" : to_string(current->nextSibling->index)) << "\tDistance: " << (current->nextSibling == nullptr ? "NULL" : to_string(current->nextSibling->distance));
			cout << "\n4) Exit\nSelect Option: ";

			cin >> input;
			switch (input)
			{
			case 1:
				return current->parent !=nullptr ? traverseTree(current->parent) : traverseTree(current);
				break;
			case 2:
				return current->leftChild != nullptr ? traverseTree(current->leftChild) : traverseTree(current);
				break;
			case 3:
				return current->nextSibling != nullptr ? traverseTree(current->nextSibling) : traverseTree(current);
				break;
			case 4:
				return nullptr;
				break;
			default:
				cin.ignore();
				break;
			}
		}
		else {
			cout << "\nTree Empty\n";
			return nullptr;
		}
	}

	HeapNode* traverseTree() {
		return traverseTree(root);
	}


};

