#ifndef NUMBERTEXTUREMATERIAL_HPP
#define NUMBEREXTUREMATERIAL_HPP

#include "mge/materials/AbstractMaterial.hpp"
#include "GL/glew.h"
#include "ft2build.h"
#include FT_FREETYPE_H  
#include <map>

class ShaderProgram;
class Texture;

/**
 * This material is already a little bit more complicated, instead of a color we can pass in a texture,
 * all attributes and uniforms are cached and we precompute the MVP matrix passing it in as one entity.
 */
class NumberTextureMaterial : public AbstractMaterial
{
    public:
		NumberTextureMaterial(Texture* pDiffuseTexture, int fontSize=48);
        virtual ~NumberTextureMaterial();

        virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

        void setDiffuseTexture (Texture* pDiffuseTexture);

    protected:
    private:
		void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, GLuint VAO, GLuint VBO);

		struct Character {
			GLuint     TextureID;  // ID handle of the glyph texture
			glm::ivec2 Size;       // Size of glyph
			glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
			GLuint     Advance;    // Offset to advance to next glyph
		};

		std::map<GLchar, Character> Characters;
		FT_Library ft;
		FT_Face face;


        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

        //in this example we cache all identifiers for uniforms & attributes
        static GLint _uMVPMatrix;
        static GLint _uDiffuseTexture;

        static GLint _aVertex ;
        static GLint _aNormal;
        static GLint _aUV ;

        Texture* _diffuseTexture;

		NumberTextureMaterial(const NumberTextureMaterial&);
		NumberTextureMaterial& operator=(const NumberTextureMaterial&);

};

#endif // TEXTUREMATERIAL_HPP
