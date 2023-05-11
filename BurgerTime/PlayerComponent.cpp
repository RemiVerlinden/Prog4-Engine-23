#include "PlayerComponent.h"
#include "Components.h"
#include <memory>

void dae::PlayerComponent::Initialize()
{
	GameObject* go = m_GameObject;
	auto textureComponent = go->AddComponent<Render2DComponent>();
	textureComponent->SetCanBeCloned(false);

	textureComponent->SetTexture("cheff.png");
	textureComponent->SetPosition(300, 300);
	textureComponent->SetResolution(50, 50);
	textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::positionScale);

	// ASSIGN INPUT
	dae::Input::InputSystem& inputSystem = dae::Input::InputSystem::GetInstance();

	using namespace dae::Input;

	auto* pInputCommandComponent = go->AddComponent<BindInputCommandComponent>();
	pInputCommandComponent->SetCanBeCloned(false);

	// GAMEPAD
	auto gamepadDevice = inputSystem.GetGamepadDevice(0);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ -1,0 }), gamepadDevice, ControllerButton::DPAD_LEFT, ButtonPressType::Hold);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 1,0 }), gamepadDevice, ControllerButton::DPAD_RIGHT, ButtonPressType::Hold);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0,1 }), gamepadDevice, ControllerButton::DPAD_UP, ButtonPressType::Hold);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0,-1 }), gamepadDevice, ControllerButton::DPAD_DOWN, ButtonPressType::Hold);
	//pInputCommandComponent->BindCommand(std::make_unique<DamageCommand>(go, glm::vec2{ 0,-1 }), gamepadDevice, ControllerButton::DPAD_DOWN, ButtonPressType::Hold);

	// KEYBOARD
	auto keyboardDevice = inputSystem.GetKeyboardDevice();
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ -1,0 }), keyboardDevice, KeyboardButton::KEY_A, ButtonPressType::Hold);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 1,0 }), keyboardDevice, KeyboardButton::KEY_D, ButtonPressType::Hold);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0,1 }), keyboardDevice, KeyboardButton::KEY_W, ButtonPressType::Hold);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0,-1 }), keyboardDevice, KeyboardButton::KEY_S, ButtonPressType::Hold);
	pInputCommandComponent->BindCommand(std::make_unique<DamageCommand>(go, 33), keyboardDevice, KeyboardButton::KEY_F, ButtonPressType::Press);

	// GAMEPAD ANALOG STICK
	auto stick = gamepadDevice->GetGamepadState().GetAnalogStickFilteredPtr(false);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, stick), gamepadDevice, ControllerButton::THUMBSTICK_LEFT_MOVE, ButtonPressType::Hold);
}

void dae::PlayerComponent::Clone(GameObject* clone)
{
	if (CanBeCloned() == false) return;
		clone->AddComponent<PlayerComponent>();
}
