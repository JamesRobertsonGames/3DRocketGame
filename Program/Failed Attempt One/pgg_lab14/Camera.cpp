/*!
*  \brief     Camera Class.
*  \details   This class is to Move and Project the Camera
*  \author    James Robertson
*  \version   1.0a
*  \date      2015
*  \copyright GNU Public License.
*/

#include "Camera.h"

Camera::Camera()
{
	// Setup Projection
	projection = glm::perspective(20.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
}

Camera::~Camera()
{

}

void Camera::setAbsolutePosition(glm::vec3 Position)
{
	// Never Called! Was just in case :)
	cameraPosition = Position;
}

void Camera::moveCamera(glm::vec3 Velocity)
{
	// Never Called! Was just in case :)
	cameraPosition += Velocity;
}

void Camera::update()
{
	// Update View matrix
	view = glm::translate(glm::mat4(1.0f), cameraPosition);								  
}