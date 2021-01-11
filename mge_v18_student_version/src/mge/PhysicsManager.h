#pragma once
#include <SFML/Graphics.hpp>
#include "LineSegment.h"
#include <cmath>  
#include "glm.hpp"
#include <list>
#include <map>

class GameObject;

class PhysicsManager
{
public:
	static bool PhysicsManager::ColidesWith(GameObject * one, GameObject * two);
	static glm::vec3 FlipOverNormal(glm::vec3 vec, glm::vec3 normal);

	static void ShowDebug(bool val);
	static void RegisterObject(GameObject* obj);
	static void UnRegisterObject(GameObject* obj);
	static void Update(/*sf::RenderWindow* wind*/);
	static float DistanceBetween(GameObject* one, GameObject* two);/*
															 static bool CollidesWith(sf::Vector2f, sf::Vector2f);*/
private:
	static bool _showLines;
	static std::list<sf::Vector2f*>* PhysicsManager::_ends;
	static std::list<GameObject*>* _objects;
	static std::list<sf::RectangleShape>* _unMovables;
	static std::list<LineSegment*>* _lines;
	static std::list<GameObject*>* _colidingObjects;
	static std::list<GameObject*>* _colidingWithObjects;
	static std::list<std::string>* _colidingWithTypes;
	static float distance;
};

