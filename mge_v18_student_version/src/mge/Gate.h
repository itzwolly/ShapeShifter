#pragma once
#include "mge\core\GameObject.hpp"
#include "mge/materials/ColorMaterial.hpp"
class Gate :
	public GameObject
{
public:
	Gate(const std::string& pName = nullptr, const std::string& pTag = nullptr, const glm::vec3& pPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	~Gate();
private:
};

