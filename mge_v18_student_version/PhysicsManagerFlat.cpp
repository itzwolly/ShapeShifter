//#include "stdafx.h"
#include "mge/PhysicsManagerFlat.h"
#include <SFML/Graphics.hpp>
//#include "GameObject.h"
#include <list>
#include "mge/LineSegmentFlat.h"

bool PhysicsManagerFlat::_showLines=false;/*
std::list<GameObject*>* PhysicsManager::_objects = new std::list<GameObject*>();
std::list<GameObject*>* PhysicsManager::_colidingObjects = new std::list<GameObject*>();
std::list<GameObject*>* PhysicsManager::_colidingWithObjects = new std::list<GameObject*>();
std::list<std::string>* PhysicsManager::_colidingWithTypes = new std::list<std::string>();*/
std::list<sf::RectangleShape>* PhysicsManagerFlat::_unMovables = new std::list<sf::RectangleShape>();
std::list<LineSegmentFlat*>* PhysicsManagerFlat::_lines = new std::list<LineSegmentFlat*>();
std::list<sf::Vector2f*>* PhysicsManagerFlat::_ends = new std::list<sf::Vector2f*>();


bool PhysicsManagerFlat::OnLine(sf::Vector2f q, LineSegmentFlat* p)
{
	if (q.x <= std::max(p->Start->x, p->End->x) && q.x >= std::min(p->Start->x, p->End->x) &&
		q.y <= std::max(p->Start->y, p->End->y) && q.y >= std::min(p->Start->y, p->End->y))
		return true;
	return false;
}

float PhysicsManagerFlat::DistanceBetween(sf::Vector2f one, sf::Vector2f two)
{
	return CheapLenght(two-one,10);
}

float PhysicsManagerFlat::CheapSqrt(float val,int maxIterations)
{
	float guess = 1;
	int iterations = 0;
	while (std::abs((guess * guess) / val - 1.0) >= 0.0001)
	{
		if (iterations < maxIterations)
		{
			guess = ((val / guess) + guess) / 2;
			iterations++;
		}
		else
		{
			//std::cout << "ran out of iterations";
			break;
		}
	}

	return guess;
}/*
bool PhysicsManager::ColidesWith(GameObject * one, GameObject * two)
{
	float radius1 = one->GetSprite().getRadius();
	float radius2 = two->GetSprite().getRadius();
	return false;
}*/
float PhysicsManagerFlat::Dot(sf::Vector2f vec1, sf::Vector2f vec2)
{
	return vec1.x*vec2.x + vec1.y*vec2.y;
}
float PhysicsManagerFlat::Cross(sf::Vector2f vec1, sf::Vector2f vec2)
{
	return vec1.x*vec2.y + vec1.y*vec2.x;
}
sf::Vector2f PhysicsManagerFlat::Normalize(sf::Vector2f vec)
{
	return vec/Lenght(vec);
}
sf::Vector2f PhysicsManagerFlat::CheapNormalize(sf::Vector2f vec,int iterations)
{
	return vec / CheapLenght(vec,iterations);
}
float PhysicsManagerFlat::Lenght(sf::Vector2f vec)
{
	return sqrt(vec.x*vec.x+vec.y*vec.y);
}
float PhysicsManagerFlat::CheapLenght(sf::Vector2f vec, int iterations)
{
	return CheapSqrt(vec.x*vec.x + vec.y*vec.y,iterations);
}
sf::Vector2f PhysicsManagerFlat::BounceOver(sf::Vector2f vec,sf::Vector2f normal)
{
	//r=d-2*dot(d,n)*n
	sf::Vector2f flip = vec - (2*Dot(vec,normal))*normal;
	return flip;
}
void PhysicsManagerFlat::ShowDebug(bool val)
{
	_showLines = val;
}/*
void PhysicsManager::RegisterObject(GameObject * obj)
{
	_objects->push_back(obj);
}*/

void PhysicsManagerFlat::RegisterUnMovable(sf::RectangleShape obj,float widht,float height)
{
	_unMovables->push_back(obj);

	float leftUpX = obj.getPosition().x;
	float leftUpY = obj.getPosition().y;

	RegisterLine(new LineSegmentFlat(new sf::Vector2f(leftUpX, leftUpY), new sf::Vector2f(leftUpX + widht, leftUpY)));
	RegisterLine(new LineSegmentFlat(new sf::Vector2f(leftUpX + widht, leftUpY), new sf::Vector2f(leftUpX + widht, leftUpY + height)));
	RegisterLine(new LineSegmentFlat(new sf::Vector2f(leftUpX + widht, leftUpY + height), new sf::Vector2f(leftUpX, leftUpY + height)));
	RegisterLine(new LineSegmentFlat(new sf::Vector2f(leftUpX, leftUpY +height), new sf::Vector2f(leftUpX, leftUpY)));
}

