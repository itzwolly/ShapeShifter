#include "mge\core\GameObject.hpp"
#include "PlayerBehaviour.h"
#include "SFML\Window\Keyboard.hpp"

PlayerBehaviour::PlayerBehaviour() { }

void PlayerBehaviour::update(float pStep) {

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		_owner->translate(glm::vec3(1, 0, 0));
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		_owner->translate(glm::vec3(-1, 0, 0));
	}

}


PlayerBehaviour::~PlayerBehaviour() { }