#include "GateBehaviour.h"
#include "mge/core/GameObject.hpp"
#include "mge/Gate.h"
#include "mge/GameManager.h"



GateBehaviour::GateBehaviour(Gate* gate, glm::vec3 direction) :AbstractBehaviour(),  _gate(gate)
{
	_direction = glm::normalize(direction);
	_speed = GameManager::GetGateSpeed();
}


GateBehaviour::~GateBehaviour()
{
}

void GateBehaviour::update(float pStep)
{
	//rotates 45° per second
	if (!GameManager::_paused) {
		_owner->translate(_direction*(*_speed) * pStep);
	}

	/*
	glm::vec3 pos = _owner->getWorldPosition();
	if (pos.z >_zMarker)
	{
		_owner->translate(glm::vec3(0, 0, (_initialPos.z+_excess)));
		_gate->RandColor();
	}*/
}