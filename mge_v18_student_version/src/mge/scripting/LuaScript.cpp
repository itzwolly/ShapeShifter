#include "mge\scripting\LuaScript.h"
#include "mge\ThirdPersonGame.h"
#include "mge\core\GameObject.hpp"
#include "glm.hpp"
#include "mge\config.hpp"
#include "mge\core\Mesh.hpp"
#include "mge\materials\AbstractMaterial.hpp"
#include "mge\materials\ColorMaterial.hpp"
#include "mge\materials\TextureMaterial.hpp"
#include "mge\materials\LitMaterial.h"
#include "mge\materials\TerrainMaterial.h"
#include "mge\core\Texture.hpp"
#include "mge\behaviours\AbstractBehaviour.hpp"
#include "mge\behaviours\PlayerBehaviour.h"
#include "mge\behaviours\GateBehaviour.h"
#include "mge\GameManager.h"
#include "mge\SphereCollider.h"
#include <iostream>
#include "mge\materials\ActualMaterial.h"
#include "mge\util\StatsManager.h"

std::vector<GameObject*>* LuaScript::_gameObjects = new std::vector<GameObject*>();
std::queue<sf::SoundBuffer>* LuaScript::_soundBuffers = new std::queue<sf::SoundBuffer>();
high_resolution_clock::time_point LuaScript::_timer;

LuaScript::LuaScript(AbstractGame* pGame, std::string pPath, bool pDebug) : _game(static_cast<ThirdPersonGame*>(pGame)), _debug(pDebug){
	_state = luaL_newstate();
	luaL_openlibs(_state);

	if (_debug) {
		lua_pushboolean(_state, true);
		lua_setglobal(_state, "Debug");
	}
	
	luaL_loadfile(_state, pPath.c_str());
	if (lua_pcall(_state, 0, 0, 0) != 0) {
		std::cout << lua_tostring(_state, -1) << std::endl;
	}
	registerFunctions();
	
	_timer = high_resolution_clock::now();
}

void LuaScript::registerFunctions() {
	lua_newtable(_state); // Game
		lua_pushcfunction(_state, LuaScript::interval);
			lua_setfield(_state, -2, "interval");
		lua_pushcfunction(_state, LuaScript::HasPassedWave);
			lua_setfield(_state, -2, "hasPassedWave");
		lua_pushcfunction(_state, LuaScript::passedGateSuccessfully);
			lua_setfield(_state, -2, "passedGateSuccessfully");
		lua_pushcfunction(_state, LuaScript::initializeGameManager);
			lua_setfield(_state, -2, "initializeGameManager");
		lua_pushcfunction(_state, LuaScript::setGateSpeed);
			lua_setfield(_state, -2, "setGateSpeed");
		lua_pushcfunction(_state, LuaScript::CreateProps);
			lua_setfield(_state, -2, "createProps");
		lua_pushcfunction(_state, LuaScript::IsPaused);
			lua_setfield(_state, -2, "isPaused");
		lua_pushcfunction(_state, LuaScript::SetPaused);
			lua_setfield(_state, -2, "setPaused");
		lua_pushcfunction(_state, LuaScript::HandleDeath);
			lua_setfield(_state, -2, "displayResolution"); //HandleDeath
	lua_setglobal(_state, "Game");

	lua_newtable(_state); // GameObject
		lua_pushcfunction(_state, LuaScript::createGameObject);
			lua_setfield(_state, -2, "create");
		lua_pushcfunction(_state, LuaScript::getPos);
			lua_setfield(_state, -2, "pos");
		lua_pushcfunction(_state, LuaScript::setGameObjectScale);
			lua_setfield(_state, -2, "setScale");
		lua_pushcfunction(_state, LuaScript::setGameObjectMesh);
			lua_setfield(_state, -2, "setMesh");
		lua_pushcfunction(_state, LuaScript::setGameObjectMaterial);
			lua_setfield(_state, -2, "setMaterial");
		lua_pushcfunction(_state, LuaScript::rotateGameObject);
			lua_setfield(_state, -2, "rotate"); // todo: add to api
		lua_pushcfunction(_state, LuaScript::setGameObjectBehaviour);
			lua_setfield(_state, -2, "setBehaviour"); // todo: add to api
		lua_pushcfunction(_state, LuaScript::setGameObjectExistingMaterial);
			lua_setfield(_state, -2, "setExistingMaterial"); // todo: add to api
		lua_pushcfunction(_state, LuaScript::setGameObjectExistingMesh);
			lua_setfield(_state, -2, "setExistingMesh"); // todo: add to api
		lua_pushcfunction(_state, LuaScript::getGameObjectShape);
			lua_setfield(_state, -2, "getShape"); // todo: add to api
	lua_setglobal(_state, "GameObject");

	lua_newtable(_state); // Camera
		lua_pushcfunction(_state, LuaScript::createCamera);
			lua_setfield(_state, -2, "create"); // todo: add to api
	lua_setglobal(_state, "Camera");

	lua_newtable(_state); // Material
		lua_pushcfunction(_state, LuaScript::createMaterial);
			lua_setfield(_state, -2, "create"); // todo: add to api
	lua_setglobal(_state, "Material");

	lua_newtable(_state); // Mesh
		lua_pushcfunction(_state, LuaScript::createMesh);
			lua_setfield(_state, -2, "create"); // todo: add to api
	lua_setglobal(_state, "Mesh");

	lua_newtable(_state); // Gate
		lua_pushcfunction(_state, LuaScript::createGate);
			lua_setfield(_state, -2, "create"); // todo: add to api
	lua_setglobal(_state, "Gate");

	lua_newtable(_state); // Sound
		lua_pushcfunction(_state, LuaScript::createSound);
			lua_setfield(_state, -2, "create"); // todo: add to api
		lua_pushcfunction(_state, LuaScript::playSound);
			lua_setfield(_state, -2, "play"); // todo: add to api
		lua_pushcfunction(_state, LuaScript::stopSound);
			lua_setfield(_state, -2, "stop"); // todo: add to api
	lua_setglobal(_state, "Sound");

	lua_newtable(_state); // Music
		lua_pushcfunction(_state, LuaScript::createMusic);
			lua_setfield(_state, -2, "create"); // todo: add to api
		lua_pushcfunction(_state, LuaScript::playMusic);
			lua_setfield(_state, -2, "play"); // todo: add to api
		lua_pushcfunction(_state, LuaScript::stopMusic);
			lua_setfield(_state, -2, "stop"); // todo: add to api
	lua_setglobal(_state, "Music");

	lua_newtable(_state); // UI
		lua_pushcfunction(_state, LuaScript::InitializeText);
			lua_setfield(_state, -2, "initializeText"); // todo: add to api
		lua_pushcfunction(_state, LuaScript::SetText);
			lua_setfield(_state, -2, "setText"); // todo: add to api
	lua_setglobal(_state, "UI");

	lua_newtable(_state); // Stats
		lua_pushcfunction(_state, LuaScript::UpdateStats);
			lua_setfield(_state, -2, "updateMap"); // todo: add to api
		lua_pushcfunction(_state, LuaScript::SaveStats);
			lua_setfield(_state, -2, "save"); // todo: add to api
		lua_pushcfunction(_state, LuaScript::GetStatsValue);
			lua_setfield(_state, -2, "getValue"); // todo: add to api
	lua_setglobal(_state, "Stats");
}

