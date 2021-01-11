#include "mge/core/AbstractGame.hpp"

class DebugHud;
class PhysicsManagerFlat;
class Button;
/**
* An example subclass of AbstractGame showing how we can setup a scene.
*/
class MainMenuBefore : public AbstractGame
{
	//PUBLIC FUNCTIONS

public:
	MainMenuBefore(sf::RenderWindow* window, bool pDebug);
	virtual ~MainMenuBefore();

	//override initialize so we can add a DebugHud
	virtual void initialize();

protected:
	PhysicsManagerFlat* _physManager;
	//override so we can construct the actual scene
	virtual void _initializeScene();

	//override render to render the hud as well.
	virtual void _render();

	int* _multiplier;
	int* _lives;

private:
	DebugHud* _hud;                   //hud display
	bool _debug;
	bool showRectangles;
	void _updateHud();
	int _gatePassed;
	int _highScore;
	int _highestCombo;
	int _minutes;
	int _seconds;
	std::vector<Button*> _buttons;
	std::vector<GameObject*> _props;
	MainMenuBefore(const MainMenuBefore&);
	MainMenuBefore& operator=(const MainMenuBefore&);
};

