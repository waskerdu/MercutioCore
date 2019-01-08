#include "stdafx.h"
#include "meInput.h"
#include <iostream>
#include <fstream>
#include "meParser.h"

InputManager::InputManager()
{

}
InputManager::~InputManager()
{
	delete mkController;
	delete aggregateController;
	for (uint32_t i = 0; i < inputButtons.size(); i++)
	{
		delete inputButtons[i];
	}
	for (uint32_t i = 0; i < inputAxes.size(); i++)
	{
		delete inputAxes[i];
	}
	for (uint32_t i = 0; i < inputAxes2.size(); i++)
	{
		delete inputAxes2[i];
	}
	for (int32_t i = 0; i < maxControllers; i++)
	{
		delete controllers[i];
	}
}

void InputManager::MapKeys()
{
	keyMap["a"] = GLFW_KEY_A;
	keyMap["b"] = GLFW_KEY_B;
	keyMap["c"] = GLFW_KEY_C;
	keyMap["d"] = GLFW_KEY_D;
	keyMap["e"] = GLFW_KEY_E;
	keyMap["f"] = GLFW_KEY_F;
	keyMap["g"] = GLFW_KEY_G;

	keyMap["h"] = GLFW_KEY_H;
	keyMap["i"] = GLFW_KEY_I;
	keyMap["j"] = GLFW_KEY_J;
	keyMap["k"] = GLFW_KEY_K;
	keyMap["l"] = GLFW_KEY_L;
	keyMap["m"] = GLFW_KEY_M;
	keyMap["n"] = GLFW_KEY_N;

	keyMap["o"] = GLFW_KEY_O;
	keyMap["p"] = GLFW_KEY_P;
	keyMap["q"] = GLFW_KEY_Q;
	keyMap["r"] = GLFW_KEY_R;
	keyMap["s"] = GLFW_KEY_S;
	keyMap["t"] = GLFW_KEY_T;
	keyMap["u"] = GLFW_KEY_U;

	keyMap["v"] = GLFW_KEY_V;
	keyMap["w"] = GLFW_KEY_W;
	keyMap["x"] = GLFW_KEY_X;
	keyMap["y"] = GLFW_KEY_Y;
	keyMap["z"] = GLFW_KEY_Z;

	keyMap["0"] = GLFW_KEY_0;
	keyMap["1"] = GLFW_KEY_1;
	keyMap["2"] = GLFW_KEY_2;
	keyMap["3"] = GLFW_KEY_3;
	keyMap["4"] = GLFW_KEY_4;
	keyMap["5"] = GLFW_KEY_5;
	keyMap["6"] = GLFW_KEY_6;
	keyMap["7"] = GLFW_KEY_7;
	keyMap["8"] = GLFW_KEY_8;
	keyMap["9"] = GLFW_KEY_9;

	keyMap["`"] = GLFW_KEY_GRAVE_ACCENT;
	keyMap["-"] = GLFW_KEY_MINUS;
	keyMap["="] = GLFW_KEY_EQUAL;
	keyMap["["] = GLFW_KEY_LEFT_BRACKET;
	keyMap["]"] = GLFW_KEY_RIGHT_BRACKET;
	keyMap[";"] = GLFW_KEY_SEMICOLON;
	keyMap["'"] = GLFW_KEY_APOSTROPHE;
	keyMap[","] = GLFW_KEY_COMMA;
	keyMap["."] = GLFW_KEY_PERIOD;
	keyMap["/"] = GLFW_KEY_SLASH;
	keyMap["\\"] = GLFW_KEY_BACKSLASH;

	keyMap["up"] = GLFW_KEY_UP;
	keyMap["down"] = GLFW_KEY_DOWN;
	keyMap["left"] = GLFW_KEY_LEFT;
	keyMap["right"] = GLFW_KEY_RIGHT;

	keyMap["escape"] = GLFW_KEY_ESCAPE;
	keyMap["tab"] = GLFW_KEY_TAB;
	keyMap["backspace"] = GLFW_KEY_BACKSPACE;
	keyMap["enter"] = GLFW_KEY_ENTER;
	keyMap["space"] = GLFW_KEY_SPACE;

	keyMap["left_ctrl"] = GLFW_KEY_LEFT_CONTROL;
	keyMap["right_ctrl"] = GLFW_KEY_RIGHT_CONTROL;

	keyMap["left_shift"] = GLFW_KEY_LEFT_SHIFT;
	keyMap["right_shift"] = GLFW_KEY_RIGHT_SHIFT;

	keyMap["left_alt"] = GLFW_KEY_LEFT_ALT;
	keyMap["right_alt"] = GLFW_KEY_RIGHT_ALT;
}

int InputManager::GetKey(std::string str)
{
	if (keyMap.count(str) == 1)
	{
		return keyMap[str];
	}
	else
	{
		std::cout << "Key " << str << " not found.";
		return -1;
	}
}

