#pragma once
#include "Singleton.h"
#include <memory>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Xinput.h"
#include "Command.h"
#include <vector>
#include <unordered_map>
#include <atomic>
#include <thread>

namespace dae
{
	struct ButtonConditions
	{
		WORD buttonMask; // if all buttons from this mask are in the required phase -> then buttonCodition will trigger
		WORD buttonPhase; // the button phase: [DOWN|PRESSED|RELEASED]

		bool operator==(const ButtonConditions& other) const
		{
			return buttonMask == other.buttonMask && buttonPhase == other.buttonPhase;
		}
	};
}
namespace std
{
	template<>
	struct hash<dae::ButtonConditions>
	{
		std::size_t operator()(const dae::ButtonConditions& bc) const
		{
			return std::hash<WORD>()(bc.buttonMask) ^ std::hash<WORD>()(bc.buttonPhase);
		}
	};
}

namespace dae
{
	struct CommandTriggerCondition
	{
		int gamepadID; // [0-3]
		std::unique_ptr<Command> pCommand;
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput(const UpdateContext& context);

		void AddKeybind(ButtonConditions buttonConditions, CommandTriggerCondition& commandTriggerCondition);
		~InputManager();

	private:
		bool ProcessSDLEvent();
		void ProcessXInput();
		void ExecuteCommands(const UpdateContext& context);

		inline void UpdateGamepadButtonStates();
		inline void UpdateGamepadBatteryInformation(DWORD gamepadIndex);
		bool IsDown(WORD button, int gamepadID) const;
		bool IsPressed(WORD button, int gamepadID) const;
		bool IsReleased(WORD button, int gamepadID) const;

		bool CheckButtonConditions(const ButtonConditions& condition, int gamepadID);

		friend class Singleton<InputManager>;
		InputManager();

		void PollInputThread();

	private:

		struct Gamepad
		{
			bool connected;
			XINPUT_GAMEPAD previousButtonState;
			XINPUT_GAMEPAD currentButtonState;

			WORD buttonsPressedThisFrame;
			WORD buttonsReleasedThisFrame;

			XINPUT_BATTERY_INFORMATION batteryInformation;
		};

		std::vector<Gamepad> m_Gamepads;
		std::unordered_map<ButtonConditions, CommandTriggerCondition> m_Keybinds;

		std::atomic<bool> m_IsRunning;
	public:
		std::vector<Gamepad>& GetGamepads() { return m_Gamepads; };

	};
}


