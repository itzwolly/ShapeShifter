#include "mge/core/AbstractGame.hpp"

class DebugHud;
/**
* An example subclass of AbstractGame showing how we can setup a scene.
*/
class TestScene : public AbstractGame
{
	//PUBLIC FUNCTIONS

public:
	TestScene(sf::RenderWindow* window, bool pDebug);
	virtual ~TestScene();

	//override initialize so we can add a DebugHud
	virtual void initialize();

protected:
	//override so we can construct the actual scene
	virtual void _initializeScene();

	//override render to render the hud as well.
	virtual void _render();

	int* _multiplier;
	int* _lives;

private:
	std::vector<GameObject*> temps;
	DebugHud* _hud;                   //hud display
	void _updateHud();

	TestScene(const TestScene&);
	TestScene& operator=(const TestScene&);
};

