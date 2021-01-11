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
class FlatTextTexture : public AbstractMaterial
{
    public:
		FlatTextTexture(GameObject* object, std::string message, glm::vec3 color, float x, float y, float time = 5, float speed = 0, glm::vec2 direction = glm::vec2(0, 1), int fontSize = 48);
        virtual ~FlatTextTexture();

        virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;
        //void setDiffuseTexture (Texture* pDiffuseTexture);

    protected:
    private:
		void _renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, GLuint VAO, GLuint VBO);

		struct Character {
			GLuint     TextureID;  // ID handle of the glyph texture
			glm::ivec2 Size;       // Size of glyph
			glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
			GLuint     Advance;    // Offset to advance to next glyph
		};

		GameObject* _obj;

		std::map<GLchar, Character> _characters;
		FT_Library _ft;
		FT_Face _face;
		std::string _message;
		glm::vec3 _color;
		float _x;
		float _y;
		float _duration;
		float _speed;
		glm::vec2 _direction;

        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

        ////in this example we cache all identifiers for uniforms & attributes
        //static GLint _uMVPMatrix;
        //static GLint _uDiffuseTexture;

        static GLint _aVertex ;
        static GLint _aNormal;
        static GLint _aUV ;

        //Texture* _diffuseTexture;

		FlatTextTexture(const FlatTextTexture&);
		FlatTextTexture& operator=(const FlatTextTexture&);

};

