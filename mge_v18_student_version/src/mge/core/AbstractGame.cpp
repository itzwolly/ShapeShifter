
#include "mge/core/KeyHandler.h"
#include <iostream>

#include "AbstractGame.hpp"
#include "mge/core/Renderer.hpp"
#include "mge/core/World.hpp"
#include "mge/behaviours/PlayerBehaviour.h"
#include "mge/PhysicsManager.h"
#include "mge/GameManager.h"
#include "mge/config.hpp"
#include "mge/core/Camera.hpp"



GameObject* player = nullptr;
glm::vec2* AbstractGame::windowSize = new glm::vec2(1280, 960);

AbstractGame::AbstractGame(sf::RenderWindow* window, bool pDebug) : _window(window), _renderer(NULL), _world(NULL), _fps(0), _script(this, config::MGE_SCRIPT_PATH + "main.lua", pDebug)
{
    //cto
}

AbstractGame::~AbstractGame()
{
    //dtor
    //delete _window;
	std::cout << "deleting game\n\n\n\n\n\n\n\n\n";
	//system("pause");
    delete _renderer;
    delete _world;
}

void AbstractGame::initialize() {
    std::cout << "Initializing engine..." << std::endl << std::endl;
    //_initializeWindow();
    _printVersionInfo();
    _initializeGlew();
    _initializeRenderer();
    _initializeWorld();
    _initializeScene();
    std::cout << std::endl << "Engine initialized." << std::endl << std::endl;
}

///SETUP

void AbstractGame::_initializeWindow() {
	std::cout << "Initializing window..." << std::endl;
	//_window = new sf::RenderWindow(sf::VideoMode(windowSize->x, windowSize->y), "Space Extravaganza Deluxe!", sf::Style::Default, sf::ContextSettings(24,8,0,3,3));
	
	//glHint(GL_SAMPLES, 4);
	//glEnable(GL_MULTISAMPLE);
	//_window->setVerticalSyncEnabled(true);
    std::cout << "Window initialized." << std::endl << std::endl;
}

void AbstractGame::_printVersionInfo() {
	std::cout << "Context info:" << std::endl;
    std::cout << "----------------------------------" << std::endl;
    //print some debug stats for whoever cares
    const GLubyte *vendor = glGetString( GL_VENDOR );
    const GLubyte *renderer = glGetString( GL_RENDERER );
    const GLubyte *version = glGetString( GL_VERSION );
    const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
    //nice consistency here in the way OpenGl retrieves values
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    std::cout<<("GL Vendor : %s\n", vendor);
	std::cout<<("GL Renderer : %s\n", renderer);
	std::cout<<("GL Version (string) : %s\n", version);
	std::cout<<("GL Version (integer) : %d.%d\n", major, minor);
	std::cout<<("GLSL Version : %s\n", glslVersion);

    std::cout << "----------------------------------" << std::endl << std::endl;
}

void AbstractGame::_initializeGlew() {
	std::cout << "Initializing GLEW..." << std::endl;
    //initialize the opengl extension wrangler
    GLint glewStatus = glewInit();
	std::cout << "Initialized GLEW, status (1 == OK, 0 == FAILED):" << (glewStatus == GLEW_OK) << std::endl << std::endl;
}

void AbstractGame::_initializeRenderer() {
    //setup our own renderer
	std::cout << "Initializing renderer..." << std::endl;
	_renderer = new Renderer();
    _renderer->setClearColor(0,0,0);
    std::cout << "Renderer done." << std::endl << std::endl;
}

void AbstractGame::_initializeWorld() {
    //setup the world
	std::cout << "Initializing world..." << std::endl;
	_world = new World();
    std::cout << "World initialized." << std::endl << std::endl;
}

///MAIN GAME LOOP

