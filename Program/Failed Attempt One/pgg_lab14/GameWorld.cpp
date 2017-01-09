/*!
*  \brief     GameWorld Class.
*  \details   This class is to create and run the Game World
*  \author    James Robertson
*  \version   1.0a
*  \date      2015
*  \copyright GNU Public License.
*/

#include "GameWorld.h"

GameWorld::GameWorld()
{
	programWindow.x = 20;
	programWindow.y = 40;
	programWindow.pixelWidth = 960;
	programWindow.pixelHeight = 540;

	// Initialise the Pointers to NULL
	window = nullptr;
	renderer = nullptr;
	glContext = NULL;

	camera = new Camera();

	// The game is looping
	go = true;

	// Initialise SDL - OpenGL - Scene
	initialiseAll();
}
//---------------------------------------------------------120 limit right here----------------------------------------

GameWorld::~GameWorld()
{
	// Delete Pointers!
	delete camera;
	
	// Destroy SDL Specific Stuff
	SDL_DestroyRenderer(renderer);
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
}

void GameWorld::initialiseAll()
{
	// Initialise Everything in this order
	initialiseSDL();
	initialiseOpenGL();
	initialiseScene();
}

void GameWorld::initialiseSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		// Something went very wrong in initialisation, all we can do is exit
		std::cout << "Whoops! Something went very wrong, cannot initialise SDL :(" << std::endl;
	}

	// OpenGL Everything Initialised
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Creae a Window with that information
	window = SDL_CreateWindow("Template 3D Window",
			 programWindow.x, programWindow.y, programWindow.pixelWidth, programWindow.pixelHeight,
			 SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	// Make a Renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	glContext = SDL_GL_CreateContext(window);

	// Get Ticks
	timings.lastTime = SDL_GetTicks();
}

bool GameWorld::initialiseOpenGL()
{
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cerr << "Error: GLEW failed to initialise with message: " << glewGetErrorString(err) << std::endl;
		return false;
	}

	std::cout << "INFO: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "INFO: OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "INFO: OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "INFO: OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	
	// Enable the depth test to make sure triangles in front are always in front no matter the order they are drawn
	glEnable(GL_DEPTH_TEST);
	return true;
}

void GameWorld::initialiseScene()
{
	// Initialise with GameObjects
}

void GameWorld::render2DImages(SDL_Texture* Image, SDL_Rect Location, bool Update)
{
	// Tell it wherer to render
	SDL_QueryTexture(Image, NULL, NULL, &Location.w, &Location.h);
	SDL_RenderCopy(renderer, Image, NULL, &Location);

	if (Update)
	{
		// Get Renderer
		SDL_RenderPresent(renderer);
		// Clear the entire screen to our selected colour
		SDL_RenderClear(renderer);
	}
}

SDL_Texture* GameWorld::createImage(std::string filename)
{
	SDL_Texture* temp = nullptr;
	
	// Set the file to the surface
	surface = SDL_LoadBMP(filename.c_str());

	// Always do the check for the most likely first to save time
	if (surface)
	{
		// Transparent for all of the cyan images
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0xFF, 0xff));
		
		temp = SDL_CreateTextureFromSurface(renderer, surface);
		
		SDL_FreeSurface(surface);
		return temp;
	}
	// If not - Lets tell the user
	std::cout << "This hasn't worked m'dear" << std::endl;

	// Quit!
	SDL_Quit();
	return temp;
}

void GameWorld::keyInputHandler()
{
	switch (incomingEvent.type)
	{
	case SDL_QUIT:
		go = false;
		break;
	// When Key is pressed
	case SDL_KEYDOWN:
		switch (incomingEvent.key.keysym.sym)
		{

		// Escape - Exit the game
		case SDLK_ESCAPE:
			go = false;
			break;

		// A - Spin Left
		case SDLK_a:
			break;

		// D - Spin Right
		case SDLK_d:
			break;
		}
		break;
	}
}

void GameWorld::updateObjects()
{
	// Update our world
	timings.current = SDL_GetTicks();
	timings.deltaTime = (float)(timings.current - timings.lastTime) / 1000.0f;

	// Now that we've done this we can use the current time as the next frame's previous time
	timings.lastTime = timings.current;

	// Specify the colour to clear the framebuffer to
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// This writes the above colour to the colour part of the framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GameWorld::drawObjects()
{
	// Update the Camera
	camera->update();

	// Double Buffering Stuff Yes!
	SDL_GL_SwapWindow(window);

	// Limit to 60FPS
	if (timings.deltaTime < (1.0f / 60.0f))
	{
		SDL_Delay((unsigned int)(((1.0f / 60.0f) - timings.deltaTime)*1000.0f));
	}
}

bool GameWorld::updateGame()
{
	// While the game loop is going
	while (go == true)
	{
		// Keyboard input 
		while (SDL_PollEvent(&incomingEvent))
		{
			keyInputHandler();
		}
	// Update and Draw the Objects
	updateObjects();
	drawObjects();
	}
	// Exit out
	return false;
}
