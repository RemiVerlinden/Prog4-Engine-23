#include "InputDeviceGamepad.h"
#include <assert.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Xinput.h"

#include <array>
#include <vector>


// okay this is terrible, I dont know how else to implement this 
// sorry for this giant clusterfuck
//-------------------------------------------------------------------------

namespace dae::Input
{

	static float const g_maxThumbstickRange = 32767.0f;
	static float const g_maxTriggerRange = 255.0f;

	class dae::Input::InputStateGamepad::InputStateGamepadImpl
	{
	public:
		struct GamepadButtonStates
		{
			bool connected = false;
			XINPUT_GAMEPAD previousButtonState{};
			XINPUT_GAMEPAD currentButtonState{};

			WORD buttonsPressedThisFrame{};
			WORD buttonsReleasedThisFrame{};

			XINPUT_BATTERY_INFORMATION batteryInformation{};
		};
	public:
		GamepadButtonStates								m_ButtonStates;
		std::array<glm::vec2, Direction::size>			m_analogInputRaw = { glm::vec2{}, glm::vec2{} };
		std::array<glm::vec2, Direction::size>			m_analogInputFiltered = { glm::vec2{}, glm::vec2{} };
		std::array<float, Direction::size>				m_triggerRaw = { 0.0f, 0.0f };
		std::array<float, Direction::size>				m_triggerFiltered = { 0.0f, 0.0f };
		bool											m_IsConnected = false;
	public:

		inline glm::vec2 GetLeftAnalogStickValue() const { return m_analogInputFiltered[Left]; }
		const glm::vec2* GetAnalogStickFilteredPtr(bool rightStick) const { return &m_analogInputFiltered[static_cast<Direction>(rightStick)]; }
		inline glm::vec2 GetRightAnalogStickValue() const { return m_analogInputFiltered[Right]; }
		inline float GetLeftTriggerValue() const { return m_triggerFiltered[Left]; }
		inline float GetRightTriggerValue() const { return m_triggerFiltered[Right]; }
		inline glm::vec2 GetLeftAnalogStickRawValue() const { return m_analogInputRaw[Left]; }
		inline glm::vec2 GetRightAnalogStickRawValue() const { return m_analogInputRaw[Right]; }
		inline float GetLeftTriggerRawValue() const { return m_triggerRaw[Left]; }
		inline float GetRightTriggerRawValue() const { return m_triggerRaw[Right]; }
		void SetConnection(bool connected) { m_IsConnected = connected; }

		//-------------------------------------------------------------------------

		bool WasPressed(deviceButton button) const
		{
			return (m_ButtonStates.buttonsPressedThisFrame & GetGamepadButton(button));
		}

		bool WasReleased(deviceButton button) const
		{
			return (m_ButtonStates.buttonsReleasedThisFrame & GetGamepadButton(button));
		}

		bool IsHeldDown(deviceButton button) const
		{
			return (m_ButtonStates.currentButtonState.wButtons & GetGamepadButton(button));
		}

		bool IsHeldDown(int button) const
		{
			return (m_ButtonStates.currentButtonState.wButtons & button);
		}

		inline bool IsConnected() const { return m_IsConnected; }

		inline int GetGamepadButton(deviceButton button) const
		{
			const int buttonType = static_cast<int>(DeviceButtonType::Gamepad);
			return std::get<buttonType>(button);
		}

		bool ProcessInput(Seconds deltaTime, XINPUT_STATE& gamepadState)
		{
			std::swap(m_ButtonStates.previousButtonState, m_ButtonStates.currentButtonState);
			m_ButtonStates.currentButtonState = gamepadState.Gamepad;

			ProcesStickInput(m_ButtonStates.currentButtonState);

			WORD buttonChanges = m_ButtonStates.currentButtonState.wButtons ^ m_ButtonStates.previousButtonState.wButtons;
			m_ButtonStates.buttonsPressedThisFrame = buttonChanges & m_ButtonStates.currentButtonState.wButtons;
			m_ButtonStates.buttonsReleasedThisFrame = buttonChanges & (~m_ButtonStates.currentButtonState.wButtons);


			(deltaTime);
			return true;
		};

