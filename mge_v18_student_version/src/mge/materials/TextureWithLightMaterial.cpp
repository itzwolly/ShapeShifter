#include "glm.hpp"

#include "TextureWithLightMaterial.h"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/config.hpp"

ShaderProgram* TextureWithLightMaterial::_shader = NULL;

GLint TextureWithLightMaterial::_uDiffuseTexture = 0;

GLint TextureWithLightMaterial::_aVertex = 0;
GLint TextureWithLightMaterial::_aNormal = 0;
GLint TextureWithLightMaterial::_aUV = 0;


TextureWithLightMaterial::TextureWithLightMaterial(Texture * pDiffuseTexture,glm::vec3 pColor, float pShinyness, float pTransparency , glm::vec3 pSpecularColor) :
	_diffuseTexture(pDiffuseTexture),_transparency(pTransparency),_diffuseColor(pColor),_shinyness(pShinyness),_specularColor(pSpecularColor)
{
	_lazyInitializeShader();
}

TextureWithLightMaterial::~TextureWithLightMaterial() {}

void TextureWithLightMaterial::_lazyInitializeShader() {
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "textureWithLighting.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "textureWithLighting.fs");
		_shader->finalize();

		//cache all the uniform and attribute indexes
		_uDiffuseTexture = _shader->getUniformLocation("textureT");

		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");
	}
}

void TextureWithLightMaterial::setDiffuseTexture(Texture* pDiffuseTexture) {
	_diffuseTexture = pDiffuseTexture;
}

void TextureWithLightMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	if (!_diffuseTexture) return;

	_shader->use();
	//set the material color
	glUniform3fv(_shader->getUniformLocation("diffuseColor"), 1, glm::value_ptr(_diffuseColor));

	//pass in all MVP matrices separately
	glUniformMatrix4fv(_shader->getUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("viewMatrix"), 1, GL_FALSE, glm::value_ptr(pViewMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(pModelMatrix));
	int lightNr = pWorld->getLightCount();


	//setup texture slot 0
	glActiveTexture(GL_TEXTURE20);
	//bind the texture to the current active slot
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
	//tell the shader the texture slot for the diffuse texture is slot 0
	glUniform1i(_uDiffuseTexture, 20); 
	for (int i = 0; i<lightNr; i++)
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
		glUniform1i(_shader->getUniformLocation("lights[" + std::to_string(i) + "].type"), tLight->GetType());
		glUniform3fv(_shader->getUniformLocation("lights[" + std::to_string(i) + "].lightDirection"), 1, glm::value_ptr(tLight->GetDirection()));
		glUniform1f(_shader->getUniformLocation("lights[" + std::to_string(i) + "].spotlightAngleStart"), tLight->GetSpotAngleStart());
		glUniform1f(_shader->getUniformLocation("lights[" + std::to_string(i) + "].spotlightAngleEnd"), tLight->GetSpotAngleEnd());
	}
	glUniform3fv(_shader->getUniformLocation("diffuseColor"), 1, glm::value_ptr(_diffuseColor));
	glUniform1f(_shader->getUniformLocation("shinyness"), _shinyness);
	glUniform1f(_shader->getUniformLocation("transparency"), _transparency);
	glUniform1i(_shader->getUniformLocation("lightNr"), lightNr);
	glUniform3fv(_shader->getUniformLocation("specularColor"), 1, glm::value_ptr(_specularColor));
	glUniform3fv(_shader->getUniformLocation("cameraPos"), 1, glm::value_ptr(pWorld->getMainCamera()->getWorldPosition()));

	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV,-1,-1);
}
