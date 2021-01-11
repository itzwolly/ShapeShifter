#pragma once

#include "mge/materials/AbstractMaterial.hpp"
#include "GL/glew.h"
#include <chrono>

class ShaderProgram;
class Texture;

using namespace std::chrono;
/**
* This material is already a little bit more complicated, instead of a color we can pass in a texture,
* all attributes and uniforms are cached and we precompute the MVP matrix passing it in as one entity.
*/
class ActualMaterial : public AbstractMaterial
{
public:
	ActualMaterial(Texture* pDifuse1, Texture* pDifuse2, Texture* pDifuse3, Texture* pSplatMap, Texture* pNormalMap, Texture* pGlossMap,/* float tHeightMultiplier = 1,*/
		glm::vec3 pDiffuseCOlor = glm::vec3(1, 1, 1), glm::vec3 pSpecularColor = glm::vec3(1, 1, 1), float pShinyness = 5, float pScrollMultiplier1 = 1, float pScrollMultiplier2 = 1,
		float pScrollMultiplier3 = 1, float transparency1 = 1, float transparency2 = 1, float transparency3 = 1, float useCompleteReflection = 0, bool useNormalMap = true, bool useGlossMap = true);
	virtual ~ActualMaterial();

	virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

protected:
private:
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();

	glm::vec3 _diffuseColor;
	float _shinyness;
	glm::vec3 _specularColor;
	float* _gateSpeed;
	float _transparency1;
	float _transparency2;
	float _transparency3;

	bool _useNormalMap;
	bool _useGlossMap;
	float _useCompleteReflection;

	//in this example we cache all identifiers for uniforms & attributes
	static GLint _uMVPMatrix;
	static GLint _uDiffuseTexture1;
	static GLint _uDiffuseTexture2;
	static GLint _uDiffuseTexture3;
	static GLint _uNormalMap;
	static GLint _uGlossMap;
	static GLint _uSplatMap;

	static GLint _aVertex;
	static GLint _aNormal;
	static GLint _aUV;
	static GLint _aTangent;
	static GLint _aBitangent;

	//Texture* _heightMap;
	//Texture* _mainTex;
	Texture* _difuse1;
	Texture* _difuse2;
	Texture* _difuse3;
	Texture* _splatMap;
	Texture* _normalMap;
	Texture* _glossMap;

	//float _heightMultiplier;
	float _scrollMultiplier1;
	float _scrollMultiplier2;
	float _scrollMultiplier3;

	ActualMaterial(const ActualMaterial&);
	ActualMaterial& operator=(const ActualMaterial&);


	high_resolution_clock::time_point t1;
	high_resolution_clock::time_point t2;

	float _time;
};
