#include "Prop.h"



Prop::Prop(const std::string & pName, const glm::vec3 & pPosition,bool visible)
	: GameObject(pName, "Prop", pPosition,visible)
{
	SetTag("Prop");
}


Prop::~Prop()
{
}
