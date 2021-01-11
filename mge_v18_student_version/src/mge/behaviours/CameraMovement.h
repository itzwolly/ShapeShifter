#ifndef CAMERAMOVEMENT_HPP
#define CAMERAMOVEMENT_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"

class CameraMovement : public AbstractBehaviour
{
public:
	//move speed is in units per second, turnspeed in degrees per second
	CameraMovement(GameObject* tTarget, float pMoveSpeed = 1, float pTurnSpeed = 45,float angle = 45);
	virtual ~CameraMovement();
	virtual void update(float pStep);

private:
	float _moveSpeed;
	float _turnSpeed;
	GameObject* _target;
	float _dist;
	float _upDownAngle;
};

#endif // KEYSBEHAVIOUR_HPP

