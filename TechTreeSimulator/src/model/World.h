#pragma once
#include <memory>

#include "Graph.h"

namespace rm
{
namespace model
{
	using std::shared_ptr;
	using std::string;
	class World
	{
	private:
		shared_ptr<Graph> graph_;

	public:
		World();

		shared_ptr<Graph> getGraph() { return graph_; }
	};
}
}


