#pragma once
#include <memory>

#include <SFML/Graphics.hpp>

#include "../model/World.h"
#include "../view/Screen.h"

namespace rm
{
namespace controller
{
	using std::shared_ptr;

	class Game
	{
	private:
		static const int WINDOW_WIDTH = 1600;
		static const int WINDOW_HEIGHT = 400;

	private:
		shared_ptr<sf::RenderWindow> window_;
		shared_ptr<model::World> world_;
		shared_ptr<view::Screen> screen_;

		std::string user_input_;

		void readResource();
		void writeResource();

		void handleEvent(sf::Event& event);
		void handleCloseEvent(sf::Event& event);
		void handleMouseWheelScrolledEvent(sf::Event& event);
		void handleTextEnteredEvent(sf::Event& event);
		void handleMouseButtonPressedEvent(sf::Event& event);

	public:
		Game();
		~Game();

		void run();
	};

}
}
