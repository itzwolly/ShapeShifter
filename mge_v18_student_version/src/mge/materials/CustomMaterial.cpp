#include "CustomMaterial.h"

#include <iostream>

#include "CustomMaterial.h"
#include "mge/config.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/ShaderProgram.hpp"
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

ShaderProgram* CustomMaterial::_shader = NULL;

using namespace std::chrono;
CustomMaterial::CustomMaterial(glm::vec3 pDiffuseColor) :_diffuseColor(pDiffuseColor)
{
	t1 = high_resolution_clock::now();
	//every time we create an instance of colormaterial we check if the corresponding shader has already been loaded
	_lazyInitializeShader();
}

void CustomMaterial::_lazyInitializeShader() {
	//this shader contains everything the material can do (it can render something in 3d using a single color)
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "custom.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "custom.fs");
		_shader->finalize();
	}
}

CustomMaterial::~CustomMaterial()
{
	//dtor
}

void CustomMaterial::setDiffuseColor(glm::vec3 pDiffuseColor) {
	_diffuseColor = pDiffuseColor;
}

void CustomMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	_shader->use();

	//set the material color
	glUniform3fv(_shader->getUniformLocation("diffuseColor"), 1, glm::value_ptr(_diffuseColor));

	//pass in all MVP matrices separately
	glUniformMatrix4fv(_shader->getUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("viewMatrix"), 1, GL_FALSE, glm::value_ptr(pViewMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(pModelMatrix));

	t2 = high_resolution_clock::now();

	duration<float> time_span = duration_cast<duration<float>>(t2 - t1);

	_time = time_span.count();

	GLint timeLoc = _shader->getUniformLocation("time");
	glUniform1f(timeLoc, _time*10);

	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(
		_shader->getAttribLocation("vertex"),
		_shader->getAttribLocation("normal"),
		_shader->getAttribLocation("uv"), -1, -1
	);

}