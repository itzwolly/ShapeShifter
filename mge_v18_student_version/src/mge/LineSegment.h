#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "glm.hpp"
class LineSegment
{
public:
	LineSegment();
	~LineSegment();
	glm::vec3 Start;
	glm::vec3 End;
	/*
	glm::vec3 GetUnderneath(glm::vec3);
	bool LineSegment::CircleTouching(glm::vec3 sphereCenter, float radius);*/
};

