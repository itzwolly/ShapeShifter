#include <iostream>

#include "mge/core/AbstractGame.hpp"
#include "mge/ThirdPersonGame.h"
#include "mge/TestScene.h"
#include "mge/MainMenu.h"
#include "mge/MainMenuBefore.h"
#include "mge/MainMenuResolution.h"
#include "mge/MGEDemo.hpp"
#include "mge/util/StatsManager.h"

int main(int pArgc, char* pArgv[]) {
	srand(time(NULL));
	bool debug = pArgc > 1 && strcmp(pArgv[1], "-debug") == 0;
	std::cout << "Starting Game" << std::endl;

	sf::RenderWindow* _window = new sf::RenderWindow(sf::VideoMode(AbstractGame::windowSize->x, AbstractGame::windowSize->y), "SDFU Arena!", sf::Style::Default, sf::ContextSettings(24, 8, 8, 3, 3));
	AbstractGame* game = new MainMenu(_window, debug);

    game->initialize();
	game->run();
	//delete game;
    return 0;
}