bool InputManager::ParseInputConfig(std::string config)
{
	std::fstream file;
	std::string line;
	std::vector<std::string> parsedLine;
	int index;

	ParseMode::Mode mode = ParseMode::none;
	bool inComment = false;
	bool ok = true;
	std::string current;

	double pulseDelay = 0.5;
	double pulseLength = 0.2;
	double axisThreshold = 0.5;

	file.open(config.c_str());
	if (!file.is_open())
	{
		std::cout << "Input config file failed to open.\n";
		return false;
	}
	MapKeys();
	while (std::getline(file, line))
	{
		Parser(line, parsedLine);
		if (parsedLine.size() == 0) { continue; }
		if (parsedLine[0][0] == '*') { inComment = !inComment; }
		if (inComment) { continue; }

		if (parsedLine[0] == "#Button")
		{
			mode = ParseMode::Button;
			current = parsedLine[1];
			AddButton(current);
		}
		else if (parsedLine[0] == "#Axis")
		{
			mode = ParseMode::Axis; AddButton(parsedLine[1]);
			current = parsedLine[1];
			AddAxis(current);
		}
		else if (parsedLine[0] == "#Axis2")
		{
			mode = ParseMode::Axis2;
			current = parsedLine[1];
			AddAxis2(current);
		}
		else
		{
			switch (mode)
			{
			case InputManager::ParseMode::Button:
				if (parsedLine[0] == "KeyPress")
				{
					index = GetKey(parsedLine[1]);
					if (index != -1)
					{
						ButtonAddKeyPress(current, index);
					}
				}
				else if (parsedLine[0] == "JoyButtonPress")
				{
					//key = GetKey(parsedLine[1]);
					index = std::stoi(parsedLine[1]);
					ButtonAddJoyButtonPress(current, index, parsedLine[2] == "ds4");
				}
				else if (parsedLine[0] == "KeyDown")
				{
					index = GetKey(parsedLine[1]);
					if (index != -1)
					{
						ButtonAddKeyDown(current, index);
					}
				}
				else if (parsedLine[0] == "JoyButtonDown")
				{
					//key = GetKey(parsedLine[1]);
					index = std::stoi(parsedLine[1]);
					ButtonAddJoyButtonDown(current, index, parsedLine[2] == "ds4");
				}
				else if (parsedLine[0] == "KeyPulse")
				{
					index = GetKey(parsedLine[1]);
					if (index != -1)
					{
						ButtonAddKeyPulse(current, index, pulseLength, pulseDelay);
					}
				}
				else if (parsedLine[0] == "JoyButtonPulse")
				{
					//key = GetKey(parsedLine[1]);
					index = std::stoi(parsedLine[1]);
					ButtonAddJoyButtonPulse(current, index, pulseLength, pulseDelay, parsedLine[2] == "ds4");
				}
				else if (parsedLine[0] == "JoyAxisPulse")
				{
					//key = GetKey(parsedLine[1]);
					index = std::stoi(parsedLine[1]);
					bool inv = false;
					double currentThreshold = axisThreshold;
					if (parsedLine[2] == "neg") { inv = !inv; }
					if (parsedLine[3] == "ds4" && (index == 1 || index == 5)) { inv = !inv; }
					if (inv) { currentThreshold = -currentThreshold; }
					ButtonAddJoyAxisPulse(current, index, pulseLength, pulseDelay, currentThreshold, inv, parsedLine[3] == "ds4");/**/
																																  //std::cout << current << " " << index << " " << pulseLength << " " << pulseDelay << " " << currentThreshold << " " << inv << " " << (parsedLine[3] == "ds4") << "\n";
																																  //ButtonAddJoyButtonPress(current, index, parsedLine[2] == "ds4");
				}
				else
				{
					std::cout << parsedLine[0] << " not recognized\n";
				}
				break;
			case InputManager::ParseMode::Axis:
				break;
			case InputManager::ParseMode::Axis2:
				ok = true;
				if (parsedLine[0] == "Key")
				{
					for (size_t i = 0; i < 4; i++)
					{
						if (GetKey(parsedLine[1 + i]) == -1) { ok = false; }
					}
					if (ok == false) { continue; }
					Axis2AddKey(current, GetKey(parsedLine[1]), GetKey(parsedLine[2]), GetKey(parsedLine[3]), GetKey(parsedLine[4]));
				}
				else if (parsedLine[0] == "JoyAxis")
				{
					Axis2AddJoyAxis(current, std::stoi(parsedLine[1]), std::stoi(parsedLine[2]), parsedLine[3] == "ds4");
				}
				break;
			case InputManager::ParseMode::none:
				break;
			default:
				break;
			}
		}
	}

	return true;
}

void InputManager::BuildControllers()
{
	mkController = new InputController(&inputButtons, &inputButtonMap, &inputAxes, &inputAxisMap, &inputAxes2, &inputAxis2Map);
	mkController->controllerType = InputController::mk;
	aggregateController = new InputController(&inputButtons, &inputButtonMap, &inputAxes, &inputAxisMap, &inputAxes2, &inputAxis2Map);
	for (int32_t i = 0; i < maxControllers; i++)
	{
		controllers.push_back(new InputController(&inputButtons, &inputButtonMap, &inputAxes, &inputAxisMap, &inputAxes2, &inputAxis2Map));
		controllers.back()->index = i;
	}
}

void InputManager::AddButton(std::string alias)
{
	if (inputButtonMap.count(alias) == 0)
	{
		inputButtonMap[alias] = (int)inputButtons.size();
		inputButtons.push_back(new InputButton());
	}
	else
	{
		std::cout << "'" << alias << "' is already the name of a button.\n";
	}
}
void InputManager::AddAxis(std::string alias)
{
	if (inputAxisMap.count(alias) == 0)
	{
		inputAxisMap[alias] = (int)inputAxes.size();
		inputAxes.push_back(new InputAxis());
	}
	else
	{
		std::cout << "'" << alias << "' is already the name of a axis.\n";
	}
}
void InputManager::AddAxis2(std::string alias)
{
	if (inputAxis2Map.count(alias) == 0)
	{
		inputAxis2Map[alias] = (int)inputAxes2.size();
		inputAxes2.push_back(new InputAxis2());
	}
	else
	{
		std::cout << "'" << alias << "' is already the name of a axis2.\n";
	}
}

