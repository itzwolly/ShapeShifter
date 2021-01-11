#include "mge/core/AbstractGame.hpp"
#include "mge/PhysicsManagerFlat.h"

class DebugHud;
/**
* An example subclass of AbstractGame showing how we can setup a scene.
*/
class ThirdPersonGame : public AbstractGame
{
	//PUBLIC FUNCTIONS

public:
	ThirdPersonGame(sf::RenderWindow* window, bool pDebug);
	virtual ~ThirdPersonGame();

	//override initialize so we can add a DebugHud
	virtual void initialize();
	virtual void Stop();

protected:
	//override so we can construct the actual scene
	virtual void _initializeScene();

	//override render to render the hud as well.
	virtual void _render();

	int* _multiplier;
	int* _lives;

private:
	bool _debug;
	void _updateHud();


	PhysicsManagerFlat* _physManager;
	std::vector<Button*> _buttons;

	ThirdPersonGame(const ThirdPersonGame&);
	ThirdPersonGame& operator=(const ThirdPersonGame&);
};

