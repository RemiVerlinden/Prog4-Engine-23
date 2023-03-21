#pragma once
#include "Singleton.h"
#include <memory>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Xinput.h"
#include "Command.h"
#include <vector>
#include <unordered_map>

namespace dae
{
	struct ButtonConditions
	{
		WORD buttonMask; // if all buttons from this mask are in the required phase -> then buttonCodition will trigger
		WORD buttonPhase; // the button phase: [DOWN|PRESSED|RELEASED]
	};


	enum class PLAYERACTION
	{
		MOVELEFT,
		MOVERIGHT,
		MOVEUP,
		MOVEDOWN,
		JUMP,
		SHOOT,
		DASH,
		QUIT
	};
	struct CommandTriggerCondition
	{
		PLAYERACTION action;
		int gamepadID; // [0-3]
		std::unique_ptr<Command> pCommand;
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput(const UpdateContext& context);

		void AddKeybind(PLAYERACTION action, ButtonConditions buttonConditions);
		void AddCommandLinkedToPlayerAction(CommandTriggerCondition& commandTriggerCondition);
	private:
		bool ProcessSDLEvent();
		void ProcessXInput();
		void ExecuteCommands(const UpdateContext& context);

		struct GamepadState;
		inline void UpdateGamepadButtonStates(DWORD gamepadIndex);
		inline void UpdateGamepadBatteryInformation(DWORD gamepadIndex);
		bool IsDown(WORD button, int gamepadID) const;
		bool IsPressed(WORD button, int gamepadID) const;
		bool IsReleased(WORD button, int gamepadID) const;

		bool CheckButtonConditions(const ButtonConditions& condition, int gamepadID);

		friend class Singleton<InputManager>;
		InputManager();

	private:

		struct GamepadState
		{
			bool connected;
			XINPUT_GAMEPAD previousButtonState;
			XINPUT_GAMEPAD currentButtonState;

			WORD buttonsPressedThisFrame;
			WORD buttonsReleasedThisFrame;

			XINPUT_BATTERY_INFORMATION batteryInformation;
		};

		std::vector<GamepadState> m_GamepadStates;
		std::unordered_map<PLAYERACTION, ButtonConditions> m_Keybinds;
		std::vector<CommandTriggerCondition> m_CommandsLinkedToPlayerAction;
	};

}


