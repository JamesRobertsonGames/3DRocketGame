/*!
*  \brief     GameModel Class.
*  \details   This class is to create and update the GameModel
*  \author    James Robertson
*  \version   1.0a
*  \date      2015
*  \copyright GNU Public License.
*/

#pragma once
#include "SDKS/glm/glm.hpp"
#include "SDKS/SDL2-2.0.3/include/SDL.h"
#include <string>
#include "glew.h"
#include "ObjLoader.h"

/// Class to store and display a model
__declspec(align(16)) class GameModel
{
public:

	/// Constructor calls InitialiseVAO and InitialiseShaders
	GameModel(std::string objFileName);
	~GameModel();

	/// Loads object model into OpenGL
	void InitialiseVAO();

	/// Loads shaders for the object into OpenGL
	void InitialiseShaders();

	/// Currently just updates rotation to make the model rotate
	void Update( float deltaTs );

	/// Draws object using the given camera view and projection matrices
	void Draw(glm::mat4& viewMatrix, glm::mat4& projMatrix);

	/// For setting the position of the model
	void SetPosition( float posX, float posY, float posZ ) {_position.x = posX; _position.y = posY; _position.z = posZ;}

	void SetRotation(float posX, float posY, float posZ);
	
	void SetForwardVelocity(float velocity, float deltaTime);

	void SetSidewaysVelocity(float velocity, float deltaTime);

	void CollisionCheck(float RowXPosition, float Width);

	void CheckCollisionsWithEnviroment();

	void SetRoll(float angle, float deltaTime);

	glm::vec3 GetModelPosition() {	return _position; }

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

		void operator delete(void* p)
	{
		_mm_free(p);
	}

protected:

	ObjLoader objLoader;

	/// Object position vector
	glm::vec3 _position;

	/// Euler angles for rotation
	glm::vec3 _rotation;

	glm::vec3 row1Positions[5];
	glm::vec3 row2Positions[2];
	glm::vec3 row3Positions[3];
	glm::vec3 row4Positions[2];
	glm::vec3 row5Positions[2];
	glm::vec3 row6Positions[2];
	glm::vec3 row7Positions[2];
	glm::vec3 row8Positions[2];
	glm::vec3 row9Positions[1];
	glm::vec3 row10Positions[2];
	glm::vec3 row11Positions[2];

	/// Vertex Array Object for model in OpenGL
	GLuint _VAO;

	/// Shader program
	GLuint _program;

	/// Uniform locations
	GLint _shaderModelMatLocation, _shaderViewMatLocation, _shaderProjMatLocation;

	/// Object's model matrix
	/// This is rebuilt in the update function
	glm::mat4 _modelMatrix;

	/// Number of vertices in the model
	GLsizei _numVertices;
	uint16_t numberOfTries = 0;

	GLuint diffuseTexID;
	GLuint normalTexID;

private:
	void TextureInit();

	GLuint positionBuffer;
	GLuint normalBuffer;
	GLuint texCoordBuffer;
	GLuint tangentBuffer;
	GLuint biTangentBuffer;

	//SDL_Surface* surface;

	std::string ReadFile(std::string fileName);
};