void AbstractGame::run()
{
    //setting to calculate fps
	sf::Clock renderClock;
    int frameCount = 0;
    float timeSinceLastFPSCalculation = 0;

	//settings to make sure the update loop runs at 60 fps
	sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
	sf::Clock updateClock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (_window->isOpen()) {
		if (_running) {
			_handleLuaCreatedObjects();
			_handleProps();

			_processEvents();
			timeSinceLastUpdate += updateClock.restart();

			if (timeSinceLastUpdate > timePerFrame)
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				while (timeSinceLastUpdate > timePerFrame) {
					timeSinceLastUpdate -= timePerFrame;
					_update(timePerFrame.asSeconds());
					PhysicsManager::Update();
				}

				//create shadow map here
				_render();
				//_renderShadows();
				//_render();

				_window->display();

				//fps count is updated once every 1 second
				frameCount++;
				timeSinceLastFPSCalculation += renderClock.restart().asSeconds();
				if (timeSinceLastFPSCalculation > 1) {
					_fps = frameCount / timeSinceLastFPSCalculation;
					timeSinceLastFPSCalculation -= 1;
					frameCount = 0;
				}

			}

			KeyHandler::Reset();
		}
		else
		{
			break;
		}
    }
	delete this;
}

void AbstractGame::_update(float pStep) {
    _world->update(pStep);
	if (GameManager::_inGame) {
		_script.update(pStep);
	}
	GameManager::DeleteOldGates();
}

void AbstractGame::_render () {
    _renderer->render(_world);
}

void AbstractGame::_renderShadows()
{
		glGenFramebuffers(1, GameManager::GetFBOInt());
		glBindFramebuffer(GL_FRAMEBUFFER, *GameManager::GetFBOInt());

		// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
		glGenTextures(1, GameManager::GetShadowMapInt());
		glBindTexture(GL_TEXTURE_2D, *GameManager::GetShadowMapInt());
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, *GameManager::GetShadowMapInt(), 0);

		glDrawBuffer(GL_NONE); // No color buffer is drawn to.

							   // Always check that our framebuffer is ok
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "buffer not ok /n";
		}
		glViewport(0, 0, _window->getSize().x, _window->getSize().y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void AbstractGame::_processEvents()
{
	sf::Event event;
	bool exit = false;

	if (player == nullptr) {
		player = _world->find("player");
	}

	//we must empty the event queue
	while( _window->pollEvent( event ) ) {
        //give all system event listeners a chance to handle events
        //optionally to be implemented by you for example you could implement a
        //SystemEventDispatcher / SystemEventListener pair which allows Listeners to
        //register with the dispatcher and then do something like:
        //SystemEventDispatcher::dispatchEvent(event);
		KeyHandler::NewEvent(event);

        switch (event.type) {
            case sf::Event::Closed:
                exit = true;
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
					exit = true;
                }

				if (event.key.code == sf::Keyboard::P) {
					GameManager::Pause();
				}

				if (!GameManager::_paused) {
					if (event.key.code == sf::Keyboard::A) {
						static_cast<PlayerBehaviour*>(player->getBehaviour())->Move(event.key.code);
					}
					else if (event.key.code == sf::Keyboard::D) {
						static_cast<PlayerBehaviour*>(player->getBehaviour())->Move(event.key.code);
					}
				}
                break;
            case sf::Event::Resized:
                //would be better to move this to the renderer
                //this version implements nonconstrained match viewport scaling
                glViewport(0, 0, event.size.width, event.size.height);
                break;
            default:
                break;
        }
	}

	if (exit) {
        _window->close();
	}
}

void AbstractGame::_handleLuaCreatedObjects() {
	if (_script.GetObjects()->size() > 0) {
		std::vector<GameObject*>::iterator it = _script.GetObjects()->begin();
		while (it != _script.GetObjects()->end()) {
			GameObject* obj = *it;
			_world->add(obj);

			if (obj->GetTag() == "Player") {
				PhysicsManager::RegisterObject(obj);
			} else if (obj->GetTag() == "MainCamera") {
				Camera* mainCamera = static_cast<Camera*>(obj);
				_world->setMainCamera(mainCamera);
			} 
			it = _script.GetObjects()->erase(it);
		}
	}
}

void AbstractGame::Stop() {
	_running = false;
}

void AbstractGame::_handleProps() {
	if (GameManager::GetProps()->size() > 0) {
		std::vector<Prop*>::iterator it = GameManager::GetProps()->begin();
		while (it != GameManager::GetProps()->end()) {
			Prop* obj = *it;
			_world->add(obj);

			it = GameManager::GetProps()->erase(it);
		}
	}
}
