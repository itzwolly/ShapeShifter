#ifndef PLAYERBEHAVIOUR_HPP
#define PLAYERBEHAVIOUR_HPP

#include "mge\behaviours\AbstractBehaviour.hpp"
#include "SFML\Window.hpp"
#include "SFML/Audio.hpp"

class PlayerBehaviour : public AbstractBehaviour {
	public:
		enum State {
			NONE,
			DEFAULT,
			SHAPE_TWO,
			SHAPE_THREE
		};

		PlayerBehaviour(GameObject* pAnchor, float pPadding, Mesh* pMeshOne, Mesh* pMeshTwo, Mesh* pMeshThree, std::string swapSound, std::string swapSphereSound, std::string swapCubeSound, std::string swapPyramidSound);
		virtual ~PlayerBehaviour();

		virtual void update(float pStep);
		void Move(sf::Keyboard::Key pKey);
	
	private:
		sf::Sound _swapSound;
		sf::SoundBuffer _swapBuffer;

		sf::Sound _cubeSound;
		sf::SoundBuffer _cubeBuffer;
		sf::Sound _sphereSound;
		sf::SoundBuffer _sphereBuffer;
		sf::Sound _pyramidSound;
		sf::SoundBuffer _pyramidBuffer;

		GameObject* _anchor;
		bool _canMove;
		float _anchorScale;
		float _playerScale = -1;
		float _padding;
		glm::mat4x4 _anchorTransform;
		State _currentState = State::NONE;
		State _nextState = State::NONE;
		Mesh* _meshOne;
		Mesh* _meshTwo;
		Mesh* _meshThree;
		void SetShape(State pState);
		Mesh* GetMesh(State pState);
		void HandleStates();
};

#endif // PLAYERBEHAVIOUR_HPP