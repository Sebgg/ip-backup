#include <SFML/Graphics.hpp>
#include <vector>
#include "GameEngine.h"

using namespace std;

const size_t width = 928;
const size_t height = 793;

/** GameEngine constructor implementation.
*/
GameEngine::GameEngine(){}

/** Implementation of GameEngines run function. We create a new PlayState*
 *  called current_game. We then use check_win and check_loss to check if
 *  the game is won or lost every frame. If it is either we call display_win
 *  or display_loss. If the game is not won or lost we call for update_screen
 *  and update_gravity and lastly we draw the screen with draw_screen.
 */
int GameEngine::run() {
	sf::RenderWindow window{sf::VideoMode{width, height}, "hehexd"};
	window.setVerticalSyncEnabled(true);

	PlayState* current_game{new PlayState{1}};

	bool quit = false;
	while (!quit) {
		while (current_game->check_win())
		{
			current_game->display_win(window);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			{
				delete current_game;
				return 0;
			}
		}
		while (current_game->check_loss())
		{
			current_game->display_loss(window);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			{
				delete current_game;
				return 0;
			}
		}
		current_game->update_screen();
		current_game->update_gravity(725.f);

		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Q)
				{
					quit = true;
				}
				break;
			default:
				break;
			}
		}

		current_game->draw_screen(window);
		window.display();
	}
	delete current_game;
	return 0;
}