void PhysicsManagerFlat::RegisterLine(LineSegmentFlat * line)
{
	_lines->push_back(line);
	_ends->push_back(line->Start);
	_ends->push_back(line->End);
}
/**
void PhysicsManagerFlat::Update(sf::RenderWindow* wind)
{
	//std::cout << _colidingObjects->size();
	sf::Vector2f colPos;
	sf::Vector2f objPos;
	sf::Vector2f obj2Pos;
	sf::Vector2f underneath;
	sf::Vector2f normal;
	sf::Vector2f normalizedVel;
	sf::Vector2f vel;
	float distance;
	float size;/*
	for (std::list<GameObject*>::iterator it = _objects->begin(); it != _objects->end(); ++it) {
		//it._Ptr->_Myval;
		for (std::list<LineSegment*>::iterator ti = _lines->begin(); ti != _lines->end(); ++ti) {
			LineSegment seg = it._Ptr->_Myval->GetSegment();
			objPos = it._Ptr->_Myval->GetActualPosition();
			underneath = ti._Ptr->_Myval->GetUnderneath(objPos);
			//if (_showLines)
			//{
			//	sf::CircleShape point = sf::CircleShape(10);
			//	point.setFillColor(sf::Color::Red);
			//	point.setPosition(underneath.x - 10, underneath.y - 10);
			//	wind->draw(point);
			//	sf::CircleShape point2 = sf::CircleShape(10);
			//	point2.setFillColor(sf::Color::Green);
			//	point2.setPosition(seg.End->x - 10, seg.End->y - 10);
			//	wind->draw(point2);
			//	//std::cout << "dist = " << CheapLenght((objPos-underneath),10) << std::endl;
			//}
			if (seg.Intersects(ti._Ptr->_Myval, colPos, false) || (CheapLenght(underneath - objPos, 20) < it._Ptr->_Myval->GetSprite().getRadius()&& OnLine(underneath, ti._Ptr->_Myval )))
			{
				normal = CheapNormalize((objPos - underneath),20);

				normalizedVel = Normalize(it._Ptr->_Myval->GetVelocity());
				//sf::Vector2f distanceMover = (normalizedVel*((CheapLenght(it._Ptr->_Myval->GetActualPosition()-colPos,20))+ it._Ptr->_Myval->GetSprite().getRadius()/2);

				it._Ptr->_Myval->SetPosition(colPos );
				int index = 0;
				while (true)
				{
					underneath = ti._Ptr->_Myval->GetUnderneath(objPos);
					//std::cout << objPos.x << ", "<< objPos.y << " | " << underneath.x << ", "<< underneath.y << std::endl;
					if (index<it._Ptr->_Myval->GetSprite().getRadius() &&  (CheapLenght(underneath- objPos,20) < it._Ptr->_Myval->GetSprite().getRadius()))
					{
						//std::cout << "moving"<<std::endl;
						objPos+=(-normalizedVel*3.0f);
						index++;
					}
					else
					{
						if(index>=it._Ptr->_Myval->GetSprite().getRadius() )
						std::cout << "stopped resseting " <<std::endl;
						break;
					}
				}
				it._Ptr->_Myval->SetPosition(objPos);
				//it._Ptr->_Myval->SetPosition(colPos - distanceMover);//not correct yet, fix if time
				vel = BounceOver(it._Ptr->_Myval->GetVelocity(),normal);
				it._Ptr->_Myval->SetVelocity(vel);
			}
		}
		for (std::list<sf::Vector2f*>::iterator ti = _ends->begin(); ti != _ends->end(); ++ti)
		{
			objPos = it._Ptr->_Myval->GetActualPosition();
			distance = DistanceBetween(objPos, *ti._Ptr->_Myval);
			if (distance<it._Ptr->_Myval->GetSprite().getRadius())
			{
				normal = CheapNormalize(objPos - *ti._Ptr->_Myval,20);
				it._Ptr->_Myval->SetPosition(*ti._Ptr->_Myval+normal*(it._Ptr->_Myval->GetSprite().getRadius()+1));
				vel = BounceOver(it._Ptr->_Myval->GetVelocity(), normal);
				it._Ptr->_Myval->SetVelocity(vel);
			}
		}
		for (std::list<GameObject*>::iterator ti = it ; ti != _objects->end(); ++ti)
		{
			if (it._Ptr->_Myval != ti._Ptr->_Myval)
			{
				objPos = it._Ptr->_Myval->GetActualPosition();
				distance = DistanceBetween(objPos, ti._Ptr->_Myval->GetActualPosition());
				if (distance<(it._Ptr->_Myval->GetSprite().getRadius()+ ti._Ptr->_Myval->GetSprite().getRadius()))
				{
					it._Ptr->_Myval->StartsCollisionsWith(ti._Ptr->_Myval);
					ti._Ptr->_Myval->StartsCollisionsWith(it._Ptr->_Myval);
					objPos = it._Ptr->_Myval->GetActualPosition();
					obj2Pos = ti._Ptr->_Myval->GetActualPosition();

					normal = CheapNormalize(objPos - obj2Pos, 20);

					it._Ptr->_Myval->SetPosition(ti._Ptr->_Myval->GetActualPosition() + normal*(it._Ptr->_Myval->GetSprite().getRadius()+ ti._Ptr->_Myval->GetSprite().getRadius()+1.0f));
					vel = BounceOver(it._Ptr->_Myval->GetVelocity(), normal);
					it._Ptr->_Myval->SetVelocity(vel);
				}
				else
				{
					it._Ptr->_Myval->StopsCollisionsWith(ti._Ptr->_Myval);
					ti._Ptr->_Myval->StopsCollisionsWith(it._Ptr->_Myval);
				}
			}
		}
	}*/
/**
	for (std::list<sf::RectangleShape>::iterator it = _unMovables->begin(); it != _unMovables->end(); ++it)
	{
		wind->draw(it._Ptr->_Myval);
	}

	if (_showLines)
	{
		for (std::list<LineSegment*>::iterator it = _lines->begin(); it != _lines->end(); ++it) {
			it._Ptr->_Myval->Draw(wind);
		}
	}
}/**/
/**/