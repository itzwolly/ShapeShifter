
#include "GL/glew.h"
#include "mge/materials/AbstractMaterial.hpp"
#include "mge/core/Camera.hpp"

class ShaderProgram;

/**
* This is about the simplest material we can come up with, it demonstrates how to
* render a single color material without caching, passing in all the matrices we require separately.
*/
class LitMaterial : public AbstractMaterial
{
public:
	LitMaterial(glm::vec3 pColor = glm::vec3(1, 0, 0),float pShinyness = 5, glm::vec3 pSpecularColor = glm::vec3(1, 1, 1));
	virtual ~LitMaterial();

	virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

	//in rgb values
	void setDiffuseColor(glm::vec3 pDiffuseColor);

private:
	//all the static properties are shared between instances of ColorMaterial
	//note that they are all PRIVATE, we do not expose this static info to the outside world
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();

	//this one is unique per instance of color material

	//Camera* _camera;
	glm::vec3 _diffuseColor;
	float _shinyness;
	glm::vec3 _specularColor;
};