LuaScript::~LuaScript() {
	lua_close(_state);
}

void LuaScript::start() {
	lua_getglobal(_state, "Start"); // pushes global update
	if (lua_isfunction(_state, -1)) {
		if (lua_pcall(_state, 0, 0, 0) != 0) {
			printf("Error %s\n", lua_tostring(_state, -1));
			exit(-1);
		}
	}
}

void LuaScript::update(float pStep) {
	lua_getglobal(_state, "Update"); // pushes global update
	if (lua_isfunction(_state, -1)) {
		lua_pushnumber(_state, pStep);
		if (lua_pcall(_state, 1, 0, 0) != 0) {
			printf("Error %s\n", lua_tostring(_state, -1));
			exit(-1);
		}
	}
}

std::vector<GameObject*>* LuaScript::GetObjects() {
	return _gameObjects;
}

int LuaScript::createGameObject(lua_State* pState) {
	if (lua_gettop(pState)) {
		std::string name = (std::string) lua_tostring(pState, 1);
		std::string tag = (std::string) lua_tostring(pState, 2);
		glm::vec3 pos = glm::vec3((float)lua_tonumber(pState, 3), (float)lua_tonumber(pState, 4), (float)lua_tonumber(pState, 5));

		GameObject** obj = (GameObject **)lua_newuserdata(pState, sizeof(GameObject));
		*obj = new GameObject(name, (tag == "") ? "Untagged" : tag, pos);
		_gameObjects->push_back(*obj);

		luaL_getmetatable(pState, "GameObject");
		lua_setmetatable(pState, -2);
		return 1;
	}
	return luaL_error(pState, "GameObject.create(name, tag, x, y, z), faulty arguments.");
}

int LuaScript::gameObjectToString(lua_State* pState) {
	lua_pushfstring(pState, "GameObject: %p", lua_touserdata(pState, 1));
	return 1;
}

