#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "SFML\Graphics.hpp"

RotatingBehaviour::RotatingBehaviour(float speed):AbstractBehaviour(),_speed(speed)
{
	//ctor
	//rotation = 0;
}

RotatingBehaviour::~RotatingBehaviour()
{
	//dtor
}

void RotatingBehaviour::update(float pStep)
{
	_owner->rotate(glm::radians(_speed*pStep), glm::vec3(0.0f, 1.0f, 0.0f));
    //rotates 45° per second
	/*if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		rotation += pStep* glm::radians(45.0f);
		_owner->rotate(pStep * glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		std::cout << rotation << "\n";
	}*/
}
