#ifndef POPTEXTBEHAVIOUR_HPP
#define POPTEXTBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include <glm.hpp>

/**
 * Simply rotates the object around its origin with a fixed speed.
 */
class PopTextBehaviour : public AbstractBehaviour
{
	public:
		PopTextBehaviour(float time = 1, float speed = 1, glm::vec3 direction = glm::vec3(0, 1, 0));
		virtual ~PopTextBehaviour();

		virtual void update(float pStep);
	private:

		float _time;
		float _speed;
		glm::vec3 _direction;

};

#endif 
