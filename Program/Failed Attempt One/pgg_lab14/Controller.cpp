/*!
*  \brief     Controller Class.
*  \details   This class is to Handle XINPUT and the Xbox Controller
*  \author    James Robertson
*  \version   1.0a
*  \date      2015
*  \copyright GNU Public License.
*/
#include "Controller.h"

Controller::Controller()
{
	// Initialise pointers
	controllerState = new XINPUT_STATE();
	vibrationState = new XINPUT_VIBRATION();
}

Controller::~Controller()
{
	delete controllerState;
	delete vibrationState;
}

bool Controller::isControllerConnected()
{
	// Zeros all memory values in the controller
	ZeroMemory(controllerState, sizeof(XINPUT_STATE));

	// Get the current state of controller 1
	dwResult = XInputGetState(0, controllerState);

	// Zeros all the memory values in the vibration modual
	ZeroMemory(vibrationState, sizeof(XINPUT_VIBRATION));

	// Set the state of the vibration moduel
	dvResult = XInputSetState(0, vibrationState);

	// If it worked return true if not then its not connected
	if (dwResult == ERROR_SUCCESS && dvResult == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Controller::vibrate(uint16_t leftMotor, uint16_t rightMotor)
{
	// Create a new Vibraton 
	XINPUT_VIBRATION Vibration;

	// Set first num bytes to block of memory (I Have no clue)
	memset(&Vibration, 0, sizeof(XINPUT_VIBRATION));

	// Vibrate amount
	WORD leftVib = (WORD)(leftMotor*65535.0f);
	WORD rightVib = (WORD)(rightMotor*65535.0f);

	// Set the Vibration Values
	Vibration.wLeftMotorSpeed = leftVib;
	Vibration.wRightMotorSpeed = rightVib;

	// Vibrate the controller
	XInputSetState((int)0, &Vibration);
}

float Controller::movement()
{
	// Movement for the character
	float movement = 0.0f;

	// Get all the current states of controller 1
	XInputGetState(0, controllerState);

	// LB is Pressed...
	if (controllerState->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
	{
		// Go Left and Vibrate
		movement = -1.3f;
		vibrate(0.5, 0);
	}

	// RB is Pressed...
	if (controllerState->Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		// Go Right and Vibrate
		movement = 1.3f;
		vibrate(0, 0.5);
	}

	// Retrun speed of the ship
	return movement;
}