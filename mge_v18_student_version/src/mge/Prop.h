#pragma once
#include "mge\core\GameObject.hpp"
class Prop :
	public GameObject
{
public:
	Prop(const std::string& pName = nullptr, const glm::vec3& pPosition = glm::vec3(0.0f, 0.0f, 0.0f),bool visible=true);
	~Prop();
};

