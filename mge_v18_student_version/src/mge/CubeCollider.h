#pragma once
#include "Collider.h"
class CubeCollider :
	public Collider
{
public:
	CubeCollider(GameObject* owner);
	~CubeCollider();
};

