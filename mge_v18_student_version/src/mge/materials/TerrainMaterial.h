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
class TerrainMaterial : public AbstractMaterial
{
public:
	TerrainMaterial(Texture* pDiffuseTexture, Texture* pMainTex, Texture* pDifuse1,
		Texture* pDifuse2, Texture* pDifuse3, Texture* pDifuse4, Texture* pSplatMap, Texture* pWaterTexture, float tHeightMultiplier=1,
		glm::vec3 pDiffuseCOlor = glm::vec3(1, 1, 1), glm::vec3 pSpecularColor = glm::vec3(1, 1, 1), float pShinyness = 5);
	virtual ~TerrainMaterial();

	virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

	void setDiffuseTexture(Texture* pDiffuseTexture);

protected:
private:
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();

	glm::vec3 _diffuseColor;
	float _shinyness;
	glm::vec3 _specularColor;

	//in this example we cache all identifiers for uniforms & attributes
	static GLint _uMVPMatrix;
	static GLint _uDiffuseTexture1;
	static GLint _uDiffuseTexture2;
	static GLint _uDiffuseTexture3;
	static GLint _uDiffuseTexture4;
	static GLint _uMainTexture;
	static GLint _uHeightMap;
	static GLint _uSplatMap;
	static GLint _uWaterTexture;

	static GLint _aVertex;
	static GLint _aNormal;
	static GLint _aUV;

	Texture* _heightMap;
	Texture* _mainTex;
	Texture* _difuse1;
	Texture* _difuse2;
	Texture* _difuse3;
	Texture* _difuse4;
	Texture* _splatMap;
	Texture* _waterTexture;


	float _heightMultiplier;

	TerrainMaterial(const TerrainMaterial&);
	TerrainMaterial& operator=(const TerrainMaterial&);


	high_resolution_clock::time_point t1;
	high_resolution_clock::time_point t2;

	float _time;

};
