/*!
*  \brief     Camera Class.
*  \details   This class is to Move and Project the Camera
*  \author    James Robertson
*  \version   1.0a
*  \date      2015
*  \copyright GNU Public License.
*/

#pragma once 
#include "SDKS/glm/glm.hpp"
#include "SDKS/glm/gtc/matrix_transform.hpp"


__declspec(align(16)) class Camera
{
public:
	/// Constructor and Destructor
	Camera();
	~Camera();

	/// Position Setters
	void setAbsolutePosition(glm::vec3 Position);
	void moveCamera(glm::vec3 Velocity);

	/// Update Camera
	void update();

	/// Get Projection and View
	glm::mat4 getProjection() { return projection; }
	glm::mat4 getView() { return view; }

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

		void operator delete(void* p)
	{
		_mm_free(p);
	}

private:
	glm::vec3 cameraPosition;
	glm::mat4 projection;
	glm::mat4 view;
};