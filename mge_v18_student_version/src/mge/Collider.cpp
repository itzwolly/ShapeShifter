#include "Collider.h"

#include "mge\core\GameObject.hpp"


Collider::Collider(GameObject* owner, glm::vec3 offset) :_owner(owner), _offset(offset)
{
}


Collider::~Collider()
{
}

std::string Collider::GetType()
{
	return _type;
}

GameObject * Collider::GetOwner()
{
	return _owner;
}

glm::vec3 Collider::GetPosition()
{
	return _owner->getWorldPosition() + _offset;
}
