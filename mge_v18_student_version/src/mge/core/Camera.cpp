#include "glm.hpp"
#include "mge/core/Camera.hpp"

Camera::Camera( std::string pName, std::string pTag, glm::vec3 pPosition, glm::mat4 pProjectionMatrix )
:	GameObject(pName, pTag, pPosition), _projection(pProjectionMatrix)
{
}

Camera::~Camera()
{
	//dtor
}

glm::mat4& Camera::getProjection() {
    return _projection;
}

