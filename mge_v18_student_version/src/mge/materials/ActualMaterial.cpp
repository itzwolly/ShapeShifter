#include "glm.hpp"

#include "mge/materials/ActualMaterial.h"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/core/Camera.hpp"
#include "mge/config.hpp"
#include "mge/GameManager.h"
#include <ctime>
#include <ratio>
#include <chrono>


ShaderProgram* ActualMaterial::_shader = NULL;

GLint ActualMaterial::_uMVPMatrix = 0;
GLint ActualMaterial::_uDiffuseTexture1 = 0;
GLint ActualMaterial::_uDiffuseTexture2 = 0;
GLint ActualMaterial::_uDiffuseTexture3 = 0;
GLint ActualMaterial::_uSplatMap = 0;
GLint ActualMaterial::_uNormalMap = 0;
GLint ActualMaterial::_uGlossMap = 0;

GLint ActualMaterial::_aVertex = 0;
GLint ActualMaterial::_aNormal = 0;
GLint ActualMaterial::_aUV = 0;
GLint ActualMaterial::_aTangent = 0;
GLint ActualMaterial::_aBitangent = 0;


using namespace std::chrono;
ActualMaterial::ActualMaterial(Texture* pDifuse1, Texture* pDifuse2, Texture* pDifuse3, Texture* pSplatMap, Texture* pNormalMap, Texture* pGlossMap,/* float tHeightMultiplier,*/
	glm::vec3 pDiffuseColor, glm::vec3 pSpecularColor, float pShinyness, float pScrollMultiplier1, float pScrollMultiplier2,
	float pScrollMultiplier3, float transparency1, float transparency2, float transparency3, float pUseCompleteReflection, bool pUseNormalMap, bool pUseGlossMap) :
	_transparency1(transparency1), _transparency2(transparency2), _transparency3(transparency3),/* _heightMultiplier(tHeightMultiplier),*/ _difuse1(pDifuse1),
	_difuse2(pDifuse2), _difuse3(pDifuse3), _scrollMultiplier1(pScrollMultiplier1), _scrollMultiplier2(pScrollMultiplier2), _scrollMultiplier3(pScrollMultiplier3), _splatMap(pSplatMap), _diffuseColor(pDiffuseColor), _specularColor(pSpecularColor),
	_shinyness(pShinyness), _normalMap(pNormalMap), _useNormalMap(pUseNormalMap), _glossMap(pGlossMap), _useGlossMap(pUseGlossMap), _useCompleteReflection(pUseCompleteReflection)
{
	t1 = high_resolution_clock::now();
	_lazyInitializeShader();
	_gateSpeed = GameManager::GetGateSpeed();
}

ActualMaterial::~ActualMaterial() {}

void ActualMaterial::_lazyInitializeShader() {
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "actualTexture.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "actualTexture.fs");
		_shader->finalize();

		//cache all the uniform and attribute indexes
		_uMVPMatrix = _shader->getUniformLocation("mvpMatrix");
		_uDiffuseTexture1 = _shader->getUniformLocation("diffuseTexture1");
		_uDiffuseTexture2 = _shader->getUniformLocation("diffuseTexture2");
		_uDiffuseTexture3 = _shader->getUniformLocation("diffuseTexture3");
		_uNormalMap = _shader->getUniformLocation("normalMap");
		_uSplatMap = _shader->getUniformLocation("splatMap");
		_uGlossMap = _shader->getUniformLocation("glossMap");

		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");
		_aTangent = _shader->getAttribLocation("tangent");
		_aBitangent = _shader->getAttribLocation("bitangent");
	}
}

void ActualMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	if (!_difuse1) return;
	glEnable(GL_BLEND); //
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//glDisable(GL_DEPTH_TEST);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	//glEnable(GL_POLYGON_SMOOTH);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	//glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	//glDepthMask(false);
	_shader->use();
	//Light* tLight = pWorld->getLightAt(0);

	//glm::vec3 ambientLightColor = tLight->GetAmbientLightColor();
	//glm::vec3 lightColor = tLight->GetLightColor();
	//glm::vec3 direction = tLight->GetDirection();

	t2 = high_resolution_clock::now();

	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

	_time = time_span.count();


	//Print the number of lights in the scene and the position of the first light.
	//It is not used, but this demo is just meant to show you THAT materials can access the lights in a world
	//if (pWorld->getLightCount() > 0) {
	//    std::cout << "TextureMaterial has discovered light is at position:" << pWorld->getLightAt(0)->getLocalPosition() << std::endl;
	//}

	//setup texture slot 0
	//bind the texture to the current active slot


	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, _splatMap->getId());
	glUniform1i(_uSplatMap, 5);
	////pass in a precalculate mvp matrix (see texture material for the opposite)
	//glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	//glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

	glUniformMatrix4fv(_shader->getUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("viewMatrix"), 1, GL_FALSE, glm::value_ptr(pViewMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(pModelMatrix));

	GLint timeLoc = _shader->getUniformLocation("time");
	glUniform1f(timeLoc, _time);//tell the shader the texture slot for the diffuse texture is slot 0

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
	glBindTexture(GL_TEXTURE_2D, _normalMap->getId());
	glUniform1i(_uNormalMap, 4);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, _glossMap->getId());
	glUniform1i(_uGlossMap, 6);

	glUniform1i(_shader->getUniformLocation("useNormalMap"), _useNormalMap);
	glUniform1i(_shader->getUniformLocation("useGlossMap"), _useGlossMap);
	glUniform1f(_shader->getUniformLocation("useCompleteReflection"), _useCompleteReflection);

	glUniform3fv(_shader->getUniformLocation("diffuseColor"), 1, glm::value_ptr(_diffuseColor));
	int lightNr = pWorld->getLightCount();
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
	glUniform1f(_shader->getUniformLocation("transparency1"), _transparency1);
	glUniform1f(_shader->getUniformLocation("transparency2"), _transparency2);
	glUniform1f(_shader->getUniformLocation("transparency3"), _transparency3);
	glUniform1i(_shader->getUniformLocation("lightNr"), lightNr);
	glUniform1f(_shader->getUniformLocation("scrollMultiplier1"), _scrollMultiplier1*(*_gateSpeed));
	glUniform1f(_shader->getUniformLocation("scrollMultiplier2"), _scrollMultiplier2*(*_gateSpeed));
	glUniform1f(_shader->getUniformLocation("scrollMultiplier3"), _scrollMultiplier3*(*_gateSpeed));
	glUniform3fv(_shader->getUniformLocation("specularColor"), 1, glm::value_ptr(_specularColor));
	glUniform3fv(_shader->getUniformLocation("cameraPos"), 1, glm::value_ptr(pWorld->getMainCamera()->getWorldPosition()));

	glUniform1f(_shader->getUniformLocation("scale"), 2);


	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV, _aTangent, _aBitangent);

	//glDepthMask(true);

	//glDisable(GL_POLYGON_SMOOTH); 
	//glEnable(GL_DEPTH_TEST);
}
