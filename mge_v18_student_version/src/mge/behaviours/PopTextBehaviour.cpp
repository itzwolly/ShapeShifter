#include "mge/behaviours/PopTextBehaviour.hpp"
#include "mge/core/GameObject.hpp"

PopTextBehaviour::PopTextBehaviour(float time, float speed, glm::vec3 direction):AbstractBehaviour(), _time(time), _speed(speed), _direction(direction)
{
	//ctor
}

PopTextBehaviour::~PopTextBehaviour()
{
	//dtor
}

void PopTextBehaviour::update(float pStep)
{
    //rotates 45° per second
	_owner->rotate(pStep * glm::radians(45.0f), glm::vec3( 1.0f, 1.0f, 0.0f ) );
}
