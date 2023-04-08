#pragma once
#include "BaseComponent.h"
#include "UpdateContext.h"
#include "InputSystem.h"
#include "DeviceButtons.hpp"
#include <memory>
namespace dae
{
	class BindInputCommandComponent final : public BaseComponent
	{
	public:
		BindInputCommandComponent() :BaseComponent() {};
		void Initialize() {};

		void BindCommand( std::unique_ptr<Command>&& pCommand,Input::InputDevice* pDevice, Input::deviceButton button, Input::ButtonPressType pressType)
		{
			Input::InputSystem& inputSystem = dae::Input::InputSystem::GetInstance();
			Input::CommandHandler& commandHandler = inputSystem.GetCommandHandler();

			dae::Input::InputAction inputAction;
			inputAction.command = std::move(pCommand);
			inputAction.device = pDevice;
			inputAction.pressType = pressType;

			commandHandler.BindNewAction(button, inputAction);
		}


		BindInputCommandComponent(const BindInputCommandComponent& other) = delete;
		BindInputCommandComponent(BindInputCommandComponent&& other) = delete;
		BindInputCommandComponent& operator=(const BindInputCommandComponent& other) = delete;
		BindInputCommandComponent& operator=(BindInputCommandComponent&& other) = delete;
	private:

	};
}

