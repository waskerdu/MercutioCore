#pragma once
#include <vector>
#include <map>
#include <string>
#include "meMath.h"
#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\GL.h>
#include <GLFW\glfw3.h>

namespace Input
{
	enum ClampMode
	{
		circle,
		box,
		eightDirection,
		none,
	};
	enum EaseMode
	{
		linear,
		exponential
		//add bezier option
		//maybe another for (sigh) mouse acceleration
	};
}

class InputController;
class InputButton;
class InputAxis;
class InputAxis2;
class RawButton;
class RawAxis;
class InputManager;

class InputController
{
	std::vector<InputButton*>* inputButtons;
	std::map<std::string, int>* inputButtonMap;
	std::vector<InputAxis*>* inputAxes;
	std::map<std::string, int>* inputAxisMap;
	std::vector<InputAxis2*>* inputAxes2;
	std::map<std::string, int>* inputAxis2Map;

	int axesCount;
	int buttonCount;
	const float *axisData;
	const unsigned char *buttonData;

public:
	enum Type
	{
		mk,
		xbox,
		dualShock,
		all
	};

	Type controllerType = all;

	bool connected = false;
	int index = 0;

	InputController(std::vector<InputButton*>* inputButtons, std::map<std::string, int>* inputButtonMap, std::vector<InputAxis*>* inputAxes,
		std::map<std::string, int>* inputAxisMap, std::vector<InputAxis2*>* inputAxes2, std::map<std::string, int>* inputAxis2Map);
	~InputController();
	bool GetButton(std::string alias);
	double GetAxis(std::string alias);
	glm::vec2 GetAxis2(std::string alias);
	void SetAxis2Deadzone(std::string alias, double deadZone);
	void SetAxis2ClampMode(std::string alias, Input::ClampMode clampMode);
	void SetAxis2InvertX(std::string alias, bool invert);
	void SetAxis2InvertY(std::string alias, bool invert);
	void SetAxis2SensitivityX(std::string alias, double sensitivity);
	void SetAxis2SensitivityY(std::string alias, double sensitivity);
	void Update(double deltaTime, GLFWwindow* window);
	void Aggregate(InputController* controller);
	void Clear();
	void SetXbox();
	void SetDualShock();
};

class InputButton
{
	friend InputManager;

	bool value = false;
public:
	std::vector<RawButton*> rawButtons;
	InputButton()
	{

	}
	~InputButton();
public:
	void Update(double deltaTime, InputController::Type controllerType, GLFWwindow* window, const float *axisData, int numAxes, const unsigned char *buttonData, int numButtons);
	bool IsFiring() { return value; }
	void SetValue(bool value) { this->value = value; }
};

class InputAxis
{
	friend InputManager;

	double value = 0;
	double greatestValue = 0;

public:
	std::vector<RawAxis*> rawAxes;
	InputAxis()
	{

	}
	~InputAxis();
	double GetValue() { return value; }
	void SetValue(double value) { this->value = value; }
	void Update(InputController::Type controllerType, GLFWwindow* window, const float *axisData, int numAxes, const unsigned char *buttonData, int numButtons);
};

class InputAxis2
{
	glm::vec2 value;
	double greatestMag = 0;
public:
	std::vector<RawAxis*> rawAxesX;
	std::vector<RawAxis*> rawAxesY;
	/*enum ClampMode
	{
	circle,
	box,
	eightDirection,
	};*/
	/*enum EaseMode
	{
	linear,
	exponential
	//add bezier option
	//maybe another for (sigh) mouse acceleration
	};*/
	Input::ClampMode clampMode = Input::circle;
	Input::EaseMode easeMode = Input::linear;
	bool invertX = false;
	bool invertY = false;
	double sensitivity = 1;
	double xSensitivity = 1;
	double ySensitivity = 1;
	double deadZone = 0;

	glm::vec2 GetValue() { return value; }
	void SetValue(glm::vec2 value) { this->value = value; }

