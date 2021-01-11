#include "mge\GameManager.h"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/SphereCollider.h"
#include "behaviours\GateBehaviour.h"
#include "behaviours/PropBehaviour.h"
#include "core/World.hpp"
#include "mge/PhysicsManager.h"
#include "mge/config.hpp"
#include "mge/materials/ActualMaterial.h"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/MainMenuResolution.h"

bool GameManager::_playerHit = false;
bool GameManager::_paused = false;
bool GameManager::_inGame = false;

std::vector<Gate*> GameManager::_gates;
std::vector<Gate*>* GameManager::_destroyedGates = new std::vector<Gate*>();
std::vector<Mesh*>* GameManager::_propMeshes = new std::vector<Mesh*>();
std::vector<AbstractMaterial*>* GameManager::_propMats = new std::vector<AbstractMaterial*>();
std::vector<Prop*>* GameManager::_props = new std::vector<Prop*>();
std::vector<Button*> GameManager::_buttons;

GameObject* GameManager::_player;
float GameManager::_gateZDestructionOffset;
float GameManager::_gateSpeed;
float GameManager::_oldGateSpeed;
float GameManager::_waveSpawnTimeInSec;

GLuint GameManager::_shadowMapInt = 0;
GLuint GameManager::_fBOInt = 0;

GameObject* GameManager::_menu;
AbstractGame* GameManager::_next;
AbstractGame* GameManager::_current;

high_resolution_clock::time_point GameManager::t1;
high_resolution_clock::time_point GameManager::t2;

int GameManager::_stageOneGateCount;
int GameManager::_stageTwoGateCount;
int GameManager::_amountOfGates;

//GameManager::GameManager() { }
//GameManager::~GameManager() { }

void GameManager::Initialize(
	GameObject * pPlayer,
	int pStageOneGateCount,
	int pStageTwoGateCount,
	int pAmountOfGates,
	float pGateSpeed,
	float pWaveSpawnTimeInSec,
	float pGateZDestructionOffset) 
{
	// Initialize
	_player = pPlayer;
	_stageOneGateCount = pStageOneGateCount;
	_stageTwoGateCount = pStageTwoGateCount;
	_amountOfGates = pAmountOfGates;
	_gateSpeed = pGateSpeed;
	_waveSpawnTimeInSec = pWaveSpawnTimeInSec;
	_gateZDestructionOffset = pGateZDestructionOffset;	
	_paused = false;
	Mesh* building = Mesh::load(config::MGE_MODEL_PATH + "meteorite.obj");
	TextureMaterial* material = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Meteorite\\MeteoriteColor.jpg"));

	_propMeshes->push_back(building);
	_propMats->push_back(material);
}

/*
GameManager::GameManager(GameObject* menu, std::vector<Button*> buttons, AbstractGame* current, AbstractGame* next,std::vector<Mesh*> meshes,std::vector<AbstractMaterial*> propMats, float maxPropSpacing, std::string death, std::string wrongGate, std::string correctGate ,int wavesStage1, int wavesStage2, World* world,GameObject* player, int gateNR,float spacing,float waveTime,float waveTimeIncrease, glm::vec3 initial, glm::vec3 direction, float zMarker, float excess, Mesh * mesh1, Mesh * mesh2, Mesh * mesh3, Mesh * mesh4, Mesh* mesh5, float gateSpeedIncrease, int pGateAmountOffset, float minMultiplier, float gateSpeed, glm::vec3 col1, glm::vec3 col2, glm::vec3 col3, glm::vec3 col4)
{
	//_lives = 3;//commnent out if lives reset done in lua
	_buttons = buttons;
	_next = next;
	_current = current;
	_menu = menu;
	_maxPropSpacing = maxPropSpacing;
	_minMultiplier = minMultiplier;
	_propMeshes = meshes;
	_propMats = propMats;
	if (!buffer.loadFromFile(death))
		throw new std::exception("sound doesn't exist");
	_death.setBuffer(buffer);

	if (!buffer1.loadFromFile(wrongGate))
		throw new std::exception("sound doesn't exist");
	_wrongGate.setBuffer(buffer1);

	if (!buffer2.loadFromFile(correctGate))
		throw new std::exception("sound doesn't exist");
	_correctGate.setBuffer(buffer2);

	_wavesStage1 = wavesStage1;
	_wavesStage2 = wavesStage2;

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	_waveTime = waveTime;
	_waveTimeIncrease = waveTimeIncrease;
	_gateSpeedIncrease = gateSpeedIncrease;
	_zMarker = zMarker;
	_initialPos = initial;
	_excess = excess;
	_gateSpeed = gateSpeed;
	_player = player;
	_gateNR = gateNR;
	_spacing = spacing;
	_direction = direction;
	_meshes[0] = mesh1;
	_meshes[1] = mesh2;
	_meshes[2] = mesh3;
	_meshes[3] = mesh4;
	_meshes[4] = mesh5;
	_gateAmountOffset = pGateAmountOffset;

	_world = world;

	colors[0] = new ColorMaterial(col1);
	colors[1] = new ColorMaterial(col2);
	colors[2] = new ColorMaterial(col3);
	colors[3] = new ColorMaterial(col4);
	//for (int i = 0; i < gateNR; i++)
	//{
	//	Gate* gate = new Gate(("Gate"+std::to_string(i)), glm::vec3(i*spacing, 1, -30));
	//	//player->scale(glm::vec3(5, 5, 5));
	//	gate->setMesh(mesh1);
	//	gate->SetCollider(new SphereCollider(gate, 1));
	//	gate->setBehaviour(new GateBehaviour(gate, glm::vec3(0, 0, 15)));
	//	gate->setMaterial(colors[3]);
	//	gate->SetColorType(1);
	//	gate->SetMeshType(0);
	//	world->add(gate);

	//	_gates.push_back(gate);
	//}
	for (int i = 0; i < 4; i++)
	{
		colorsLeft.push_back(colors[i]);
		colorsType.push_back(i);
	}
}
*/