		void ProcesStickInput(XINPUT_GAMEPAD& gamepadState)
		{

			glm::vec2& left = m_analogInputFiltered[Left];
			glm::vec2& right = m_analogInputFiltered[Right];

			if ((left.x + left.y) != 0.f)
				gamepadState.wButtons |= XINPUT_GAMEPAD_LEFT_STICK;
			if ((right.x + right.y) != 0.f)
				gamepadState.wButtons |= XINPUT_GAMEPAD_RIGHT_STICK;
		}

	};
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------

	class dae::Input::InputDeviceGamepad::InputDeviceGamepadImpl
	{
	public:

		InputDeviceGamepadImpl(uint8_t GamepadIndex) 
			: m_GamepadIndex(GamepadIndex)
			, m_GamepadState{}
			, m_GamepadStateImpl(m_GamepadState.m_pImpl)
		{}

		const GamepadSettings& GetSettings() const { return m_settings; }
		const InputStateGamepad& GetGamepadState() const { return m_GamepadState; }

		bool IsConnected() const { return m_GamepadState.IsConnected(); }
		void SetConnection(bool connection) { m_GamepadState.SetConnection(connection); }
		uint8_t GetGamepadIndex()const { return m_GamepadIndex; }

		void Initialize()
		{
			XINPUT_STATE gamepadState;
			DWORD result = XInputGetState(m_GamepadIndex, &gamepadState);
			m_GamepadState.SetConnection((result == ERROR_SUCCESS));

			m_settings.m_leftStickInnerDeadzone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / g_maxThumbstickRange;
			m_settings.m_leftStickOuterDeadzone = 0.0f;
			m_settings.m_leftTriggerThreshold = XINPUT_GAMEPAD_TRIGGER_THRESHOLD / g_maxTriggerRange;

			m_settings.m_rightStickInnerDeadzone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / g_maxThumbstickRange;
			m_settings.m_rightStickOuterDeadzone = 0.0f;
			m_settings.m_rightTriggerThreshold = XINPUT_GAMEPAD_TRIGGER_THRESHOLD / g_maxTriggerRange;
		}
		void Shutdown();


		bool ProcessInput(Seconds deltaTime)
		{
			XINPUT_STATE gamepadState;
			DWORD result = XInputGetState(m_GamepadIndex, &gamepadState);
			m_GamepadState.SetConnection((result == ERROR_SUCCESS));

			if (m_GamepadState.IsConnected())
			{
				SetTriggerValues(gamepadState.Gamepad.bLeftTrigger / g_maxTriggerRange, gamepadState.Gamepad.bRightTrigger / g_maxTriggerRange);
				SetAnalogStickValues(glm::vec2(gamepadState.Gamepad.sThumbLX / g_maxThumbstickRange, gamepadState.Gamepad.sThumbLY / g_maxThumbstickRange), glm::vec2(gamepadState.Gamepad.sThumbRX / g_maxThumbstickRange, gamepadState.Gamepad.sThumbRY / g_maxThumbstickRange));

				m_GamepadStateImpl->ProcessInput(deltaTime, gamepadState);
			}

			return true;
		};

