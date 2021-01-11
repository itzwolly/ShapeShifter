#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <fstream>
#include <set>
#include <iostream>
class KeyHandler
{
public:
	static bool KeyPressed(sf::Keyboard::Key Key);
	static bool KeyDown(sf::Keyboard::Key Key);
	static bool KeyUp(sf::Keyboard::Key Key);
	static bool KeyPressed(std::string);
	static bool KeyDown(std::string);
	static bool KeyUp(std::string);
	static sf::Keyboard::Key GetKeyCode(std::string);
	static void NewEvent(sf::Event event);
	static void Reset();
	static sf::Keyboard::Key ConvertIntoKey(std::string key); 
private:
	static std::set<sf::Keyboard::Key> _keys;
	static std::set<sf::Keyboard::Key> _prevKeys;
	static std::set<sf::Keyboard::Key>* _justPressed;
	static std::set<sf::Keyboard::Key>* _justReleased;
};

