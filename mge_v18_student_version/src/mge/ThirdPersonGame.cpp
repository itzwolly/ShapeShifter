#include <iostream>
#include <string>

#include "mge/materials/CustomMaterial.h"
#include "glm.hpp"
#include "mge/Button.h"

#include "mge/core/Renderer.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/GameObject.hpp"

#include "mge/GameManager.h"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/materials/LitMaterial.h"
#include "mge/materials/ActualMaterial.h"
#include "mge/materials/TextureWithLightMaterial.h"
#include "mge/materials/TerrainMaterial.h"

#include "mge/behaviours/CameraMovement.h"
#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/PlayerBehaviour.h"
#include "mge/behaviours/GateBehaviour.h"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/PhysicsManager.h"
#include "mge/SphereCollider.h"
#include "mge/ThirdPersonGame.h"
#include "mge/MainMenuResolution.h"
#include "mge/MainMenu.h"
#include "mge/Gate.h"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
ThirdPersonGame::ThirdPersonGame(sf::RenderWindow* window, bool pDebug)
	: AbstractGame(window, pDebug), _debug(pDebug)
{ }

void ThirdPersonGame::initialize() {
	GameManager::_inGame = true;
	GameManager::_paused = false;

	//setup the core part
	AbstractGame::initialize();
}

void ThirdPersonGame::Stop()
{
	AbstractGame::Stop();
}

//
////build the game _world
void ThirdPersonGame::_initializeScene() {
	_physManager = new PhysicsManagerFlat(false);

	Mesh* smallButton = Mesh::load(config::MGE_MODEL_PATH + "Button2.obj"); // cube_flat
	Mesh* menuMesh = Mesh::load(config::MGE_MODEL_PATH + "plane20x20_2048tris_aligned_uvs.obj");

	AbstractMaterial* menuMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "IngameFrame.png"));

	GameObject* menu = new GameObject("plane", "Floor", glm::vec3(0, 7, 30),false); //only add buttons as children
	menu->scale(glm::vec3(0.15f, 0.15f, 0.15f));
	menu->rotate(1.256f, glm::vec3(1, 0, 0));
	menu->setMesh(menuMesh);
	menu->setMaterial(menuMaterial);
	_world->add(menu);
	
	Button* button1 = new Button(
		new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Leave1Flip.png")),
		new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Leave2Flip.png")),
		this, 
		new MainMenu(_window, _debug),
		sf::Vector2f(350, 95), sf::Vector2f(470, 720), glm::vec3(0, 5.15f, 35), false, false);

	button1->setMesh(smallButton);
	button1->rotate(2.86f, glm::vec3(1, 0, 0));
	_buttons.push_back(button1);
	button1->scale(glm::vec3(1, 1, 1));
	_physManager->RegisterButton(button1);
	_world->add(button1);

	GameManager::SetMenuData(menu, this, new MainMenuResolution(_window, _debug), _buttons);
	
	std::cout << "== Initializing... Lua Scripting Engine ==" << std::endl << std::endl;
	_script.start();
	std::cout << "== Initialized Lua Scripting Engine ==" << std::endl;

	Light* light = new Light("light", 1, 0, 0, glm::vec3(0, 8, 37), 0, 20, 0, 5, glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(0, -1, 0));
	light->scale(glm::vec3(0.1f, 0.1f, 0.1f));
	_world->registerLight(light);
	_world->add(light);

	Light* light2 = new Light("light", 1, 0, 0, glm::vec3(4, 6, 10), 1, 0, 0, 5, glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(0, -1, 0));
	light2->scale(glm::vec3(0.1f, 0.1f, 0.1f));
	_world->registerLight(light2);
	_world->add(light2);

	DebugHud::Instance().setText("gatesPassed", "", glm::vec2(515, 133), glm::vec2(0, 0), true);
	DebugHud::Instance().setText("highScore", "", glm::vec2(262, 133), glm::vec2(0, 0), true);
	DebugHud::Instance().setText("highestCombo", "", glm::vec2(765, 133), glm::vec2(0, 0), true);
	DebugHud::Instance().setText("timeSpent", "", glm::vec2(1025, 133), glm::vec2(0, 0), true);
}

void ThirdPersonGame::_render() {
	_physManager->Update(_window);
	AbstractGame::_render();
	_updateHud();
}

void ThirdPersonGame::_updateHud() {
	std::string debugInfo = std::string("FPS:") + std::to_string((int)_fps) + "\n";

	DebugHud::Instance().setText("fps", debugInfo, glm::vec2(0, 0), glm::vec2(0, 0));
	DebugHud::Instance().draw();
}

ThirdPersonGame::~ThirdPersonGame()
{
	delete(_physManager);
	//dtor
}
