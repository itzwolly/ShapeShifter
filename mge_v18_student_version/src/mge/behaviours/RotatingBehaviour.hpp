#ifndef ROTATINGBEHAVIOUR_HPP
#define ROTATINGBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"

/**
 * Simply rotates the object around its origin with a fixed speed.
 */
class RotatingBehaviour : public AbstractBehaviour
{
	public:
		RotatingBehaviour(float speed);
		virtual ~RotatingBehaviour();

		virtual void update(float pStep);
	private:
		float _rotation;
		float _speed;
};

#endif // ROTATINGBEHAVIOUR_HPP
