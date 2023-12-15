#include "Screen.h"

namespace rm
{
namespace view
{
	void Screen::initFonts()
	{
		if (!font_.loadFromFile("Fonts/arial.ttf"))
		{
			throw std::runtime_error("Font file not found.");
		}
	}

	void Screen::initView()
	{
		window_->setFramerateLimit(60);
		game_view_ = window_->getDefaultView();
		hud_view_ = window_->getDefaultView();
	}

	void Screen::initText()
	{
		desc_.setFont(font_);
		desc_.setCharacterSize(24);
		desc_.setFillColor(sf::Color::Blue);
		desc_.setString(" Nodes in topological order. Please try to click on the node or scroll the mouse wheel  \n Or type command and return: addnode A / addedge A B(B relies on A)/ enable A / disable A / printgraph");

		user_input_.setFont(font_);
		user_input_.setCharacterSize(24);
		user_input_.setFillColor(sf::Color::Blue);
		user_input_.setPosition(0.f, static_cast<float>(window_->getSize().y) - 50.f);

		node_name_.setFont(font_);
		node_name_.setCharacterSize(18);
		node_name_.setFillColor(sf::Color::Black);
	}

	void Screen::initCircle()
	{
		node_shape_.setRadius(radius);
		node_shape_.setOrigin(node_shape_.getRadius(), node_shape_.getRadius());
	}

	void Screen::drawDAG()
	{
		std::vector<model::Node*> nodeList = world_->getGraph()->getTopologicalOrder();
		sf::Vector2f position(nodeStartX, nodeStartY);

		std::unordered_map<model::Node*, size_t> nodeIndexMap;
		for (size_t i = 0; i < nodeList.size(); ++i) {
			nodeIndexMap[nodeList[i]] = i;
		}

		for (const auto& node : nodeList)
		{
			node_shape_.setPosition(position);
			node_shape_.setFillColor(node->enabled ? sf::Color::Blue : sf::Color::Red);
			window_->draw(node_shape_);

			node_name_.setString(node->name);
			sf::FloatRect textBounds = node_name_.getLocalBounds();
			node_name_.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
			node_name_.setPosition(position.x, position.y + node_shape_.getRadius() * 2.f);
			window_->draw(node_name_);

			for (const auto& child : node->children) {
				size_t nodeIndex = nodeIndexMap[node];
				size_t childIndex = nodeIndexMap[child];
				sf::Vector2f childPosition = sf::Vector2f(position.x + static_cast<float>(childIndex - nodeIndex) * interval, position.y);

				// Draw arcs between nodes
				sf::VertexArray arc(sf::PrimitiveType::LineStrip, 50);
				sf::Vector2f diff = childPosition - position;
				float diffX = childPosition.x - position.x;
				float sumX = childPosition.x + position.x;
				float radius = diffX / 2.f / std::sin(degree/2.f);
				sf::Vector2f center(sumX/2.f, position.y + diffX / 2.f / std::tan(degree/2.f));

				for (size_t i = 0; i < 50; ++i) {
					float x = center.x + radius * cos(startDegree + i * degree / 49.f);
					float y = center.y + radius * sin(startDegree + i * degree / 49.f);
					arc[i].position = sf::Vector2f(x, y);
					arc[i].color = node->enabled ? sf::Color::Blue : sf::Color::Red;
				}
				window_->draw(arc);
			}


			position.x += interval;
		}
	}

	Screen::Screen(const shared_ptr<sf::RenderWindow>& window, const shared_ptr<model::World>& world)
		:world_(world),window_(window)
	{
		initFonts();
		initView();
		initText();
		initCircle();
	}

	void Screen::moveView(float delta)
	{
		game_view_.move(delta, 0.f);
	}

	void Screen::setUserInput(std::string text)
	{
		user_input_.setString(text);
	}

	model::Node* Screen::getNodeAtMousePosition()
	{
		sf::Vector2f worldPos = window_->mapPixelToCoords(sf::Mouse::getPosition(*window_), game_view_);
		std::vector<model::Node*> nodeList = world_->getGraph()->getTopologicalOrder();
		for (size_t i = 0; i < nodeList.size(); ++i)
		{
			float nodeCenterX = nodeStartX + interval * i;
			float nodeCenterY = nodeStartY;
			float distance = std::sqrt(std::pow(worldPos.x - nodeCenterX, 2) + std::pow(worldPos.y - nodeCenterY, 2));
			if (distance <= radius) {
				std::cout << "Node Clicked: " << nodeList[i]->name << std::endl;
				return nodeList[i];
			}
		}
		return nullptr;
	}

	void Screen::draw()
	{
		// game view
		window_->setView(game_view_);
		drawDAG();

		// hud view
		window_->setView(hud_view_);
		window_->draw(user_input_);
		window_->draw(desc_);
		
	}
}
}
