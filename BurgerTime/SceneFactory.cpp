#include "SceneFactory.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Scene.h"
#include "Components.h"
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

using namespace engine;

void engine::SceneFactory::CreateScenes()
{
	CONSOLE_NEWLINE();
	CONSOLE_NEWLINE();
	APP_TRACE("INPUT PLAYER 1: [WASD]       -	[GAMEPAD 1: DPAD] - [GAMEPAD 1: LEFT STICK]");
	APP_TRACE("INPUT PLAYER 2: [ARROW KEYS] -	[GAMEPAD 2: DPAD] - [GAMEPAD 2: LEFT STICK]");

	Scene* activeScene;

	InitSceneKeybinds();
	activeScene = InitMainMenu();
	InitBurgerTimeScene();

	InitSteamTestScene();
	InitDefaultScene();
	InitFpsDemoScene();
	InitBonusScene();

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

Scene* engine::SceneFactory::InitBurgerTimeScene()
{
	Scene* pScene = engine::SceneManager::GetInstance().AddGameScene("BurgerTime");
	pScene->SetPhysicsWorld(std::make_unique<WorldPhysics>());

	GameObject* pWorldObject = pScene->MakeGameObject("world");
	pWorldObject->AddComponent<WorldDataComponent>();
	//Render2DComponent* textureComponent = pWorldObject->AddComponent<Render2DComponent>();
	//textureComponent->SetTexture("stage1.tga");
	//textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::customResolution);
	//textureComponent->SetResolution(WindowSettings::width,WindowSettings::height);

	GameObject* pPlayer = pScene->MakeGameObject("player");
	pPlayer->AddComponent<PlayerComponent>();
	CirclePhysicsComponent* physicsComp = pPlayer->AddComponent<CirclePhysicsComponent>();
	physicsComp->GetCircleCollider().radius = WorldData::defaultTileSize;
	pPlayer->m_Transform->SetLocalPosition(110, 168, 0);


	return pScene;
}

Scene* engine::SceneFactory::InitMainMenu()
{
	Scene* pScene = engine::SceneManager::GetInstance().AddGameScene("IntroSequence");

	GameObject* go = pScene->MakeGameObject();

	go->AddComponent<IntroSequenceComponent>();
	
	return pScene;
}


Scene* engine::SceneFactory::InitSteamTestScene()
{
	Scene* pScene = engine::SceneManager::GetInstance().AddGameScene("SteamTest");
	//engine::SceneManager::GetInstance().SetActiveGameScene("SteamTest");

	// Background
	{
		GameObject* go = pScene->MakeGameObject();
		RenderComponent* textureComponent = go->AddComponent<RenderComponent>();
		textureComponent->SetTexture("demo/background.tga");
		textureComponent->SetDrawStyle(RenderComponent::DrawStyle::background);

		go->AddComponent<engine::ScoreBoardComponent>();
		go->AddComponent<engine::TestSoundComponent>();
	}

	// text comp 1
	{
		GameObject* go;

		auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/Lingua.otf", 20);
		go = pScene->MakeGameObject();

		auto textComponent = go->AddComponent<TextComponent>();
		textComponent->SetText("[K] start music");
		textComponent->SetFont(font);
		textComponent->SetPosition(100, 20);
	}

	// text comp 2
	{
		GameObject* go;

		auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/Lingua.otf", 20);
		go = pScene->MakeGameObject();

		auto textComponent = go->AddComponent<TextComponent>();
		textComponent->SetText("[L] fade out music");
		textComponent->SetFont(font);
		textComponent->SetPosition(100, 40);
	}

	// text comp 3
	{
		GameObject* go;

		auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/Lingua.otf", 20);
		go = pScene->MakeGameObject();

		auto textComponent = go->AddComponent<TextComponent>();
		textComponent->SetText("[I] play Sound Effect (can be played multiple times at once)");
		textComponent->SetFont(font);
		textComponent->SetPosition(100, 60);
	}

	// text comp 4
	{
		GameObject* go;

		auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/Lingua.otf", 20);
		go = pScene->MakeGameObject();

		auto textComponent = go->AddComponent<TextComponent>();
		textComponent->SetText("[J] toggle pause on all sounds");
		textComponent->SetFont(font);
		textComponent->SetPosition(100, 80);
	}

	return pScene;

}

Scene* engine::SceneFactory::InitDefaultScene()
{
	Scene* pScene = engine::SceneManager::GetInstance().AddGameScene("Demo");

	GameObject* go = pScene->MakeGameObject();
	//go->AddComponent<TrashTheCacheComponent>();

	// Background
	{
		RenderComponent* textureComponent = go->AddComponent<RenderComponent>();
		textureComponent->SetTexture("demo/background.tga");
		textureComponent->SetDrawStyle(RenderComponent::DrawStyle::background);
	}


	// texure 1 LOGO
	{
		go = pScene->MakeGameObject();
		RenderComponent* textureComponent = go->AddComponent<RenderComponent>();
		textureComponent->SetTexture("demo/logo.tga");
		textureComponent->SetPosition(216, 180);

	}


	// text comp 1
	{
		auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/Lingua.otf", 36);
		go = pScene->MakeGameObject();

		auto textComponent = go->AddComponent<TextComponent>();
		textComponent->SetText("Programming 4 Assignment");
		textComponent->SetFont(font);
		textComponent->SetPosition(80, 20);
	}


	// text comp 2
	{
		go = pScene->MakeGameObject();

		auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/lowres.ttf", 36);
		auto textComponent = go->AddComponent<TextComponent>();
		textComponent->SetText("Press [PAGE UP|DOWN] to change scene");
		textComponent->SetFont(font);
		textComponent->SetPosition(60, 420);
		textComponent->SetColor(255, 160, 100, 255);

	}

	// fps comp 1
	{
		go = pScene->MakeGameObject();
		auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/raju-bold.otf", 42);
		//auto font = engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
		FpsComponent* fpsComponent = go->AddComponent<FpsComponent>();
		fpsComponent->SetFont(font);
		go->SetPosition(270, 80);
		fpsComponent->SetColor(255, 255, 255, 255);
	}

	//// orbiting texture 1
	//{
	//	go = pScene->MakeGameObject();
	//	auto textureComponent = go->AddComponent<Render2DComponent>();
	//	textureComponent->SetTexture("cheff.png");
	//	textureComponent->SetPosition(300, 300);
	//	textureComponent->SetResolution(50, 50);
	//	textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::customResolution);

	//	auto orbitComp = go->AddComponent<OrbitComponent>();
	//	orbitComp->SetSpeed(-2.6f);
	//	orbitComp->SetRadius(50.f);


	//}

	//// orbiting texture 2
	//{
	//	auto parentGo = go; // previous go from RAII block above
	//	go = pScene->MakeGameObject();

	//	auto textureComponent = go->AddComponent<Render2DComponent>();
	//	textureComponent->SetTexture("bean.png");
	//	textureComponent->SetPosition(0, 0);
	//	textureComponent->SetResolution(50, 50);
	//	textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::customResolution);

	//	go->SetParent(parentGo.get(), true);
	//	auto orbitComp = go->AddComponent<OrbitComponent>();
	//	orbitComp->SetSpeed(6.4f);
	//	orbitComp->SetRadius(75.f);
	//}

	{
		go = pScene->MakeGameObject("Player1");
		auto textureComponent = go->AddComponent<RenderComponent>();
		textureComponent->SetTexture("cheff.png");
		textureComponent->SetPosition(300, 300);
		textureComponent->SetResolution(50, 50);
		textureComponent->SetDrawStyle(RenderComponent::DrawStyle::customResolution);


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

		// KEYBOARD
		auto keyboardDevice = inputSystem.GetKeyboardDevice();
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ -1, 0 }), keyboardDevice, KeyboardButton::KEY_A, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 1, 0 }), keyboardDevice, KeyboardButton::KEY_D, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0, 1 }), keyboardDevice, KeyboardButton::KEY_W, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0, -1 }), keyboardDevice, KeyboardButton::KEY_S, ButtonPressType::Hold);

		// GAMEPAD ANALOG STICK
		auto stick = gamepadDevice->GetGamepadState().GetAnalogStickFilteredPtr(false);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, stick), gamepadDevice, ControllerButton::THUMBSTICK_LEFT_MOVE, ButtonPressType::Hold);
	}

	{
		go = pScene->MakeGameObject("Player2");

		auto textureComponent = go->AddComponent<RenderComponent>();
		textureComponent->SetTexture("bean.png");
		textureComponent->SetPosition(350, 350);
		textureComponent->SetResolution(50, 50);
		textureComponent->SetDrawStyle(RenderComponent::DrawStyle::customResolution);

		auto moveComponent = go->AddComponent<MoveComponent>();
		moveComponent->SetMoveSpeed(300.f);

		// ASSIGN INPUT

		engine::Input::InputSystem& inputSystem = engine::Input::InputSystem::GetInstance();

		auto* pInputCommandComponent = go->AddComponent<BindInputCommandComponent>();

		using namespace engine::Input;

		// GAMEPAD
		auto gamepadDevice = inputSystem.GetGamepadDevice(1);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ -1, 0 }), gamepadDevice, ControllerButton::DPAD_LEFT, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 1, 0 }), gamepadDevice, ControllerButton::DPAD_RIGHT, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0, 1 }), gamepadDevice, ControllerButton::DPAD_UP, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0, -1 }), gamepadDevice, ControllerButton::DPAD_DOWN, ButtonPressType::Hold);

		// KEYBOARD
		auto keyboardDevice = inputSystem.GetKeyboardDevice();
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ -1, 0 }), keyboardDevice, KeyboardButton::KEY_LEFT, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 1, 0 }), keyboardDevice, KeyboardButton::KEY_RIGHT, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0, 1 }), keyboardDevice, KeyboardButton::KEY_UP, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0, -1 }), keyboardDevice, KeyboardButton::KEY_DOWN, ButtonPressType::Hold);



		// GAMEPAD ANALOG STICK
		auto stick = gamepadDevice->GetGamepadState().GetAnalogStickFilteredPtr(false);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, stick), gamepadDevice, ControllerButton::THUMBSTICK_LEFT_MOVE, ButtonPressType::Hold);

		// KEYBOARD DAMAGE PLAYER
		pInputCommandComponent->BindCommand(std::make_unique<DamageCommand>(go, 33), keyboardDevice, KeyboardButton::KEY_F, ButtonPressType::Press);
	}

	{
		go = pScene->MakeGameObject();

		go->AddComponent<engine::Input::InputDebugImguiComponent>();
		go->AddComponent<engine::ScoreBoardComponent>();
	}
	return pScene;

}

