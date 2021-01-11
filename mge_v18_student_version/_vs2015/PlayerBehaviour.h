#ifndef PLAYERBEHAVIOUR_HPP
#define PLAYERBEHAVIOUR_HPP

#include "mge\behaviours\AbstractBehaviour.hpp"

class PlayerBehaviour : public AbstractBehaviour {
	public:
		PlayerBehaviour();
		virtual ~PlayerBehaviour();

		virtual void update(float pStep);
};

#endif // PLAYERBEHAVIOUR_HPP