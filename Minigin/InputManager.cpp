#include <SDL.h>
#include "InputManager.h"
#include "SceneManager.h"
#include <iostream>

#include "ImguiWrapper.h"


dae::InputManager::InputManager()
	:m_GamepadStates(XUSER_MAX_COUNT)
{
}


bool dae::InputManager::ProcessInput(const UpdateContext& context)
{
	if (!ProcessSDLEvent()) return false;

	ProcessXInput();
	ExecuteCommands(context);

	return true;
}

void dae::InputManager::ProcessXInput()
{
	for (DWORD gamepadIndex = 0; gamepadIndex < XUSER_MAX_COUNT; ++gamepadIndex)
	{
		UpdateGamepadButtonStates(gamepadIndex);

		UpdateGamepadBatteryInformation(gamepadIndex);
	}
}

void dae::InputManager::ExecuteCommands(const UpdateContext& context)
{
	for (const CommandTriggerCondition& commandCondition : m_CommandsLinkedToPlayerAction)
	{
		auto buttonConditionsItr = m_Keybinds.find(commandCondition.action);
		if (buttonConditionsItr == m_Keybinds.end()) continue;

		if (!CheckButtonConditions(buttonConditionsItr->second, commandCondition.gamepadID)) continue;

		commandCondition.pCommand->Execute(context);
	}
}


bool dae::InputManager::CheckButtonConditions(const ButtonConditions& buttonConditions, int gamepadID)
{
	WORD phase = buttonConditions.buttonPhase;
	if (phase & XINPUT_KEYSTROKE_REPEAT)
	{
		return IsPressed(buttonConditions.buttonMask, gamepadID);
	}
	else if (phase & XINPUT_KEYSTROKE_KEYDOWN)
	{
		return IsDown(buttonConditions.buttonMask, gamepadID);
	}
	else if (phase & XINPUT_KEYSTROKE_KEYUP)
	{
		return IsReleased(buttonConditions.buttonMask, gamepadID);
	}
	return false;
}

inline void dae::InputManager::UpdateGamepadButtonStates(DWORD gamepadIndex)
{
	XINPUT_STATE buttonState;
	DWORD result = XInputGetState(gamepadIndex, &buttonState);

	GamepadState& gamepadState = m_GamepadStates[gamepadIndex];
	switch (result)
	{
		case ERROR_SUCCESS:
		{
			gamepadState.connected = true;
			std::swap(gamepadState.previousButtonState, gamepadState.currentButtonState);
			gamepadState.currentButtonState = buttonState.Gamepad;

			WORD buttonChanges = gamepadState.currentButtonState.wButtons ^ gamepadState.previousButtonState.wButtons;
			gamepadState.buttonsPressedThisFrame = buttonChanges & gamepadState.currentButtonState.wButtons;
			gamepadState.buttonsReleasedThisFrame = buttonChanges & (~gamepadState.currentButtonState.wButtons);
			break;
		}

		case ERROR_DEVICE_NOT_CONNECTED:
		{
		default:
			ZeroMemory(&gamepadState, sizeof(GamepadState));
			break;
		}
	}
}

inline void dae::InputManager::UpdateGamepadBatteryInformation(DWORD gamepadIndex)
{
	XINPUT_BATTERY_INFORMATION batteryInfo;
	DWORD result = XInputGetBatteryInformation(gamepadIndex, BATTERY_DEVTYPE_GAMEPAD, &batteryInfo);
	if (result == ERROR_SUCCESS)
	{
		m_GamepadStates[gamepadIndex].batteryInformation = batteryInfo;
	}
}

bool dae::InputManager::IsDown(WORD button, int controllerID) const
{
	return (m_GamepadStates[controllerID].buttonsPressedThisFrame & button);
}
bool dae::InputManager::IsPressed(WORD button, int controllerID) const
{
	return (m_GamepadStates[controllerID].currentButtonState.wButtons & button);
}
bool dae::InputManager::IsReleased(WORD button, int controllerID) const
{
	return (m_GamepadStates[controllerID].buttonsReleasedThisFrame & button);
}

void dae::InputManager::AddKeybind(PLAYERACTION action, ButtonConditions buttonConditions)
{
	m_Keybinds.insert({ action,buttonConditions });
}

void dae::InputManager::AddCommandLinkedToPlayerAction(CommandTriggerCondition& commandTriggerCondition)
{
	m_CommandsLinkedToPlayerAction.push_back(std::move(commandTriggerCondition));
}


bool dae::InputManager::ProcessSDLEvent()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		//process event for IMGUI
		ImGui_ImplSDL2_ProcessEvent(&e);

		switch (e.type)
		{
			case SDL_QUIT:
			{
				return false;
				break;
			}

			case SDL_KEYDOWN:
			{
				break;
			}

			case SDL_KEYUP:
			{
				switch (e.key.keysym.sym)
				{
					case SDLK_PAGEUP:
					{
						SceneManager::GetInstance().NextScene();
						break;
					}

					case SDLK_PAGEDOWN:
					{
						SceneManager::GetInstance().PreviousScene();
						break;
					}
				}
				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				break;
			}

			default:
			{
				break;
			}
		}

	}
	return true;
}