int LuaScript::setGameObjectScale(lua_State* pState) {
	if (lua_gettop(pState) == 4) {
		glm::vec3 scale = glm::vec3((float)lua_tonumber(pState, 2), (float)lua_tonumber(pState, 3), (float)lua_tonumber(pState, 4));

		GameObject** obj = (GameObject**) lua_touserdata(pState, 1);
		(*obj)->scale(scale);
		return 0;
	} 
	return luaL_error(pState, "GameObject.setScale(gameobject, x, y, z), faulty arguments");
}

int LuaScript::setGameObjectMesh(lua_State* pState) {
	if (lua_gettop(pState) == 2) {
		std::string meshName = (std::string) lua_tostring(pState, 2);

		GameObject** obj = (GameObject**) lua_touserdata(pState, 1);
		Mesh* mesh = Mesh::load(config::MGE_MODEL_PATH + meshName);
		(*obj)->setMesh(mesh);

		return 0;
	}
	return luaL_error(pState, "GameObject.setMesh(gameobject, meshName), faulty arguments");
}

int LuaScript::setGameObjectMaterial(lua_State* pState) {
	std::string materialType = (std::string) lua_tostring(pState, 2);
	std::transform(materialType.begin(), materialType.end(), materialType.begin(), ::tolower);

	AbstractMaterial* material;
	if (materialType == "color") {
		glm::vec3 color = glm::vec3((float) lua_tonumber(pState, 3), (float) lua_tonumber(pState, 4), (float) lua_tonumber(pState, 5));
		material = new ColorMaterial(color);
	} else if (materialType == "texture-legacy") {
		std::string textureName = (std::string) lua_tostring(pState, 3);
		material = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + textureName));
	} else if (materialType == "texture") {
		std::string diffuse;
		std::string diffuse1;
		std::string diffuse2;
		std::string splatMap;
		std::string normalMap;
		std::string glossyMap;
		float heightMultiplier;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;
		float shinyness;
		float scrollMultiplier1;
		float scrollMultiplier2;
		float scrollMultiplier3;
		float transparency1;
		float transparency2;
		float transparency3;
		float completeReflection;

		switch (lua_gettop(pState)) {
			case 20:
				diffuse = (std::string) lua_tostring(pState, 3);
				splatMap = (std::string) lua_tostring(pState, 4);
				normalMap = (std::string) lua_tostring(pState, 5);
				glossyMap = (std::string) lua_tostring(pState, 6);
				diffuseColor = glm::vec3((float)lua_tonumber(pState, 7), (float)lua_tonumber(pState, 8), (float)lua_tonumber(pState, 9));
				specularColor = glm::vec3((float)lua_tonumber(pState, 10), (float)lua_tonumber(pState, 11), (float)lua_tonumber(pState, 12));
				shinyness = (float)lua_tonumber(pState, 13);
				scrollMultiplier1 = (float)lua_tonumber(pState, 14);
				scrollMultiplier2 = (float)lua_tonumber(pState, 15);
				scrollMultiplier3 = (float)lua_tonumber(pState, 16);
				transparency1 = (float)lua_tonumber(pState, 17);
				transparency2 = (float)lua_tonumber(pState, 18);
				transparency3 = (float)lua_tonumber(pState, 19);
				completeReflection = (float)lua_tonumber(pState, 20);

				material = new ActualMaterial(
					Texture::load(config::MGE_TEXTURE_PATH + diffuse),
					Texture::load(config::MGE_TEXTURE_PATH + diffuse),
					Texture::load(config::MGE_TEXTURE_PATH + diffuse),
					Texture::load(config::MGE_TEXTURE_PATH + splatMap),
					Texture::load(config::MGE_TEXTURE_PATH + normalMap),
					Texture::load(config::MGE_TEXTURE_PATH + glossyMap),
					diffuseColor, specularColor, shinyness, scrollMultiplier1, scrollMultiplier2, scrollMultiplier3,
					transparency1, transparency2, transparency3, completeReflection, false, false
				);
				break;
			case 22:
				diffuse = (std::string) lua_tostring(pState, 3);
				diffuse1 = (std::string) lua_tostring(pState, 4);
				diffuse2 = (std::string) lua_tostring(pState, 5);
				splatMap = (std::string) lua_tostring(pState, 6);
				normalMap = (std::string) lua_tostring(pState, 7);
				glossyMap = (std::string) lua_tostring(pState, 8);
				diffuseColor = glm::vec3((float)lua_tonumber(pState, 9), (float)lua_tonumber(pState, 10), (float)lua_tonumber(pState, 11));
				specularColor = glm::vec3((float)lua_tonumber(pState, 12), (float)lua_tonumber(pState, 13), (float)lua_tonumber(pState, 14));
				shinyness = (float)lua_tonumber(pState, 15);
				scrollMultiplier1 = (float)lua_tonumber(pState, 16);
				scrollMultiplier2 = (float)lua_tonumber(pState, 17);
				scrollMultiplier3 = (float)lua_tonumber(pState, 18);
				transparency1 = (float)lua_tonumber(pState, 19);
				transparency2 = (float)lua_tonumber(pState, 20);
				transparency3 = (float)lua_tonumber(pState, 21);
				completeReflection = (float)lua_tonumber(pState, 22);

				material = new ActualMaterial(
					Texture::load(config::MGE_TEXTURE_PATH + diffuse),
					Texture::load(config::MGE_TEXTURE_PATH + diffuse1),
					Texture::load(config::MGE_TEXTURE_PATH + diffuse2),
					Texture::load(config::MGE_TEXTURE_PATH + splatMap),
					Texture::load(config::MGE_TEXTURE_PATH + normalMap),
					Texture::load(config::MGE_TEXTURE_PATH + glossyMap),
					diffuseColor, specularColor, shinyness, scrollMultiplier1, scrollMultiplier2, scrollMultiplier3,
					transparency1, transparency2, transparency3, completeReflection, false, false
				);
				break;
			default:
				return luaL_error(pState, "GameObject.setMaterial(gameobject, \"texture\", ...) has invalid arguments.");
		}
	}

	GameObject** obj = (GameObject**) lua_touserdata(pState, 1);
	(*obj)->setMaterial(material);
	return 0;
}

