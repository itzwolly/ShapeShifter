#include "mge/behaviours/SlidingBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "SFML\Graphics.hpp"

using namespace std::chrono;


SlidingBehaviour::SlidingBehaviour(glm::mat4x4 rot,float dirDuration,glm::vec3 refPos,float distLeft, float distRight, float distBack, float distForwards, float distDown, float distUp, float speed,
	bool rotate, float rotSpeed, glm::vec3 angle):_distLeft(distLeft),_distRight(distRight),_distBack(distBack),_distForwards(distForwards),
	_distUp(distUp),_distDown(distDown),_speed(speed),_rotate(rotate),_rotSpeed(rotSpeed),_angle(angle),_initialPos(refPos),_dirDuration(dirDuration)
{
	t1 = high_resolution_clock::now();
	//_dir = glm::vec3(0, 0, 0);
	//float up = rand() % 3;
	//float right = rand() % 3;
	//float forward = rand() % 3;
	//up--;
	//right--;
	//forward--;
	//_dir = glm::normalize(glm::vec3(glm::vec4(right,up,forward,0)/**rot*/));
}

SlidingBehaviour::~SlidingBehaviour()
{
	//dtor
}

void SlidingBehaviour::update(float pStep)
{

	t2 = high_resolution_clock::now();

	duration<double> time_span = duration_cast<std::chrono::seconds>(t2 - t1);

	float dur = time_span.count();
	if(dur>=_dirDuration)
	{
		_dir = _nextDir;
		t1 = high_resolution_clock::now();
		float up = rand() % 3;
		float right = rand() % 3;
		float forward = rand() % 3;
		up--;
		right--;
		forward--;
		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		glm::vec3 currentPos = _owner->getWorldPosition();

		float distX = currentPos.x - _initialPos.x;
		float distY = currentPos.y - _initialPos.y;
		float distZ = currentPos.z - _initialPos.z;

		if (_distLeft - _distRight == 0)
		{
			right = 0;
		}
		else if (distX < _distLeft)
		{
			std::cout << "going right \n";
			right = 1;
		}
		else if (distX > _distRight)
		{
			std::cout << "going left \n";
			right = -1;
		}


		if (_distDown == _distUp)
		{
			up = 0;
		}
		else if (distY < _distDown)
		{
			std::cout << "going up \n";
			up = 1;
		}
		else if (distY > _distUp)
		{
			std::cout << "going down \n";
			up = -1;
		}

		//std::cout << "\n distances z "<< _distBack << " " << _distForwards << "\n";

		if (_distBack - _distForwards == 0)
		{
			forward = 0;
		}
		else if (distZ < _distBack)
		{
			std::cout << "going forward \n";
			forward = 1;
		}
		else if (distZ > _distForwards)
		{
			std::cout << "going back \n";
			forward = -1;
		}


		std::cout << right << "," << up << "," << forward << "\n";
		std::cout << r1 << "," << r2 << "," << r3 << "\n";
		std::cout << distX << "," << distY << "," << distZ << "\n\n";
		std::cout << currentPos.x << "," << currentPos.y << "," << currentPos.z << "\n\n";

		_nextDir = glm::vec3(right, up, forward);
		//_nextDir = glm::normalize(_nextDir)/**_owner->getWorldTransform()*/;
		std::cout << _dir.x << "," << _dir.y << "," << _dir.z << "\n\n\n";
	}
	
	_owner->translate(_dir*_speed*pStep);
	if (_rotate)
	{
		_owner->rotate(_rotSpeed*pStep, _angle);
	}
    //rotates 45° per second
	//add posibility of rotation
}

glm::vec3 SlidingBehaviour::lerp(glm::vec3 start, glm::vec3 end, float val)
{
	return (1-val)*start + val*end;
}
