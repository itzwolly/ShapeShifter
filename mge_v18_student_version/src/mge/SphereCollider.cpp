#include "SphereCollider.h"



SphereCollider::SphereCollider(GameObject* owner,float radius,glm::vec3 offset):Collider(owner, offset),_radius(radius)
{
	_type = std::string("sphere");
}


SphereCollider::~SphereCollider()
{
}

float SphereCollider::GetRadius()
{
	return _radius;
}
