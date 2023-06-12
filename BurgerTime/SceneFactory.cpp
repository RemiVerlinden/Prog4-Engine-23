#include "SceneFactory.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Scene.h"
#include "AppComponents.h"
#include "GameObject.h"
#include "InputSystem.h"
#include "DeviceButtons.hpp"
#include <iostream>
#include "Locator.h"
#include "WorldPhysics.h"
#include "Structs.h"
#include <fstream>
#include <format>
#include "Common.h"
#include "Gamemode.h"

using namespace engine;

void engine::SceneFactory::CreateScenes()
{
	CONSOLE_NEWLINE();
	CONSOLE_NEWLINE();
	APP_TRACE("INPUT PLAYER 1: [WASD]       -	[GAMEPAD 1: DPAD] - [GAMEPAD 1: LEFT STICK]");
	APP_TRACE("INPUT PLAYER 2: [ARROW KEYS] -	[GAMEPAD 2: DPAD] - [GAMEPAD 2: LEFT STICK]");

	Scene* activeScene;

	Gamemode::GetInstance().SetGamemode(Gamemode::GameType::SinglePlayer);

	InitSceneKeybinds();
	activeScene = InitMainMenu();
	//InitBurgerTimeScene();
	InitEndScreen();

	engine::SceneManager::GetInstance().SetActiveGameScene(activeScene);

	Renderer::GetInstance().SetBackgroundColor(SDL_Color{ 0,0,0,255 });
}

void engine::SceneFactory::InitSceneKeybinds()
{
	engine::Input::InputSystem& inputSystem = engine::Input::InputSystem::GetInstance();
	engine::Input::CommandHandler& commandHandler = inputSystem.GetCommandHandler();

	using namespace engine::Input;

	auto bindChangeScene = [&commandHandler](InputDevice* pDevice, deviceButton button, ButtonPressType pressType, bool nextScene)
	{

		engine::Input::InputAction inputAction;
		inputAction.command = std::make_unique<ChangeSceneCommand>(nextScene);
		inputAction.device = pDevice;
		inputAction.pressType = pressType;

		commandHandler.BindNewAction(button, inputAction);
	};

	// KEYBOARD
	auto inputDevice = inputSystem.GetKeyboardDevice();
	bindChangeScene(inputDevice, KeyboardButton::KEY_PAGEDOWN, ButtonPressType::Release, false);
	bindChangeScene(inputDevice, KeyboardButton::KEY_PAGEUP, ButtonPressType::Release, true);
}

