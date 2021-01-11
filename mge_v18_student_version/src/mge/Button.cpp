#include "Button.h"
#include "mge/core/AbstractGame.hpp"


Button::Button(TextureMaterial* pressedTexture, TextureMaterial* notPressedTexture, AbstractGame* current, AbstractGame* next, sf::Vector2f size,
	sf::Vector2f pos, const glm::vec3 & pPosition, bool visible,bool clickable
	,const std::string & pName, sf::Color colPressed, sf::Color colNotPressed)
	: GameObject(pName, "Button", pPosition,visible),_clickable(clickable),_pressedCol(colPressed),_notPressedCol(colNotPressed),_next(next),_current(current),
	_pressedTexture(pressedTexture),_notPressedTexture(notPressedTexture),_pressed(false)
{
	setMaterial(notPressedTexture);
	_boundaryBox = new sf::RectangleShape(size);
	_boundaryBox->setPosition(pos);
	SetTag("Button");
}

Button::~Button()
{
}

sf::RectangleShape * Button::GetRectangle()
{
	return _boundaryBox;
}

bool Button::IsClickable()
{
	return _clickable;
}

void Button::SetClickable()
{
	_clickable = !_clickable;
}

void Button::SetClickable(bool val)
{
	_clickable = val;
}

sf::Color Button::GetColorPressed()
{
	return _pressedCol;
}

sf::Color Button::GetColorNotPressed()
{
	return _notPressedCol;
}

void Button::Press(bool val)
{
	if (_clickable && _pressed != val)
	{
		_pressed = val;
		if (val)
		{
			scale(glm::vec3(1.1f,1.1f,1.1f));
			setMaterial(_pressedTexture);
		}
		else
		{
			scale(glm::vec3(0.90909090909f, 0.90909090909f, 0.90909090909f));
			setMaterial(_notPressedTexture);
		}
	}
}

void Button::Clicked()
{
	_current->Stop();
	_next->initialize();
	_next->run();
}
