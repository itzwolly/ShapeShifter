#include "mge/core/KeyHandler.h"
#include <iostream>

std::set<sf::Keyboard::Key> KeyHandler::_keys = std::set<sf::Keyboard::Key>();
std::set<sf::Keyboard::Key> KeyHandler::_prevKeys = std::set<sf::Keyboard::Key>();
std::set<sf::Keyboard::Key>* KeyHandler::_justPressed = new std::set<sf::Keyboard::Key>();
std::set<sf::Keyboard::Key>* KeyHandler::_justReleased = new std::set<sf::Keyboard::Key>();

bool KeyHandler::KeyDown(sf::Keyboard::Key key)
{
	return (_keys.find(key) != _keys.end());
}

bool KeyHandler::KeyPressed(sf::Keyboard::Key key)
{
	//return (_justPressed->find(key) != _justPressed->end());
	return ((_keys.find(key) != _keys.end() && (_prevKeys.find(key) == _prevKeys.end())));

	return false;
}

bool KeyHandler::KeyUp(sf::Keyboard::Key key)
{
	//return (_justReleased->find(key) != _justReleased->end());
	return ((_keys.find(key) == _keys.end() && (_prevKeys.find(key) != _prevKeys.end())));

	return false;
}

bool KeyHandler::KeyPressed(std::string s)
{
	return (_justPressed->find(ConvertIntoKey(s)) != _justPressed->end());
}

bool KeyHandler::KeyDown(std::string s)
{
	return (_keys.find(ConvertIntoKey(s)) != _keys.end());
}

bool KeyHandler::KeyUp(std::string s)
{
	//std::cout << _justReleased->size();
	return (_justReleased->find(ConvertIntoKey(s)) != _justReleased->end());
}

sf::Keyboard::Key KeyHandler::GetKeyCode(std::string)
{
	return sf::Keyboard::Key();
}
sf::Keyboard::Key KeyHandler::ConvertIntoKey(std::string key)
{
	if (key == "a")
		return sf::Keyboard::Key::A;

	if (key == "s")
		return sf::Keyboard::Key::S;

	if (key == "d")
		return sf::Keyboard::Key::D;

	if (key == "w")
		return sf::Keyboard::Key::W;

	if (key == "space")
		return sf::Keyboard::Key::Space;

	if (key == "escape")
		return sf::Keyboard::Key::Escape;

	if (key == "q")
		return sf::Keyboard::Key::Q;

	if (key == "e")
		return sf::Keyboard::Key::E;

	if (key == "shift")
		return sf::Keyboard::Key::LShift;

	if (key == "enter")
		return sf::Keyboard::Key::Return;

	if (key == "r")
		return sf::Keyboard::Key::R;

	if (key == "up")
		return sf::Keyboard::Key::Up;

	if (key == "down")
		return sf::Keyboard::Key::Down;

	if (key == "left")
		return sf::Keyboard::Key::Left;

	if (key == "right")
		return sf::Keyboard::Key::Right;

	if (key == "1")
		return sf::Keyboard::Key::Num1;

	if (key == "2")
		return sf::Keyboard::Key::Num2;

	if (key == "3")
		return sf::Keyboard::Key::Num3;

	if (key == "4")
		return sf::Keyboard::Key::Num4;

	if (key == "5")
		return sf::Keyboard::Key::Num5;

	if (key == "6")
		return sf::Keyboard::Key::Num6;

	if (key == "7")
		return sf::Keyboard::Key::Num7;

	if (key == "8")
		return sf::Keyboard::Key::Num8;

	if (key == "9")
		return sf::Keyboard::Key::Num9;



	throw std::invalid_argument("that key is not yet implemented");
}

void KeyHandler::NewEvent(sf::Event event)
{
	//if (event.type != sf::Event::TextEntered)
	{
		//std::cout << event.type;
		_prevKeys = _keys;/*
		_justPressed->clear();
		_justReleased->clear();*/
		if (event.type == sf::Event::KeyPressed)
		{/*
			if (!(_keys->find(event.key.code) != _keys->end()))
				_justPressed->insert(event.key.code);*/
			_keys.insert(event.key.code);
		}
		if (event.type == sf::Event::KeyReleased)
		{
			_keys.erase(event.key.code);
			//_justReleased->insert(event.key.code);
		}
		//std::cout << _keys.size() << " - old " << _prevKeys.size() << "\n";
		//std::cout << _justPressed->size() << " - " << _justReleased->size() << "\n";
	}
}

void KeyHandler::Reset()
{
	_prevKeys = _keys;
}
