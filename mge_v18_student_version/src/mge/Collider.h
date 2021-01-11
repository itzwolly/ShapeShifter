#pragma once
#include <iostream>
#include "glm.hpp"
class GameObject;
class Collider
{
public:
	Collider(GameObject* owner, glm::vec3 offset= glm::vec3(0,0,0));
	~Collider();
	std::string GetType();
	GameObject* GetOwner();
	glm::vec3 GetPosition();
protected:
	GameObject* _owner;
	std::string _type;
	glm::vec3 _offset;
};

