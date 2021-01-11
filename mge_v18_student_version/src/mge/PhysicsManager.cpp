
#include "PhysicsManager.h"
#include <SFML/Graphics.hpp>
#include "mge/core/GameObject.hpp"
#include "glm.hpp"
#include <list>
#include "LineSegment.h"
#include "mge/CubeCollider.h"
#include "mge/SphereCollider.h"

bool PhysicsManager::_showLines = false;
float PhysicsManager::distance = 10000;
std::list<GameObject*>* PhysicsManager::_objects = new std::list<GameObject*>();
std::list<GameObject*>* PhysicsManager::_colidingObjects = new std::list<GameObject*>();
std::list<GameObject*>* PhysicsManager::_colidingWithObjects = new std::list<GameObject*>();
std::list<std::string>* PhysicsManager::_colidingWithTypes = new std::list<std::string>();
std::list<sf::RectangleShape>* PhysicsManager::_unMovables = new std::list<sf::RectangleShape>();
std::list<LineSegment*>* PhysicsManager::_lines = new std::list<LineSegment*>();
std::list<sf::Vector2f*>* PhysicsManager::_ends = new std::list<sf::Vector2f*>();


//bool PhysicsManager::OnLine(sf::Vector2f q, LineSegment* p)
//{
//	if (q.x <= std::max(p->Start->x, p->End->x) && q.x >= std::min(p->Start->x, p->End->x) &&
//		q.y <= std::max(p->Start->y, p->End->y) && q.y >= std::min(p->Start->y, p->End->y))
//		return true;
//	return false;
//}

float PhysicsManager::DistanceBetween(GameObject* one, GameObject* two)
{
	return glm::length(two->getWorldPosition() - one->getWorldPosition());
}

bool PhysicsManager::ColidesWith(GameObject * one, GameObject * two)
{

	return false;
}

glm::vec3 PhysicsManager::FlipOverNormal(glm::vec3 vec, glm::vec3 normal)
{
	//r=d-2*dot(d,n)*n
	glm::vec3 flip = vec - (glm::dot(vec, normal))*2.0f;
	return flip;
}

void PhysicsManager::ShowDebug(bool val)
{
	_showLines = val;
}
void PhysicsManager::RegisterObject(GameObject * obj)
{
	_objects->push_back(obj);
}

void PhysicsManager::UnRegisterObject(GameObject * obj)
{
	_objects->remove(obj);
}

void PhysicsManager::Update()
{
	for (std::list<GameObject*>::iterator it = _objects->begin(); it != (_objects->end()); ++it) {
		//it._Ptr->_Myval;
		for (std::list<GameObject*>::iterator ti = (it); ti != _objects->end(); ++ti) {
			//std::cout << it._Ptr->_Myval->getName() << " vs " << ti._Ptr->_Myval->getName()<<"\n";
			if (it!=ti)
			{
				Collider* col1 = it._Ptr->_Myval->GetCollider();
				Collider* col2 = ti._Ptr->_Myval->GetCollider();
				//std::cout << it._Ptr->_Myval->getName() << " vs " << ti._Ptr->_Myval->getName() << "\n";

				if (col1->GetType() == "sphere")
				{
					SphereCollider* collider1 = static_cast<SphereCollider*>(col1);
					if (col2->GetType() == "sphere")
					{
						SphereCollider* collider2 = static_cast<SphereCollider*>(col2);
						distance = glm::length(collider1->GetPosition() - collider2->GetPosition());
						bool inRadius = (collider1->GetRadius() + collider2->GetRadius()) > distance;
						if (inRadius && !(it._Ptr->_Myval->CollidesWith(ti._Ptr->_Myval->GetName())))
						{
							it._Ptr->_Myval->StartsCollisionsWith(ti._Ptr->_Myval);
							ti._Ptr->_Myval->StartsCollisionsWith(it._Ptr->_Myval);
							//std::cout << it._Ptr->_Myval->GetName() << " colliding with " << ti._Ptr->_Myval->GetName() << "\n";
							//std::cout << collider1->GetPosition() << " position with " << collider2->GetPosition() << "\n";
						}
						else if(!inRadius && it._Ptr->_Myval->CollidesWith(ti._Ptr->_Myval->GetName()))
						{
							it._Ptr->_Myval->StopsCollisionsWith(ti._Ptr->_Myval);
							ti._Ptr->_Myval->StopsCollisionsWith(it._Ptr->_Myval);
						}
					}
					else if (col2->GetType() == "cube")
					{
						CubeCollider* collider2 = static_cast<CubeCollider*>(col2);
					}
				}
				else if (col1->GetType() == "cube")
				{
					static_cast<CubeCollider*>(col1);
				}
				else
				{
					throw std::invalid_argument("unknown collidor type");
				}
			}
		}
	}
}
