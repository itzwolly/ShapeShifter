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

#include "mge/GameManager.h"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/materials/NumberTextureMaterial.hpp"
#include "mge/materials/FlatTextTexture.hpp"
#include "mge/materials/LitMaterial.h"
#include "mge/materials/ActualMaterial.h"
#include "mge/materials/TextureWithLightMaterial.h"
#include "mge/materials/TerrainMaterial.h"
#include "mge/materials/ShadowGen.hpp"

#include "mge/behaviours/CameraMovement.h"
#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/SlidingBehaviour.hpp"
#include "mge/behaviours/PlayerBehaviour.h"
#include "mge/behaviours/GateBehaviour.h"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/PhysicsManager.h"
#include "mge/SphereCollider.h"
#include "mge/TestScene.h"
#include "mge/Gate.h"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
TestScene::TestScene(sf::RenderWindow* window, bool pDebug)
	: AbstractGame(window, pDebug), _hud(0)
{ }

void TestScene::initialize() {
	//setup the core part
	AbstractGame::initialize();

	//setup the custom part so we can display some text
	std::cout << "Initializing HUD" << std::endl;
	_hud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl << std::endl;
}

//
////build the game _world
void TestScene::_initializeScene()
{
	//_script.start();

	//for (size_t i = 0; i < _script.GetObjects()->size(); i++) {
	//	GameObject* obj = _script.GetObjects()->at(i);
	//	_world->add(obj);
	//}
	_multiplier = GameManager::GetMultiplier();
	_lives = GameManager::GetLives();

	Mesh* planeMesh = Mesh::load(config::MGE_MODEL_PATH + "flatPlane.obj");

	Mesh* playerMesh = Mesh::load(config::MGE_MODEL_PATH + "Cube_player.obj"); // cube_flat
	Mesh* irrelevantMesh = Mesh::load(config::MGE_MODEL_PATH + "ShapeIrrelevant_gate.obj");// "ShapeIrrelevant_gate.obj");
	Mesh* xGateMesh = Mesh::load(config::MGE_MODEL_PATH + "x_gate.obj");

	Mesh* cubeMesh = Mesh::load(config::MGE_MODEL_PATH + "Cube_gate.obj");
	Mesh* sphereMesh = Mesh::load(config::MGE_MODEL_PATH + "Sphere_gate.obj");
	Mesh* coneMesh = Mesh::load(config::MGE_MODEL_PATH + "Tis_gate.obj");
	Mesh* skyMesh = Mesh::load(config::MGE_MODEL_PATH + "Sky.obj");
	Mesh* buildingMesh = Mesh::load(config::MGE_MODEL_PATH + "Building.obj");
	Mesh* building1Mesh = Mesh::load(config::MGE_MODEL_PATH + "Building1.obj");
	Mesh* building2Mesh = Mesh::load(config::MGE_MODEL_PATH + "Building2.obj");
	Mesh* antena1Mesh = Mesh::load(config::MGE_MODEL_PATH + "Antenna1.obj");
	Mesh* antena2Mesh = Mesh::load(config::MGE_MODEL_PATH + "Antenna2.obj");


	AbstractMaterial* lightMaterial = new ColorMaterial(glm::vec3(1, 1, 1));
	AbstractMaterial* refMaterial = new ColorMaterial(glm::vec3(1, 0, 1));
	AbstractMaterial* runicStoneMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "runicfloor.png"));
	AbstractMaterial* glowTestMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "GlowTest.jpg"));

	AbstractMaterial* planeMaterial = new ActualMaterial(Texture::load(config::MGE_TEXTURE_PATH + "splatmapFlatB.png"),
		Texture::load(config::MGE_TEXTURE_PATH + "splatmapFlatB.png"),Texture::load(config::MGE_TEXTURE_PATH + "splatmapFlatB.png"), 
		Texture::load(config::MGE_TEXTURE_PATH + "splatmap2.png"), Texture::load(config::MGE_TEXTURE_PATH + "normalTestNormalMap.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "glossTestMatYes.jpg"),5,
		glm::vec3(1, 1, 1), glm::vec3(1, 0, 0), 100, 0.002f, 1, 1, 1,true,true,0.3f);

	AbstractMaterial* textureLightTest = new TextureWithLightMaterial(Texture::load(config::MGE_TEXTURE_PATH + "water.jpg"));
	AbstractMaterial* testMat = new ActualMaterial(Texture::load(config::MGE_TEXTURE_PATH + "SphereGateColor.jpg"),
		Texture::load(config::MGE_TEXTURE_PATH + "BareMetal.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "SphereGateColor.jpg"),
		Texture::load(config::MGE_TEXTURE_PATH + "SphereGate_smap.png"), Texture::load(config::MGE_TEXTURE_PATH + "normalTestNormalMap.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "glossTestMatNo.jpg"), 5,
		glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 500, 0.015f,1,1,0.5f);

	AbstractMaterial* playerMaterial = new ColorMaterial(glm::vec3(1, 0, 0));
	AbstractMaterial* planeTextureMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "TrackMaterial.jpg"));
	AbstractMaterial* shadowTextureMaterial = new ShadowGen(Texture::load(config::MGE_TEXTURE_PATH + "TrackMaterial.jpg"));
	AbstractMaterial* skyTexture = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Sky.jpg"));
	AbstractMaterial* antenna1Texture = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Antena1_mat.jpg"));
	AbstractMaterial* antenna2Texture = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Antena2_mat.jpg"));
	AbstractMaterial* cubePTex = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "SquareP_mat.jpg"));
	


	/**/
	GameObject* gate = new GameObject("temp", "temp", glm::vec3(0, 1, 0));
	gate->setMesh(sphereMesh);
	gate->rotate(90, glm::vec3(0, 1, 0));
	//gate->setBehaviour(new SlidingBehaviour(gate->getWorldTransform(),3,glm::vec3(0, 1, 0),-10, 10, -10, 10, -10, 10,5,true));
	gate->setMaterial(lightMaterial);
	_world->add(gate);
	/**/
	GameObject* nr = new GameObject("plane", "Floor", glm::vec3(0, -1, 0));
	AbstractMaterial* numberTexture = new FlatTextTexture(nr, "hello", glm::vec3(1, 1, 1), 100, 50,5,100);
	nr->scale(glm::vec3(1, 1, 1));
	//plane->rotate(90, glm::vec3(1,0, 0));
	nr->setMesh(planeMesh);
	nr->setMaterial(numberTexture);
	//temps.push_back(nr);
	_world->add(nr);
	/**
	GameObject* plane1 = new GameObject("plane", "Floor", glm::vec3(0, -2, 0));
	plane1->scale(glm::vec3(2, 2, 2));
	plane1->setMesh(planeMesh);
	plane1->setMaterial(refMaterial);
	_world->add(plane1);
/**
	GameObject* player = new GameObject("player", "Player", glm::vec3(0, 1, 23));
	player->scale(glm::vec3(1, 1, 1));
	player->setMesh(playerMesh);
	player->SetMeshType(0);
	player->setMaterial(playerMaterial);
	player->SetColorType(0);
	player->setBehaviour(new PlayerBehaviour(plane, -5, playerMesh));
	player->SetCollider(new SphereCollider(player, 1));
	_world->add(player);
	PhysicsManager::RegisterObject(player);
	/**/

	//GameObject* antenna2 = new GameObject("ant2", "antenna2", glm::vec3(-15, 5, 0));
	//antenna2->rotate(glm::radians(90.0), glm::vec3(0, 1, 0));
	//antenna2->scale(glm::vec3(1, 1, 1));
	//antenna2->setMesh(antena2Mesh);
	//antenna2->setMaterial(antenna2Texture);
	//_world->add(antenna2);
