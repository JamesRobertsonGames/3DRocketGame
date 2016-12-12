/*!
*  \brief     GameWorld Class.
*  \details   This class is to create and run the Game World
*  \author    James Robertson
*  \version   1.0a
*  \date      2015
*  \copyright GNU Public License.
*/

#pragma once

#include <SDL.h>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "glew.h"
#include "ObjLoader.h"
#include "GameModel.h"
#include "Controller.h"
#include "Camera.h"

class GameWorld
{
public:
	/// Constructor and Destructor
	GameWorld();
	~GameWorld();

	/// Initialisation
	void initialiseAll();
	void initialiseSDL();
	bool initialiseOpenGL();
	void initialiseScene();

	/// Render 2D images on the screen
	void render2DImages(SDL_Texture* Image, SDL_Rect Location, bool Update);

	/// Image Convertor
	SDL_Texture* createImage(std::string filename);

	/// In Game Loop
	void keyInputHandler();
	bool updateGame();
	void updateObjects();
	void drawObjects();

private:
	// SDL Specific Stuffs
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *surface;
	SDL_GLContext glContext;
	SDL_Event incomingEvent;

	// Models
	GameModel* playerRocket;
	GameModel* Terrain;

	// 3D Camera
	Camera* camera;

	// Boolean to keep the loop going
	bool go;

	// Store if the controller is connected
	bool xboxControllerConnected;

	// Timings
	float deltaTime = 0.0f;
	float spinAmount = 0.0f;
	uint32_t lastTime = 0;
	uint32_t current = 0;

	// Rocket Speed
	const float MAX_SPINAMOUNT = 27.0f;
	const float SPIN_ACCELERATION = 1.3f;
	const float MAX_SPEED = 38.5f;
	float currentSpeed = 0.0f;

	// Window Specific Attributes
	uint16_t winPosX = 360;
	uint16_t winPosY = 100;
	uint16_t winWidth = 1280;
	uint16_t winHeight = 720;
};
