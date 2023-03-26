#include "CommandHandler.h"
#include "InputDeviceGamepad.h"
#include "InputDeviceKeyboardMouse.h"
void dae::Input::CommandHandler::Update(Seconds elapsedTime)
{
	for (auto& bindAction : m_BindActions)
	{
		const InputAction& inputAction = InputAction{};
		//const InputAction& inputAction = bindAction.first;
		Command* command = bindAction.second;

		bool executeCommand = false;
		switch (inputAction.device->GetDeviceCategory())
		{
			case DeviceCategory::Gamepad:
			{
				InputDeviceGamepad* gamepadDevice = static_cast<InputDeviceGamepad*>(inputAction.device);

				switch (inputAction.pressType)
				{
					case ButtonPressType::Press:
						executeCommand = gamepadDevice->GetGamepadState().WasPressed(inputAction.button);
						break;
					case ButtonPressType::Hold:
						executeCommand = gamepadDevice->GetGamepadState().IsHeldDown(inputAction.button);
						break;
					case ButtonPressType::Release:
						executeCommand = gamepadDevice->GetGamepadState().WasReleased(inputAction.button);
						break;
				}
				break;
			}
			case DeviceCategory::KeyboardMouse:
			{
				InputDeviceKeyboardMouse* keyboardMouseDevice = static_cast<InputDeviceKeyboardMouse*>(inputAction.device);
				uint8_t button = (uint8_t)inputAction.button;

				if (button == SDL_BUTTON(SDL_BUTTON_LEFT))
				{
					switch (inputAction.pressType)
					{
						case ButtonPressType::Press:
							executeCommand = keyboardMouseDevice->GetMouseState().WasPressed(button);
							break;
						case ButtonPressType::Hold:
							executeCommand = keyboardMouseDevice->GetMouseState().IsHeldDown(button);
							break;
						case ButtonPressType::Release:
							executeCommand = keyboardMouseDevice->GetMouseState().WasReleased(button);
							break;
					}
				}
				else
				{

					//switch (inputAction.pressType)
					//{
					//	case ButtonPressType::Press:
					//		executeCommand = keyboardMouseDevice->GetKeyboardState().WasPressed(button);
					//		break;
					//	case ButtonPressType::Hold:
					//		executeCommand = keyboardMouseDevice->GetKeyboardState().IsHeldDown(button);
					//		break;
					//	case ButtonPressType::Release:
					//		executeCommand = keyboardMouseDevice->GetKeyboardState().WasReleased(button);
					//		break;
					//}
				}
			}
			break;
		}
		if (executeCommand) command->Execute(elapsedTime);
	}

}

void dae::Input::CommandHandler::UpdateActionsCommandsList(InputActionCommandList& actionCommandList)
{
	m_ActionCommandList = actionCommandList;
	UpdateActionBindings();
}

void dae::Input::CommandHandler::UpdateActionBindings()
{
	//for (auto& actionCommandPair : *m_ActionCommandList.GetList())
	//{
	//	//m_BindActions.emplace(std::make_pair(actionCommandPair.first, actionCommandPair.second.get()));
	//}
}
