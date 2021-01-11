#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/materials/ShadowGen.hpp"

Light::Light(const std::string& pName, float c1, float c2, float c3, const glm::vec3& pPosition, int pType, float pMaxReach, float pAngleFallOffStart, float pAngleFallOffEnd,
	glm::vec3 pAmbientLightColor, glm::vec3 pLightColor,glm::vec3 pDirection) :
	GameObject(pName,"light", pPosition),_maxReach(pMaxReach), _type(pType), _ambientLightColor(pAmbientLightColor), _lightColor(pLightColor),_direction(pDirection),
	_angleFallOffEnd(pAngleFallOffEnd),_angleFallOffStart(pAngleFallOffStart), _c1(c1),_c2(c2),_c3(c3)
{

}

Light::~Light() {
}



void Light::setShadowGen(ShadowGen* pBehaviour)
{
	_shadowGen = pBehaviour;
}

ShadowGen* Light::getShadowGen() const
{
	return _shadowGen;
}
//Override setWorldRecursively to make sure we are registered
//to the world we are a part of.


void Light::_setWorldRecursively (World* pWorld) {

    //store the previous and new world before and after storing the given world
    World* previousWorld = _world;
    GameObject::_setWorldRecursively(pWorld);
    World* newWorld = _world;

    //check whether we need to register or unregister
    if (previousWorld != nullptr) previousWorld->unregisterLight(this);
	if (newWorld != nullptr) newWorld->registerLight(this);

}

int Light::GetType()
{
	return _type;
}

float Light::GetC1()
{
	return _c1;
}

float Light::GetC2()
{
	return _c2;
}

float Light::GetC3()
{
	return _c3;
}

void Light::SwitchType()
{
	_type = (_type + 1) % 3;
}

float Light::GetMaxReach()
{
	return _maxReach;
}

glm::vec3 Light::GetDirection()
{
	return _direction;
}

glm::vec3 Light::GetAmbientLightColor()
{
	return _ambientLightColor;
}

glm::vec3 Light::GetLightColor()
{
	return _lightColor;
}

float Light::GetSpotAngleStart()
{
	return cos(_angleFallOffStart);
}

float Light::GetSpotAngleEnd()
{
	return cos(_angleFallOffEnd);
}