Scene* engine::SceneFactory::InitBurgerTimeStage(const std::string& stage, bool setActive)
{
	Scene* pScene = engine::SceneManager::GetInstance().AddGameScene(stage);
	pScene->SetPhysicsWorld(std::make_unique<WorldPhysics>());

	if (setActive)
	{
		SceneManager& sceneManager = SceneManager::GetInstance();
		sceneManager.SetActiveGameScene(pScene);
		sceneManager.CheckNewActiveGameScene();
	}

	GameObject* pWorldObject = pScene->MakeGameObject("world");
	pWorldObject->AddComponent<WorldDataComponent>();
	pWorldObject->AddComponent<GameOverComponent>();
	//Render2DComponent* textureComponent = pWorldObject->AddComponent<Render2DComponent>();
	//textureComponent->SetTexture("stage1.tga");
	//textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::customResolution);
	//textureComponent->SetResolution(WindowSettings::width,WindowSettings::height);

	GameObject* pPlayer1 = pScene->MakeGameObject("player1");
	pPlayer1->m_Transform->SetLocalPosition(16, 168, 0);
	PlayerComponent* player1Component = pPlayer1->AddComponent<PlayerComponent>();
	CirclePhysicsComponent* physicsComp = pPlayer1->AddComponent<CirclePhysicsComponent>();
	physicsComp->GetCircleCollider().radius = WorldData::defaultTileSize;

	// ASSIGN INPUT
	engine::Input::InputSystem& inputSystem = engine::Input::InputSystem::GetInstance();

	using namespace engine::Input;


	auto* pInputCommandComponent = pPlayer1->AddComponent<BindInputCommandComponent>();

	// GAMEPAD
	auto gamepadDevice = inputSystem.GetGamepadDevice(0);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(player1Component, glm::vec2{ -1, 0 }), gamepadDevice, ControllerButton::DPAD_LEFT, ButtonPressType::Hold);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(player1Component, glm::vec2{ 1, 0 }), gamepadDevice, ControllerButton::DPAD_RIGHT, ButtonPressType::Hold);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(player1Component, glm::vec2{ 0, 1 }), gamepadDevice, ControllerButton::DPAD_UP, ButtonPressType::Hold);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(player1Component, glm::vec2{ 0, -1 }), gamepadDevice, ControllerButton::DPAD_DOWN, ButtonPressType::Hold);
	//pInputCommandComponent->BindCommand(std::make_unique<DamageCommand>(go, glm::vec2{ 0,-1 }), gamepadDevice, ControllerButton::DPAD_DOWN, ButtonPressType::Hold);

	// KEYBOARD
	auto keyboardDevice = inputSystem.GetKeyboardDevice();
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(player1Component, glm::vec2{ -1, 0 }), keyboardDevice, KeyboardButton::KEY_A, ButtonPressType::Hold);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(player1Component, glm::vec2{ 1, 0 }), keyboardDevice, KeyboardButton::KEY_D, ButtonPressType::Hold);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(player1Component, glm::vec2{ 0, 1 }), keyboardDevice, KeyboardButton::KEY_W, ButtonPressType::Hold);
	pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(player1Component, glm::vec2{ 0, -1 }), keyboardDevice, KeyboardButton::KEY_S, ButtonPressType::Hold);


	Gamemode::GameType gamemode = Gamemode::GetInstance().GetGamemode();
	if (gamemode != Gamemode::GameType::SinglePlayer)
	{
		GameObject* pPlayer2 = pScene->MakeGameObject("player2");
		pPlayer2->m_Transform->SetLocalPosition(WindowSettings::defaultwidth - 16, 56, 0);
		PlayerComponent* player2Component = pPlayer2->AddComponent<PlayerComponent>();
		physicsComp = pPlayer2->AddComponent<CirclePhysicsComponent>();
		physicsComp->GetCircleCollider().radius = WorldData::defaultTileSize;


		// ASSIGN INPUT


		auto* pInputCommandComponent2 = pPlayer2->AddComponent<BindInputCommandComponent>();

		// GAMEPAD
		auto gamepadDevice2 = inputSystem.GetGamepadDevice(1);
		pInputCommandComponent2->BindCommand(std::make_unique<MoveCommand>(player2Component, glm::vec2{ -1, 0 }), gamepadDevice2, ControllerButton::DPAD_LEFT, ButtonPressType::Hold);
		pInputCommandComponent2->BindCommand(std::make_unique<MoveCommand>(player2Component, glm::vec2{ 1, 0 }), gamepadDevice2, ControllerButton::DPAD_RIGHT, ButtonPressType::Hold);
		pInputCommandComponent2->BindCommand(std::make_unique<MoveCommand>(player2Component, glm::vec2{ 0, 1 }), gamepadDevice2, ControllerButton::DPAD_UP, ButtonPressType::Hold);
		pInputCommandComponent2->BindCommand(std::make_unique<MoveCommand>(player2Component, glm::vec2{ 0, -1 }), gamepadDevice2, ControllerButton::DPAD_DOWN, ButtonPressType::Hold);
		//pInputCommandComponent->BindCommand(std::make_unique<DamageCommand>(go, glm::vec2{ 0,-1 }), gamepadDevice, ControllerButton::DPAD_DOWN, ButtonPressType::Hold);

		// KEYBOARD
		pInputCommandComponent2->BindCommand(std::make_unique<MoveCommand>(player2Component, glm::vec2{ -1, 0 }), keyboardDevice, KeyboardButton::KEY_LEFT, ButtonPressType::Hold);
		pInputCommandComponent2->BindCommand(std::make_unique<MoveCommand>(player2Component, glm::vec2{ 1, 0 }), keyboardDevice, KeyboardButton::KEY_RIGHT, ButtonPressType::Hold);
		pInputCommandComponent2->BindCommand(std::make_unique<MoveCommand>(player2Component, glm::vec2{ 0, 1 }), keyboardDevice, KeyboardButton::KEY_UP, ButtonPressType::Hold);
		pInputCommandComponent2->BindCommand(std::make_unique<MoveCommand>(player2Component, glm::vec2{ 0, -1 }), keyboardDevice, KeyboardButton::KEY_DOWN, ButtonPressType::Hold);
	}



	return pScene;
}


Scene* engine::SceneFactory::InitMainMenu()
{
	Scene* pScene = engine::SceneManager::GetInstance().AddGameScene("IntroSequence");

	GameObject* go = pScene->MakeGameObject();

	go->AddComponent<IntroSequenceComponent>();

	return pScene;
}

Scene* engine::SceneFactory::InitEndScreen()
{
	Scene* pScene = engine::SceneManager::GetInstance().AddGameScene("GameOverScene");

	GameObject* go = pScene->MakeGameObject();


	TextComponent* textComponet = go->AddComponent<TextComponent>();

	auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/burger-time.otf", 15);
	textComponet->SetFont(font);
	textComponet->SetText("GAME OVER");
	textComponet->SetPosition(WindowSettings::defaultwidth / 5, WindowSettings::defaultheight / 2);

	return nullptr;
}
