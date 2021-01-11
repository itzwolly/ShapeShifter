#include "CubeCollider.h"



CubeCollider::CubeCollider(GameObject* owner):Collider(owner)
{
	_type = std::string("cube");
}


CubeCollider::~CubeCollider()
{
}
