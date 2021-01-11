#include "glm.hpp"

#include "mge/materials/FlatTextTexture.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/config.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#define GLEW_STATIC
using namespace std::chrono;

ShaderProgram* FlatTextTexture::_shader = NULL;

GLint FlatTextTexture::_aVertex = 0;
GLint FlatTextTexture::_aNormal = 0;
GLint FlatTextTexture::_aUV = 0;


FlatTextTexture::FlatTextTexture(GameObject* obj, std::string message, glm::vec3 color, float x, float y, float time, float speed, glm::vec2 direction, int fontSize) :
	_color(color), _message(message), _x(x), _y(y), _duration(time), _speed(speed), _direction(direction), _obj(obj)
{
    _lazyInitializeShader();

	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	if (FT_Init_FreeType(&_ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	if (FT_New_Face(_ft, "mge/fonts/arial.ttf", 0, &_face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	FT_Set_Pixel_Sizes(_face, 0, fontSize);

	//if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
	//	std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl; glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(_face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			_face->glyph->bitmap.width,
			_face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			_face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(_face->glyph->bitmap.width, _face->glyph->bitmap.rows),
			glm::ivec2(_face->glyph->bitmap_left, _face->glyph->bitmap_top),
			_face->glyph->advance.x
		};
		_characters.insert(std::pair<GLchar, Character>(c, character));
	}

	FT_Done_Face(_face);
	FT_Done_FreeType(_ft);
}

FlatTextTexture::~FlatTextTexture() {}

void FlatTextTexture::_lazyInitializeShader() {
    if (!_shader) {
        _shader = new ShaderProgram();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH+"2DTextTexture.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH+"2DTextTexture.fs");
        _shader->finalize();

        //cache all the uniform and attribute indexes

        _aVertex = _shader->getAttribLocation("vertex");
        _aNormal = _shader->getAttribLocation("normal");
        _aUV =     _shader->getAttribLocation("uv");
    }
}
//
//void FlatTextTexture::setDiffuseTexture (Texture* pDiffuseTexture) {
//    _diffuseTexture = pDiffuseTexture;
//}
void FlatTextTexture::_renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, GLuint VAO, GLuint VBO)
{
	/*std::string text = "ABCDEGHIJKLMONPQRSTUVWXYZ";
	GLfloat x = 0;
	GLfloat y = 300;
	GLfloat scale = 1.0f;
	glm::vec3 color = glm::vec3(0.5, 0.8f, 0.2f);*/


	glUniform3f(_shader->getUniformLocation("textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = _characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
}

void FlatTextTexture::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    if (_message.size()==0) return;
	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	//duration<double> time_span = duration_cast<std::chrono::seconds>(t2 - t1);

	double time =_obj->GetLifeTime();
	//std::cout << time << "\n";
	if (time > _duration)
	{/*
		pWorld->remove(_obj);
		delete(_obj);*/
		//std::cout << time<<" delete/n";
		_toDel = true;
		return;
	}
	float x = _x + time / _duration*_speed*_direction.x;
	float y = _y + time / _duration*_speed*_direction.y;


	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _shader->use();
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);//= pProjectionMatrix * pViewMatrix * pModelMatrix;// 
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	glUniformMatrix4fv(_shader->getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));


	_renderText(_message,x,y, 1.0f, _color,VAO, VBO);
	//RenderText("(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f), VAO, VBO);
	
	/*
    //setup texture slot 0
    glActiveTexture(GL_TEXTURE0);
    //bind the texture to the current active slot
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
    //tell the shader the texture slot for the diffuse texture is slot 0
    glUniform1i (_uDiffuseTexture, 0);

    //pass in a precalculate mvp matrix (see texture material for the opposite)

    //now inform mesh of where to stream its data*/
    pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV, -1, -1);
	glDisable(GL_BLEND);
}