InputButton* InputManager::GetButton(std::string alias)
{
	if (inputButtonMap.count(alias) == 1)
	{
		return inputButtons[inputButtonMap[alias]];
	}
	else
	{
		std::cout << "'" << alias << "' is not a valid button name. Internal error check source.\n";
		return nullptr;
	}
}

InputAxis* InputManager::GetAxis(std::string alias)
{
	if (inputAxisMap.count(alias) == 1)
	{
		return inputAxes[inputAxisMap[alias]];
	}
	else
	{
		std::cout << "'" << alias << "' is not a valid axis name. Internal error check source.\n";
		return nullptr;
	}
}
InputAxis2* InputManager::GetAxis2(std::string alias)
{
	if (inputAxis2Map.count(alias) == 1)
	{
		return inputAxes2[inputAxis2Map[alias]];
	}
	else
	{
		std::cout << "'" << alias << "' is not a valid axis2 name. Internal error check source.\n";
		return nullptr;
	}
}

void InputManager::ButtonAddKeyPress(std::string alias, int index)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }
	RawButton* newRawButton = new RawButton(index, RawButton::key, RawButton::press, 0, 0, 0, false, 0.5, InputController::mk);
	button->rawButtons.push_back(newRawButton);
}
void InputManager::ButtonAddKeyRelease(std::string alias, int index)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }
	RawButton* newRawButton = new RawButton(index, RawButton::key, RawButton::release, 0, 0, 0, false, 0.5, InputController::mk);
	button->rawButtons.push_back(newRawButton);
}
void InputManager::ButtonAddKeyHold(std::string alias, int index, double heldThreshold)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }
	RawButton* newRawButton = new RawButton(index, RawButton::key, RawButton::hold, heldThreshold, 0, 0, false, 0.5, InputController::mk);
	button->rawButtons.push_back(newRawButton);
}
void InputManager::ButtonAddKeyPulse(std::string alias, int index, double pulseTime, double pulseDelay)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }
	RawButton* newRawButton = new RawButton(index, RawButton::key, RawButton::pulse, 0, pulseTime, pulseDelay, false, 0.5, InputController::mk);
	button->rawButtons.push_back(newRawButton);
}
void InputManager::ButtonAddKeyDown(std::string alias, int index)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }
	RawButton* newRawButton = new RawButton(index, RawButton::key, RawButton::down, 0, 0, 0, false, 0.5, InputController::mk);
	button->rawButtons.push_back(newRawButton);
}

void InputManager::ButtonAddMouseButtonPress(std::string alias, int index)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }
	RawButton* newRawButton = new RawButton(index, RawButton::mouseButton, RawButton::press, 0, 0, 0, false, 0.5, InputController::mk);
	button->rawButtons.push_back(newRawButton);
}
void InputManager::ButtonAddMouseButtonRelease(std::string alias, int index)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }
	RawButton* newRawButton = new RawButton(index, RawButton::mouseButton, RawButton::release, 0, 0, 0, false, 0.5, InputController::mk);
	button->rawButtons.push_back(newRawButton);
}
void InputManager::ButtonAddMouseButtonHold(std::string alias, int index, double heldThreshold)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }
	RawButton* newRawButton = new RawButton(index, RawButton::mouseButton, RawButton::hold, heldThreshold, 0, 0, false, 0.5, InputController::mk);
	button->rawButtons.push_back(newRawButton);
}
void InputManager::ButtonAddMouseButtonPulse(std::string alias, int index, double pulseTime, double pulseDelay)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }
	RawButton* newRawButton = new RawButton(index, RawButton::mouseButton, RawButton::pulse, 0, pulseTime, pulseDelay, false, 0.5, InputController::mk);
	button->rawButtons.push_back(newRawButton);
}
void InputManager::ButtonAddMouseButtonDown(std::string alias, int index)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }
	RawButton* newRawButton = new RawButton(index, RawButton::key, RawButton::down, 0, 0, 0, false, 0.5, InputController::mk);
	button->rawButtons.push_back(newRawButton);
}

void InputManager::ButtonAddMouseAxisPress(std::string alias, int index, double threshold, bool negate, RawButton::Type type)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }
	RawButton* newRawButton = new RawButton(index, type, RawButton::press, threshold, 0, 0, negate, 0.5, InputController::mk);
	button->rawButtons.push_back(newRawButton);
}
void InputManager::ButtonAddMouseAxisRelease(std::string alias, int index, double threshold, bool negate, RawButton::Type type)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }
	RawButton* newRawButton = new RawButton(index, type, RawButton::release, threshold, 0, 0, negate, 0.5, InputController::mk);
	button->rawButtons.push_back(newRawButton);
}

