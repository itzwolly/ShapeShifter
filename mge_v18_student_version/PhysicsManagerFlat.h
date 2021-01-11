#pragma once
#include <SFML/Graphics.hpp>
#include "LineSegmentFlat.h"
#include <cmath>  
#include <list>
#include <map>

class PhysicsManagerFlat
{
public:
	static float CheapSqrt(float val, int maxIterations);
	static sf::Vector2f BounceOver(sf::Vector2f vec, sf::Vector2f normal);
	static float Dot(sf::Vector2f vec1, sf::Vector2f vec2);
	static float Cross(sf::Vector2f vec1, sf::Vector2f vec2);
	static sf::Vector2f Normalize(sf::Vector2f vec);
	static sf::Vector2f CheapNormalize(sf::Vector2f vec, int iterations);
	static float Lenght(sf::Vector2f vec);
	static float CheapLenght(sf::Vector2f vec, int iterations);
	
	static void ShowDebug(bool val);
	//static void RegisterObject(GameObject* obj);
	static void RegisterUnMovable(sf::RectangleShape obj, float widht, float height);
	static void RegisterLine(LineSegmentFlat* line);
	//static void Update(sf::RenderWindow* wind);
	static bool OnLine(sf::Vector2f, LineSegmentFlat*);
	static float DistanceBetween(sf::Vector2f, sf::Vector2f);/*
	static bool CollidesWith(sf::Vector2f, sf::Vector2f);*/
private:
	static bool _showLines;
	static std::list<sf::Vector2f*>* _ends;
	//static std::list<GameObject*>* _objects;
	static std::list<sf::RectangleShape>* _unMovables;
	static std::list<LineSegmentFlat*>* _lines;
	//static std::list<GameObject*>* _colidingObjects;
	//static std::list<GameObject*>* _colidingWithObjects;
	//static std::list<std::string>* _colidingWithTypes;
};

