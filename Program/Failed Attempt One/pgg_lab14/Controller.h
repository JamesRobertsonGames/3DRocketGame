/*!
*  \brief     Camera Class.
*  \details   This class is to Handle XINPUT and the Xbox Controller
*  \author    James Robertson
*  \version   1.0a
*  \date      2015
*  \copyright GNU Public License.
*/

#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <stdint.h>
#include <SDL.h>
#include <iostream>

#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200

class Controller
{
public:
	/// Constructor and Destructor
	Controller();
	~Controller();

	/// Check if the controller is connected
	bool isControllerConnected();

	/// Vibrate the controller
	void vibrate(uint16_t leftMotor, uint16_t rightMotor);

	/// Move the player
	float movement();

private:
	// States for the controller
	XINPUT_STATE *controllerState;
	XINPUT_VIBRATION *vibrationState;

	// Store if the controller is connected
	DWORD dwResult;
	DWORD dvResult;

};
