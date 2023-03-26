#include <SDL.h>
#include "InputManager.h"
#include "SceneManager.h"
#include <iostream>

#include "ImguiWrapper.h"
#include <algorithm>
#include "Timers.h"

dae::InputManager::InputManager()
	:m_Gamepads(XUSER_MAX_COUNT)
{
	m_IsRunning = true;
	//std::thread inputPollingThread(&dae::InputManager::PollInputThread, this);
	//inputPollingThread.detach();
}

dae::InputManager::~InputManager()

{
	m_IsRunning = false;
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
	const int maxPollingRate = 500;
	const Milliseconds maximumPollingTime = 1000.f / maxPollingRate;
	static CyclicTimer<EngineClock> cyclicTimer{maximumPollingTime};

	if (cyclicTimer.Update())
	{
		UpdateGamepadButtonStates();
	}

	for (DWORD gamepadIndex = 0; gamepadIndex < XUSER_MAX_COUNT; ++gamepadIndex)
	{
		//UpdateGamepadBatteryInformation(gamepadIndex);
	}
}

void dae::InputManager::ExecuteCommands(const UpdateContext& context)
{
	std::for_each(m_Keybinds.begin(), m_Keybinds.end(), [this, &context](const auto& keybind)
		{
			if (!CheckButtonConditions(keybind.first, keybind.second.gamepadID)) return;

			keybind.second.pCommand->Execute(context.GetDeltaTime());
		});
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

inline void dae::InputManager::UpdateGamepadButtonStates()
{
	for (DWORD gamepadIndex = 0; gamepadIndex < XUSER_MAX_COUNT; ++gamepadIndex)
	{
		XINPUT_STATE buttonState;
		ZeroMemory(&buttonState, sizeof(XINPUT_STATE));
		DWORD result = XInputGetState(gamepadIndex, &buttonState);

		Gamepad& Gamepad = m_Gamepads[gamepadIndex];
		switch (result)
		{
		case ERROR_SUCCESS:
			{
				Gamepad.connected = true;
				std::swap(Gamepad.previousButtonState, Gamepad.currentButtonState);
				Gamepad.currentButtonState = buttonState.Gamepad;

				WORD buttonChanges = Gamepad.currentButtonState.wButtons ^ Gamepad.previousButtonState.wButtons;
				Gamepad.buttonsPressedThisFrame = buttonChanges & Gamepad.currentButtonState.wButtons;
				Gamepad.buttonsReleasedThisFrame = buttonChanges & (~Gamepad.currentButtonState.wButtons);
				break;
			}

		case ERROR_DEVICE_NOT_CONNECTED:
			{
			default:
				ZeroMemory(&Gamepad, sizeof(Gamepad));
				break;
			}
		}
	}
}

inline void dae::InputManager::UpdateGamepadBatteryInformation(DWORD gamepadIndex)
{
	XINPUT_BATTERY_INFORMATION batteryInfo;
	DWORD result = XInputGetBatteryInformation(gamepadIndex, BATTERY_DEVTYPE_GAMEPAD, &batteryInfo);
	if (result == ERROR_SUCCESS)
	{
		m_Gamepads[gamepadIndex].batteryInformation = batteryInfo;
	}
}

bool dae::InputManager::IsDown(WORD button, int controllerID) const
{
	return (m_Gamepads[controllerID].currentButtonState.wButtons & button);
}
bool dae::InputManager::IsPressed(WORD button, int controllerID) const
{
	return (m_Gamepads[controllerID].buttonsPressedThisFrame & button);
}
bool dae::InputManager::IsReleased(WORD button, int controllerID) const
{
	return (m_Gamepads[controllerID].buttonsReleasedThisFrame & button);
}

void dae::InputManager::AddKeybind(ButtonConditions buttonConditions, CommandTriggerCondition& commandTriggerCondition)
{
	m_Keybinds.insert(std::move(std::make_pair(buttonConditions, std::move(commandTriggerCondition))));
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


void dae::InputManager::PollInputThread()
{
	while (m_IsRunning)
	{
		UpdateGamepadButtonStates();
		std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Adjust the sleep duration as needed
	}
}