		void SetTriggerValues(float leftRawValue, float rightRawValue)
		{
			auto calculateFilteredTriggerValue = [](float rawValue, float threshold)
			{
				assert(threshold >= 0 && threshold <= 1.0f);

				float filteredValue = 0.0f;
				if (rawValue > threshold)
				{
					filteredValue = (rawValue - threshold) / (1.0f - threshold);
				}

				return filteredValue;
			};

			//-------------------------------------------------------------------------
			m_GamepadStateImpl->m_triggerRaw[Direction::Left] = leftRawValue;
			m_GamepadStateImpl->m_triggerRaw[Direction::Right] = rightRawValue; 
			m_GamepadStateImpl->m_triggerFiltered[Direction::Left] = calculateFilteredTriggerValue(leftRawValue, m_settings.m_leftTriggerThreshold);
			m_GamepadStateImpl->m_triggerFiltered[Direction::Right] = calculateFilteredTriggerValue(rightRawValue, m_settings.m_rightTriggerThreshold);
		};
		void SetAnalogStickValues(const glm::vec2& leftRawValue, const glm::vec2& rightRawValue)
		{
			auto calculateRawValue = [](const glm::vec2 rawValue, bool bInvertY)
			{
				float const normalizedX = glm::clamp(rawValue.x, -1.0f, 1.0f);
				float const normalizedY = glm::clamp(rawValue.y, -1.0f, 1.0f);
				return glm::vec2(normalizedX, bInvertY ? -normalizedY : normalizedY);
			};

			m_GamepadStateImpl->m_analogInputRaw[Direction::Left] = calculateRawValue(leftRawValue, m_settings.m_leftStickInvertY);
			m_GamepadStateImpl->m_analogInputRaw[Direction::Right] = calculateRawValue(rightRawValue, m_settings.m_rightStickInvertY);

			auto calculateFilteredValue = [](const glm::vec2& rawValue, const float innerDeadzoneRange, const float outerDeadzoneRange) -> glm::vec2
			{
				assert(innerDeadzoneRange >= 0 && innerDeadzoneRange <= 1.0f && outerDeadzoneRange >= 0 && outerDeadzoneRange <= 1.0f);

				glm::vec2 filteredValue;

				// Get the direction and magnitude
				glm::vec2 direction{ rawValue };
				float magnitude = glm::length(direction);
				direction = glm::normalize(direction);

				// Apply dead zones
				if (magnitude > innerDeadzoneRange)
				{
					const float remainingRange = (1.0f - outerDeadzoneRange - innerDeadzoneRange);
					const float magnitudeWithoutDeadzones = ((magnitude - innerDeadzoneRange) / remainingRange);
					const float clampedMagnitude = (((1.0f) < (magnitudeWithoutDeadzones)) ? (1.0f) : (magnitudeWithoutDeadzones)); // THIS IS LITERALLY COPY PASTE FROM GLM::MIN BECAUSE YOU DONT WANT TO WORK WHEN CALLED DIRECTLY
					filteredValue = (direction * clampedMagnitude);
				}
				else // Set the value to zero
				{
					filteredValue = glm::vec2{};
				}

				return filteredValue;
			};

			m_GamepadStateImpl->m_analogInputFiltered[Direction::Left] = calculateFilteredValue(m_GamepadStateImpl->m_analogInputRaw[Direction::Left], m_settings.m_leftStickInnerDeadzone, m_settings.m_leftStickOuterDeadzone);
			m_GamepadStateImpl->m_analogInputFiltered[Direction::Right] = calculateFilteredValue(m_GamepadStateImpl->m_analogInputRaw[Direction::Right], m_settings.m_rightStickInnerDeadzone, m_settings.m_rightStickOuterDeadzone);

		}


		DeviceCategory GetDeviceCategory() const { return DeviceCategory::Gamepad; }

	private:
		GamepadSettings                             m_settings;
		InputStateGamepad							m_GamepadState;
		InputStateGamepad::InputStateGamepadImpl*	m_GamepadStateImpl;
		uint8_t                                     m_GamepadIndex;
	};

//-------------------------------------------------------------------------


//-------------------------------------------------------------------------


	InputDeviceGamepad::InputDeviceGamepad(uint8_t gamepadIndex)
		:m_pImpl(new InputDeviceGamepadImpl(gamepadIndex))
	{
	}

	InputDeviceGamepad::~InputDeviceGamepad()
	{
                  		delete m_pImpl;
	}

	const GamepadSettings& InputDeviceGamepad::GetSettings() const
	{
		return m_pImpl->GetSettings();
	}

	const InputStateGamepad& InputDeviceGamepad::GetGamepadState() const
	{
		return m_pImpl->GetGamepadState();
	}

	bool InputDeviceGamepad::IsConnected() const
	{
		return m_pImpl->IsConnected();
	}