void InputManager::ButtonAddJoyButtonPress(std::string alias, int index, bool dualShock)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }

	InputController::Type ctrlType = InputController::xbox;
	if (dualShock) { ctrlType = InputController::dualShock; }
	RawButton* newRawButton = new RawButton(index, RawButton::joyButton, RawButton::press, 0, 0, 0, false, 0.5, ctrlType);
	button->rawButtons.push_back(newRawButton);
}
void InputManager::ButtonAddJoyButtonRelease(std::string alias, int index, bool dualShock)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }

	InputController::Type ctrlType = InputController::xbox;
	if (dualShock) { ctrlType = InputController::dualShock; }
	RawButton* newRawButton = new RawButton(index, RawButton::joyButton, RawButton::release, 0, 0, 0, false, 0.5, ctrlType);
	button->rawButtons.push_back(newRawButton);
}
void InputManager::ButtonAddJoyButtonHold(std::string alias, int index, double holdThreshold, bool dualShock)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }

	InputController::Type ctrlType = InputController::xbox;
	if (dualShock) { ctrlType = InputController::dualShock; }
	RawButton* newRawButton = new RawButton(index, RawButton::joyButton, RawButton::hold, holdThreshold, 0, 0, false, 0.5, ctrlType);
	button->rawButtons.push_back(newRawButton);
}
void InputManager::ButtonAddJoyButtonPulse(std::string alias, int index, double pulseTime, double pulseDelay, bool dualShock)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }

	InputController::Type ctrlType = InputController::xbox;
	if (dualShock) { ctrlType = InputController::dualShock; }
	RawButton* newRawButton = new RawButton(index, RawButton::joyButton, RawButton::pulse, 0, pulseTime, pulseDelay, false, 0.5, ctrlType);
	button->rawButtons.push_back(newRawButton);
}
void InputManager::ButtonAddJoyButtonDown(std::string alias, int index, bool dualShock)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }

	InputController::Type ctrlType = InputController::xbox;
	if (dualShock) { ctrlType = InputController::dualShock; }
	RawButton* newRawButton = new RawButton(index, RawButton::joyButton, RawButton::down, 0, 0, 0, false, 0.5, ctrlType);
	button->rawButtons.push_back(newRawButton);
}

void InputManager::ButtonAddJoyAxisPress(std::string alias, int index, double threshold, bool negate, bool dualShock)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }

	InputController::Type ctrlType = InputController::xbox;
	if (dualShock) { ctrlType = InputController::dualShock; }
	RawButton* newRawButton = new RawButton(index, RawButton::joystickAxis, RawButton::press, 0, 0, 0, false, 0.5, ctrlType);
	button->rawButtons.push_back(newRawButton);
}
void InputManager::ButtonAddJoyAxisRelease(std::string alias, int index, double threshold, bool negate, bool dualShock)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }

	InputController::Type ctrlType = InputController::xbox;
	if (dualShock) { ctrlType = InputController::dualShock; }
	RawButton* newRawButton = new RawButton(index, RawButton::joystickAxis, RawButton::release, 0, 0, 0, false, 0.5, ctrlType);
	button->rawButtons.push_back(newRawButton);
}
void InputManager::ButtonAddJoyAxisHold(std::string alias, int index, double holdThreshold, double threshold, bool negate, bool dualShock)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }

	InputController::Type ctrlType = InputController::xbox;
	if (dualShock) { ctrlType = InputController::dualShock; }
	RawButton* newRawButton = new RawButton(index, RawButton::joystickAxis, RawButton::hold, holdThreshold, 0, 0, false, 0.5, ctrlType);
	button->rawButtons.push_back(newRawButton);
}
void InputManager::ButtonAddJoyAxisPulse(std::string alias, int index, double pulseTime, double pulseDelay, double threshold, bool negate, bool dualShock)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }

	//if (alias == "menuDown") { std::cout << "got to here\n"; }

	InputController::Type ctrlType = InputController::xbox;
	if (dualShock) { ctrlType = InputController::dualShock; }
	RawButton* newRawButton = new RawButton(index, RawButton::joystickAxis, RawButton::pulse, 0, pulseTime, pulseDelay, negate, threshold, ctrlType);
	button->rawButtons.push_back(newRawButton);
}
void InputManager::ButtonAddJoyAxisDown(std::string alias, int index, double threshold, bool negate, bool dualShock)
{
	InputButton* button = GetButton(alias);
	if (button == nullptr) { return; }

	InputController::Type ctrlType = InputController::xbox;
	if (dualShock) { ctrlType = InputController::dualShock; }
	RawButton* newRawButton = new RawButton(index, RawButton::joystickAxis, RawButton::down, 0, 0, 0, negate, threshold, ctrlType);
	button->rawButtons.push_back(newRawButton);
	std::cout << "got here\n";
}

void InputManager::AxisAddKey(std::string alias, int posIndex, int negIndex, bool negate)
{
	InputAxis* axis = GetAxis(alias);
	if (axis == nullptr) { return; }
	RawAxis* newRawAxis = new RawAxis(posIndex, negIndex, RawAxis::key, InputController::mk, negate);
	axis->rawAxes.push_back(newRawAxis);
}
void InputManager::AxisAddMouseAxis(std::string alias, int index, bool negate, RawAxis::Type type)
{
	InputAxis* axis = GetAxis(alias);
	if (axis == nullptr) { return; }
	RawAxis* newRawAxis = new RawAxis(index, -1, type, InputController::mk, negate);
	axis->rawAxes.push_back(newRawAxis);
}
void InputManager::AxisAddMouseButton(std::string alias, int index, bool negate)
{
	InputAxis* axis = GetAxis(alias);
	if (axis == nullptr) { return; }
	RawAxis* newRawAxis = new RawAxis(index, -1, RawAxis::mouseButton, InputController::mk, negate);
	axis->rawAxes.push_back(newRawAxis);
}
void InputManager::AxisAddJoyButton(std::string alias, int posIndex, int negIndex, bool negate, bool dualShock)
{
	InputAxis* axis = GetAxis(alias);
	if (axis == nullptr) { return; }

	InputController::Type ctrlType = InputController::xbox;
	if (dualShock) { ctrlType = InputController::dualShock; }
	RawAxis* newRawAxis = new RawAxis(posIndex, negIndex, RawAxis::joyButton, ctrlType, negate);
	axis->rawAxes.push_back(newRawAxis);
}
void InputManager::AxisAddJoyAxis(std::string alias, int index, bool negate, bool dualShock)
{
	InputAxis* axis = GetAxis(alias);
	if (axis == nullptr) { return; }

	InputController::Type ctrlType = InputController::xbox;
	if (dualShock) { ctrlType = InputController::dualShock; }
	RawAxis* newRawAxis = new RawAxis(index, -1, RawAxis::joystickAxis, ctrlType, negate);
	axis->rawAxes.push_back(newRawAxis);
}

