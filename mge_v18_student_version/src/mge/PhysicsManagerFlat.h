#pragma once
#include <SFML/Graphics.hpp>
#include "LineSegmentFlat.h"
#include <cmath>  
#include <list>
#include <map>
class Button;
class PhysicsManagerFlat
{
public:
	PhysicsManagerFlat(bool show=false);
	~PhysicsManagerFlat();

	static float CheapSqrt(float val, int maxIterations);
	static sf::Vector2f BounceOver(sf::Vector2f vec, sf::Vector2f normal);
	static float Dot(sf::Vector2f vec1, sf::Vector2f vec2);
	static float Cross(sf::Vector2f vec1, sf::Vector2f vec2);
	static sf::Vector2f Normalize(sf::Vector2f vec);
	static sf::Vector2f CheapNormalize(sf::Vector2f vec, int iterations);
	static float Lenght(sf::Vector2f vec);
	static float CheapLenght(sf::Vector2f vec, int iterations);

	void ShowButtonAreas(bool val);
	//static void ShowDebug(bool val);
	//static void RegisterObject(GameObject* obj);
	void RegisterUnMovable(sf::RectangleShape obj, float widht, float height);
	void RegisterButton(Button* obj);
	void RegisterLine(LineSegmentFlat* line);
	void Update(sf::RenderWindow* wind);
	static bool OnLine(sf::Vector2f, LineSegmentFlat*);
	static float DistanceBetween(sf::Vector2f, sf::Vector2f);/*
	static bool CollidesWith(sf::Vector2f, sf::Vector2f);*/
private:
	bool _showButtonAreas;
	//static bool _showLines;
	std::list<sf::Vector2f*>* _ends;
	//static std::list<GameObject*>* _objects;
	std::list<sf::RectangleShape>* _unMovables;
	std::list<Button*>* _buttons;
	std::list<LineSegmentFlat*>* _lines;
	//static std::list<GameObject*>* _colidingObjects;
	//static std::list<GameObject*>* _colidingWithObjects;
	//static std::list<std::string>* _colidingWithTypes;
};

