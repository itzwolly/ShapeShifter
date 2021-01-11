#include "glm.hpp"

#include "ShadowGen.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/config.hpp"
#include "mge/GameManager.h"

ShaderProgram* ShadowGen::_shader = NULL;

GLint ShadowGen::_uMVPMatrix = 0;
GLint ShadowGen::_uDiffuseTexture = 0;

GLint ShadowGen::_aVertex = 0;
GLint ShadowGen::_aNormal = 0;
GLint ShadowGen::_aUV = 0;


ShadowGen::ShadowGen(Texture * pDiffuseTexture) :_diffuseTexture(pDiffuseTexture) {
	_lazyInitializeShader();
}

ShadowGen::~ShadowGen() {}

void ShadowGen::_lazyInitializeShader() {
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "shadowBuffer.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "shadowBuffer.fs");
		_shader->finalize();

		//cache all the uniform and attribute indexes
		_uMVPMatrix = _shader->getUniformLocation("mvpMatrix");
		_uDiffuseTexture = _shader->getUniformLocation("diffuseTexture");

		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");
	}
}

void ShadowGen::setDiffuseTexture(Texture* pDiffuseTexture) {
	_diffuseTexture = pDiffuseTexture;
}

void ShadowGen::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	if (!_diffuseTexture) return;

	_shader->use();

	//Print the number of lights in the scene and the position of the first light.
	//It is not used, but this demo is just meant to show you THAT materials can access the lights in a world
	//if (pWorld->getLightCount() > 0) {
	//    std::cout << "TextureMaterial has discovered light is at position:" << pWorld->getLightAt(0)->getLocalPosition() << std::endl;
	//}

	//setup texture slot 0
	//glActiveTexture(GL_TEXTURE0);
	////bind the texture to the current active slot
	//glBindTexture(GL_TEXTURE_2D, *GameManager::GetShadowMapInt());
	////tell the shader the texture slot for the diffuse texture is slot 0
	//glUniform1i(_uDiffuseTexture, 0);

	/*GLint m_viewpower[4];
glGetInteger(GL_VIEWPORT,m_viewport)

configure shader and matrices - light projectios * inverse of light position
render scene - uise only meshes not materials, from rendering of children in renderer

glViewport(m_viewport[0],1,2,3);

render material*/

	/**/
	glm::vec3 lightInvDir = glm::vec3(0,-1,0);

	// Compute the MVP matrix from the light's point of view
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, 0.1f, 20);
	glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, pWorld->getLightAt(0)->getWorldPosition(), glm::vec3(0, 1, 0));
	glm::mat4 depthModelMatrix = glm::mat4(1.0);
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, &depthMVP[0][0]);

	//pass in a precalculate mvp matrix (see texture material for the opposite)
	//glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	//glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV, -1, -1);
}
