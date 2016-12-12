/*!
*  \brief     GameModel Class.
*  \details   This class is to create and update the GameModel
*  \author    James Robertson
*  \version   1.0a
*  \date      2015
*  \copyright GNU Public License.
*/

#include "GameModel.h"

#include <iostream>
#include "SDKS/glm/gtc/type_ptr.hpp"
#include "SDKS/glm/gtc/matrix_transform.hpp"


bool CheckShaderCompiled( GLint shader )
{
	// Check if it is compiled
	GLint compiled;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
	if ( !compiled )
	{
		// Store Error in Log
		GLsizei len;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );

		GLchar* log = new GLchar[len+1];
		glGetShaderInfoLog( shader, len, &len, log );

		// Tell user of error
		std::cerr << "ERROR: Shader compilation failed: " << log << std::endl;

		// Delete Memory
		delete [] log;

		return false;
	}
	return true;
}

GameModel::GameModel(std::string objFileName)
{
	// Initialise variables
	_VAO = 0;
	_program = 0;
	_shaderModelMatLocation = _shaderViewMatLocation = _shaderProjMatLocation = 0;
	_numVertices = 0;

	numberOfTries = 0;

	// Row 1 Position
	row1Positions[0] = { -116.10f, 0, -165 };
	row1Positions[1] = { -57, 0, -165 };
	row1Positions[2] = { -22, 0, -165 };
	row1Positions[3] = { 11, 0, -165 };
	row1Positions[4] = { 84, 0, -165 };

	// Row 2 Position
	row2Positions[0] = { -83, 0, -255};
	row2Positions[1] = { 43, 0, -255 };

	// Row 3 Position
	row3Positions[0] = { -116.10f, 0, -333 };
	row3Positions[1] = { -59, 0, -333 };
	row3Positions[2] = { 59.50f, 0, -333 };

	// Row 4 Position
	row4Positions[0] = { -99, 0, -420 };
	row4Positions[1] = { 9, 0, -420 };

	// Row 5 Position
	row5Positions[0] = { -116.10f, 0, -510 };
	row5Positions[1] = { 68, 0, -510 };

	// Row 6 Position
	row6Positions[0] = { -116.10f, 0, -606 };
	row6Positions[1] = { 60, 0, -606 };

	// Row 7 Position
	row7Positions[0] = { -116.10f, 0, -716 };
	row7Positions[1] = { 23, 0, -716 };

	// Row 8 Position
	row8Positions[0] = { -116.10f, 0, -815 };
	row8Positions[1] = { 68, 0, -815 };

	// Row 9 Position
	row9Positions[0] = { -116.10f, 0, -940 };

	// Row 10 Position
	row10Positions[0] = { -116.10f, 0, -1070 };
	row10Positions[1] = { 41, 0, -1070 };

	// Row 11 Position
	row11Positions[0] = { -116.10f, 0, -1190 };
	row11Positions[1] = { 6, 0, -1190 };

	// Load Object
	objLoader.Load(objFileName);

	// Create the model
	InitialiseVAO();

	// Create the shaders
	InitialiseShaders();
}

GameModel::~GameModel()
{
	// TODO: destroy VAO, shaders etc
}

void GameModel::InitialiseVAO()
{
	// Creates one VAO and binds it
	glGenVertexArrays( 1, &_VAO );
	glBindVertexArray( _VAO );

	// Finds vertex amount
	_numVertices = objLoader.GetMeshVertices().size() / 3;

	// Variable for storing a VBO
	GLuint positionBuffer = 0;

	// Create a generic 'buffer'
	glGenBuffers(1, &positionBuffer);

	// Tell OpenGL that we want to activate the buffer and that it's a VBO
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);

	// Draw Verts
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _numVertices * 3, &objLoader.GetMeshVertices()[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray(0);

	// Variable for storing a VBO
	GLuint normalBuffer = 0;

	// Create a generic 'buffer'
	glGenBuffers(1, &normalBuffer);

	// Tell OpenGL that we want to activate the buffer and that it's a VBO
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);

	// With this buffer active, we can now send our data to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _numVertices * 3, &objLoader.GetMeshNormals()[0], GL_STATIC_DRAW);

	// This tells OpenGL how we link the vertex data to the shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray(1);
	
	// Bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray( 0 );

	// Technically we can do this, because the enabled / disabled state is stored in the VAO
	glDisableVertexAttribArray(0);
}