int LuaScript::getPos(lua_State* state) {
	if (lua_gettop(state) == 1) {
		GameObject** object = (GameObject**) lua_touserdata(state, 1);
		glm::vec3 pos = (*object)->getWorldPosition();
		float x = pos.x;
		float y = pos.y;
		float z = pos.z;

		lua_settop(state, 0); // pop object
		lua_pushnumber(state, x);
		lua_pushnumber(state, y);
		lua_pushnumber(state, z);
		return 3;
	}
	return luaL_error(state, "GameObject.getPos(gameObject), faulty arguments");
}

int LuaScript::rotateGameObject(lua_State* pState) {
	if (lua_gettop(pState) == 5) {
		float angleInDeg = (float) lua_tonumber(pState, 2);
		glm::vec3 rotationAxis = glm::vec3((float)lua_tonumber(pState, 3), (float)lua_tonumber(pState, 4), (float)lua_tonumber(pState, 5));

		GameObject** object = (GameObject**) lua_touserdata(pState, 1);
		(*object)->rotate(glm::radians(angleInDeg), rotationAxis);
		return 0;
	}
	return luaL_error(pState, "GameObject.rotate(gameobject, angleInDeg, x, y, z), faulty arguments");
}

int LuaScript::createCamera(lua_State* pState) {
	if (lua_gettop(pState) == 5) {
		std::string name = (std::string) lua_tostring(pState, 1);
		std::string tag = (std::string) lua_tostring(pState, 2);
		glm::vec3 pos = glm::vec3((float)lua_tonumber(pState, 3), (float)lua_tonumber(pState, 4), (float)lua_tonumber(pState, 5));

		Camera** obj = (Camera **)lua_newuserdata(pState, sizeof(Camera));
		*obj = new Camera(name, tag, pos);
		_gameObjects->push_back(*obj);

		luaL_getmetatable(pState, "Camera");
		lua_setmetatable(pState, -2);

		return 1;
	}
	return luaL_error(pState, "Camera.create(name, tag, x, y, z), faulty arguments");
}

int LuaScript::setGameObjectBehaviour(lua_State* pState) {
	// TODO: Add API design
	std::string behaviourType = (std::string) lua_tostring(pState, 2);
	std::transform(behaviourType.begin(), behaviourType.end(), behaviourType.begin(), ::tolower);

	GameObject** obj = (GameObject**) lua_touserdata(pState, 1);

	if (behaviourType == "playerbehaviour") {
		float padding = (float) lua_tonumber(pState, 3);

		Mesh* meshOne = Mesh::load(config::MGE_MODEL_PATH + (std::string) lua_tostring(pState, 4));
		Mesh* meshTwo = Mesh::load(config::MGE_MODEL_PATH + (std::string) lua_tostring(pState, 5));
		Mesh* meshThree = Mesh::load(config::MGE_MODEL_PATH + (std::string) lua_tostring(pState, 6));
		std::string swapSound = config::MGE_SOUND_PATH + (std::string) lua_tostring(pState, 7);
		std::string cubeSwapSound = config::MGE_SOUND_PATH + (std::string) lua_tostring(pState, 8);
		std::string sphereSwapSound = config::MGE_SOUND_PATH + (std::string) lua_tostring(pState, 9);
		std::string pyramidSwapSound = config::MGE_SOUND_PATH + (std::string) lua_tostring(pState, 10);
		
		GameObject* player = (*obj);
		player->SetColorType(0);
		player->SetMeshType(0);
		player->SetCollider(new SphereCollider(player, 1));
		player->setBehaviour(new PlayerBehaviour(player, padding, meshOne, meshTwo, meshThree, swapSound, sphereSwapSound, cubeSwapSound, pyramidSwapSound));
		player->setMesh(meshOne);
	} else if (behaviourType == "gatebehaviour") {
		glm::vec3 pos = glm::vec3((float) lua_tonumber(pState, 3), (float) lua_tonumber(pState, 4), (float) lua_tonumber(pState, 5));
		int colorType = (int) lua_tonumber(pState, 6);
		int shapeType = (int) lua_tonumber(pState, 7);

		Gate* gate = static_cast<Gate*>(*obj);
		gate->SetCollider(new SphereCollider(gate, 1));
		gate->setBehaviour(new GateBehaviour(gate, pos));
		gate->SetColorType(colorType);
		gate->SetMeshType(shapeType);
	}
	return 0;
}

