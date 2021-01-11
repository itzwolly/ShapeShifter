#ifndef DEBUGHUD_HPP
#define DEBUGHUD_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>
#include "glm.hpp"
#include <map>

/**
 * Very simple class capable of rendering a simple piece of text through SFML.
 */
class DebugHud {
	public:
		static DebugHud& Instance() {
			static DebugHud instance;
			return instance;
		}
		DebugHud(); // sf::RenderWindow * aWindow
		virtual ~DebugHud();
		void initialize(sf::RenderWindow * aWindow);
		void draw();
		void initializeText(std::string pId, int pSize, glm::vec4 pColor);
		void setText (std::string pId, std::string pText, glm::vec2 pPos, glm::vec2 pAnchor, bool pCenterAlign = false);

	private:
		sf::RenderWindow * _window;
        std::string _debugInfo;
        sf::Font _font;

        sf::Text _debugText;
		std::map<std::string, sf::Text>* _textMap;
		float clamp(float x, float upper, float lower);


		DebugHud(DebugHud const&) = delete;
		void operator=(DebugHud const&) = delete;
};

#endif // DEBUGHUD_HPP