//
	//GameManager manager = GameManager(4,4,_world,player,3,5,3,0.015f, glm::vec3(0, 1, -30), glm::vec3(0, 0, 15), 27, -10, cubeMesh,sphereMesh, coneMesh, irrelevantMesh, xGateMesh,0.5f,1,10);

	//add camera first (it will be updated last)
	/**
	Light* light = new Light("light", 1, 0, 0, glm::vec3(-4, 5, 10), 0, 10, 0, 5, glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(0, -1, 0));
	light->scale(glm::vec3(0.1f, 0.1f, 0.1f));
	light->setMesh(playerMesh);
	light->setMaterial(lightMaterial);
	//_world->registerLight(light);
	_world->add(light);
	/**/
	Light* light2 = new Light("light", 1, 0, 0, glm::vec3(2, 0, 1),0, 10, 0, 5, glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(0, -1, 0));
	light2->scale(glm::vec3(0.1f, 0.1f, 0.1f));
	light2->setMesh(playerMesh);
	light2->setMaterial(lightMaterial);
	light2->setBehaviour(new KeysBehaviour(light2,10,30));
	//_world->registerLight(light2);
	_world->add(light2);

	Camera* camera = new Camera("camera", "MainCamera", glm::vec3(-4, 0, 36)); // -4, 4, 16
	camera->rotate(glm::radians(-20.0f), glm::vec3(1, 0, 0));
	camera->setBehaviour(new KeysBehaviour(gate,10));
	_world->add(camera);
	_world->setMainCamera(camera);

	GameManager::ResetGates();

}

void TestScene::_render() {
	for (int i = 0; i < temps.size(); i++)
	{
		GameObject* t = temps.at(i);
		if (t->getMaterial()->ToDel())
		{
			std::cout << "delete";
			_world->remove(t);
			temps.erase(temps.begin() + i);
			delete(t);
		}
	}
	AbstractGame::_render();
	_updateHud();
}

void TestScene::_updateHud() {
	std::string debugInfo = "";
	debugInfo += std::string("FPS:") + std::to_string((int)_fps) + "\n";
	Light* l;
	for (int i = 0; i<_world->getLightCount() - 1; i++)
	{
		l = _world->getLightAt(i);
		debugInfo += l->GetName() + std::string(" - ") + std::to_string(l->GetType()) + "\n";
	}
	debugInfo += std::string("\nMultiplier:") + std::to_string(*_multiplier) + "\n";
	debugInfo += std::string("\nLives:") + std::to_string(*_lives) + "\n";

	_hud->setDebugInfo(debugInfo);
	_hud->draw();
}

TestScene::~TestScene()
{
	//dtor
}
