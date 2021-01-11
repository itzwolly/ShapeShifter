#include "mge\core\GameObject.hpp"
#include "mge\behaviours\PlayerBehaviour.h"
#include "mge\core\KeyHandler.h"
#include "mge/config.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/PhysicsManager.h"
#include "mge/Gate.h"
#include "mge/GameManager.h"

PlayerBehaviour::PlayerBehaviour(GameObject* pAnchor, float pPadding, Mesh* pMeshOne, Mesh* pMeshTwo, Mesh* pMeshThree, std::string swapSound, std::string swapSphereSound, std::string swapCubeSound, std::string swapPyramidSound) 
	: _anchor(pAnchor), _padding(pPadding), _meshOne(pMeshOne), _meshTwo(pMeshTwo), _meshThree(pMeshThree) {
	_anchorTransform = _anchor->getTransform();
	_anchorScale = glm::length(glm::vec3(_anchorTransform[0].x, _anchorTransform[1].x, _anchorTransform[2].x));

	if (!_swapBuffer.loadFromFile(swapSound)) {
		throw new std::exception("swap sound doesn't exist");
	}
	_swapSound.setBuffer(_swapBuffer);

	if (!_cubeBuffer.loadFromFile(swapCubeSound)) {
		throw new std::exception("cube sound doesn't exist");
	}
	_cubeSound.setBuffer(_cubeBuffer);

	if (!_sphereBuffer.loadFromFile(swapSphereSound)) {
		throw new std::exception("cube sound doesn't exist");
	}
	_sphereSound.setBuffer(_sphereBuffer);

	if (!_pyramidBuffer.loadFromFile(swapPyramidSound)) {
		throw new std::exception("cube sound doesn't exist");
	}
	_pyramidSound.setBuffer(_pyramidBuffer);

	_swapSound.setVolume(40);
	_cubeSound.setVolume(30);
	_sphereSound.setVolume(30);
	_pyramidSound.setVolume(30);
}

void PlayerBehaviour::update(float pStep) {
	if (!GameManager::_paused) {
		HandleStates();
		SetShape(_nextState);

		Gate* pointer = static_cast<Gate*>(_owner->CollisionEnter("Gate"));
		if (pointer != NULL) {
			if ((pointer->GetColorType() == _owner->GetColorType() || pointer->GetColorType() == 3) && (pointer->GetMeshType() == _owner->GetMeshType() || pointer->GetMeshType() == 3)) {
				GameManager::PlayerWentThroughtCorrectGate();
			}
		}
	}
}

void PlayerBehaviour::HandleStates() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
		_nextState = PlayerBehaviour::State::DEFAULT;
		_owner->SetMeshType(0); // cube
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
		_nextState = PlayerBehaviour::State::SHAPE_TWO;
		_owner->SetMeshType(1); // sphere
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
		_nextState = PlayerBehaviour::State::SHAPE_THREE;
		_owner->SetMeshType(2); // triangle
	}
}

void PlayerBehaviour::Move(sf::Keyboard::Key pKey) {
	// initializing values only once.
	if (_playerScale == -1) { // The scale shouldn't be negative.
		glm::mat4x4 playerTransform = _owner->getTransform();
		_playerScale = glm::length(glm::vec3(playerTransform[0].x, playerTransform[1].x, playerTransform[2].x));
	}

	if (pKey == sf::Keyboard::A) {
		float ownerX = _owner->getLocalPosition().x;
		float anchorX = _anchor->getLocalPosition().x;

		if (ownerX >= anchorX - ownerX) {
			_owner->translate(glm::vec3(-(_anchorScale - _playerScale - _padding), 0, 0));
			_swapSound.play();
		}
	} else if (pKey == sf::Keyboard::D) {
		float ownerX = _owner->getLocalPosition().x;
		float anchorX = _anchor->getLocalPosition().x;

		if (ownerX <= anchorX - ownerX) {
			_owner->translate(glm::vec3(_anchorScale - _playerScale - _padding, 0, 0));
			_swapSound.play();
		}
	}
}

void PlayerBehaviour::SetShape(State pState) {
	if (pState != _currentState) {
		_owner->setMesh(GetMesh(pState));
		_currentState = pState;
	}
}

Mesh* PlayerBehaviour::GetMesh(State pState) {
	switch (pState) {
		case PlayerBehaviour::State::SHAPE_TWO:
			_pyramidSound.play();
			return _meshThree;
		case PlayerBehaviour::State::SHAPE_THREE:
			_sphereSound.play();
			return _meshTwo;
		case PlayerBehaviour::State::DEFAULT:
		default:
			_cubeSound.play();
			return _meshOne; // default mesh
	}
}

PlayerBehaviour::~PlayerBehaviour() { }