void InputManager::Axis2AddKey(std::string alias, int posIndexX, int negIndexX, int posIndexY, int negIndexY)
{
	InputAxis2* axis = GetAxis2(alias);
	if (axis == nullptr) { return; }
	RawAxis* newRawAxis = new RawAxis(posIndexX, negIndexX, RawAxis::key, InputController::mk, false);
	axis->rawAxesX.push_back(newRawAxis);
	newRawAxis = new RawAxis(posIndexY, negIndexY, RawAxis::key, InputController::mk, false);
	axis->rawAxesY.push_back(newRawAxis);
}
void InputManager::Axis2AddMouseAxis(std::string alias, int indexX, int indexY)
{
	InputAxis2* axis = GetAxis2(alias);
	if (axis == nullptr) { return; }
	RawAxis* newRawAxis = new RawAxis(indexX, -1, RawAxis::mouseAxisX, InputController::mk, false);
	axis->rawAxesX.push_back(newRawAxis);
	newRawAxis = new RawAxis(indexY, -1, RawAxis::mouseAxisY, InputController::mk, false);
	axis->rawAxesY.push_back(newRawAxis);
}
void InputManager::Axis2AddMouseDeltaAxis(std::string alias, int indexX, int indexY)
{
	InputAxis2* axis = GetAxis2(alias);
	if (axis == nullptr) { return; }
	RawAxis* newRawAxis = new RawAxis(indexX, -1, RawAxis::mouseAxisDeltaX, InputController::mk, false);
	axis->rawAxesX.push_back(newRawAxis);
	newRawAxis = new RawAxis(indexY, -1, RawAxis::mouseAxisDeltaY, InputController::mk, false);
	axis->rawAxesY.push_back(newRawAxis);
}
void InputManager::Axis2AddJoyButton(std::string alias, int posIndexX, int negIndexX, int posIndexY, int negIndexY, bool dualShock)
{
	InputAxis2* axis = GetAxis2(alias);
	if (axis == nullptr) { return; }
	InputController::Type ctrlType = InputController::xbox;
	if (dualShock) { ctrlType = InputController::dualShock; }
	RawAxis* newRawAxis = new RawAxis(posIndexX, negIndexX, RawAxis::joyButton, ctrlType, false);
	axis->rawAxesX.push_back(newRawAxis);
	newRawAxis = new RawAxis(posIndexY, negIndexY, RawAxis::joyButton, ctrlType, false);
	axis->rawAxesY.push_back(newRawAxis);
}
void InputManager::Axis2AddJoyAxis(std::string alias, int indexX, int indexY, bool dualShock)
{
	InputAxis2* axis = GetAxis2(alias);
	if (axis == nullptr) { return; }
	InputController::Type ctrlType = InputController::xbox;
	if (dualShock) { ctrlType = InputController::dualShock; }
	RawAxis* newRawAxis = new RawAxis(indexX, -1, RawAxis::joystickAxis, ctrlType, false);
	axis->rawAxesX.push_back(newRawAxis);
	newRawAxis = new RawAxis(indexY, -1, RawAxis::joystickAxis, ctrlType, false);
	axis->rawAxesY.push_back(newRawAxis);
}

void InputManager::Update(double deltaTime, GLFWwindow* window)
{
	mkController->Update(deltaTime, window);
	for (int i = 0; i < maxControllers; i++)
	{
		if (glfwJoystickPresent(i))
		{
			if (controllers[i]->connected == false)
			{
				//connection event
				if (glfwGetJoystickName(i)[0] == 'W')
				{
					//set as dual shock
					//controllers[i]->controllerType = InputController::dualShock;
					controllers[i]->SetDualShock();
				}
				else
				{
					//controllers[i]->controllerType = InputController::xbox;
					controllers[i]->SetXbox();
				}
				controllers[i]->connected = true;
				std::cout << glfwGetJoystickName(i) << " has been connected as controller " << i << ".\n";
			}
			controllers[i]->Update(deltaTime, window);
		}
		else
		{
			if (controllers[i]->connected == true)
			{
				//disconnection event
				controllers[i]->connected = false;
				std::cout << "controller " << i << " has been disconnected.\n";
			}
		}
	}
	aggregateController->Clear();
	aggregateController->Aggregate(mkController);
	for (int i = 0; i < maxControllers; i++)
	{
		aggregateController->Aggregate(controllers[i]);
	}/**/
}

