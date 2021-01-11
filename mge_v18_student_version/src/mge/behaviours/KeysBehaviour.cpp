#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "mge/core/Light.hpp"

KeysBehaviour::KeysBehaviour(GameObject* obj,float pMoveSpeed, float pTurnSpeed):_obj(obj), AbstractBehaviour(), _moveSpeed(pMoveSpeed), _turnSpeed(pTurnSpeed)
{
	_pressed = false;
}

KeysBehaviour::~KeysBehaviour()
{
}

void KeysBehaviour::update( float pStep )
{
	float moveSpeed = 0.0f; //default if no keys
	float turnSpeed = 0.0f;
	float upSpeed = 0.0f;
	float sideSpeed = 0.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		upSpeed = _moveSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		upSpeed = -_moveSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
		moveSpeed = _moveSpeed;
	}/*
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
		std::cout << _owner->getWorldPosition();
	}*/
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::K )) {
		moveSpeed = -_moveSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
		turnSpeed = -_turnSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
		turnSpeed = +_turnSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
		sideSpeed = -_moveSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
		sideSpeed = +_moveSpeed;
	}
	/*if (!_pressed&&sf::Keyboard::isKeyPressed(sf::Keyboard::Quote)) {
		_pressed = true;
		_light->SwitchType();
	}
	if (_pressed&&sf::Keyboard::isKeyPressed(sf::Keyboard::BackSlash)) {
		_pressed = false;
	}*/
	//translate the object in its own local space
	_owner->translate( glm::vec3(sideSpeed*pStep, upSpeed*pStep, moveSpeed*pStep ) );

	//we can also translate directly, basically we take the z axis from the matrix
	//which is normalized and multiply it by moveSpeed*step, then we add it to the
	//translation component
	//glm::mat4 transform = _owner->getTransform();
	//transform[3] += transform[2] * moveSpeed*pStep;
	//_owner->setTransform(transform);

	//rotate the object in its own local space
	_owner->rotate( glm::radians(turnSpeed*pStep), glm::vec3(0.0f, 1.0f, 0.0f ) );

	//NOTE:
	//The reason the above happens in the local space of the object and not in the world space
	//is that we use the _owner->translate / rotate shortcuts which in turn call glm::rotate / glm::translate
	//The methods multiply the current transform with the translation/rotation matrix from left to right
	//meaning everything happens in local space.
}
