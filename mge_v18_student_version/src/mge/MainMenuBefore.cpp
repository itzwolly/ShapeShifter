#include <iostream>
#include <string>

#include "mge/materials/CustomMaterial.h"
#include "glm.hpp"

#include "mge/core/Renderer.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/Button.h"

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
#include "mge/PhysicsManagerFlat.h"
//#include "PhysicsManagerFlat.h"
#include "mge/SphereCollider.h"
#include "mge/MainMenuBefore.h"
#include "mge/MainMenu.h"
#include "mge/Gate.h"
#include "mge/util/StatsManager.h"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MainMenuBefore::MainMenuBefore(sf::RenderWindow* window, bool pDebug)
	: AbstractGame(window, pDebug), _hud(0), _debug(pDebug)
{ }

void MainMenuBefore::initialize() {
	//setup the core part
	AbstractGame::initialize();
}

//
////build the game _world
void MainMenuBefore::_initializeScene()
{
	showRectangles = false;
	_physManager = new PhysicsManagerFlat(showRectangles);

	//std::cout << "Initializing HUD" << std::endl;
	//DebugHud::Instance().initialize(_window);
	//std::cout << "HUD initialized." << std::endl << std::endl;

	//Mesh* smallButton = Mesh::load(config::MGE_MODEL_PATH + "Button2.obj");
	Mesh* bigButton = Mesh::load(config::MGE_MODEL_PATH + "Button1.obj");
	Mesh* smallButton = Mesh::load(config::MGE_MODEL_PATH + "Button2.obj"); // cube_flat

	Mesh* planeMesh = Mesh::load(config::MGE_MODEL_PATH + "plane_8192.obj");
	Mesh* skyMesh = Mesh::load(config::MGE_MODEL_PATH + "Sky.obj");

	AbstractMaterial* lightMaterial = new ColorMaterial(glm::vec3(1, 1, 1));
	AbstractMaterial* menuMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "BeforeMenuScreenFrame.png"));
	AbstractMaterial* planeMaterial = new ActualMaterial(Texture::load(config::MGE_TEXTURE_PATH + "custom_material.png"),
		Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "TrackMaterial.jpg"),
		Texture::load(config::MGE_TEXTURE_PATH + "splatmap2.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "normalTestNormalMap.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "glossTestMatNo.jpg"),
		glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 5, 0.1, 1, 0.01f, 1, 1, 1, false, false, 0.3f);

	AbstractMaterial* playerMaterial = new ColorMaterial(glm::vec3(1, 0, 0));
	//AbstractMaterial* planeTextureMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "TrackMaterial.jpg"));
	AbstractMaterial* skyTexture = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Sky.jpg"));
	/**/
	GameObject* menu = new GameObject("plane", "Floor", glm::vec3(0, 0, 1.5f));
	menu->scale(glm::vec3(1, 1, 1));
	menu->rotate(-90, glm::vec3(1, 0, 0));
	menu->setMesh(planeMesh);
	menu->setMaterial(menuMaterial);
	_world->add(menu);
	/**/
	GameObject* sky = new GameObject("sphere", "sky", glm::vec3(0, -25, -45));
	sky->setMesh(skyMesh);
	sky->setMaterial(skyTexture);
	_world->add(sky);
	/**/
	/**

	GameObject* sky = new GameObject("sphere", "sky", glm::vec3(0, 0, -45));
	sky->setMesh(skyMesh);
	plane->rotate(-180, glm::vec3(, 0));
	sky->setMaterial(skyTexture);
	_world->add(sky);
	/**/

	Button* button = new Button(new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH+"Continue1.png")), new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Continue2.png")),
							this,new MainMenu(_window, _debug),sf::Vector2f(400, 85), sf::Vector2f(500, 730),glm::vec3(0,-0.43f, 1.5f));
	//button->setPosition(sf::Vector2f(200, 300));
	button->setMesh(smallButton);
	button->rotate(glm::radians(-27.0f), glm::vec3(1, 0, 0));
	button->scale(glm::vec3(0.35f, 0.35f, 0.35f));
	_world->add(button);
	_buttons.push_back(button);
	_physManager->RegisterButton(button);

	Button* button1 = new Button(
		new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Reset1.png")),
		new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Reset2.png")), 
		this, 
		new MainMenu(_window, _debug),
		sf::Vector2f(400, 85),
		sf::Vector2f(500, 840),
		glm::vec3(0, -0.58f, 1.5f));

	button1->setMesh(smallButton);
	button1->rotate(glm::radians(-27.0f), glm::vec3(1, 0, 0));
	button1->scale(glm::vec3(0.35f, 0.35f, 0.35f));
	_world->add(button1);
	_buttons.push_back(button1);
	_physManager->RegisterButton(button1);

	Camera* camera = new Camera("camera", "MainCamera", glm::vec3(0,-0.6f, 0));
	camera->rotate(glm::radians(-180.0f), glm::vec3(0, 1, 0));
	camera->rotate(glm::radians(27.0f), glm::vec3(1, 0, 0));
	_world->add(camera);
	_world->setMainCamera(camera);

	Light* light = new Light("light", 1, 0, 0, glm::vec3(0,5,1), 0, 10, 0, 5, glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(0, -1, 0));
	light->scale(glm::vec3(0.1f, 0.1f, 0.1f));
	_world->add(light);
	
	DebugHud::Instance().initializeText("gatesPassed", 24, glm::vec4(1, 1, 1, 1));
	DebugHud::Instance().initializeText("highScore", 24, glm::vec4(1, 1, 1, 1));
	DebugHud::Instance().initializeText("highestCombo", 24, glm::vec4(1, 1, 1, 1));
	DebugHud::Instance().initializeText("timeSpent", 24, glm::vec4(1, 1, 1, 1));

	/*
		UI.setText("feedback", "", 0, 0, 0, 0, false)
		UI.setText("score", "", 0, 0, 0, 0, false)
		UI.setText("combo", "", 0, 0, 0, 0, false)
	*/

	_gatePassed = std::stoi((StatsManager::GetFileValue("stats.ini", "gatesPassed")));
	_highScore = std::stoi((StatsManager::GetFileValue("stats.ini", "highScore")));
	_highestCombo = std::stoi((StatsManager::GetFileValue("stats.ini", "highestCombo")));

	int time = std::stoi((StatsManager::GetFileValue("stats.ini", "timeSpent")));
	_minutes = (time % 3600) / 60;
	_seconds = (time % 3600) % 60;
}

void MainMenuBefore::_render() {
	_physManager->Update(_window);
	if (!showRectangles) {
		AbstractGame::_render();
		_updateHud();
	}
}

void MainMenuBefore::_updateHud() {
	std::string debugInfo = std::string("FPS:") + std::to_string((int)_fps) + "\n";

	DebugHud::Instance().setText("fps", debugInfo, glm::vec2(0, 0), glm::vec2(0, 0));
	DebugHud::Instance().setText("gatesPassed", std::to_string(_gatePassed), glm::vec2(515, 133), glm::vec2(0, 0), true);
	DebugHud::Instance().setText("highScore", std::to_string(_highScore), glm::vec2(262, 133), glm::vec2(0, 0), true);
	DebugHud::Instance().setText("highestCombo", std::to_string(_highestCombo), glm::vec2(765, 133), glm::vec2(0, 0), true);
	DebugHud::Instance().setText("timeSpent", std::to_string(_minutes) + ":" + ((_seconds >= 10) ? std::to_string(_seconds) : "0" + std::to_string(_seconds)), glm::vec2(1025, 133), glm::vec2(0, 0), true);
	DebugHud::Instance().draw();
}

MainMenuBefore::~MainMenuBefore()
{
	//dtor
}
