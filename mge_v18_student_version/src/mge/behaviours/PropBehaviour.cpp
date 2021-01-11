#include "PropBehaviour.h"
#include "mge/core/GameObject.hpp"
#include "mge/Prop.h"
#include "mge/GameManager.h"



PropBehaviour::PropBehaviour(Prop* prop,float minMultiplier, glm::vec3 direction) :AbstractBehaviour(),  _prop(prop)
{
	_direction = glm::normalize(direction);
	_speed = GameManager::GetGateSpeed();
	_multiplier = (minMultiplier + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)));
}


PropBehaviour::~PropBehaviour()
{
}

void PropBehaviour::update(float pStep)
{
	//rotates 45° per second
	_owner->translate(_direction*(*_speed)*pStep*_multiplier);
	/*if (_owner->getWorldPosition().z>_zMarker)
	{
		_owner->getParent()->remove(_owner);
		delete(_owner);
	}*/
/*
	glm::vec3 pos = _owner->getWorldPosition();
	if (pos.z >_zMarker)
	{
		_owner->translate(glm::vec3(0, 0, (_initialPos.z+_excess)));
		_gate->RandColor();
	}*/
}