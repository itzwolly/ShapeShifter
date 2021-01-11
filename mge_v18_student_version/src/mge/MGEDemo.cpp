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

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/MGEDemo.hpp"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MGEDemo::MGEDemo(sf::RenderWindow* window, bool pDebug) : AbstractGame (window, pDebug), _hud(0)
{ }

void MGEDemo::initialize() {
    //setup the core part
    AbstractGame::initialize();

    //setup the custom part so we can display some text
	std::cout << "Initializing HUD" << std::endl;
	//_hud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl << std::endl;
}

//build the game _world
void MGEDemo::_initializeScene()
{
    //MESHES

    //load a bunch of meshes we will be using throughout this demo
    //each mesh only has to be loaded once, but can be used multiple times:
    //F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
	Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
	Mesh* planeMeshTerrain = Mesh::load(config::MGE_MODEL_PATH + "plane_8192.obj");
    Mesh* cubeMeshF = Mesh::load (config::MGE_MODEL_PATH+"cube_flat.obj");
	Mesh* sphereMeshS = Mesh::load(config::MGE_MODEL_PATH + "sphere_smooth.obj");
	Mesh* teaPotMesh = Mesh::load(config::MGE_MODEL_PATH + "teapot_flat.obj");
	Mesh* monkeyMesh = Mesh::load(config::MGE_MODEL_PATH + "suzanna_smooth.obj");

    //MATERIALS

    //create some materials to display the cube, the plane and the light
	AbstractMaterial* lightMaterial = new ColorMaterial(glm::vec3(1, 1, 1));
	AbstractMaterial* customColor = new ColorMaterial(glm::vec3(0, 0, 1));
	AbstractMaterial* runicStoneMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "runicfloor.png"));

	AbstractMaterial* customMaterial = new CustomMaterial(glm::vec3(0, 0, 1));
	AbstractMaterial* litMaterial = new LitMaterial(glm::vec3(1, 1, 1), 100);
	AbstractMaterial* terrainMaterial = new TerrainMaterial(Texture::load(config::MGE_TEXTURE_PATH + "heightmap.png"), 
		Texture::load(config::MGE_TEXTURE_PATH + "land.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "diffuse1.jpg"), 
		Texture::load(config::MGE_TEXTURE_PATH + "diffuse2.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "diffuse3.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "diffuse4.jpg"), 
		Texture::load(config::MGE_TEXTURE_PATH + "splatmap2.png"), Texture::load(config::MGE_TEXTURE_PATH + "water.jpg"), 5);
	//AbstractMaterial* customMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"));

    //SCENE SETUP


	GameObject* plane = new GameObject("plane","ground", glm::vec3(5, 0, 0));
	plane->scale(glm::vec3(5, 5, 5));
	plane->setMesh(planeMeshTerrain);
	plane->setMaterial(litMaterial);
	//plane->setMaterial(terrainMaterial);
	_world->add(plane);
	/**
    //add the floor
    //add a spinning sphere

	GameObject* pot = new GameObject("pot", glm::vec3(-10, 3, 0));
	//sphere->scale(glm::vec3(2.5, 2.5, 2.5));
	pot->setMesh(teaPotMesh);
	pot->setMaterial(runicStoneMaterial);
	pot->setBehaviour(new RotatingBehaviour());
	_world->add(pot);

	GameObject* pot2 = new GameObject("pot", glm::vec3(-10, 3, 0));
	//sphere->scale(glm::vec3(2.5, 2.5, 2.5));
	pot->setMesh(teaPotMesh);
	pot->setMaterial(runicStoneMaterial);
	pot->setBehaviour(new RotatingBehaviour());
	_world->add(pot);

	GameObject* pot3 = new GameObject("pot", glm::vec3(-10, 5, 0));
	//sphere->scale(glm::vec3(2.5, 2.5, 2.5));
	pot2->setMesh(teaPotMesh);
	pot2->setMaterial(runicStoneMaterial);
	_world->add(pot2);
	pot2->setParent(pot);
	
	GameObject* custom = new GameObject("sphere", glm::vec3(0, 3, 3));
	//custom->scale(glm::vec3(2.5, 2.5, 2.5));
	custom->setMesh(sphereMeshS);
	custom->setMaterial(customMaterial);
	//custom->setBehaviour(new RotatingBehaviour());
	_world->add(custom);
	/**/
    //add a light. Note that the light does ABSOLUTELY ZIP! NADA ! NOTHING !
    //It's here as a place holder to get you started.
    //Note how the texture material is able to detect the number of lights in the scene
    //even though it doesn't implement any lighting yet!

	Light* light = new Light("light", 1, 0, 0, glm::vec3(0, 1, 0), 0, 1, 0, 5);
	light->scale(glm::vec3(0.1f, 0.1f, 0.1f));
	light->setMesh(cubeMeshF);
	light->setMaterial(lightMaterial);
	light->setBehaviour(new KeysBehaviour(light, 25));
	_world->registerLight(light);
	_world->add(light);

	Light* light2 = new Light("light", 1, 0, 0, glm::vec3(0, 0.3f, 0), 0, 1, 0, 5);
	light2->scale(glm::vec3(0.1f, 0.1f, 0.1f));
	light2->setMesh(cubeMeshF);
	light2->setMaterial(lightMaterial);
	_world->registerLight(light2);
	_world->add(light2);

	//add camera first (it will be updated last)
	Camera* camera = new Camera("camera","untagged", glm::vec3(5, 10, 7));
	//camera->rotate(glm::radians(-40.0f), glm::vec3(1, 0, 0));
	camera->setBehaviour(new CameraMovement(plane,5, 10, 0));
	_world->add(camera);
	_world->setMainCamera(camera);
}

void MGEDemo::_render() {
    AbstractGame::_render();
    _updateHud();
}

void MGEDemo::_updateHud() {
 //   std::string debugInfo = "";
	//debugInfo += std::string("FPS:") + std::to_string((int)_fps) + "\n";
	//Light* l;
	//for(int i=0;i<_world->getLightCount()-1;i++)
	//{
	//	l = _world->getLightAt(i);
	//	debugInfo += l->GetName() + std::string(" - ") + std::to_string(l->GetType()) + "\n";
	//}

 //   _hud->setText(debugInfo);
 //   _hud->draw();
}

void HandleCameraMove()
{

}

MGEDemo::~MGEDemo()
{
	//dtor
}