Scene* engine::SceneFactory::InitFpsDemoScene()
{

	Scene* pScene = engine::SceneManager::GetInstance().AddGameScene("Demo2");

	auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/raju-bold.otf", 72);

	//GameObject* fpsGO = pScene->MakeGameObject();
	//FpsComponent* fpsComponent = fpsGO->AddComponent<FpsComponent>();
	//fpsComponent->SetFont(font);
	//fpsComponent->SetPosition(200, 180);
	//fpsComponent->SetColor(200, 255, 255, 170);

	GameObject* go = pScene->MakeGameObject();
	RenderComponent* textureComponent = go->AddComponent<RenderComponent>();
	textureComponent->SetTexture("demo/colorbars.png");
	textureComponent->SetDrawStyle(engine::RenderComponent::DrawStyle::background);


	return pScene;

}

Scene* engine::SceneFactory::InitBonusScene()
{
	Scene* pScene = engine::SceneManager::GetInstance().AddGameScene("Bonus");

	GameObject* go;
	{
		go = pScene->MakeGameObject();
		RenderComponent* textureComponent = go->AddComponent<RenderComponent>();
		textureComponent->SetTexture("demo/backdrop_trees.png");
		textureComponent->SetPosition(-80, 0);
		textureComponent->SetResolution(800, 600);
		textureComponent->SetDrawStyle(engine::RenderComponent::DrawStyle::customResolution);
		Renderer::GetInstance().SetBackgroundColor(SDL_Color{ 100,140,230,255 });
	}
	// orbiting texture 1
	{
		go = pScene->MakeGameObject();
		auto textureComponent = go->AddComponent<RenderComponent>();
		textureComponent->SetTexture("demo/fatalerror.png");
		textureComponent->SetPosition(300, 200);
		textureComponent->SetResolution(100, 100);
		textureComponent->SetDrawStyle(RenderComponent::DrawStyle::customResolution);

		auto orbitComp = go->AddComponent<OrbitComponent>();
		orbitComp->SetSpeed(2.6f);
		orbitComp->SetRadius(100.f);
	}

	// orbiting texture 2
	{
		auto* parentGo = go; // previous go from RAII block above
		go = pScene->MakeGameObject();

		auto textureComponent = go->AddComponent<RenderComponent>();
		textureComponent->SetTexture("demo/colorbars.png");
		textureComponent->SetPosition(0, 0);
		textureComponent->SetResolution(100, 100);
		textureComponent->SetDrawStyle(RenderComponent::DrawStyle::customResolution);

		go->SetParent(parentGo, true);
		auto orbitComp = go->AddComponent<OrbitComponent>();
		orbitComp->SetSpeed(8.4f);
		orbitComp->SetRadius(150.f);
	}

	return pScene;

}
