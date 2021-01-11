#pragma once
#include "mge\materials\AbstractMaterial.hpp"
#include "GL/glew.h"

class ShaderProgram;
class Texture;

/**
* This material is already a little bit more complicated, instead of a color we can pass in a texture,
* all attributes and uniforms are cached and we precompute the MVP matrix passing it in as one entity.
*/
class TextureWithLightMaterial : public AbstractMaterial
{
public:
	TextureWithLightMaterial(Texture* pDiffuseTexture, glm::vec3 pColor = glm::vec3(1, 1, 1), float pShinyness = 5, float pTransparency=1, glm::vec3 pSpecularColor = glm::vec3(1, 1, 1));
	virtual ~TextureWithLightMaterial();

	virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

	void setDiffuseTexture(Texture* pDiffuseTexture);

protected:
private:
	float _transparency;
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();

	//in this example we cache all identifiers for uniforms & attributes
	static GLint _uDiffuseTexture;

	static GLint _aVertex;
	static GLint _aNormal;
	static GLint _aUV;

	Texture* _diffuseTexture;
	glm::vec3 _diffuseColor;
	float _shinyness;
	glm::vec3 _specularColor;

	TextureWithLightMaterial(const TextureWithLightMaterial&);
	TextureWithLightMaterial& operator=(const TextureWithLightMaterial&);

};



