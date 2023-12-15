#include "Game.h"
#include <fstream>

namespace rm
{
namespace controller
{
	const std::string resourceFile("resource.txt");

	void Game::readResource()
	{
		std::ifstream inputFile(resourceFile);
		if (!inputFile.is_open())
		{
			throw std::runtime_error("Cannot open resource.txt for reading");
		}
		std::string line;
		while (getline(inputFile, line)) {
			std::stringstream ss(line);
			std::string childName, parentName;

			ss >> childName >> parentName;
			world_->getGraph()->addNode(childName);
			world_->getGraph()->addNode(parentName);
			world_->getGraph()->addEdge(parentName, childName);
		}
		inputFile.close();
	}

	void Game::writeResource()
	{
		std::ofstream outputFile(resourceFile);
		if (!outputFile.is_open())
		{
			throw std::runtime_error("Cannot open resource.txt for writing");
		}
		std::vector<model::Node*> nodeList = world_->getGraph()->getTopologicalOrder();
		for (const auto& node : nodeList) {
			if (node->children.empty()) {
				// If node has no children, print this node only
				outputFile << node->name << std::endl;
			}
			else {
				// If node has children, print edge (child parent)
				for (const auto& child : node->children) {
					outputFile << child->name << " " << node->name << std::endl;
				}
			}
		}
	}

	void Game::handleEvent(sf::Event& event)
	{
		switch (event.type) {
		case sf::Event::Closed:
			handleCloseEvent(event);
			break;
		case sf::Event::MouseWheelScrolled:
			handleMouseWheelScrolledEvent(event);
			break;
		case sf::Event::TextEntered:
			handleTextEnteredEvent(event);
			break;
		case sf::Event::MouseButtonPressed:
			handleMouseButtonPressedEvent(event);
			break;
		default:
			break;
		}
	}

	void Game::handleCloseEvent(sf::Event& event)
	{
		window_->close();
	}

	void Game::handleMouseWheelScrolledEvent(sf::Event& event)
	{
		if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
			float deltaMove = event.mouseWheelScroll.delta > 0 ? -50.f : 50.f;
			screen_->moveView(deltaMove);
		}
	}

	void Game::handleTextEnteredEvent(sf::Event& event)
	{
		if (event.text.unicode < 128) {
			char inputChar = static_cast<char>(event.text.unicode);
			if (std::isdigit(inputChar) || std::isalpha(inputChar) || std::isspace(inputChar)) {
				if (inputChar == '\r') {
					std::cout << "User input: " << user_input_ << std::endl;
					world_->getGraph()->processUserInput(user_input_);
					user_input_.clear();
				}
				else {
					user_input_ += inputChar;
				}
				screen_->setUserInput(user_input_);
			}
		}
	}

	void Game::handleMouseButtonPressedEvent(sf::Event& event)
	{
		model::Node* choosenNode = screen_->getNodeAtMousePosition();
		if (choosenNode != nullptr)
		{
			world_->getGraph()->toggleNode(choosenNode);
		}
		else
		{
			std::cout << "None of nodes are clicked." << std::endl;
		}
	}

	Game::Game()
	  : window_(std::make_shared<sf::RenderWindow>(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tech Tree Simulator", sf::Style::Titlebar | sf::Style::Close)),
		world_(std::make_shared<model::World>()),
		screen_(std::make_shared<view::Screen>(window_, world_))
	{
		readResource();
	}

	Game::~Game()
	{
		writeResource();
	}

	void Game::run()
	{
		while (window_->isOpen())
		{
			sf::Event event;
			while (window_->pollEvent(event))
			{
				handleEvent(event);
			}
			window_->clear(sf::Color::White);
			screen_->draw();
			window_->display();
		}
	}
}
}


