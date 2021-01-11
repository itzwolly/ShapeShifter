#include "glm.hpp"
#include <iostream>

#include "LitMaterial.h"
#include "mge/config.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"

ShaderProgram* LitMaterial::_shader = NULL;

LitMaterial::LitMaterial(glm::vec3 pDiffuseColor,float pShinyness, glm::vec3 pSpecularColor) : _diffuseColor(pDiffuseColor),_specularColor(pSpecularColor),_shinyness(pShinyness)
{
	//every time we create an instance of colormaterial we check if the corresponding shader has already been loaded
	_lazyInitializeShader();
}

void LitMaterial::_lazyInitializeShader() {
	//this shader contains everything the material can do (it can render something in 3d using a single color)
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "lit.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "lit.fs");


		_shader->finalize();
	}
}

LitMaterial::~LitMaterial()
{
	//dtor
}

void LitMaterial::setDiffuseColor(glm::vec3 pDiffuseColor) {
	_diffuseColor = pDiffuseColor;
}

void LitMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	_shader->use();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//set the material color
	glUniform3fv(_shader->getUniformLocation("diffuseColor"), 1, glm::value_ptr(_diffuseColor));

	//pass in all MVP matrices separately
	glUniformMatrix4fv(_shader->getUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("viewMatrix"), 1, GL_FALSE, glm::value_ptr(pViewMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(pModelMatrix));
	int lightNr = pWorld->getLightCount();
	for(int i=0;i<lightNr;i++)
	{
		Light* tLight = pWorld->getLightAt(i);

		glm::vec3 ambientLightColor = tLight->GetAmbientLightColor();
		glm::vec3 lightColor = tLight->GetLightColor();
		glm::vec3 direction = tLight->GetDirection();

		glUniform3fv(_shader->getUniformLocation("lights[" + std::to_string(i) + "].lightPosition"), 1, glm::value_ptr(tLight->getWorldPosition()));
		//glUniform3fv(_shader->getUniformLocation("directionalLightVector"), 1, glm::value_ptr(direction));

		glUniform3fv(_shader->getUniformLocation(("lights[" + std::to_string(i) + "].ambientLightColor")), 1, glm::value_ptr(ambientLightColor));
		glUniform3fv(_shader->getUniformLocation("lights[" + std::to_string(i) + "].directionalLightColor"), 1, glm::value_ptr(lightColor));

		glUniform1f(_shader->getUniformLocation("lights[" + std::to_string(i) + "].maxReach"), tLight->GetMaxReach());
		glUniform1f(_shader->getUniformLocation("lights[" + std::to_string(i) + "].c1"), tLight->GetC1());
		glUniform1f(_shader->getUniformLocation("lights[" + std::to_string(i) + "].c2"), tLight->GetC2());
		glUniform1f(_shader->getUniformLocation("lights[" + std::to_string(i) + "].c3"), tLight->GetC3());
		glUniform1i(_shader->getUniformLocation("lights[" + std::to_string(i )+ "].type"), tLight->GetType());
		glUniform3fv(_shader->getUniformLocation("lights[" + std::to_string(i) + "].lightDirection"), 1, glm::value_ptr(tLight->GetDirection()));
		glUniform1f(_shader->getUniformLocation("lights[" + std::to_string(i) + "].spotlightAngleStart"), tLight->GetSpotAngleStart());
		glUniform1f(_shader->getUniformLocation("lights[" + std::to_string(i) + "].spotlightAngleEnd"), tLight->GetSpotAngleEnd());
	}
	glUniform3fv(_shader->getUniformLocation("diffuseColor"), 1, glm::value_ptr(_diffuseColor));
	glUniform1f(_shader->getUniformLocation("shinyness"), _shinyness);
	glUniform1i(_shader->getUniformLocation("lightNr"), lightNr);
	glUniform3fv(_shader->getUniformLocation("specularColor"), 1, glm::value_ptr(_specularColor));
	glUniform3fv(_shader->getUniformLocation("cameraPos"), 1, glm::value_ptr(pWorld->getMainCamera()->getWorldPosition()));
	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(
		_shader->getAttribLocation("vertex"),
		_shader->getAttribLocation("normal"),
		_shader->getAttribLocation("uv"), -1, -1
	);

}