	InputAxis2()
	{

	}
	~InputAxis2();
	void Update(InputController::Type controllerType, GLFWwindow* window, const float *axisData, int numAxes, const unsigned char *buttonData, int numButtons);
};
/*raw input classes concern themselves with a single input,
These include:
a key press
a joystick button
a mouse button
a joystick axis
a mouse axis
*/
class RawButton
{
public:
	enum Type
	{
		key,
		joyButton,
		mouseButton,
		joystickAxis,
		mouseAxisX,
		mouseAxisY,
		mouseAxisDeltaX,
		mouseAxisDeltaY,
	};
	enum Activation
	{
		press,
		hold,
		release,
		pulse,
		down
	};
private:
	double lastTimePulsed = 0;
	double timeHeld = 0;
	double lastAxisValue = 0;

	Type type = key;
	Activation activation = press;
	double heldThreshold = 0;
	double pulseTime = 0;
	double pulseDelay = 0;
	bool invertAxis = false;
	double axisThreshold = 0.5;
	bool value = false;
	bool lastValue = false;
	int index = 0;
public:
	GLFWwindow* window;
	InputController::Type controllerType = InputController::mk;
	void Update(double deltaTime, GLFWwindow* window, const float *axisData, int numAxes, const unsigned char *buttonData, int numButtons);
	bool IsFiring() { return value; }
	//void Update(double deltaTime, const float *axisData, const unsigned char *buttonData);
	RawButton(int index, Type type, Activation activation, double heldThreshold, double pulseTime,
		double pulseDelay, bool invertAxis, double axisThreshold, InputController::Type controllerType)
	{
		this->index = index;
		this->type = type;
		this->activation = activation;
		this->heldThreshold = heldThreshold;
		this->pulseTime = pulseTime;
		this->pulseDelay = pulseDelay;
		this->invertAxis = invertAxis;
		this->axisThreshold = axisThreshold;
		this->controllerType = controllerType;
	}
	~RawButton() {}
	//RawButton(index, RawButton::key, RawButton::press, 0, 0, 0, false, 0.5, false)
};/**/

class RawAxis
{
public:
	enum Type
	{
		key,
		joyButton,
		mouseButton,
		joystickAxis,
		mouseAxisX,
		mouseAxisY,
		mouseAxisDeltaX,
		mouseAxisDeltaY,
	};
private:
	Type type = key;
	bool invert = false;
	double value = 0;
	double lastValue = 0;
	int posIndex = 0;
	int negIndex = 0;
public:
	GLFWwindow* window;
	InputController::Type controllerType = InputController::mk;
	RawAxis(int posIndex, int negIndex, Type type, InputController::Type controllerType, bool invert)
	{
		this->posIndex = posIndex;
		this->negIndex = negIndex;
		this->type = type;
		this->controllerType = controllerType;
		this->invert = invert;
	}
	~RawAxis() {}
	void Update(GLFWwindow* window, const float *axisData, int numAxes, const unsigned char *buttonData, int numButtons);
	double GetValue() { return value; }
};

class InputManager
{
	std::vector<InputButton*> inputButtons;
	std::map<std::string, int> inputButtonMap;
	std::vector<InputAxis*> inputAxes;
	std::map<std::string, int> inputAxisMap;
	std::vector<InputAxis2*> inputAxes2;
	std::map<std::string, int> inputAxis2Map;
	std::map<std::string, int> keyMap;
	struct ParseMode
	{
		enum Mode
		{
			Button,
			Axis,
			Axis2,
			none
		};
	};

public:
	std::vector<InputController*> controllers;
	InputController* mkController;
	InputController* aggregateController;

	const int maxControllers = 4;

private:
	InputButton* GetButton(std::string alias);
	InputAxis* GetAxis(std::string alias);
	InputAxis2* GetAxis2(std::string alias);
	void MapKeys();
	int GetKey(std::string str);

public:
	InputManager();
	~InputManager();
	bool ParseInputConfig(std::string);
	void BuildControllers();

	void AddButton(std::string alias);

	void ButtonAddKeyPress(std::string alias, int index);
	void ButtonAddKeyRelease(std::string alias, int index);
	void ButtonAddKeyHold(std::string alias, int index, double holdThreshold);
	void ButtonAddKeyPulse(std::string alias, int index, double pulseTime, double pulseDelay);
	void ButtonAddKeyDown(std::string alias, int index);

