#pragma once
#include "Collider.h"
#include "glm.hpp"
class SphereCollider :
	public Collider
{
public:
	SphereCollider(GameObject* owner, float radius, glm::vec3 offset = glm::vec3(0, 0, 0));
	~SphereCollider();
	float GetRadius();
protected:
	float _radius;
	glm::vec3 _offset;
};