int LuaScript::interval(lua_State* pState) {
	if (lua_gettop(pState) == 1) {
		high_resolution_clock::time_point timer = high_resolution_clock::now();
		duration<double> timeSpan = duration_cast<duration<double>>(timer - _timer);

		float time = (float)lua_tonumber(pState, 1);

		if (timeSpan.count() > time) {
			lua_pushboolean(pState, true);
			_timer = timer;
		}
		else {
			lua_pushboolean(pState, false);
		}
		return 1;
	}
	return luaL_error(pState, "Game.interval(float), faulty arguments");
}

int LuaScript::HasPassedWave(lua_State* pState) {
	if (lua_gettop(pState) == 1) {
		GameObject** obj = (GameObject**) lua_touserdata(pState, 1);

		if (GameManager::hasPassedWave((*obj))) {
			lua_pushboolean(pState, true);
		}
		else {
			lua_pushboolean(pState, false);
		}

		return 1;
	}
	return luaL_error(pState, "Game.hasPassedWave(gameobject), faulty arguments");
}

int LuaScript::passedGateSuccessfully(lua_State* pState) {
	lua_pushboolean(pState, GameManager::GetPlayerHit());
	return 1;
}

int LuaScript::initializeGameManager(lua_State* pState) {
	if (lua_gettop(pState) == 7) {
		GameObject** player = (GameObject**)lua_touserdata(pState, 1);
		int stageOneGateCount = (int)lua_tonumber(pState, 2);
		int stageTwoGateCount = (int)lua_tonumber(pState, 3);
		int amountOfGates = (int)lua_tonumber(pState, 4);
		float gateSpeed = (float)lua_tonumber(pState, 5);
		float waveSpawnTimeInSec = (float)lua_tonumber(pState, 6);
		float gateZDestructionOffset = (float)lua_tonumber(pState, 7);

		//player, stageOneGateCount, stageTwoGateCount, amountOfGates, gateSpeed, gateWaveSpawnTimeInSec, gateZDestructionOffset
		GameManager::Initialize((*player), stageOneGateCount, stageTwoGateCount, amountOfGates, gateSpeed, waveSpawnTimeInSec, gateZDestructionOffset);
		return 0;
	}
	return luaL_error(pState, "Game.InitializeManager(player, stageOneGateCount, stageTwoGateCount, amountOfGates, gateSpeed, waveSpawnTimeInSec, gateZDestructionOffset, faulty arguments");
}

