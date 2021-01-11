#include "Gate.h"
#include "mge/PhysicsManager.h"



Gate::Gate(const std::string& pName, const std::string& pTag, const glm::vec3 & pPosition)
	: GameObject(pName, pTag, pPosition)
{
	PhysicsManager::RegisterObject(this);
}

Gate::~Gate() { }