bool GameManager::hasPassedWave(GameObject* pPlayer) {
	if (_gates.size() > 0) {
		glm::vec3 pos = _gates.at(0)->getWorldPosition();
		if (pos.z > pPlayer->getWorldPosition().z) {
			for (size_t i = 0; i < _amountOfGates; i++) {
				_destroyedGates->push_back(_gates.at(i));
			}
			_gates.erase(_gates.begin(), _gates.begin() + _amountOfGates);
			return true;
		}
	}
	return false;
}

void GameManager::PlayerWentThroughtCorrectGate() {
	_playerHit = true;
}

void GameManager::Pause() {
	if (!_paused) {
		_menu->ChangeVisible(true);
		int size = _buttons.size();
		for (int i = 0; i < size; i++)
		{
			Button* but = _buttons.at(i);
			but->SetClickable(true);
			but->ChangeVisible(true);
		}
		_paused = true;
	} else {
		int size = _buttons.size();
		for (int i = 0; i < size; i++)
		{
			Button* but = _buttons.at(i);
			but->SetClickable(false);
			but->ChangeVisible(false);
		}
		_menu->ChangeVisible(false);
		_paused = false;
	}
}

void GameManager::HandleDeath() {
	Pause();
	
	_gates.clear();
	_current->Stop();
	_next->initialize();
	_next->run();
}

GLuint* GameManager::GetShadowMapInt()
{
	return &_shadowMapInt;
}

GLuint * GameManager::GetFBOInt()
{
	return &_fBOInt;
}

void GameManager::SetMenuData(GameObject * pMenu, AbstractGame * pCurrentScene, AbstractGame * pNextScene, std::vector<Button*> pButtons) {
	_menu = pMenu;
	_current = pCurrentScene;
	_next = pNextScene;
	_buttons = pButtons;
}

void GameManager::DeleteOldGates() {
	if (_destroyedGates->size() > 0) {
		if (_gateZDestructionOffset < 3) {
			_gateZDestructionOffset = 3;
		}

		glm::vec3 pos = _destroyedGates->at(0)->getWorldPosition();
		if (pos.z > _player->getWorldPosition().z + _gateZDestructionOffset) {
			for (size_t i = 0; i < _amountOfGates; i++) {
				Gate* waveGate = _destroyedGates->at(i);
				waveGate->getParent()->remove(waveGate);
				PhysicsManager::UnRegisterObject(waveGate);
				delete (waveGate);
			}
			_destroyedGates->erase(_destroyedGates->begin(), _destroyedGates->begin() + _amountOfGates);
		}
		_playerHit = false;
	}
}

void GameManager::CreateProps(int pAmountOfGates, float pGateAmountOffset, float pSpacing, float pMaxPropSpacing, float pMinMultiplier) {
	int nr = rand() % 3;
	int side = rand() % 2;

	if (nr > 1) {
		side = 0;
	}

	int cor; //math stuff for no if statement
	int dec; //choise for prop mesh

	for (int i = 0; i < nr; i++) {
		cor = (-1 + 2 * side);
		float rnd = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
		Prop* prop = new Prop("prop", glm::vec3((pAmountOfGates - pGateAmountOffset) * pSpacing * cor + rnd / pMaxPropSpacing * cor, 1, -50));
		prop->scale(glm::vec3(rnd, rnd, rnd));
		prop->setBehaviour(new PropBehaviour(prop, pMinMultiplier, glm::vec3(0, 0, 15)));
		dec = rand() % _propMeshes->size();
		prop->setMesh(_propMeshes->at(dec));
		prop->setMaterial(_propMats->at(dec));
		
		if (nr > 1) {
			side++;
		}
		_props->push_back(prop);
	}
}

std::vector<Prop*>* GameManager::GetProps() {
	return _props;
}

bool GameManager::GetPlayerHit() {
	return _playerHit;
}

float* GameManager::GetGateSpeed() {
	return &_gateSpeed;
}

void GameManager::SetGateSpeed(float pSpeed) {
	_gateSpeed = pSpeed;
}