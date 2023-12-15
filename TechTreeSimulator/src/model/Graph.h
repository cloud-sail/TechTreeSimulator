#pragma once
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <sstream>

namespace rm
{
namespace model
{
	using std::string;
	using std::vector;
	using std::queue;
	using std::unordered_map;

	class Node
	{
	public:
		string name;
		bool enabled;
		vector<Node*> children;

		explicit Node(string n);

	};

	class Graph
	{
	private:
		unordered_map<string, Node*> nodes_;
		vector<Node*> topological_order_;

		bool checkParentEnabled(Node* node);
		vector<Node*> topologicalSort();
		void disableSubTree(Node* node);


	public:
		void addNode(string name);
		void addEdge(string parentName, string childName);
		void disableNode(string name);
		void enableNode(string name);
		void printGraph();

		void processUserInput(string userInput);
		vector<Node*> getTopologicalOrder();
		void toggleNode(Node* node);
		~Graph();
	};
}
}


