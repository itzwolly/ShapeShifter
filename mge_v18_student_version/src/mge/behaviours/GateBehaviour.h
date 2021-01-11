#pragma once
#include "mge\behaviours\AbstractBehaviour.hpp"
#include "glm.hpp"
class Gate;
class GateBehaviour :
	public AbstractBehaviour
{
public:
	GateBehaviour(Gate* gate,glm::vec3 movement);
	virtual ~GateBehaviour();

	virtual void update(float pStep);
private:
	Gate* _gate;
	glm::vec3 _direction;
	float * _speed;
};

