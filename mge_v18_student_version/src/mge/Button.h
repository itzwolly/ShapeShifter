#pragma once
#include "mge\core\GameObject.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include <SFML/Graphics.hpp>

class AbstractGame;
class Button :
	public GameObject
{
public:
	Button(TextureMaterial* pressedTexture, TextureMaterial* notPressedTexture,AbstractGame* current, AbstractGame* next,sf::Vector2f size,sf::Vector2f pos, const glm::vec3& pPosition = glm::vec3(0.0f, 0.0f, 0.0f),
		bool visible = true, bool clickable = true, const std::string& pName = "button", sf::Color colPressed = sf::Color(255, 255, 255, 255), sf::Color colNotPressed = sf::Color(255, 255, 255, 155));
	~Button();
	sf::RectangleShape* GetRectangle();
	bool IsClickable();
	void SetClickable();
	void SetClickable(bool val);
	sf::Color GetColorPressed();
	sf::Color GetColorNotPressed();
	void Press(bool val);
	void Clicked();
private:
	bool _pressed;
	AbstractGame* _current;
	AbstractGame* _next;

	TextureMaterial* _pressedTexture;
	TextureMaterial* _notPressedTexture;

	sf::Color _pressedCol;
	sf::Color _notPressedCol;
	sf::RectangleShape* _boundaryBox;
	bool _clickable;
};

