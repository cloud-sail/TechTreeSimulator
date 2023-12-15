#pragma once
#include <memory>
#include <SFML/Graphics.hpp>

#include "../model/World.h"

namespace rm
{
namespace view
{
	using std::shared_ptr;

	class Screen
	{
	private:
		shared_ptr<model::World> world_;
		shared_ptr<sf::RenderWindow> window_;

		sf::Font font_;

		sf::View game_view_;
		sf::View hud_view_;

		sf::Text desc_;
		sf::Text user_input_;
		sf::Text node_name_;

		sf::CircleShape node_shape_;

		// DAG parameter
		const float radius = 15.f;
		const float nodeStartX = 50.f;
		const float nodeStartY = 300.f;
		const float interval = 150.f;
		const float degree = 40.f * 3.14159265359f / 180.f;
		const float startDegree = (270.f - 20.f) * 3.14159265359f / 180.f;

		
		void initFonts();
		void initView();
		void initText();
		void initCircle();

		void drawDAG();

	public:
		Screen(const shared_ptr<sf::RenderWindow>& window, const shared_ptr<model::World>& world);

		void moveView(float delta);
		void setUserInput(std::string text);
		model::Node* getNodeAtMousePosition();

		void draw();
	};

}
}
