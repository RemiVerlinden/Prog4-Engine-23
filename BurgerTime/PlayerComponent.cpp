#include "PlayerComponent.h"
#include "Components.h"
#include <memory>
#include "WorldDataComponent.h"

void engine::PlayerComponent::Initialize()
{
	GameObject* go = m_GameObject;
	auto textureComponent = go->AddComponent<RenderComponent>();

	textureComponent->SetTexture("cheff.png");

	textureComponent->SetPosition(-WorldData::defaultTileSize, -WorldData::defaultTileSize);
	textureComponent->SetResolution(static_cast<int>(WorldData::defaultTileSize * 2), static_cast<int>(WorldData::defaultTileSize *2));
	textureComponent->SetDrawStyle(RenderComponent::DrawStyle::customResolution);

	InitializeInputCommands();

}

void engine::PlayerComponent::OnSceneActivate()
{
	using namespace engine;
	std::string soundPath = "main.wav";
	Locator::GetSoundSystem().Play(soundPath, 1.f, false);

}



void engine::PlayerComponent::InitializeInputCommands()
{
	GameObject* go = m_GameObject;

	// ASSIGN INPUT
	engine::Input::InputSystem& inputSystem = engine::Input::InputSystem::GetInstance();

	using namespace engine::Input;

	auto* pInputCommandComponent = go->AddComponent<BindInputCommandComponent>();

	// GAMEPAD
	auto gamepadDevice = inputSystem.GetGamepadDevice(0);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ -1, 0 }), gamepadDevice, ControllerButton::DPAD_LEFT, ButtonPressType::Hold);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 1, 0 }), gamepadDevice, ControllerButton::DPAD_RIGHT, ButtonPressType::Hold);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0, 1 }), gamepadDevice, ControllerButton::DPAD_UP, ButtonPressType::Hold);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0, -1 }), gamepadDevice, ControllerButton::DPAD_DOWN, ButtonPressType::Hold);
	//pInputCommandComponent->BindCommand(std::make_unique<DamageCommand>(go, glm::vec2{ 0,-1 }), gamepadDevice, ControllerButton::DPAD_DOWN, ButtonPressType::Hold);

	// KEYBOARD
	auto keyboardDevice = inputSystem.GetKeyboardDevice();
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ -1, 0 }), keyboardDevice, KeyboardButton::KEY_A, ButtonPressType::Hold);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 1, 0 }), keyboardDevice, KeyboardButton::KEY_D, ButtonPressType::Hold);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0, 1 }), keyboardDevice, KeyboardButton::KEY_W, ButtonPressType::Hold);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0, -1 }), keyboardDevice, KeyboardButton::KEY_S, ButtonPressType::Hold);
	pInputCommandComponent->BindCommand(std::make_unique<DamageCommand>(go, 33), keyboardDevice, KeyboardButton::KEY_F, ButtonPressType::Press);

	// GAMEPAD ANALOG STICK
	auto stick = gamepadDevice->GetGamepadState().GetAnalogStickFilteredPtr(false);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, stick), gamepadDevice, ControllerButton::THUMBSTICK_LEFT_MOVE, ButtonPressType::Hold);
}

