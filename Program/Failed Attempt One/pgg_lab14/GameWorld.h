/*!
*  \brief     GameWorld Class.
*  \details   This class is to create and run the Game World
*  \author    James Robertson
*  \version   1.0a
*  \date      2015
*  \copyright GNU Public License.
*/

#pragma once

#include "SDKS/SDL2-2.0.3/include/SDL.h"
#include <iostream>
#include "SDKS/glm/glm.hpp"
#include "SDKS/glm/gtc/matrix_transform.hpp"

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

	// 3D Camera
	Camera* camera;

	// Boolean to keep the loop going
	bool go;

	// Timings
	float deltaTime;
	float spinAmount;
	uint32_t lastTime;
	uint32_t current;

	// Window Specific Attributes
	uint16_t winPosX;
	uint16_t winPosY;
	uint16_t winWidth;
	uint16_t winHeight;
};
