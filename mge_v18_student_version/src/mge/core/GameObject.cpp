#include <iostream>
#include "GameObject.hpp"
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/SphereCollider.h"
#include "mge/CubeCollider.h"
#include "mge/core/KeyHandler.h"
#include "mge/core/World.hpp"
using namespace std::chrono;

GameObject::GameObject(const std::string& pName, const std::string& pTag, const glm::vec3& pPosition, bool visible)
:	_name( pName ), _tag(pTag), _transform( glm::translate( pPosition ) ), _parent(nullptr), _children(),
    _mesh( nullptr ),_behaviour( nullptr ), _material(nullptr), _world(nullptr), _isVisible(visible)
{
	t1 = high_resolution_clock::now();
	t2 = high_resolution_clock::now();
	_nameCols = std::set<GameObject*>();
	_prevNameCols = std::set<GameObject*>();
	//_typeCols = std::list<std::string>();
}
float GameObject::GetLifeTime()
{
	t2 = high_resolution_clock::now();
	duration<double> time_span = duration_cast<std::chrono::milliseconds>(t2 - t1);
	return time_span.count();
}


GameObject::~GameObject()
{
    //detach all children
    std::cout << "GC running on:" << _name << std::endl;

    while (_children.size() > 0) {
        GameObject* child = _children[0];
        remove (child);
        delete child;
    }
	//delete (_collider);
	//delete(_behaviour);
    //do not forget to delete behaviour, material, mesh, collider manually if required!
}

int GameObject::GetMeshType()
{
	return _type;
}

int GameObject::GetColorType()
{
	return _color;
}

void GameObject::SetMeshType(int a)
{
	_type = a;
}

void GameObject::SetColorType(int a)
{
	_color = a;
}

World* GameObject::GetWorld() {
	return _world;
}

//void GameObject::AddToWorld(GameObject* pGameObject) {
//	_world->add(pGameObject);
//}

void GameObject::SetName (const std::string& pName)
{
    _name = pName;
}

std::string GameObject::GetName() const
{
	return _name;
}

void GameObject::SetTag(const std::string & pTag)
{
	_tag = pTag;
}

std::string GameObject::GetTag() const
{
	return _tag;
}

void GameObject::setTransform (const glm::mat4& pTransform)
{
    _transform = pTransform;
}

const glm::mat4& GameObject::getTransform() const
{
    return _transform;
}

void GameObject::setLocalPosition (glm::vec3 pPosition)
{
    _transform[3] = glm::vec4 (pPosition,1);
}

glm::vec3 GameObject::getLocalPosition() const
{
	return glm::vec3(_transform[3]);
}

void GameObject::setMaterial(AbstractMaterial* pMaterial)
{
    _material = pMaterial;
}

AbstractMaterial * GameObject::getMaterial() const
{
    return _material;
}

void GameObject::setMesh(Mesh* pMesh)
{
	if (_firstMesh == nullptr) {
		_firstMesh = _mesh;
	}
	_mesh = pMesh;
}

Mesh * GameObject::getMesh() const
{
    return _mesh;
}

void GameObject::setBehaviour(AbstractBehaviour* pBehaviour)
{
	_behaviour = pBehaviour;
	_behaviour->setOwner(this);
}

AbstractBehaviour* GameObject::getBehaviour() const
{
    return _behaviour;
}

void GameObject::setParent (GameObject* pParent) {
    //remove from previous parent
    if (_parent != nullptr) {
        _parent->_innerRemove(this);
    }

    //set new parent
    if (pParent != nullptr) {
        pParent->_innerAdd(this);
    }

    //if we have been detached from our parent, make sure
    //the world reference for us and all our children is set to null
    //if we have been attached to a parent, make sure
    //the world reference for us and all our children is set to our parent world reference
    //(this could still be null if the parent or parent's parent is not attached to the world)
    if (_parent == nullptr) {
        _setWorldRecursively(nullptr);
    } else {
        //might still not be available if our parent is not part of the world
        _setWorldRecursively(_parent->_world);
    }
}

void GameObject::_innerRemove (GameObject* pChild) {
    for (auto i = _children.begin(); i != _children.end(); ++i) {
        if (*i == pChild) {
            _children.erase(i);
            pChild->_parent = nullptr;
            return;
        }
    }
}

void GameObject::_innerAdd(GameObject* pChild)
{
	_children.insert(_children.begin(),pChild);
	//_children.push_back(pChild);
   pChild->_parent = this;
}

void GameObject::add (GameObject* pChild) {
    pChild->setParent(this);
}

void GameObject::remove (GameObject* pChild) {
    pChild->setParent(nullptr);
}

GameObject* GameObject::getParent() const {
    return _parent;
}

////////////

