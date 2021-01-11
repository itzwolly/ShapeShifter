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
#include "mge/SphereCollider.h"
#include "mge/MainMenuResolution.h"
#include "mge/ThirdPersonGame.h"
#include "mge/MainMenu.h"
#include "mge/Gate.h"
#include "mge/util/StatsManager.h"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MainMenuResolution::MainMenuResolution(sf::RenderWindow* window, bool pDebug)
	: AbstractGame(window, pDebug), _hud(0), _debug(pDebug)
{ }

void MainMenuResolution::initialize() {
	GameManager::_inGame = false;
	//setup the core part
	AbstractGame::initialize();
}

//
////build the game _world
void MainMenuResolution::_initializeScene()
{
	showRectangles = false;
	_physManager = new PhysicsManagerFlat(showRectangles);

	Mesh* planeMesh = Mesh::load(config::MGE_MODEL_PATH + "planeNew.obj");
	Mesh* smallButton = Mesh::load(config::MGE_MODEL_PATH + "Button2.obj"); // cube_flat

	Mesh* menuMesh = Mesh::load(config::MGE_MODEL_PATH + "plane_8192.obj");
	Mesh* cubeMesh = Mesh::load(config::MGE_MODEL_PATH + "Cube_player.obj"); // cube_flat

	Mesh* skyMesh = Mesh::load(config::MGE_MODEL_PATH + "Sky.obj");

	AbstractMaterial* lightMaterial = new ColorMaterial(glm::vec3(1, 1, 1));
	AbstractMaterial* testMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "MainMenuFrame.png"));
	/* AbstractMaterial* planeMaterial = new ActualMaterial(
		Texture::load(config::MGE_TEXTURE_PATH + "custom_material.png"),
		Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"),
		Texture::load(config::MGE_TEXTURE_PATH + "TrackMaterial.jpg"),
		Texture::load(config::MGE_TEXTURE_PATH + "splatmap2.jpg"),
		Texture::load(config::MGE_TEXTURE_PATH + "normalTestNormalMap.jpg"),
		Texture::load(config::MGE_TEXTURE_PATH + "glossTestMatNo.jpg"),
		glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 5, 0.1, 1, 0.01f, 1, 1, 1, false, false, 0.3f); */

	AbstractMaterial* planeTextureMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "TrackMaterial.jpg"));
	AbstractMaterial* skyTexture = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Sky.jpg"));
	AbstractMaterial* plane1Material = new ActualMaterial(
		Texture::load(config::MGE_TEXTURE_PATH + "Track/TrackMaterial_dots.png"),
		Texture::load(config::MGE_TEXTURE_PATH + "green.jpg"),
		Texture::load(config::MGE_TEXTURE_PATH + "blue.jpg"),
		Texture::load(config::MGE_TEXTURE_PATH + "red.jpg"),
		Texture::load(config::MGE_TEXTURE_PATH + "normalTestNormalMap.jpg"),
		Texture::load(config::MGE_TEXTURE_PATH + "glossTestMatNo.jpg"),
		glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 0, 0.02f, 0, 0, 1, 1, 1, false, false, 0);

	AbstractMaterial* plane2Material = new ActualMaterial(
		Texture::load(config::MGE_TEXTURE_PATH + "Track/TrackMaterial3.jpg"),
		Texture::load(config::MGE_TEXTURE_PATH + "green.jpg"),
		Texture::load(config::MGE_TEXTURE_PATH + "blue.jpg"),
		Texture::load(config::MGE_TEXTURE_PATH + "red.jpg"),
		Texture::load(config::MGE_TEXTURE_PATH + "Track/TrackMaterial2_Normal.jpg"),
		Texture::load(config::MGE_TEXTURE_PATH + "Track/TrackMaterial2_Glossiness.jpg"),
		glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 0, 0.01f, 0, 0, 0.5f, 1, 1, false, false, 0);

	AbstractMaterial* test = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Track/TrackMaterial2_Normal.jpg"));

	Mesh* antenna1Mesh = Mesh::load(config::MGE_MODEL_PATH + "Antenna1.obj");
	Mesh* antenna2Mesh = Mesh::load(config::MGE_MODEL_PATH + "Antenna2.obj");
	Mesh* buildingMesh = Mesh::load(config::MGE_MODEL_PATH + "Building.obj");
	Mesh* building1Mesh = Mesh::load(config::MGE_MODEL_PATH + "Building1.obj");
	Mesh* building2Mesh = Mesh::load(config::MGE_MODEL_PATH + "Building2.obj");
	Mesh* meteoriteMesh = Mesh::load(config::MGE_MODEL_PATH + "meteorite.obj");
	Mesh* house1Mesh = Mesh::load(config::MGE_MODEL_PATH + "House1.obj");
	Mesh* house2Mesh = Mesh::load(config::MGE_MODEL_PATH + "house2new.obj");

	AbstractMaterial* Antenna1 = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Antenna1/Antenna1Color.jpg"));
	AbstractMaterial* Antenna2 = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Antenna2/Antenna2Color.jpg"));
	AbstractMaterial* Building = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Building/BuildingColor.jpg"));
	AbstractMaterial* Building1 = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Building1/Building1Color.jpg"));
	AbstractMaterial* Building2 = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Building2/Building2Color.jpg"));
	AbstractMaterial* Meteorite = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Meteorite/MeteoriteColor.jpg"));

	AbstractMaterial* House1 = new ActualMaterial(Texture::load(config::MGE_TEXTURE_PATH + "House1/House1Color.jpg"),
		Texture::load(config::MGE_TEXTURE_PATH + "green.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "blue.jpg"),
		Texture::load(config::MGE_TEXTURE_PATH + "red.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "House1/House1Normal.jpg"),
		Texture::load(config::MGE_TEXTURE_PATH + "House1/House1Roughness.jpg"),
		glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 0, 0, 0, 0, 1, 1, 1, true, true, 0);
	AbstractMaterial* House2 = new ActualMaterial(Texture::load(config::MGE_TEXTURE_PATH + "House2/House2Color.png"),
		Texture::load(config::MGE_TEXTURE_PATH + "green.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "blue.jpg"),
		Texture::load(config::MGE_TEXTURE_PATH + "red.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "House2/House2Normal.png"),
		Texture::load(config::MGE_TEXTURE_PATH + "House2/House2Roughness.png"),
		glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 0, 0, 0, 0, 1, 1, 1, true, true, 0);
	/**/
	GameObject* menu = new GameObject("plane", "Floor", glm::vec3(0, 0, 1.5f));
	menu->scale(glm::vec3(1, 1, 1));
	menu->rotate(-90, glm::vec3(1, 0, 0));
	menu->setMesh(menuMesh);
	menu->setMaterial(testMaterial);
	_world->add(menu);
	/**/
	GameObject* sky = new GameObject("sphere", "sky", glm::vec3(0, 70, 50));
	sky->rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
	sky->rotate(glm::radians(-27.0f), glm::vec3(1, 0, 0));
	sky->setMesh(skyMesh);
	sky->setMaterial(skyTexture);
	_world->add(sky);
	/**/

	GameObject* plane2 = new GameObject("plane1", "plane", glm::vec3(0, -6.1f, 3));
	plane2->setMesh(planeMesh);
	plane2->rotate(glm::radians(-45.0f), glm::vec3(1, 0, 0));
	plane2->setMaterial(plane2Material);
	_world->add(plane2);
#pragma region Props

	GameObject* house1 = new GameObject("meteorite", "pproplane", glm::vec3(-25, 70, 60));
	house1->setMesh(house1Mesh);
	house1->setMaterial(House1);
	house1->rotate(glm::radians(-45.0f), glm::vec3(1, 0, 0));
	house1->setBehaviour(new RotatingBehaviour(10));
	_world->add(house1);
	GameObject* house2 = new GameObject("meteorite", "pproplane", glm::vec3(25, 70, 60));
	house2->setMesh(house2Mesh);
	house2->setMaterial(House2);
	house2->rotate(glm::radians(-45.0f), glm::vec3(1, 0, 0));
	house2->setBehaviour(new RotatingBehaviour(10));
	_world->add(house2);

	GameObject* antenna1 = new GameObject("antenna1", "prop", glm::vec3(10, 12, 10));
	antenna1->setMesh(antenna1Mesh);
	antenna1->setMaterial(Antenna1);
	antenna1->rotate(glm::radians(-45.0f), glm::vec3(1, 0, 0));
	antenna1->setBehaviour(new RotatingBehaviour(45));
	_world->add(antenna1);
	GameObject* antenna11 = new GameObject("antenna1", "prop", glm::vec3(-10, 12, 10));
	antenna11->setMesh(antenna1Mesh);
	antenna11->setMaterial(Antenna1);
	antenna11->rotate(glm::radians(-45.0f), glm::vec3(1, 0, 0));
	antenna11->setBehaviour(new RotatingBehaviour(45));
	_world->add(antenna11);

	GameObject* antenna2 = new GameObject("antenna2", "prop", glm::vec3(30, 40, 50));
	antenna2->setMesh(antenna2Mesh);
	antenna2->setMaterial(Antenna2);
	antenna2->rotate(glm::radians(-45.0f), glm::vec3(1, 0, 0));
	antenna2->setBehaviour(new RotatingBehaviour(45));
	_world->add(antenna2);
	GameObject* antenna21 = new GameObject("antenna2", "prop", glm::vec3(-30, 40, 50));
	antenna21->setMesh(antenna2Mesh);
	antenna21->setMaterial(Antenna2);
	antenna21->rotate(glm::radians(-45.0f), glm::vec3(1, 0, 0));
	antenna21->setBehaviour(new RotatingBehaviour(45));
	_world->add(antenna21);

	GameObject* building = new GameObject("building", "prop", glm::vec3(0, 52, 60));
	building->setMesh(buildingMesh);
	building->setMaterial(Building);
	building->rotate(glm::radians(-45.0f), glm::vec3(1, 0, 0));
	building->setBehaviour(new RotatingBehaviour(45));
	_world->add(building);

	GameObject* building1 = new GameObject("building1", "prop", glm::vec3(-10, 52, 60));
	building1->setMesh(building1Mesh);
	building1->setMaterial(Building1);
	building1->rotate(glm::radians(-45.0f), glm::vec3(1, 0, 0));
	building1->setBehaviour(new RotatingBehaviour(45));
	_world->add(building1);

	GameObject* building2 = new GameObject("building2", "prop", glm::vec3(10, 52, 60));
	building2->setMesh(building2Mesh);
	building2->setMaterial(Building2);
	building2->rotate(glm::radians(-45.0f), glm::vec3(1, 0, 0));
	building2->setBehaviour(new RotatingBehaviour(45));
	_world->add(building2);

	GameObject* meteorite = new GameObject("meteorite", "pproplane", glm::vec3(0, 0, 20));
	meteorite->setMesh(meteoriteMesh);
	meteorite->setMaterial(Meteorite);
	meteorite->rotate(glm::radians(-45.0f), glm::vec3(1, 0, 0));
	meteorite->setBehaviour(new RotatingBehaviour(10));
	_world->add(meteorite);
	GameObject* meteorite1 = new GameObject("meteorite", "pproplane", glm::vec3(0, 20, 40));
	meteorite1->setMesh(meteoriteMesh);
	meteorite1->setMaterial(Meteorite);
	meteorite1->rotate(glm::radians(-45.0f), glm::vec3(1, 0, 0));
	meteorite1->setBehaviour(new RotatingBehaviour(10));
	_world->add(meteorite1);
	GameObject* meteorite2 = new GameObject("meteorite", "pproplane", glm::vec3(0, 40, 60));
	meteorite2->setMesh(meteoriteMesh);
	meteorite2->setMaterial(Meteorite);
	meteorite2->rotate(glm::radians(-45.0f), glm::vec3(1, 0, 0));
	meteorite2->setBehaviour(new RotatingBehaviour(10));
	_world->add(meteorite2);
#pragma endregion
	Button* button = new Button(new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH+"Restart1.png")), new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Restart2.png")),
							this,new ThirdPersonGame(_window,_debug),sf::Vector2f(400, 85), sf::Vector2f(500, 730),glm::vec3(0,-0.43f, 1.5f));
	button->setMesh(smallButton);
	button->rotate(glm::radians(-27.0f), glm::vec3(1, 0, 0));
	button->scale(glm::vec3(0.35f, 0.35f, 0.35f));
	_world->add(button);
	_buttons.push_back(button);
	_physManager->RegisterButton(button);


	Button* button1 = new Button(
		new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Leave1.png")),
		new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Leave2.png")), 
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

	Light* light = new Light("light", 1, 0, 0, glm::vec3(0, 5, 7), 0, 10, 0, 5, glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(0, -1, 0));
	light->scale(glm::vec3(0.1f, 0.1f, 0.1f));
	light->setMaterial(lightMaterial);
	_world->add(light);

	Light* light2 = new Light("light", 1, 0, 0, glm::vec3(0, 1, 1), 1, 1000, 0, 5, glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(0, -1, 1));
	light2->scale(glm::vec3(0.1f, 0.1f, 0.1f));
	light2->setMaterial(lightMaterial);
	_world->add(light2);

	Light* light3 = new Light("light", 1, 0, 0, glm::vec3(-17, 70, 50), 0, 50, 0, 5, glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(0, -1, 0));
	light3->scale(glm::vec3(0.1f, 0.1f, 0.1f));
	light3->setMesh(cubeMesh);
	light3->setMaterial(lightMaterial);
	_world->add(light3);

	Light* light4 = new Light("light", 1, 0, 0, glm::vec3(17, 70, 50), 0, 50, 0, 5, glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(0, -1, 0));
	light4->scale(glm::vec3(0.1f, 0.1f, 0.1f));
	light4->setMesh(cubeMesh);
	light4->setMaterial(lightMaterial);
	_world->add(light4);

	DebugHud::Instance().initializeText("gatesPassed", 24, glm::vec4(1, 1, 1, 1));
	DebugHud::Instance().initializeText("highScore", 24, glm::vec4(1, 1, 1, 1));
	DebugHud::Instance().initializeText("highestCombo", 24, glm::vec4(1, 1, 1, 1));
	DebugHud::Instance().initializeText("timeSpent", 24, glm::vec4(1, 1, 1, 1));

	_gatePassed = std::stoi((StatsManager::GetFileValue("stats.ini", "gatesPassed")));
	_highScore = std::stoi((StatsManager::GetFileValue("stats.ini", "highScore")));
	_highestCombo = std::stoi((StatsManager::GetFileValue("stats.ini", "highestCombo")));

	int time = std::stoi((StatsManager::GetFileValue("stats.ini", "timeSpent")));
	_minutes = (time % 3600) / 60;
	_seconds = (time % 3600) % 60;

	DebugHud::Instance().setText("feedback", "", glm::vec2(0, 0), glm::vec2(0, 0));
	DebugHud::Instance().setText("score", "", glm::vec2(0, 0), glm::vec2(0, 0));
	DebugHud::Instance().setText("combo", "", glm::vec2(0, 0), glm::vec2(0, 0));
	DebugHud::Instance().setText("achievement", "", glm::vec2(0, 0), glm::vec2(0, 0));
}

void MainMenuResolution::_render() {
	_physManager->Update(_window);
	if (!showRectangles) {
		AbstractGame::_render();
		_updateHud();
	}
}

void MainMenuResolution::_updateHud() {
	std::string debugInfo = std::string("FPS:") + std::to_string((int)_fps) + "\n";

	DebugHud::Instance().setText("fps", debugInfo, glm::vec2(0, 0), glm::vec2(0, 0), false);
	DebugHud::Instance().setText("gatesPassed", std::to_string(_gatePassed), glm::vec2(515, 133), glm::vec2(0, 0), true);
	DebugHud::Instance().setText("highScore", std::to_string(_highScore), glm::vec2(262, 133), glm::vec2(0, 0), true);
	DebugHud::Instance().setText("highestCombo", std::to_string(_highestCombo), glm::vec2(765, 133), glm::vec2(0, 0), true);
	DebugHud::Instance().setText("timeSpent", std::to_string(_minutes) + ":" + ((_seconds >= 10) ? std::to_string(_seconds) : "0" + std::to_string(_seconds)), glm::vec2(1025, 133), glm::vec2(0, 0), true);
	DebugHud::Instance().draw();
}

MainMenuResolution::~MainMenuResolution()
{
	//dtor
}