int LuaScript::createMaterial(lua_State* pState) {
	std::string materialType = (std::string) lua_tostring(pState, 1);
	std::transform(materialType.begin(), materialType.end(), materialType.begin(), ::tolower);

	// Color, Lit, Texture, Terrain
	AbstractMaterial* material;
	if (materialType == "color") {
		glm::vec3 color = glm::vec3((float)lua_tonumber(pState, 2), (float)lua_tonumber(pState, 3), (float)lua_tonumber(pState, 4));

		material = new ColorMaterial(color);
	} else if (materialType == "texture-legacy") {
		std::string textureName = (std::string) lua_tostring(pState, 2);
		material = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + textureName));
	} else if (materialType == "texture") {
		std::string diffuse;
		std::string diffuse1;
		std::string diffuse2;
		std::string splatMap;
		std::string normalMap;
		std::string glossyMap;
		float heightMultiplier;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;
		float shinyness;
		float scrollMultiplier1;
		float scrollMultiplier2;
		float scrollMultiplier3;
		float transparency1;
		float transparency2;
		float transparency3;
		float completeReflection;

		switch (lua_gettop(pState)) {
			case 19:
				diffuse = (std::string) lua_tostring(pState, 2);
				splatMap = (std::string) lua_tostring(pState, 3);
				normalMap = (std::string) lua_tostring(pState, 4);
				glossyMap = (std::string) lua_tostring(pState, 5);
				diffuseColor = glm::vec3((float)lua_tonumber(pState, 6), (float)lua_tonumber(pState, 7), (float)lua_tonumber(pState, 8));
				specularColor = glm::vec3((float)lua_tonumber(pState, 9), (float)lua_tonumber(pState, 10), (float)lua_tonumber(pState, 11));
				shinyness = (float)lua_tonumber(pState, 12);
				scrollMultiplier1 = (float)lua_tonumber(pState, 13);
				scrollMultiplier2 = (float)lua_tonumber(pState, 14);
				scrollMultiplier3 = (float)lua_tonumber(pState, 15);
				transparency1 = (float)lua_tonumber(pState, 16);
				transparency2 = (float)lua_tonumber(pState, 17);
				transparency3 = (float)lua_tonumber(pState, 18);
				completeReflection = (float)lua_tonumber(pState, 19);

				material = new ActualMaterial(
					Texture::load(config::MGE_TEXTURE_PATH + diffuse),
					Texture::load(config::MGE_TEXTURE_PATH + diffuse),
					Texture::load(config::MGE_TEXTURE_PATH + diffuse),
					Texture::load(config::MGE_TEXTURE_PATH + splatMap),
					Texture::load(config::MGE_TEXTURE_PATH + normalMap),
					Texture::load(config::MGE_TEXTURE_PATH + glossyMap),
					diffuseColor, specularColor, shinyness, scrollMultiplier1, scrollMultiplier2, scrollMultiplier3,
					transparency1, transparency2, transparency3, completeReflection, false, false
				);
				break;
			case 21:
				diffuse = (std::string) lua_tostring(pState, 2);
				diffuse1 = (std::string) lua_tostring(pState, 3);
				diffuse2 = (std::string) lua_tostring(pState, 4);
				splatMap = (std::string) lua_tostring(pState, 5);
				normalMap = (std::string) lua_tostring(pState, 6);
				glossyMap = (std::string) lua_tostring(pState, 7);
				diffuseColor = glm::vec3((float)lua_tonumber(pState, 8), (float)lua_tonumber(pState, 9), (float)lua_tonumber(pState, 10));
				specularColor = glm::vec3((float)lua_tonumber(pState, 11), (float)lua_tonumber(pState, 12), (float)lua_tonumber(pState, 13));
				shinyness = (float)lua_tonumber(pState, 14);
				scrollMultiplier1 = (float)lua_tonumber(pState, 15);
				scrollMultiplier2 = (float)lua_tonumber(pState, 16);
				scrollMultiplier3 = (float)lua_tonumber(pState, 17);
				transparency1 = (float)lua_tonumber(pState, 18);
				transparency2 = (float)lua_tonumber(pState, 19);
				transparency3 = (float)lua_tonumber(pState, 20);
				completeReflection = (float)lua_tonumber(pState, 21);

				material = new ActualMaterial(
					Texture::load(config::MGE_TEXTURE_PATH + diffuse),
					Texture::load(config::MGE_TEXTURE_PATH + diffuse1),
					Texture::load(config::MGE_TEXTURE_PATH + diffuse2),
					Texture::load(config::MGE_TEXTURE_PATH + splatMap),
					Texture::load(config::MGE_TEXTURE_PATH + normalMap),
					Texture::load(config::MGE_TEXTURE_PATH + glossyMap),
					diffuseColor, specularColor, shinyness, scrollMultiplier1, scrollMultiplier2, scrollMultiplier3,
					transparency1, transparency2, transparency3, completeReflection, false, false
				);
				break;
			default:
				return luaL_error(pState, "Material.create(\"texture\", ...) has invalid arguments.");
		}
	}

	AbstractMaterial** obj = (AbstractMaterial **) lua_newuserdata(pState, sizeof(AbstractMaterial));
	(*obj) = material;

	luaL_getmetatable(pState, "Material");
	lua_setmetatable(pState, -2);
	return 1;
}

int LuaScript::createMesh(lua_State* pState) {
	if (lua_gettop(pState) == 1) {
		std::string meshName = (std::string) lua_tostring(pState, 1);
		Mesh* mesh = Mesh::load(config::MGE_MODEL_PATH + meshName);

		Mesh** obj = (Mesh **) lua_newuserdata(pState, sizeof(Mesh));
		(*obj) = mesh;

		luaL_getmetatable(pState, "Mesh");
		lua_setmetatable(pState, -2);
		return 1;
	}
	return luaL_error(pState, "GameObject.setMesh(gameobject, meshName), faulty arguments");
}

