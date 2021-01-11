#ifndef LUASCRIPTING
#define LUASCRIPTING

#include <cstdio>
#include <string>
#include <ctime>
#include <ratio>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <SFML\Audio.hpp>
#include <queue>
#include "mge\util\DebugHud.hpp"

extern "C" {
	#include "lauxlib.h"
	#include "lualib.h"
	#include <lua.hpp>
}

class GameObject;
class ThirdPersonGame;
class AbstractGame;
class LuaScript
{
	public:
		LuaScript(AbstractGame* pGame, std::string pPath, bool pDebug);
		virtual ~LuaScript();

		static std::chrono::steady_clock::time_point _timer;

		void start();
		void update(float pStep);
		std::vector<GameObject*>* GetObjects();

		static int createGameObject(lua_State* pState);
		static int gameObjectToString(lua_State* pState);
		static int setGameObjectScale(lua_State* pState);
		static int setGameObjectMesh(lua_State* pState);
		static int setGameObjectMaterial(lua_State* pState);
		static int getPos(lua_State* state);
		static int rotateGameObject(lua_State* pState);
		static int createCamera(lua_State* pState);
		static int setGameObjectBehaviour(lua_State* pState);
		static int interval(lua_State* pState);
		static int HasPassedWave(lua_State* pState);
		static int passedGateSuccessfully(lua_State* pState);
		static int initializeGameManager(lua_State* pState);
		static int createMaterial(lua_State* pState);
		static int setGameObjectExistingMaterial(lua_State* pState);
		static int createGate(lua_State* pState);
		static int createMesh(lua_State* pState);
		static int setGameObjectExistingMesh(lua_State* pState);
		static int getGameObjectShape(lua_State* pState);
		static int setGateSpeed(lua_State* pState);
		static int playSound(lua_State* pState);
		static int stopSound(lua_State* pState);
		static int createSound(lua_State* pState);
		static int createMusic(lua_State* pState);
		static int playMusic(lua_State* pState);
		static int stopMusic(lua_State* pState);
		static int CreateProps(lua_State* pState);
		static int InitializeText(lua_State* pState);
		static int SetText(lua_State* pState);
		static int IsPaused(lua_State* pState);
		static int SetPaused(lua_State* pState);
		static int UpdateStats(lua_State* pState);
		static int SaveStats(lua_State* pState);
		static int GetStatsValue(lua_State* pState);
		static int HandleDeath(lua_State* pState);

	private:
		ThirdPersonGame* _game;
		lua_State* _state;
		bool _debug;
		static std::vector<GameObject*>* _gameObjects;
		static std::queue<sf::SoundBuffer>* _soundBuffers;

		void registerFunctions();
		
};

#endif // LUASCRIPT_H