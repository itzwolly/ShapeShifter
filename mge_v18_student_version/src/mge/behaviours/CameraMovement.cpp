#include "CameraMovement.h"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <algorithm>



CameraMovement::CameraMovement(GameObject* tTarget, float pMoveSpeed, float pTurnSpeed,float angle) : AbstractBehaviour(), _moveSpeed(pMoveSpeed), _turnSpeed(pTurnSpeed)
{
	_target = tTarget;
	_dist = 1;
	_upDownAngle = cos(angle);
	std::cout<<_upDownAngle << " --------------------------- "<<std::endl;
}

CameraMovement::~CameraMovement()
{
}

void CameraMovement::update(float pStep)
{
	glm::vec3 dir = glm::vec3(0, 0, 0);
	float moveSpeed = 0.0f; //default if no keys
	float turnSpeed = 0.0f; 

	glm::vec3 cameraPos = _owner->getWorldPosition();
    glm::vec3 targetPos = _target->getWorldPosition();
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 lookAt = glm::lookAt(cameraPos, targetPos, cameraUp);


	glm::vec3 cameraLook = targetPos-cameraPos;
	cameraLook /= glm::length(cameraLook);
	glm::vec3 tagetY = glm::vec3(0,1,0);

	float dot = cameraLook.x*tagetY.x + cameraLook.y*tagetY.y + cameraLook.z*tagetY.z;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		//if(sqrt(cameraLook.x*cameraLook.x + cameraLook.y*cameraLook.y + cameraLook.z*cameraLook.z) >1)
			_dist --;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		_dist++;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		dir.x = _turnSpeed;
		//turnSpeed = -_turnSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		dir.x = -_turnSpeed;
		//turnSpeed = +_turnSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
		//std::cout << dot << " --------------------------- " << std::endl;
		if (dot<=_upDownAngle /*|| cameraLook.y<0*/)
		dir.y = -_moveSpeed;
		//turnSpeed = +_turnSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
		//std::cout << dot << " --------------------------- " << std::endl;
		if (dot>=-_upDownAngle /*|| cameraLook.y>0*/)
		dir.y = _moveSpeed;
		//turnSpeed = +_turnSpeed;
	}
	glm::mat4 view_matrix = glm::translate(glm::vec3(0, 0, -_dist)) * lookAt;//* glm::translate(-targetPos);
	_owner->setTransform(glm::inverse(view_matrix));
	//translate the object in its own local space
	dir *= pStep;
	_owner->translate(dir);
	_dist = 0;
}