int LuaScript::setGameObjectExistingMaterial(lua_State* pState) {
	if (lua_gettop(pState) == 2) {
		GameObject** obj = (GameObject**) lua_touserdata(pState, 1);
		AbstractMaterial** material = (AbstractMaterial**) lua_touserdata(pState, 2);
		(*obj)->setMaterial(*material);

		return 0;
	} else if (lua_gettop(pState) == 3) {
		GameObject** obj = (GameObject**)lua_touserdata(pState, 1);
		AbstractMaterial** material = (AbstractMaterial**) lua_touserdata(pState, 2);
		(*obj)->setMaterial(*material);

		int colorType = (int) lua_tonumber(pState, 3);
		if ((*obj)->GetTag() == "Player") {
			(*obj)->SetColorType(colorType);
		}
		return 0;
	}
	return luaL_error(pState, "GameObject.setExistingMaterial(gameobject, material, colorType = optional), faulty arguments");
}

int LuaScript::createGate(lua_State* pState) {
	if (lua_gettop(pState) == 5) {
		std::string name = (std::string) lua_tostring(pState, 1);
		std::string tag = (std::string) lua_tostring(pState, 2);
		glm::vec3 pos = glm::vec3((float)lua_tonumber(pState, 3), (float)lua_tonumber(pState, 4), (float)lua_tonumber(pState, 5));

		Gate** obj = (Gate **)lua_newuserdata(pState, sizeof(Gate));
		*obj = new Gate(name, (tag == "") ? "Untagged" : tag, pos);

		_gameObjects->push_back(*obj);
		GameManager::_gates.push_back(*obj);

		luaL_getmetatable(pState, "Gate");
		lua_setmetatable(pState, -2);
		return 1;
	}
	return luaL_error(pState, "Gate.create(name, tag, x, y, z), faulty arguments");
}

int LuaScript::setGameObjectExistingMesh(lua_State* pState) {
	if (lua_gettop(pState) == 2) {
		GameObject** obj = (GameObject**)lua_touserdata(pState, 1);
		Mesh** mesh = (Mesh**)lua_touserdata(pState, 2);
		(*obj)->setMesh(*mesh);

		return 0;
	}
	return luaL_error(pState, "GameObject.setExistingMaterial(gameobject, mesh), faulty arguments");
}

int LuaScript::getGameObjectShape(lua_State * pState) {
	GameObject** obj = (GameObject**) lua_touserdata(pState, 1);
	lua_pushnumber(pState, (*obj)->GetMeshType());
	return 1;
}

int LuaScript::setGateSpeed(lua_State* pState) {
	float speed = (float) lua_tonumber(pState, 1);
	if (*GameManager::GetGateSpeed() != speed) {
		GameManager::SetGateSpeed(speed);
		std::cout << "Changing gate speed to: " << speed << std::endl;
	}
	return 0;
}

int LuaScript::playSound(lua_State* pState) {
	sf::Sound** sound = (sf::Sound**) lua_touserdata(pState, 1);
	(*sound)->play();
	return 0;
}

int LuaScript::stopSound(lua_State* pState) {
	sf::Sound** sound = (sf::Sound**) lua_touserdata(pState, 1);
	(*sound)->stop();
	return 0;
}

int LuaScript::createSound(lua_State * pState) {
	std::string soundFile = config::MGE_SOUND_PATH + (std::string) lua_tostring(pState, 1);
	float volume = (float) lua_tonumber(pState, 2);
	bool loop = (bool) lua_toboolean(pState, 3);

	sf::Sound** sound = (sf::Sound**) lua_newuserdata(pState, sizeof(sf::Sound));

	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(soundFile)) {
		throw new std::exception("sound doesn't exist");
	}
	_soundBuffers->emplace(buffer);

	sf::Sound* snd = new sf::Sound(_soundBuffers->back());
	snd->setVolume(volume);
	snd->setLoop(loop);

	*sound = snd;

	luaL_getmetatable(pState, "Sound");
	lua_setmetatable(pState, -2);
	return 1;
}

int LuaScript::createMusic(lua_State* pState) {
	std::string soundFile = config::MGE_SOUND_PATH + (std::string) lua_tostring(pState, 1);
	float volume = (float)lua_tonumber(pState, 2);
	bool loop = (bool)lua_toboolean(pState, 3);

	sf::Music** musicPtrPtr = (sf::Music**) lua_newuserdata(pState, sizeof(sf::Music));
	sf::Music* musicPtr = new sf::Music();

	if (!musicPtr->openFromFile(soundFile)) {
		throw new std::exception("Music file doesn't exist.");
	}

	musicPtr->setVolume(volume);
	musicPtr->setLoop(loop);

	*musicPtrPtr = musicPtr;

	luaL_getmetatable(pState, "Music");
	lua_setmetatable(pState, -2);
	return 1;
}