void GameModel::InitialiseShaders()
{
	// This is the vertex shader
	const GLchar *vShaderText = "#version 430 core\n\
						 layout(location = 0) in vec4 vPosition;\n\
						 layout(location = 1) in vec3 vNormalIn;\n\
						 \n\
						 uniform mat4 modelMat;\n\
						 uniform mat4 invModelMat;\n\
						 uniform mat4 viewMat;\n\
						 uniform mat4 projMat;\n\
						 \n\
						 uniform vec4 worldSpaceLightPos = {1000,400,-3000,0};\n\
						 \n\
						 out vec3 vNormalV;\n\
						 out vec3 lightDirV;\n\
						 \n\
						 void main()\n\
						 {\n\
								gl_Position = projMat * viewMat * modelMat * vPosition;\n\
								\n\
								vec4 eyeSpaceVertPos = viewMat * modelMat * vPosition;\n\
								vec4 eyeSpaceLightPos = viewMat * worldSpaceLightPos;\n\
								\n\
								lightDirV =  normalize( vec3(eyeSpaceLightPos) - vec3(eyeSpaceVertPos) );\n\
								\n\
								vNormalV = mat3(viewMat * modelMat) * vNormalIn;\n\
						 }";

	// This is the fragment shader
	const GLchar *fShaderText = "#version 430 core\n\
								in vec3 lightDirV;\n\
								in vec3 vNormalV;\n\
								\n\
								uniform vec3 lightColour = {1,1,1};\n\
								uniform vec3 emissiveColour = {0.0,0,0.1};\n\
								uniform vec3 ambientColour  = {0.1f,0.3f,0.2f};\n\
								uniform vec3 diffuseColour  = {0.2f,0.6f,0.5f};\n\
								uniform vec3 specularColour = {0.3f,0.3f,0.3f};\n\
								uniform float shininess     = 200.0f;\n\
								uniform float alpha         = 2.0f;\n\
								\n\
								out vec4 fragColour;\n\
								\n\
								void main()\n\
								{\n\
									vec3 lightDir = normalize( lightDirV );\n\
									vec3 vNormal = normalize( vNormalV );\n\
									\n\
										vec3 diffuse = diffuseColour * lightColour * max( dot( vNormal, lightDir ), 0);\n\
										\n\
										fragColour = vec4( emissiveColour + ambientColour + diffuse, alpha);\n\
								}";

	// The 'program' stores the shaders
	_program = glCreateProgram();

	// Create the vertex shader
	GLuint vShader = glCreateShader( GL_VERTEX_SHADER );
	// Give GL the source for it
	glShaderSource( vShader, 1, &vShaderText, NULL );
	// Compile the shader
	glCompileShader( vShader );
	// Check it compiled and give useful output if it didn't work!
	if( !CheckShaderCompiled( vShader ) )
	{
		return;
	}
	// This links the shader to the program
	glAttachShader( _program, vShader );

	// Same for the fragment shader
	GLuint fShader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fShader, 1, &fShaderText, NULL );
	glCompileShader( fShader );
	if( !CheckShaderCompiled( fShader ) )
	{
		return ;
	}
	glAttachShader( _program, fShader );

	// This makes sure the vertex and fragment shaders connect together
	glLinkProgram( _program );
	// Check this worked
	GLint linked;
	glGetProgramiv( _program, GL_LINK_STATUS, &linked );
	if ( !linked )
	{
		GLsizei len;
		glGetProgramiv( _program, GL_INFO_LOG_LENGTH, &len );

		GLchar* log = new GLchar[len+1];
		glGetProgramInfoLog( _program, len, &len, log );
		std::cerr << "ERROR: Shader linking failed: " << log << std::endl;
		delete [] log;

		return;
	}

	// We need to get the location of the uniforms in the shaders
	// This is so that we can send the values to them from the application
	// We do this in the following way: 
	_shaderModelMatLocation = glGetUniformLocation( _program, "modelMat" );
	_shaderViewMatLocation = glGetUniformLocation( _program, "viewMat" );
	_shaderProjMatLocation = glGetUniformLocation( _program, "projMat" );

}

void GameModel::Update( float deltaTs )
{

}

void GameModel::Draw(glm::mat4& viewMatrix, glm::mat4& projMatrix)
{
	// Next, we translate this matrix according to the object's _position vector:
	_modelMatrix = glm::translate(glm::mat4(1.0f), _position);

	// Next, we rotate this matrix in the x-axis by the object's x-rotation:
	_modelMatrix = glm::rotate(_modelMatrix, _rotation.x, glm::vec3(1, 0, 0));
	// Next, we rotate this matrix in the y-axis by the object's y-rotation:
	_modelMatrix = glm::rotate(_modelMatrix, _rotation.y, glm::vec3(0, 1, 0));
	// Next, we rotate this matrix in the z-axis by the object's z-rotation:
	_modelMatrix = glm::rotate(_modelMatrix, _rotation.z, glm::vec3(0, 0, 1));
	// Activate the shader program
	glUseProgram( _program );

		// Activate the VAO
		glBindVertexArray( _VAO );

			// Send matrices to the shader as uniforms like this:
			glUniformMatrix4fv(_shaderModelMatLocation, 1, GL_FALSE, glm::value_ptr(_modelMatrix) );
			glUniformMatrix4fv(_shaderViewMatLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix) );
			glUniformMatrix4fv(_shaderProjMatLocation, 1, GL_FALSE, glm::value_ptr(projMatrix) );


			// Tell OpenGL to draw it
			// Must specify the type of geometry to draw and the number of vertices
			glDrawArrays(GL_TRIANGLES, 0, _numVertices);
			
		// Unbind VAO
		glBindVertexArray( 0 );
	
	// Technically we can do this, but it makes no real sense because we must always have a valid shader program to draw geometry
	glUseProgram( 0 );
}

