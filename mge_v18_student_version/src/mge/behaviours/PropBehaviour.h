#pragma once
#include "mge\behaviours\AbstractBehaviour.hpp"
#include "glm.hpp"
class Prop;
class PropBehaviour :
	public AbstractBehaviour
{
public:
	PropBehaviour(Prop* gate, float minMultiplier,glm::vec3 movement);
	virtual ~PropBehaviour();

	virtual void update(float pStep);
private:
	Prop* _prop;
	float _multiplier;
	glm::vec3 _direction;
	float * _speed;
};

