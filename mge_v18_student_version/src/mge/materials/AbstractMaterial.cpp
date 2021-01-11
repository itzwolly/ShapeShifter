#include "mge/materials/AbstractMaterial.hpp"

AbstractMaterial::AbstractMaterial()
{
	_toDel = false;
    //ctor
}

AbstractMaterial::~AbstractMaterial()
{
    //dtor
}

bool AbstractMaterial::ToDel()
{
	return _toDel;
}

