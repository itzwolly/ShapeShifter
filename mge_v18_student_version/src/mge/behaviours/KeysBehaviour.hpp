#ifndef KEYSBEHAVIOUR_HPP
#define KEYSBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/core/Light.hpp"
#include <list>
/**
 * KeysBehaviour allows you to move an object using the keyboard in its own local space.
 * Left right turns, forward back moves.
 */
class KeysBehaviour : public AbstractBehaviour
{
	public:
	    //move speed is in units per second, turnspeed in degrees per second
		KeysBehaviour(GameObject* obj,float pMoveSpeed = 5, float pTurnSpeed = 45);
		virtual ~KeysBehaviour();
		virtual void update( float pStep );

    private:
		bool _pressed;
        float _moveSpeed;
        float _turnSpeed;
		GameObject* _obj;
};

#endif // KEYSBEHAVIOUR_HPP