InputController::InputController(std::vector<InputButton*>* inputButtons, std::map<std::string, int>* inputButtonMap, std::vector<InputAxis*>* inputAxes,
	std::map<std::string, int>* inputAxisMap, std::vector<InputAxis2*>* inputAxes2, std::map<std::string, int>* inputAxis2Map)
{
	//this->inputButtons = inputButtons;
	this->inputButtonMap = inputButtonMap;
	//this->inputAxes = inputAxes;
	this->inputAxisMap = inputAxisMap;
	//this->inputAxes2 = inputAxes2;
	this->inputAxis2Map = inputAxis2Map;

	this->inputButtons = new std::vector<InputButton*>();
	for (uint32_t i = 0; i < inputButtons->size(); i++)
	{
		this->inputButtons->push_back(new InputButton(*inputButtons->at(i)));
		this->inputButtons->back()->rawButtons.clear();
		for (uint32_t f = 0; f < inputButtons->at(i)->rawButtons.size(); f++)
		{
			this->inputButtons->back()->rawButtons.push_back(new RawButton(*inputButtons->at(i)->rawButtons.at(f)));
		}
	}

	this->inputAxes = new std::vector<InputAxis*>();
	for (uint32_t i = 0; i < inputAxes->size(); i++)
	{
		this->inputAxes->push_back(new InputAxis(*inputAxes->at(i)));
		this->inputAxes->back()->rawAxes.clear();
		for (uint32_t f = 0; f < inputAxes->at(i)->rawAxes.size(); f++)
		{
			this->inputAxes->back()->rawAxes.push_back(new RawAxis(*inputAxes->at(i)->rawAxes.at(f)));
		}
	}/**/

	this->inputAxes2 = new std::vector<InputAxis2*>();
	for (uint32_t i = 0; i < inputAxes2->size(); i++)
	{
		this->inputAxes2->push_back(new InputAxis2(*inputAxes2->at(i)));
		this->inputAxes2->back()->rawAxesX.clear();
		this->inputAxes2->back()->rawAxesY.clear();
		for (uint32_t f = 0; f < inputAxes2->at(i)->rawAxesX.size(); f++)
		{
			this->inputAxes2->back()->rawAxesX.push_back(new RawAxis(*inputAxes2->at(i)->rawAxesX.at(f)));
			this->inputAxes2->back()->rawAxesY.push_back(new RawAxis(*inputAxes2->at(i)->rawAxesY.at(f)));
		}
		//std::cout << this->inputAxes2->back()->rawAxesX.size() << "\n";
	}/**/
	 //std::cout << this->inputAxes2->size() << "\n";

}
InputController::~InputController()
{
	for (uint32_t i = 0; i < inputButtons->size(); i++)
	{
		delete inputButtons->at(i);
	}
	delete inputButtons;

	for (uint32_t i = 0; i < inputAxes->size(); i++)
	{
		delete inputAxes->at(i);
	}
	delete inputAxes;

	for (uint32_t i = 0; i < inputAxes2->size(); i++)
	{
		delete inputAxes2->at(i);
	}
	delete inputAxes2;
}

void InputController::Update(double deltaTime, GLFWwindow* window)
{
	axisData = glfwGetJoystickAxes(index, &axesCount);
	buttonData = glfwGetJoystickButtons(index, &buttonCount);
	for (uint32_t i = 0; i < inputButtons->size(); i++)
	{
		inputButtons->at(i)->Update(deltaTime, controllerType, window, axisData, axesCount, buttonData, buttonCount);
	}
	for (uint32_t i = 0; i < inputAxes->size(); i++)
	{
		inputAxes->at(i)->Update(controllerType, window, axisData, axesCount, buttonData, buttonCount);
	}
	for (uint32_t i = 0; i < inputAxes2->size(); i++)
	{
		inputAxes2->at(i)->Update(controllerType, window, axisData, axesCount, buttonData, buttonCount);
	}
}

void InputController::Aggregate(InputController* controller)
{
	for (uint32_t i = 0; i < inputButtons->size(); i++)
	{
		if (controller->inputButtons->at(i)->IsFiring())
		{
			inputButtons->at(i)->SetValue(true);
		}
	}
	double tempValue, greatestValue;
	for (uint32_t i = 0; i < inputAxes->size(); i++)
	{
		tempValue = controller->inputAxes->at(i)->GetValue();
		greatestValue = inputAxes->at(i)->GetValue();
		if (abs(tempValue) > abs(greatestValue))
		{
			inputAxes->at(i)->SetValue(tempValue);
		}
	}
	for (uint32_t i = 0; i < inputAxes2->size(); i++)
	{
		tempValue = glm::length(controller->inputAxes2->at(i)->GetValue());
		greatestValue = glm::length(inputAxes2->at(i)->GetValue());

		if (tempValue > greatestValue)
		{
			inputAxes2->at(i)->SetValue(controller->inputAxes2->at(i)->GetValue());
			//std::cout << "got here\n";
		}
	}
}

void InputController::Clear()
{
	for (uint32_t i = 0; i < inputButtons->size(); i++)
	{
		inputButtons->at(i)->SetValue(false);
	}
	//double tempValue, greatestValue;
	for (uint32_t i = 0; i < inputAxes->size(); i++)
	{
		inputAxes->at(i)->SetValue(0);
	}
	for (uint32_t i = 0; i < inputAxes2->size(); i++)
	{
		inputAxes2->at(i)->SetValue(glm::vec2());
	}
}

bool InputController::GetButton(std::string alias)
{
	if (inputButtonMap->count(alias) == 1)
	{
		//if (alias == "menuDown") { std::cout << inputButtons->at(inputButtonMap->at(alias))->IsFiring() << " got here\n"; }
		return inputButtons->at(inputButtonMap->at(alias))->IsFiring();
	}
	else
	{
		std::cout << "'" << alias << "' is not a valid button name.\n";
		return false;
	}
}
double InputController::GetAxis(std::string alias)
{
	if (inputAxisMap->count(alias) == 1)
	{
		return inputAxes->at(inputAxisMap->at(alias))->GetValue();
	}
	else
	{
		std::cout << "'" << alias << "' is not a valid axis name.\n";
		return 0;
	}
}
glm::vec2 InputController::GetAxis2(std::string alias)
{
	if (inputAxis2Map->count(alias) == 1)
	{
		return inputAxes2->at(inputAxis2Map->at(alias))->GetValue();
	}
	else
	{
		std::cout << "'" << alias << "' is not a valid axis2 name.\n";
		return glm::vec2();
	}
}

