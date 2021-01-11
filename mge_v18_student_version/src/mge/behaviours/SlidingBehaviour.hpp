#ifndef SLIDINGBEHAVIOUR_HPP
#define SLIDINGBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "glm.hpp"
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

using namespace std::chrono;

/**
 * Simply rotates the object around its origin with a fixed speed.
 */
class SlidingBehaviour : public AbstractBehaviour
{
	public:
		SlidingBehaviour(glm::mat4x4 rot, float dirDuration,glm::vec3 refPos,float distLeft, float distRight, float distBack, float distForwards, float distDown, float distUp,
						float speed, bool rotate = false, float rotSpeed = 45.0f,glm::vec3 angle = glm::vec3(0,1,0));
		virtual ~SlidingBehaviour();

		virtual void update(float pStep);
	private:
		high_resolution_clock::time_point t1;
		high_resolution_clock::time_point t2;

		glm::vec3 _initialPos;
		float _distLeft;
		float _distRight;
		float _distBack;
		float _distForwards;
		float _distUp;
		float _distDown;
		float _speed;
		bool _rotate;
		float _rotSpeed;
		glm::vec3 _angle;
		glm::vec3 _dir;
		glm::vec3 _nextDir;
		float _dirDuration;
		glm::vec3 lerp(glm::vec3 start,glm::vec3 end,float val);
};

#endif // ROTATINGBEHAVIOUR_HPP