//costly operation, use with care
glm::vec3 GameObject::getWorldPosition() const
{
	return glm::vec3(getWorldTransform()[3]);
}

//costly operation, use with care
glm::mat4 GameObject::getWorldTransform() const
{
	if (_parent == nullptr) return _transform;
	else return _parent->getWorldTransform() * _transform;
}

////////////

void GameObject::translate(glm::vec3 pTranslation)
{
	setTransform(glm::translate(_transform, pTranslation));
}

void GameObject::scale(glm::vec3 pScale)
{
	setTransform(glm::scale(_transform, pScale));
}

void GameObject::rotate(float pAngle, glm::vec3 pAxis)
{
	setTransform(glm::rotate(_transform, pAngle, pAxis));
}

void GameObject::update(float pStep)
{
	//std::cout<<_name <<" "<<getLocalPosition()<<"\n";
	
    //make sure behaviour is updated after worldtransform is set
	if (_behaviour) {
		_behaviour->update(pStep);
	}

    for (int i = _children.size()-1; i >= 0; --i ) {
		if (_children[i] == NULL)
		{

		}
        _children[i]->update(pStep);
    }/*
	int prevsize = _prevNameCols.size();
	int size = _nameCols.size();
	if (prevsize != 0 || size != 0)
		std::cout << prevsize << " / " << size << "\n";*/
	_prevNameCols = _nameCols;
}

void GameObject::StartsCollisionsWith(GameObject * obj)
{
	_nameCols.insert(obj);
	//_typeCols.push_back(obj->GetTag());
	//std::cout<< _nameCols.size() << " - " << _typeCols.size()<<"\n";
	//std::cout << "adding collision";
}

void GameObject::StopsCollisionsWith(GameObject * obj)
{
	//std::cout << "calls remove on " << _name << "for type " << obj->GetTag() << std::endl;
	_nameCols.erase(obj);
	//if (std::find(_typeCols.begin(), _typeCols.end(), obj->GetTag()) != _typeCols.end())
	//{
	//	_typeCols.remove(obj->GetTag());
	//}
	//std::cout << _nameCols.size() << " - " << _typeCols.size() << "\n";
}

GameObject* GameObject::CollidesWith(std::string typeorname)
{
	//std::cout <<_typeCols.size() << " | "<<_nameCols.size()<<"\n";
	std::set<GameObject*>::iterator it;
	for (it = _nameCols.begin(); it != _nameCols.end(); ++it)
	{
		if (it._Ptr->_Myval->GetName() == typeorname || it._Ptr->_Myval->GetTag() == typeorname)
			return it._Ptr->_Myval;
	}

	return NULL;
}

GameObject * GameObject::CollisionEnter(std::string typeorname)
{
	std::set<GameObject*>::iterator it;
	GameObject* prev = nullptr;
	GameObject* now = nullptr;
	if (_prevNameCols.size() == _nameCols.size())
		return NULL;
	for (it = _prevNameCols.begin(); it != _prevNameCols.end(); ++it)
	{
		if (it._Ptr->_Myval->GetName() == typeorname || it._Ptr->_Myval->GetTag() == typeorname)
		{
			prev = it._Ptr->_Myval;
			//std::cout << prev->GetName()<<"\n";
		}
	}
	//std::cout << _prevNameCols.size() << " || "<<_nameCols.size() << "\n";
	_prevNameCols = _nameCols;
	if (prev == nullptr)
	{
		for (it = _nameCols.begin(); it != _nameCols.end(); ++it)
		{
			if (it._Ptr->_Myval->GetName() == typeorname || it._Ptr->_Myval->GetTag() == typeorname)
			{
				//std::cout << _name << " still have to fix multiple entrances \n";
				return it._Ptr->_Myval;
			}
		}
	}
	return NULL;
}

void GameObject::_setWorldRecursively (World* pWorld) {
    _world = pWorld;

    for (int i = _children.size()-1; i >= 0; --i ) {
        _children[i]->_setWorldRecursively (pWorld);
    }
}

int GameObject::getChildCount() const {
    return _children.size();
}

GameObject* GameObject::getChildAt(int pIndex) const {
    return _children[pIndex];
}


GameObject * GameObject::find(std::string pName) {
	for (size_t i = 0; i < getChildCount(); i++) {
		GameObject* obj = getChildAt(i);
		if (obj->GetName() == pName) {
			return obj;
		}
	}
	return nullptr;
}

void GameObject::SetCollider(Collider* collider)
{
	_collider = collider;
}

Collider* GameObject::GetCollider()
{
	return _collider;
}

bool GameObject::IsVisible()
{
	return _isVisible;
}

void GameObject::ChangeVisible(bool val)
{
	_isVisible = val;
}

void GameObject::ChangeVisible()
{
	_isVisible = !_isVisible;
}