void GameModel::SetForwardVelocity(float velocity, float deltaTime)
{
	_position.z -= velocity * deltaTime;
}

void GameModel::SetSidewaysVelocity(float velocity, float deltaTime)
{
	// Times the velocity by delta to get sideways speed up
	_position.x -= velocity * deltaTime;

	// Set Boundries of the Level
	if (_position.x > 110.01f)
		_position.x = 110.00f;
	if (_position.x < -116.01f)
		_position.x = -116.00f;
}

void GameModel::CollisionCheck(float RowXPosition, float Width)
{
	// Have you hit?
	if (_position.x > RowXPosition && _position.x < RowXPosition + Width)
	{
		// If you crash, Increment Tries
		numberOfTries++;

		// Move to start
		_position.z = 100;

		// Output to Console to show how well/bad they're doing
		std::cout << "-------Tries--------" << std::endl;
		std::cout << "-------- " << numberOfTries << " ---------" << std::endl;
	}
}

void GameModel::CheckCollisionsWithEnviroment()
{
	// ROW 1 COLLISION CHECK
	if (_position.z > row1Positions[0].z && _position.z < row1Positions[0].z + 10)
	{
		CollisionCheck(row1Positions[0].x, 43);
		CollisionCheck(row1Positions[1].x, 15);
		CollisionCheck(row1Positions[2].x, 15);
		CollisionCheck(row1Positions[3].x, 49);
		CollisionCheck(row1Positions[4].x, 16);
	}

	// ROW 2 COLLISION CHECK
	if (_position.z > row2Positions[0].z && _position.z < row2Positions[0].z + 10)
	{
		CollisionCheck(row2Positions[0].x, 76);
		CollisionCheck(row2Positions[1].x, 76);
	}

	// ROW 3 COLLISION CHECK
	if (_position.z > row3Positions[0].z && _position.z < row3Positions[0].z + 10)
	{
		CollisionCheck(row3Positions[0].x, 22);
		CollisionCheck(row3Positions[1].x, 84);
		CollisionCheck(row3Positions[2].x, 84);
	}

	// ROW 4 COLLISION CHECK
	if (_position.z > row4Positions[0].z && _position.z < row4Positions[0].z + 10)
	{
		CollisionCheck(row4Positions[0].x, 74);
		CollisionCheck(row4Positions[1].x, 76);
	}

	// ROW 5 COLLISION CHECK
	if (_position.z > row5Positions[0].z && _position.z < row5Positions[0].z + 10)
	{
		CollisionCheck(row5Positions[0].x, 116);
		CollisionCheck(row5Positions[1].x, 90);
	}

	// ROW 6 COLLISION CHECK
	if (_position.z > row6Positions[0].z && _position.z < row6Positions[0].z + 10)
	{
		CollisionCheck(row6Positions[0].x, 136);
		CollisionCheck(row6Positions[1].x, 90);
	}

	// ROW 7 COLLISION CHECK
	if (_position.z > row7Positions[0].z && _position.z < row7Positions[0].z + 10)
	{
		CollisionCheck(row7Positions[0].x, 65);
		CollisionCheck(row7Positions[1].x, 90);
	}
	
	// ROW 8 COLLISION CHECK
	if (_position.z > row8Positions[0].z && _position.z < row8Positions[0].z + 10)
	{
		CollisionCheck(row8Positions[0].x, 126);
		CollisionCheck(row8Positions[1].x, 90);
	}

	// ROW 9 COLLISION CHECK
	if (_position.z > row9Positions[0].z && _position.z < row9Positions[0].z + 10)
	{
		CollisionCheck(row9Positions[0].x, 189);
	}

	// ROW 10 COLLISION CHECK
	if (_position.z > row10Positions[0].z && _position.z < row10Positions[0].z + 10)
	{
		CollisionCheck(row10Positions[0].x, 80);
		CollisionCheck(row10Positions[1].x, 90);
	}

	// ROW 11 COLLISION CHECK
	if (_position.z > row11Positions[0].z && _position.z < row11Positions[0].z + 10)
	{
		CollisionCheck(row11Positions[0].x, 98);
		CollisionCheck(row11Positions[1].x, 108);
	}

	// HAVE YOU WON COLLISION CHECK
	if (_position.z < -1280)
	{
		std::cout << "YOU WIN" << std::endl; 
		_position.z = 0;
	}
}

void GameModel::SetRoll(float angle, float deltaTime)
{
	// update the rotation angle of our cube
	_rotation.y += deltaTime * angle;
}

void GameModel::SetRotation(float posX, float posY, float posZ)
{
	// Update all of the Coordinates
	_rotation.x = posX; 
	_rotation.y = posY; 
	_rotation.z = posZ;
}
