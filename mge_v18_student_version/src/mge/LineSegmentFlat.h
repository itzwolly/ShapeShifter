#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
class LineSegmentFlat
{
public:
	LineSegmentFlat(sf::Vector2f* start, sf::Vector2f* end);
	bool Intersects(LineSegmentFlat* other, sf::Vector2f& outIntersection, bool considerCollinearOverlapAsIntersect = false);
	~LineSegmentFlat();
	void Draw(sf::RenderWindow* wind);
	sf::Vector2f GetNormal();
	sf::Vector2f GetUnderneath(sf::Vector2f);
	sf::Vector2f* Start;
	sf::Vector2f* End;
	bool LineSegmentFlat::CircleTouching(sf::Vector2f circle, float radius);
private:
	float Cross(sf::Vector2f start, sf::Vector2f end);
	float Multiply(sf::Vector2f, sf::Vector2f);
};

