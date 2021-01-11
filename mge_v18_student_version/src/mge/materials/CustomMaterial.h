#pragma once

#include "GL/glew.h"
#include "mge/materials/AbstractMaterial.hpp"
#include <chrono>

class ShaderProgram;

using namespace std::chrono;
class CustomMaterial : public AbstractMaterial
{
public:
	CustomMaterial(glm::vec3 pColor = glm::vec3(1, 0, 0));
	virtual ~CustomMaterial();

	virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

	//in rgb values
	void setDiffuseColor(glm::vec3 pDiffuseColor);

private:
	//all the static properties are shared between instances of ColorMaterial
	//note that they are all PRIVATE, we do not expose this static info to the outside world
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();
	high_resolution_clock::time_point t1;
	high_resolution_clock::time_point t2;

	float _time;
	//this one is unique per instance of color material

	glm::vec3 _diffuseColor;
};

