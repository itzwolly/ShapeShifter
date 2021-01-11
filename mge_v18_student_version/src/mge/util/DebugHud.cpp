#include <cassert>
#include <iostream>

#include <GL/glew.h>
#include "mge/util/DebugHud.hpp"
#include "mge/config.hpp"
#include "mge/core/AbstractGame.hpp"

DebugHud::DebugHud() : _debugInfo(), _font(), _debugText() { }

DebugHud::~DebugHud() {
	//dtor
	delete _textMap;
}

void DebugHud::initialize(sf::RenderWindow * aWindow) {
	assert(aWindow != NULL);
	_window = aWindow;

	if (!_font.loadFromFile(config::MGE_FONT_PATH + "Prototype.ttf")) {
		std::cout << "Could not load font, exiting..." << std::endl;
		return;
	}

	_textMap = new std::map<std::string, sf::Text>();
	initializeText("fps", 16, glm::vec4(1, 1, 1, 1));
}

float DebugHud::clamp(float x, float upper, float lower) {
	return std::min(upper, std::max(x, lower));
}

void DebugHud::initializeText(std::string pId, int pSize, glm::vec4 pColor) {
	std::transform(pId.begin(), pId.end(), pId.begin(), ::tolower);
	if (_textMap->count(pId) == 0) {
		float r = clamp(pColor.r, 1, 0) * 255;
		float g = clamp(pColor.g, 1, 0) * 255;
		float b = clamp(pColor.b, 1, 0) * 255;
		float a = clamp(pColor.a, 1, 0) * 255;

		sf::Text txt;
		txt.setString("");
		txt.setFont(_font);
		txt.setCharacterSize(pSize);
		txt.setFillColor(sf::Color::Color(r, g, b, a));

		_textMap->insert(std::pair<std::string, sf::Text>(pId, txt));
		
		int count = _textMap->size();
	} else {
		std::cout << "Map with key: " << pId << " already initialized!" << std::endl;
	}
}

void DebugHud::setText(std::string pId, std::string pText, glm::vec2 pPos, glm::vec2 pAnchor, bool pCenterAlign) {
	std::transform(pId.begin(), pId.end(), pId.begin(), ::tolower);
	if (_textMap->count(pId) == 1) {
		float anchorX = clamp(pAnchor.x, 1, 0);
		float anchorY = clamp(pAnchor.y, 1, 0);

		sf::Text txt = _textMap->at(pId);
		if (txt.getString() != pText) {
			txt.setString(pText);

			float width = txt.getLocalBounds().width;
			float height = txt.getLocalBounds().height;

			float newX;
			float newY;

			if (pAnchor.x == 1) {
				newX = (anchorX * AbstractGame::windowSize->x) + pPos.x - (width + 5);
			} else {
				newX = (anchorX * AbstractGame::windowSize->x) + pPos.x;
			}

			if (pAnchor.y == 1) {
				newY = (anchorY * AbstractGame::windowSize->y) + pPos.y - (height - 10);
			} else {
				newY = (anchorY * AbstractGame::windowSize->y) + pPos.y;
			}

			if (pCenterAlign) {
				newX -= width / 2;
			}

			txt.setPosition(newX, newY);
			_textMap->at(pId) = txt;
		}
	}
}

void DebugHud::draw() {
	//glDisable( GL_CULL_FACE );
	glActiveTexture(GL_TEXTURE0);
    _window->pushGLStates();
	if (_textMap->size() > 0) {
		for (std::map<std::string, sf::Text>::iterator it = _textMap->begin(); it != _textMap->end(); ++it) {
			_window->draw(it->second);
		}
	}
	_window->popGLStates();
}
