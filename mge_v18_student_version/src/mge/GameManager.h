#pragma once
#include "glm.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/Gate.h"
#include "mge/Prop.h"
#include <vector>
#include <ctime>
#include <ratio>
#include <chrono>
#include <SFML\Audio.hpp>
#include <mge/core/AbstractGame.hpp>
#include "mge/Button.h"

using namespace std::chrono;
class GameManager
{
public:
	//GameManager(/*GameObject* menu, AbstractGame* current, AbstractGame* next*/);
	//~GameManager();
	static void Initialize(GameObject* pPlayer, int pStageOneGateCount, int pStageTwoGateCount, int pAmountOfGates, float pGateSpeed, float pWaveSpawnTimeInSec, float pGateZDestructionOffset);
	static bool _paused;

	static bool _inGame;
	static sf::Sound _death;
	static sf::Sound _wrongGate;
	static sf::Sound _correctGate;
	static sf::SoundBuffer buffer;
	static sf::SoundBuffer buffer1;
	static sf::SoundBuffer buffer2;

	static float* GetGateSpeed();
	static void SetGateSpeed(float pSpeed);
	static void PlayerWentThroughtCorrectGate();
	static bool hasPassedWave(GameObject* pGameObject);
	static bool GetPlayerHit();
	static void DeleteOldGates();
	static void CreateProps(int pAmountOfGates, float pGateAmountOffset, float pSpacing, float pMaxPropSpacing, float pMinMultiplier);
	static void Pause();
	static void HandleDeath();

	static std::vector<Prop*>* GetProps();
	static std::vector<Gate*> _gates;

	static GLuint* GetShadowMapInt();
	static GLuint* GetFBOInt();

	static void SetMenuData(GameObject* pMenu, AbstractGame* pCurrentScene, AbstractGame* pNextScene, std::vector<Button*> pButtons);
	
private:
	static int _stageOneGateCount;
	static int _stageTwoGateCount;
	static int _amountOfGates;
	static float _gateSpeed;
	static float _waveSpawnTimeInSec;
	static float _gateZDestructionOffset;
	static GameObject* _player;
	static std::vector<Button*> _buttons;
	static GameObject* _menu;

	static GLuint _shadowMapInt;
	static GLuint _fBOInt;
	
	static AbstractGame* _current;
	static AbstractGame* _next;

	static high_resolution_clock::time_point t1;
	static high_resolution_clock::time_point t2;

	static float _oldGateSpeed;
	static bool _playerHit;

	static std::vector<Prop*>* _props;
	static std::vector<Mesh*>* _propMeshes;
	static std::vector<AbstractMaterial*>* _propMats;

	static int _gateAmountOffset;
	static std::vector<Gate*>* _destroyedGates;
};