	void ButtonAddMouseButtonPress(std::string alias, int index);
	void ButtonAddMouseButtonRelease(std::string alias, int index);
	void ButtonAddMouseButtonHold(std::string alias, int index, double holdThreshold);
	void ButtonAddMouseButtonPulse(std::string alias, int index, double pulseTime, double pulseDelay);
	void ButtonAddMouseButtonDown(std::string alias, int index);

	//void ButtonAddMouseAxisPress(std::string alias, int index, double threshold, bool negate);
	//void ButtonAddMouseAxisRelease(std::string alias, int index, double threshold, bool negate);
	void ButtonAddMouseAxisPress(std::string alias, int index, double threshold, bool negate, RawButton::Type type);
	void ButtonAddMouseAxisRelease(std::string alias, int index, double threshold, bool negate, RawButton::Type type);
	//void ButtonAddMouseAxisDown(std::string alias, int index);
	//void ButtonAddMouseAxisHold(std::string alias, int index, double holdThreshold, double threshold, bool negate, bool dualShock);
	//void ButtonAddMouseAxisPulse(std::string alias, int index, double pulseTime, double pulseDelay, double threshold, bool negate, bool dualShock);

	void ButtonAddJoyButtonPress(std::string alias, int index, bool dualShock);
	void ButtonAddJoyButtonRelease(std::string alias, int index, bool dualShock);
	void ButtonAddJoyButtonHold(std::string alias, int index, double holdThreshold, bool dualShock);
	void ButtonAddJoyButtonPulse(std::string alias, int index, double pulseTime, double pulseDelay, bool dualShock);
	void ButtonAddJoyButtonDown(std::string alias, int index, bool dualShock);

	void ButtonAddJoyAxisPress(std::string alias, int index, double threshold, bool negate, bool dualShock);
	void ButtonAddJoyAxisRelease(std::string alias, int index, double threshold, bool negate, bool dualShock);
	void ButtonAddJoyAxisHold(std::string alias, int index, double holdThreshold, double threshold, bool negate, bool dualShock);
	void ButtonAddJoyAxisPulse(std::string alias, int index, double pulseTime, double pulseDelay, double threshold, bool negate, bool dualShock);
	void ButtonAddJoyAxisDown(std::string alias, int index, double threshold, bool negate, bool dualShock);

	void AddAxis(std::string alias);
	void AxisAddKey(std::string alias, int posIndex, int negIndex, bool negate);
	void AxisAddMouseAxis(std::string alias, int index, bool negate, RawAxis::Type type);
	void AxisAddMouseButton(std::string alias, int index, bool negate);
	void AxisAddJoyButton(std::string alias, int posIndex, int negIndex, bool negate, bool dualShock);
	void AxisAddJoyAxis(std::string alias, int index, bool negate, bool dualShock);

	void AddAxis2(std::string alias);
	//void SetAxis2Deadzone(std::string alias, double deadzone);
	//void SetAxis2Clamp(std::string alias, );
	void Axis2AddKey(std::string alias, int posIndexX, int negIndexX, int posIndexY, int negIndexY);
	void Axis2AddMouseAxis(std::string alias, int indexX, int indexY);
	void Axis2AddMouseDeltaAxis(std::string alias, int indexX, int indexY);
	void Axis2AddJoyButton(std::string alias, int posIndexX, int negIndexX, int posIndexY, int negIndexY, bool dualShock);
	void Axis2AddJoyAxis(std::string alias, int indexX, int indexY, bool dualShock);
	/*void Axis2AddKey(std::string alias, int posIndexX, int negIndexX, int posIndexY, int negIndexY, bool negate, bool xAxis);
	void Axis2AddMouseButton(std::string alias, int index, bool negate, bool xAxis);
	void Axis2AddMouseAxis(std::string alias, int index, bool negate, bool xAxis);
	void Axis2AddJoyButton(std::string alias, int index, bool negate, bool xAxis, bool dualShock);
	void Axis2AddJoyAxis(std::string alias, int index, bool negate, bool xAxis, bool dualShock);*/

	void Update(double deltaTime, GLFWwindow* window);
};
