#include "Graph.h"

namespace rm
{
namespace model {
	Node::Node(string n) :name(std::move(n)), enabled(true){}

	bool Graph::checkParentEnabled(Node* node)
	{
		for (auto& pair : nodes_)
		{
			Node* parent = pair.second;
			for (Node* child : parent->children)
			{
				if (!parent->enabled && child == node)
				{
					return false;
				}
			}
		}
		return true;
	}

	vector<Node*> Graph::topologicalSort()
	{
		unordered_map<Node*, int> inDegree;
		queue<Node*> zeroIndegreeQueue;
		vector<Node*> result;

		for (auto& pair:nodes_)
		{
			inDegree[pair.second] = 0;
		}
		for (auto& pair: nodes_)
		{
			Node* node = pair.second;
			for (Node* child: node->children)
			{
				++inDegree[child];
			}

		}
		for (auto& pair: inDegree)
		{
			if (pair.second == 0)
			{
				zeroIndegreeQueue.push(pair.first);
			}
		}
		while (!zeroIndegreeQueue.empty())
		{
			Node* curNode = zeroIndegreeQueue.front();
			zeroIndegreeQueue.pop();
			result.push_back(curNode);

			for (Node* child: curNode->children)
			{
				--inDegree[child];
				if (inDegree[child]==0)
				{
					zeroIndegreeQueue.push(child);
				}
			}
		}
		return result;
	}

	void Graph::disableSubTree(Node* node)
	{
		if (node == nullptr) return;
		if (!node->enabled) return;
		node->enabled = false;
		for (Node* child: node->children)
		{
			disableSubTree(child);
		}
	}

	void Graph::addNode(string name)
	{
		if (name.empty())
		{
			std::cout << "Node name should not be empty" << std::endl;
			return;
		}
		if (nodes_.find(name) == nodes_.end())
		{
			Node* newNode = new Node(name);
			nodes_[name] = newNode;
			topological_order_ = topologicalSort();
		}else
		{
			std::cout << "Node exist: " << name << " skip add node" << std::endl;
		}
	}

	void Graph::addEdge(string parentName, string childName)
	{
		if(nodes_.find(parentName)!= nodes_.end() && nodes_.find(childName)!=nodes_.end())
		{
			Node* parent = nodes_[parentName];
			Node* child = nodes_[childName];

			bool edgeExists = std::any_of(parent->children.begin(), parent->children.end(),
				[child](Node* existedChild) {return existedChild == child; });

			if (!edgeExists){
				parent->children.push_back(child);
				auto newTopo = topologicalSort();
				if (newTopo.size() != nodes_.size())
				{
					std::cout << "there is a cycle in DAG, skip add edge" << std::endl;
					parent->children.pop_back();
				}
				else
				{
					topological_order_ = newTopo;
				}
			}
			else
			{
				std::cout << "Edge already exists between: " << parentName << " " << childName << std::endl;
			}
		}
		else
		{
			std::cout << "Node not found skip add edge: " << parentName << " " << childName << std::endl;
		}
	}

	void Graph::disableNode(string name)
	{
		if (nodes_.find(name)!=nodes_.end())
		{
			disableSubTree(nodes_[name]);
		} else
		{
			std::cout << "Node exist: " << name << " skip disable node" << std::endl;
		}
	}

	void Graph::enableNode(string name)
	{
		if (nodes_.find(name) == nodes_.end())
		{
			std::cout << "Node exist: " << name << " skip enable node" << std::endl;
		} else
		{
			Node* node = nodes_[name];
			if (checkParentEnabled(node))
			{
				node->enabled = true;
			}
			else
			{
				std::cout << "Node cannot be enabled: " << name << std::endl;
			}
		}
	}

	void Graph::printGraph()
	{
		for (auto& pair: nodes_)
		{
			Node* node = pair.second;
			std::cout << "Node: " << node->name << ", Enabled: " << node->enabled << std::endl;
			std::cout << "----Children: ";
			for (Node* child: node->children)
			{
				std::cout << child->name << " ";
			}
			std::cout << std::endl;
		}
	}

	void Graph::processUserInput(string userInput)
	{
		std::istringstream iss(userInput);
		std::string command;
		iss >> command;

		if (command == "addnode") {
			std::string nodeName;
			iss >> nodeName;
			addNode(nodeName);
		}
		else if (command == "addedge") {
			std::string parentName, childName;
			iss >> parentName >> childName;
			addEdge(parentName, childName);
		}
		else if (command == "enable") {
			std::string nodeName;
			iss >> nodeName;
			enableNode(nodeName);
		}
		else if (command == "disable") {
			std::string nodeName;
			iss >> nodeName;
			disableNode(nodeName);
		}
		else if (command == "printgraph") {
			printGraph();
		}
		else {
			std::cout << "Invalid command. Please enter one of the following commands:\n";
			std::cout << " - addnode A\n";
			std::cout << " - addedge A B (B relies on A)\n";
			std::cout << " - enable A\n";
			std::cout << " - disable A\n";
			std::cout << " - printgraph\n";

		}
	}

	vector<Node*> Graph::getTopologicalOrder()
	{
		return topological_order_;
	}

	void Graph::toggleNode(Node* node)
	{
		if (node == nullptr) return;
		if (node->enabled)
		{
			disableNode(node->name);
		}
		else
		{
			enableNode(node->name);
		}
	}

	Graph::~Graph()
	{
		for (auto &pair : nodes_)
		{
			delete pair.second;
		}
	}
}
}