	void InputDeviceGamepad::SetConnection(bool connection)
	{
		m_pImpl->SetConnection(connection);
	}

	uint8_t InputDeviceGamepad::GetGamepadIndex() const
	{
		return m_pImpl->GetGamepadIndex();
	}

	void InputDeviceGamepad::Initialize()
	{
		m_pImpl->Initialize();
	}

	void InputDeviceGamepad::Shutdown()
	{
		m_pImpl->SetConnection(false);
	}

	bool InputDeviceGamepad::ProcessInput(Seconds deltaTime)
	{
		return m_pImpl->ProcessInput(deltaTime);
	}

	void InputDeviceGamepad::SetTriggerValues(float leftRawValue, float rightRawValue)
	{
		m_pImpl->SetTriggerValues(leftRawValue, rightRawValue);
	}

	void InputDeviceGamepad::SetAnalogStickValues(glm::vec2 const& leftRawValue, glm::vec2 const& rightRawValue)
	{
		m_pImpl->SetAnalogStickValues(leftRawValue, rightRawValue);
	}
	DeviceCategory InputDeviceGamepad::GetDeviceCategory() const
	{
		return m_pImpl->GetDeviceCategory();
	}

	//-------------------------------------------------------------------------















//-------------------------------------------------------------------------

	InputStateGamepad::InputStateGamepad()
		: m_pImpl(new InputStateGamepadImpl())
	{
	}

	InputStateGamepad::~InputStateGamepad()
	{
		delete m_pImpl;
	}

	glm::vec2 dae::Input::InputStateGamepad::GetLeftAnalogStickValue() const
	{
		return m_pImpl->GetLeftAnalogStickValue();
	}

	const glm::vec2* dae::Input::InputStateGamepad::GetAnalogStickFilteredPtr(bool rightStick) const
	{
		return m_pImpl->GetAnalogStickFilteredPtr(rightStick);
	}

	glm::vec2 dae::Input::InputStateGamepad::GetRightAnalogStickValue() const
	{
		return m_pImpl->GetRightAnalogStickValue();
	}

	float dae::Input::InputStateGamepad::GetLeftTriggerValue() const
	{
		return m_pImpl->GetLeftTriggerValue();
	}

	float dae::Input::InputStateGamepad::GetRightTriggerValue() const
	{
		return m_pImpl->GetRightTriggerValue();
	}

	glm::vec2 dae::Input::InputStateGamepad::GetLeftAnalogStickRawValue() const
	{
		return m_pImpl->GetLeftAnalogStickRawValue();
	}

	glm::vec2 dae::Input::InputStateGamepad::GetRightAnalogStickRawValue() const
	{
		return m_pImpl->GetRightAnalogStickRawValue();
	}

	float dae::Input::InputStateGamepad::GetLeftTriggerRawValue() const
	{
		return m_pImpl->GetLeftTriggerRawValue();
	}

	float dae::Input::InputStateGamepad::GetRightTriggerRawValue() const
	{
		return m_pImpl->GetRightTriggerRawValue();
	}

	bool dae::Input::InputStateGamepad::WasPressed(deviceButton button) const
	{
		return m_pImpl->WasPressed(button);
	}

	bool dae::Input::InputStateGamepad::WasReleased(deviceButton button) const
	{
		return m_pImpl->WasReleased(button);
	}

	bool dae::Input::InputStateGamepad::IsHeldDown(deviceButton button) const
	{
		return m_pImpl->IsHeldDown(button);
	}

	bool dae::Input::InputStateGamepad::IsHeldDown(int button) const
	{
		return m_pImpl->IsHeldDown(button);
	}

	bool dae::Input::InputStateGamepad::IsConnected() const
	{
		return m_pImpl->IsConnected();
	}

	void dae::Input::InputStateGamepad::SetConnection(bool connected) const
	{
		m_pImpl->SetConnection(connected);
	}

	int dae::Input::InputStateGamepad::GetGamepadButton(deviceButton button) const
	{
		return m_pImpl->GetGamepadButton(button);
	}
}