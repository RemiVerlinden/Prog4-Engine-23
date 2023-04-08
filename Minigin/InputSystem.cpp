#include "InputSystem.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Xinput.h"

//-------------------------------------------------------------------------

namespace dae::Input
{
	InputStateMouse const InputSystem::s_emptyMouseState = InputStateMouse();
	InputStateKeyboard const InputSystem::s_emptyKeyboardState = InputStateKeyboard();
	InputStateGamepad const InputSystem::s_emptyGamepadState = InputStateGamepad();

	//-------------------------------------------------------------------------

	bool InputSystem::Initialize() // Currently only account for 4 gamepads, always create ONE keyboard and mouse device for now so I have no Idea how to handle multiple keyboards and mice
	{

		m_CommandHandler = std::make_unique<CommandHandler>();
		// Create Gamepads
		//-------------------------------------------------------------------------

		for (uint8_t GamepadIdx = 0; GamepadIdx < XUSER_MAX_COUNT; ++GamepadIdx)
		{
			m_inputDevices.push_back(std::move(std::make_unique<InputDeviceGamepad>(GamepadIdx)));
		}

		//-------------------------------------------------------------------------

		// Create a keyboard and mouse device
		m_inputDevices.emplace_back(std::make_unique<InputDeviceMouse>(InputDeviceMouse(m_SDLEventQueue)));
		m_inputDevices.emplace_back(std::make_unique<InputDeviceKeyboard>(InputDeviceKeyboard(m_SDLEventQueue)));

		//-------------------------------------------------------------------------
		for (auto& pDevice : m_inputDevices)
		{
			pDevice->Initialize();
		}

		return true;
	}

	void InputSystem::Shutdown()
	{
		for (auto& pInputDevice : m_inputDevices)
		{
			pInputDevice->Shutdown();
		}
	}

	bool InputSystem::ProcessInput(const UpdateContext& context)
	{
		bool doContinue = true;
		Seconds deltaTime = context.GetDeltaTime();

		static dae::CyclicTimer<EngineClock> cyclicTimer{ 1000.f / context.GetMaxInputPollingRate() };


		if (cyclicTimer.Update())
		{
			 doContinue = m_SDLEventQueue.PollEvents();
			for (auto& pInputDevice : m_inputDevices)
			{
				pInputDevice->ProcessInput(deltaTime);
			}
			m_SDLEventQueue.EmptyQueue();
		}

		m_CommandHandler.get()->Update(deltaTime);

		return doContinue;
	}

	InputStateMouse const* InputSystem::GetMouseState() const
	{
		if (auto pDevice = GetMouseDevice())
		{
			return &pDevice->GetMouseState();
		}

		return &s_emptyMouseState;
	}

	InputStateKeyboard const* InputSystem::GetKeyboardState() const
	{
		if (auto pDevice = GetKeyboardDevice())
		{
			return &pDevice->GetKeyboardState();
		}

		return &s_emptyKeyboardState;
	}

	//-------------------------------------------------------------------------

	InputDeviceMouse* InputSystem::GetMouseDevice() const
	{
		for (auto& pDevice : m_inputDevices)
		{
			if (pDevice->GetDeviceCategory() == DeviceCategory::Mouse)
			{
				return static_cast<InputDeviceMouse*>(pDevice.get());
			}
		}

		return nullptr;
	}

	InputDeviceKeyboard* InputSystem::GetKeyboardDevice() const
	{
		for (auto& pDevice : m_inputDevices)
		{
			if (pDevice->GetDeviceCategory() == DeviceCategory::Keyboard)
			{
				return static_cast<InputDeviceKeyboard*>(pDevice.get());
			}
		}

		return nullptr;
	}

	//-------------------------------------------------------------------------

	uint32_t InputSystem::GetNumConnectedGamepads() const
	{
		uint32_t numGamepads = 0;
		for (auto& pDevice : m_inputDevices)
		{
			if (pDevice->GetDeviceCategory() == DeviceCategory::Gamepad)
			{
				auto pGamepad = static_cast<InputDeviceGamepad*>(pDevice.get());
				if (pGamepad->IsConnected())
				{
					++numGamepads;
				}
			}
		}

		return numGamepads;
	}

	InputStateGamepad const* InputSystem::GetGamepadState(uint8_t GamepadIdx) const
	{
		if (auto pDevice = GetGamepadDevice(GamepadIdx))
		{
			return &pDevice->GetGamepadState();
		}

		return &s_emptyGamepadState;
	}

	InputDeviceGamepad* InputSystem::GetGamepadDevice(uint8_t GamepadIdx) const
	{
		for (auto& pDevice : m_inputDevices)
		{
			if (pDevice->GetDeviceCategory() == DeviceCategory::Gamepad)
			{
				auto pGamepad = static_cast<InputDeviceGamepad*>(pDevice.get());

				if (pGamepad->GetGamepadIndex() != GamepadIdx) continue;

				return pGamepad;
			}
		}

		return nullptr;
	}
}