int LuaScript::playMusic(lua_State * pState) {
	sf::Music** music = (sf::Music**) lua_touserdata(pState, 1);
	(*music)->play();
	return 0;
}

int LuaScript::stopMusic(lua_State * pState) {
	sf::Music** music = (sf::Music**) lua_touserdata(pState, 1);
	(*music)->stop();
	return 0;
}

int LuaScript::CreateProps(lua_State * pState) {
	if (lua_gettop(pState) == 5) {
		int amountOfGates = (int)lua_tonumber(pState, 1);
		float gateAmountOffset = (float)lua_tonumber(pState, 2);
		float spacing = (float)lua_tonumber(pState, 3);
		float maxPropSpacing = (float)lua_tonumber(pState, 4);
		float minMultiplier = (float)lua_tonumber(pState, 5);

		GameManager::CreateProps(amountOfGates, gateAmountOffset, spacing, maxPropSpacing, minMultiplier);
		return 0;
	}
	return luaL_error(pState, "Game.CreateProps(amountOfGates, gateAmountOffset, spacing, maxPropSpacing, minMultiplier), faulty arguments");
}

int LuaScript::InitializeText(lua_State * pState) {
	if (lua_gettop(pState) == 6) {
		std::string id = (std::string) lua_tostring(pState, 1);
		int size = (int)lua_tonumber(pState, 2);
		glm::vec4 color = glm::vec4((float)lua_tonumber(pState, 3), (float)lua_tonumber(pState, 4), (float)lua_tonumber(pState, 5), (float)lua_tonumber(pState, 6));

		DebugHud::Instance().initializeText(id, size, color);
		return 0;
	}
	return luaL_error(pState, "UI.initializeText(tag, fontSize, r, g, b, a), faulty arguments");
}

int LuaScript::SetText(lua_State * pState) {
	if (lua_gettop(pState) == 7) {
		std::string id = (std::string) lua_tostring(pState, 1);
		std::string text = (std::string) lua_tostring(pState, 2);
		glm::vec2 pos = glm::vec2((float) lua_tonumber(pState, 3), (float) lua_tonumber(pState, 4));
		glm::vec2 anchor = glm::vec2((float) lua_tonumber(pState, 5), (float) lua_tonumber(pState, 6));
		bool centerText = (bool)lua_toboolean(pState, 7);

		DebugHud::Instance().setText(id, text, pos, anchor, centerText);
		return 0;
	}
	return luaL_error(pState, "UI.SetText(tag, text, x, y, anchorX, anchorY), faulty arguments");
}

int LuaScript::IsPaused(lua_State * pState) {
	if (GameManager::_paused) {
		lua_pushboolean(pState, true);
	} else {
		lua_pushboolean(pState, false);
	}
	return 1;
}

int LuaScript::SetPaused(lua_State * pState) {
	if (lua_gettop(pState) == 1) {
		bool paused = (bool) lua_toboolean(pState, 1);

		GameManager::_paused = paused;
		return 0;
	}
	return luaL_error(pState, "Game.setPaused(bool), faulty arguments");
}

int LuaScript::UpdateStats(lua_State * pState) {
	if (lua_gettop(pState) == 2) {
		std::string key = (std::string) lua_tostring(pState, 1);
		float value = (float)lua_tonumber(pState, 2);

		std::string oldValue = StatsManager::GetFileValue("stats.ini", key);
		std::string::size_type sz;

		if (oldValue != "") {
			float oldValueF = std::stof(oldValue, &sz);

			if (value > oldValueF) {
				StatsManager::UpdateMap(key, std::to_string(value));
			} else {
				StatsManager::UpdateMap(key, std::to_string(oldValueF));
			}
		} else {
			StatsManager::UpdateMap(key, std::to_string(value));
		}
		return 0;
	}
	return luaL_error(pState, "Stats.UpdateMap(key, value)");
}

int LuaScript::SaveStats(lua_State * pState) {
	StatsManager::WriteFile("stats.ini");
	return 0;
}

int LuaScript::GetStatsValue(lua_State * pState) {
	if (lua_gettop(pState) == 1) {
		std::string key = (std::string) lua_tostring(pState, 1);
		std::string value = StatsManager::GetFileValue("stats.ini", key);

		std::string::size_type sz;
		float valueF = std::stof(value, &sz);

		lua_pushnumber(pState, valueF);
		return 1;
	}
	return luaL_error(pState, "Stats.getValue(key)");
}

int LuaScript::HandleDeath(lua_State * pState) {
	GameManager::HandleDeath();
	return 0;
}
