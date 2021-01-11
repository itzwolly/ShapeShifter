#include <iostream>
#include <string>

#include "mge\materials\CustomMaterial.h"
#include "glm.hpp"

#include "mge/core/Renderer.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/GameObject.hpp"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/materials/LitMaterial.h"
#include "mge/materials/TerrainMaterial.h"

#include "mge/behaviours/CameraMovement.h"
#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/PlayerBehaviour.h"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "ThirdPersonGame.h"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
ThirdPersonGame::ThirdPersonGame() :AbstractGame(), _hud(0)
{
}

void ThirdPersonGame::initialize() {
	//setup the core part
	AbstractGame::initialize();

	//setup the custom part so we can display some text
	std::cout << "Initializing HUD" << std::endl;
	_hud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl << std::endl;
}

//build the game _world
void ThirdPersonGame::_initializeScene()
{
	Mesh* planeMesh = Mesh::load(config::MGE_MODEL_PATH + "plane_8192.obj");
	Mesh* playerMesh = Mesh::load(config::MGE_MODEL_PATH + "cube_flat.obj");

	AbstractMaterial* planeMaterial = new ColorMaterial(glm::vec3(1, 1, 1));
	AbstractMaterial* playerMaterial = new ColorMaterial(glm::vec3(0.85, 0, 0));

	GameObject* plane = new GameObject("plane", glm::vec3(0, 0, 0));
	plane->scale(glm::vec3(6, 1, 27));
	plane->setMesh(planeMesh);
	plane->setMaterial(planeMaterial);
	_world->add(plane);

	GameObject* player = new GameObject("player", glm::vec3(0, 1, 23));
	player->scale(glm::vec3(1, 1, 1));
	player->setMesh(playerMesh);
	player->setMaterial(playerMaterial);
	player->setBehaviour(new PlayerBehaviour(plane));
	_world->add(player);

	//add camera first (it will be updated last)
	Camera* camera = new Camera("camera", glm::vec3(0, 8, 32));
	camera->rotate(glm::radians(-20.0f), glm::vec3(1, 0, 0));
	_world->add(camera);
	_world->setMainCamera(camera);
}

void ThirdPersonGame::_render() {
	AbstractGame::_render();
	_updateHud();
}

void ThirdPersonGame::_updateHud() {
	std::string debugInfo = "";
	debugInfo += std::string("FPS:") + std::to_string((int)_fps) + "\n";

	Light* l;
	for (int i = 0; i<_world->getLightCount() - 1; i++)
	{
		l = _world->getLightAt(i);
		debugInfo += l->getName() + std::string(" - ") + std::to_string(l->GetType()) + "\n";
	}

	_hud->setDebugInfo(debugInfo);
	_hud->draw();
}

ThirdPersonGame::~ThirdPersonGame()
{
	//dtor
}