void InputController::SetAxis2Deadzone(std::string alias, double deadZone)
{
	if (inputAxis2Map->count(alias) == 0)
	{
		std::cout << "'" << alias << "' is not a valid axis2 name.\n";
	}
	else
	{
		inputAxes2->at(inputAxis2Map->at(alias))->deadZone = deadZone;
	}
}
void InputController::SetAxis2ClampMode(std::string alias, Input::ClampMode clampMode)
{
	if (inputAxis2Map->count(alias) == 0)
	{
		std::cout << "'" << alias << "' is not a valid axis2 name.\n";
	}
	else
	{
		inputAxes2->at(inputAxis2Map->at(alias))->clampMode = clampMode;
	}
}
void InputController::SetAxis2InvertX(std::string alias, bool invert)
{
	if (inputAxis2Map->count(alias) == 0)
	{
		std::cout << "'" << alias << "' is not a valid axis2 name.\n";
	}
	else
	{
		inputAxes2->at(inputAxis2Map->at(alias))->invertX = invert;
	}
}
void InputController::SetAxis2InvertY(std::string alias, bool invert)
{
	if (inputAxis2Map->count(alias) == 0)
	{
		std::cout << "'" << alias << "' is not a valid axis2 name.\n";
	}
	else
	{
		inputAxes2->at(inputAxis2Map->at(alias))->invertY = invert;
	}
}
void InputController::SetAxis2SensitivityX(std::string alias, double sensitivity)
{
	if (inputAxis2Map->count(alias) == 0)
	{
		std::cout << "'" << alias << "' is not a valid axis2 name.\n";
	}
	else
	{
		inputAxes2->at(inputAxis2Map->at(alias))->xSensitivity = sensitivity;
	}
}
void InputController::SetAxis2SensitivityY(std::string alias, double sensitivity)
{
	if (inputAxis2Map->count(alias) == 0)
	{
		std::cout << "'" << alias << "' is not a valid axis2 name.\n";
	}
	else
	{
		inputAxes2->at(inputAxis2Map->at(alias))->ySensitivity = sensitivity;
	}
}

void InputController::SetXbox()
{
	SetAxis2InvertY("movement", false);
	SetAxis2InvertY("aim", false);
	controllerType = InputController::xbox;
}

void InputController::SetDualShock()
{
	SetAxis2InvertY("movement", true);
	SetAxis2InvertY("aim", true);
	controllerType = InputController::dualShock;
}

InputButton::~InputButton()
{
	for (uint32_t i = 0; i < rawButtons.size(); i++)
	{
		delete rawButtons[i];
	}
}

void InputButton::Update(double deltaTime, InputController::Type controllerType, GLFWwindow* window, const float *axisData, int numAxes, const unsigned char *buttonData, int numButtons)
{
	value = false;
	for (uint32_t i = 0; i < rawButtons.size(); i++)
	{
		if (rawButtons[i]->controllerType != controllerType && controllerType != InputController::all) { continue; }
		rawButtons[i]->Update(deltaTime, window, axisData, numAxes, buttonData, numButtons);
		if (rawButtons[i]->IsFiring()) { value = true; break; }
	}
	//std::cout << value << "\n";
}

InputAxis::~InputAxis()
{
	for (uint32_t i = 0; i < rawAxes.size(); i++)
	{
		delete rawAxes[i];
	}
}

void InputAxis::Update(InputController::Type controllerType, GLFWwindow* window, const float *axisData, int numAxes, const unsigned char *buttonData, int numButtons)
{
	value = 0;
	greatestValue = 0;
	for (uint32_t i = 0; i < rawAxes.size(); i++)
	{
		if (rawAxes[i]->controllerType != controllerType && controllerType != InputController::all) { continue; }
		rawAxes[i]->Update(window, axisData, numAxes, buttonData, numButtons);
		if (abs(rawAxes[i]->GetValue()) > greatestValue)
		{
			value = abs(rawAxes[i]->GetValue());
			greatestValue = abs(value);
		}
	}
}

InputAxis2::~InputAxis2()
{
	for (uint32_t i = 0; i < rawAxesX.size(); i++)
	{
		delete rawAxesX[i];
	}
	for (uint32_t i = 0; i < rawAxesY.size(); i++)
	{
		delete rawAxesY[i];
	}
}

void InputAxis2::Update(InputController::Type controllerType, GLFWwindow* window, const float *axisData, int numAxes, const unsigned char *buttonData, int numButtons)
{
	value = glm::vec2();
	greatestMag = 0;
	double tempMag;
	for (uint32_t i = 0; i < rawAxesX.size(); i++)
	{
		if (rawAxesX[i]->controllerType != controllerType && controllerType != InputController::all) { continue; }
		rawAxesX[i]->Update(window, axisData, numAxes, buttonData, numButtons);
		rawAxesY[i]->Update(window, axisData, numAxes, buttonData, numButtons);
		tempMag = glm::length(glm::vec2(rawAxesX[i]->GetValue(), rawAxesY[i]->GetValue()));
		if (tempMag> greatestMag)
		{
			value.x = (float)rawAxesX[i]->GetValue();
			value.y = (float)rawAxesY[i]->GetValue();
			greatestMag = tempMag;
		}
	}
	//begin post process
	switch (clampMode)
	{
	case Input::circle:
		if (greatestMag > 1)
		{
			value = glm::normalize(value);
			greatestMag = 1;
		}
		break;
	case Input::box:
		break;
	case Input::eightDirection:
		break;
	case Input::none:
		//should stay blank obviously
		break;
	default:
		break;
	}
	//dead zone
	if (greatestMag <= deadZone) { value *= 0; }
	else
	{
		value = glm::normalize(value);
		value *= (greatestMag - deadZone) / (1 - deadZone);
	}
	//ease
	if (invertX) { value.x = -value.x; }
	if (invertY) { value.y = -value.y; }
}

