#include "glm.hpp"

#include "TerrainMaterial.h"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/core/Camera.hpp"
#include "mge/config.hpp"
#include <ctime>
#include <ratio>
#include <chrono>


ShaderProgram* TerrainMaterial::_shader = NULL;

GLint TerrainMaterial::_uMVPMatrix = 0;
GLint TerrainMaterial::_uDiffuseTexture1 = 0;
GLint TerrainMaterial::_uDiffuseTexture2 = 0;
GLint TerrainMaterial::_uDiffuseTexture3 = 0;
GLint TerrainMaterial::_uDiffuseTexture4 = 0;
GLint TerrainMaterial::_uMainTexture = 0;
GLint TerrainMaterial::_uSplatMap = 0;
GLint TerrainMaterial::_uHeightMap = 0;
GLint TerrainMaterial::_uWaterTexture = 0;

GLint TerrainMaterial::_aVertex = 0;
GLint TerrainMaterial::_aNormal = 0;
GLint TerrainMaterial::_aUV = 0;


using namespace std::chrono;
TerrainMaterial::TerrainMaterial(Texture * pHeighMap, Texture* pMainTex, Texture* pDifuse1,
	Texture* pDifuse2, Texture* pDifuse3, Texture* pDifuse4, Texture* pSplatMap, Texture* pWaterTexture, float tHeightMultiplier,
	glm::vec3 pDiffuseColor, glm::vec3 pSpecularColor, float pShinyness):
	_heightMap(pHeighMap),_mainTex(pMainTex),_heightMultiplier(tHeightMultiplier), _difuse1(pDifuse1), _difuse2(pDifuse2), _difuse3(pDifuse3),
	_difuse4(pDifuse4), _splatMap(pSplatMap), _waterTexture(pWaterTexture), _diffuseColor(pDiffuseColor), _specularColor(pSpecularColor), _shinyness(pShinyness)
{
	t1 = high_resolution_clock::now();
	_lazyInitializeShader();
}

TerrainMaterial::~TerrainMaterial() {}

void TerrainMaterial::_lazyInitializeShader() {
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "terrain.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "terrain.fs");
		_shader->finalize();

		//cache all the uniform and attribute indexes
		_uMVPMatrix = _shader->getUniformLocation("mvpMatrix");
		_uDiffuseTexture1 = _shader->getUniformLocation("diffuseTexture1");
		_uDiffuseTexture2 = _shader->getUniformLocation("diffuseTexture2");
		_uDiffuseTexture3 = _shader->getUniformLocation("diffuseTexture3");
		_uDiffuseTexture4 = _shader->getUniformLocation("diffuseTexture4");
		_uMainTexture = _shader->getUniformLocation("mainTexture");
		_uSplatMap = _shader->getUniformLocation("splatMap");
		_uHeightMap = _shader->getUniformLocation("heightMap");
		_uWaterTexture = _shader->getUniformLocation("waterTexture");

		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");
	}
}

void TerrainMaterial::setDiffuseTexture(Texture* pDiffuseTexture) {
	_heightMap = pDiffuseTexture;
}

void TerrainMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	if (!_heightMap) return;

	_shader->use();
	Light* tLight = pWorld->getLightAt(0);

	glm::vec3 ambientLightColor = tLight->GetAmbientLightColor();
	glm::vec3 lightColor = tLight->GetLightColor();
	glm::vec3 direction = tLight->GetDirection();

	t2 = high_resolution_clock::now();

	duration<float> time_span = duration_cast<duration<float>>(t2 - t1);

	_time = time_span.count();


	//Print the number of lights in the scene and the position of the first light.
	//It is not used, but this demo is just meant to show you THAT materials can access the lights in a world
	//if (pWorld->getLightCount() > 0) {
	//    std::cout << "TextureMaterial has discovered light is at position:" << pWorld->getLightAt(0)->getLocalPosition() << std::endl;
	//}

	//setup texture slot 0
	//bind the texture to the current active slot


	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, _heightMap->getId());
	glUniform1i(_uHeightMap, 5);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, _splatMap->getId());
	glUniform1i(_uSplatMap, 6);
	////pass in a precalculate mvp matrix (see texture material for the opposite)
	//glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	//glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

	glUniformMatrix4fv(_shader->getUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("viewMatrix"), 1, GL_FALSE, glm::value_ptr(pViewMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(pModelMatrix));

	glUniform1f(_shader->getUniformLocation("heightMultiplier"), _heightMultiplier);
	GLint timeLoc = _shader->getUniformLocation("time");
	glUniform1f(timeLoc, _time);//tell the shader the texture slot for the diffuse texture is slot 0

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _mainTex->getId());
	glUniform1i(_uMainTexture, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _difuse1->getId());
	glUniform1i(_uDiffuseTexture1, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _difuse2->getId());
	glUniform1i(_uDiffuseTexture2, 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, _difuse3->getId());
	glUniform1i(_uDiffuseTexture3, 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, _difuse4->getId());
	glUniform1i(_uDiffuseTexture4, 4);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, _waterTexture->getId());
	glUniform1i(_uWaterTexture, 7);


	glUniform3fv(_shader->getUniformLocation("diffuseColor"), 1, glm::value_ptr(_diffuseColor));
	glUniform3fv(_shader->getUniformLocation("ambientLightColor"), 1, glm::value_ptr(ambientLightColor));
	glUniform3fv(_shader->getUniformLocation("directionalLightColor"), 1, glm::value_ptr(lightColor));

	glUniform1f(_shader->getUniformLocation("shinyness"), _shinyness);
	glUniform3fv(_shader->getUniformLocation("specularColor"), 1, glm::value_ptr(_specularColor));
	glUniform3fv(_shader->getUniformLocation("cameraPos"), 1, glm::value_ptr(pWorld->getMainCamera()->getWorldPosition()));
	glUniform1f(_shader->getUniformLocation("maxReach"), tLight->GetMaxReach());
	glUniform1f(_shader->getUniformLocation("c1"), tLight->GetC1());
	glUniform1f(_shader->getUniformLocation("c2"), tLight->GetC2());
	glUniform1f(_shader->getUniformLocation("c3"), tLight->GetC3());
	glUniform1i(_shader->getUniformLocation("type"), tLight->GetType());
	glUniform3fv(_shader->getUniformLocation("lightDirection"), 1, glm::value_ptr(tLight->GetDirection()));
	glUniform1f(_shader->getUniformLocation("spotlightAngleStart"), tLight->GetSpotAngleStart());
	glUniform1f(_shader->getUniformLocation("spotlightAngleEnd"), tLight->GetSpotAngleEnd());
	glUniform3fv(_shader->getUniformLocation("lightPosition"), 1, glm::value_ptr(tLight->getWorldPosition()));
	glUniform1f(_shader->getUniformLocation("scale"), 2);
	;



	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV, -1, -1);
}