void RawAxis::Update(GLFWwindow* window, const float *axisData, int numAxes, const unsigned char *buttonData, int numButtons)
{
	double xPos, yPos;
	value = 0;
	switch (type)
	{
	case RawAxis::key:
		if (posIndex > -1)
		{
			if (glfwGetKey(window, posIndex) == GLFW_PRESS)
			{
				value += 1;
			}
		}
		if (negIndex > -1)
		{
			if (glfwGetKey(window, negIndex) == GLFW_PRESS)
			{
				value -= 1;
			}
		}
		break;
	case RawAxis::joyButton:
		if (posIndex > -1)
		{
			if (buttonData[posIndex] == GLFW_PRESS) { value += 1; }
		}
		if (negIndex > -1)
		{
			if (buttonData[negIndex] == GLFW_PRESS) { value -= 1; }
		}
		break;
	case RawAxis::joystickAxis:
		value = axisData[posIndex];
		break;
	case RawAxis::mouseAxisX:
		glfwGetCursorPos(window, &xPos, &yPos);
		value = xPos;
		break;
	case RawAxis::mouseAxisY:
		glfwGetCursorPos(window, &xPos, &yPos);
		value = yPos;
		break;
	case RawAxis::mouseAxisDeltaX:
		glfwGetCursorPos(window, &xPos, &yPos);
		value = xPos - lastValue;
		break;
	case RawAxis::mouseAxisDeltaY:
		glfwGetCursorPos(window, &xPos, &yPos);
		value = xPos - lastValue;
		break;
	default:
		break;
	}
	lastValue = value;
}

void RawButton::Update(double deltaTime, GLFWwindow* window, const float *axisData, int numAxes, const unsigned char *buttonData, int numButtons)
{
	value = false;
	bool tempValue = false;
	double xPos, yPos;
	switch (type)
	{
	case RawButton::key:
		value = glfwGetKey(window, index) == GLFW_PRESS;
		//std::cout << value << "\n";
		break;
	case RawButton::joyButton:
		if (index < numButtons)
		{
			value = (buttonData[index] == GLFW_PRESS);
		}
		break;
	case RawButton::mouseButton:
		value = glfwGetMouseButton(window, index) == GLFW_PRESS;
		break;
	case RawButton::joystickAxis:
		if (index < numAxes)
		{
			if (invertAxis)
			{
				value = axisData[index] < axisThreshold;
			}
			else
			{
				value = axisData[index] > axisThreshold;
			}
		}
		break;
	case RawButton::mouseAxisX:
		glfwGetCursorPos(window, &xPos, &yPos);
		if (invertAxis)
		{
			value = xPos < axisThreshold;
		}
		else
		{
			value = xPos > axisThreshold;
		}
		break;
	case RawButton::mouseAxisY:
		glfwGetCursorPos(window, &xPos, &yPos);
		if (invertAxis)
		{
			value = yPos < axisThreshold;
		}
		else
		{
			value = yPos > axisThreshold;
		}
		break;
	case RawButton::mouseAxisDeltaX:
		glfwGetCursorPos(window, &xPos, &yPos);
		if (invertAxis)
		{
			value = xPos - lastAxisValue < axisThreshold;
		}
		else
		{
			value = xPos - lastAxisValue > axisThreshold;
		}
		lastAxisValue = xPos;
		break;
	case RawButton::mouseAxisDeltaY:
		glfwGetCursorPos(window, &xPos, &yPos);
		if (invertAxis)
		{
			value = yPos - lastAxisValue < axisThreshold;
		}
		else
		{
			value = yPos - lastAxisValue > axisThreshold;
		}
		lastAxisValue = yPos;
		break;
	default:
		break;
	}

	switch (activation)
	{
	case RawButton::press:
		if (value == false) { timeHeld = 0; }
		else
		{
			if (timeHeld > 0) { value = false; }
			else { value = true; }
			timeHeld += deltaTime;
		}
		//if (value == true && lastValue == false) { lastValue = true; }
		//else { value = false; }
		break;
	case RawButton::release:
		if (value == false)
		{
			if (timeHeld != 0) { value = true; }
			timeHeld = 0;
		}
		else
		{
			value = false;
			timeHeld += deltaTime;
		}
		break;
	case RawButton::hold:
		if (value == false) { timeHeld = 0; }
		else
		{
			if (timeHeld > heldThreshold) { value = true; }
			else { value = false; }
			timeHeld += deltaTime;
		}
		break;
	case RawButton::pulse:
		if (value == false) { timeHeld = 0; lastTimePulsed = 0; }
		else
		{
			value = false;
			if (timeHeld == 0) { value = true; }
			else if (timeHeld > pulseDelay)
			{
				if (lastTimePulsed == 0)
				{
					value = true;
					lastTimePulsed = timeHeld;
				}
				else if (timeHeld - lastTimePulsed > pulseTime)
				{
					value = true;
					lastTimePulsed = timeHeld;
				}
			}
			timeHeld += deltaTime;
		}
		break;
	case RawButton::down:
		//do nothing
		break;
	default:
		break;
	}
}