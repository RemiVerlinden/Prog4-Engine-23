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
#include "Prefab.h"
#include "WorldPhysics.h"

using namespace dae;

void dae::SceneFactory::CreateScenes()
{
	CONSOLE_NEWLINE();
	CONSOLE_NEWLINE();
	APP_TRACE("INPUT PLAYER 1: [WASD]       -	[GAMEPAD 1: DPAD] - [GAMEPAD 1: LEFT STICK]");
	APP_TRACE("INPUT PLAYER 2: [ARROW KEYS] -	[GAMEPAD 2: DPAD] - [GAMEPAD 2: LEFT STICK]");

	InitSceneKeybinds();

	InitBurgerTimeScene();

	InitSteamTestScene();
	InitDefaultScene();
	InitFpsDemoScene();
	InitBonusScene();
}

void dae::SceneFactory::InitSceneKeybinds()
{
	dae::Input::InputSystem& inputSystem = dae::Input::InputSystem::GetInstance();
	dae::Input::CommandHandler& commandHandler = inputSystem.GetCommandHandler();

	using namespace dae::Input;

	auto bindChangeScene = [&commandHandler](InputDevice* pDevice, deviceButton button, ButtonPressType pressType, bool nextScene)
	{

		dae::Input::InputAction inputAction;
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

void dae::SceneFactory::InitBurgerTimeScene()
{
	Scene* pScene = dae::SceneManager::GetInstance().AddGameScene("BurgerTime");
	pScene->SetPhysicsWorld(std::make_unique<WorldPhysics>());

	GameObject* pWorldObject = pScene->MakeGameObject();
	pWorldObject->AddComponent<WorldDataComponent>();
	

	GameObject* pPlayer = pScene->MakeGameObject();
	pPlayer->AddComponent<PlayerComponent>();
	pPlayer->AddComponent<CirclePhysicsComponent>();
	pPlayer->m_Transform->SetLocalPosition(240, 240,0);

	dae::SceneManager::GetInstance().SetActiveGameScene("BurgerTime");

}

void dae::SceneFactory::InitSteamTestScene()
{
	Scene* pScene = dae::SceneManager::GetInstance().AddGameScene("SteamTest");
	//dae::SceneManager::GetInstance().SetActiveGameScene("SteamTest");

	// Background
	{
		GameObject* go = pScene->MakeGameObject();
		Render2DComponent* textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("demo/background.tga");
		textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::background);

		go->AddComponent<dae::ScoreBoardComponent>();
		go->AddComponent<dae::TestSoundComponent>();
	}

	// text comp 1
	{
		GameObject* go;

		auto font = dae::ResourceManager::GetInstance().LoadFont("fonts/Lingua.otf", 20);
		go = pScene->MakeGameObject();

		auto textComponent = go->AddComponent<TextComponent>();
		textComponent->SetText("[K] start music");
		textComponent->SetFont(font);
		textComponent->SetPosition(100, 20);
	}

	// text comp 2
	{
		GameObject* go;

		auto font = dae::ResourceManager::GetInstance().LoadFont("fonts/Lingua.otf", 20);
		go = pScene->MakeGameObject();

		auto textComponent = go->AddComponent<TextComponent>();
		textComponent->SetText("[L] fade out music");
		textComponent->SetFont(font);
		textComponent->SetPosition(100, 40);
	}

	// text comp 3
	{
		GameObject* go;

		auto font = dae::ResourceManager::GetInstance().LoadFont("fonts/Lingua.otf", 20);
		go = pScene->MakeGameObject();

		auto textComponent = go->AddComponent<TextComponent>();
		textComponent->SetText("[I] play Sound Effect (can be played multiple times at once)");
		textComponent->SetFont(font);
		textComponent->SetPosition(100, 60);
	}

	// text comp 4
	{
		GameObject* go;

		auto font = dae::ResourceManager::GetInstance().LoadFont("fonts/Lingua.otf", 20);
		go = pScene->MakeGameObject();

		auto textComponent = go->AddComponent<TextComponent>();
		textComponent->SetText("[J] toggle pause on all sounds");
		textComponent->SetFont(font);
		textComponent->SetPosition(100, 80);
	}

	{
		GameObject* go = pScene->MakeGameObject();

		SpawnerComponent* spawnerComp = go->AddComponent<SpawnerComponent>();

		std::string player1PrefabName{ "Player1" };

		spawnerComp->AddSpawnableObject(Prefab{ player1PrefabName });
		GameObject* Player1Object = spawnerComp->GetPrefabObject(player1PrefabName);

		Player1Object->AddComponent<PlayerComponent>();

		spawnerComp->SpawnGameObject(player1PrefabName, pScene);
	}
}

void dae::SceneFactory::InitDefaultScene()
{
	Scene* pScene = dae::SceneManager::GetInstance().AddGameScene("Demo");

	GameObject* go = pScene->MakeGameObject();
	//go->AddComponent<TrashTheCacheComponent>();

	// Background
	{
		Render2DComponent* textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("demo/background.tga");
		textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::background);
	}


	// texure 1 LOGO
	{
		go = pScene->MakeGameObject();
		Render2DComponent* textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("demo/logo.tga");
		textureComponent->SetPosition(216, 180);

	}


	// text comp 1
	{
		auto font = dae::ResourceManager::GetInstance().LoadFont("fonts/Lingua.otf", 36);
		go = pScene->MakeGameObject();

		auto textComponent = go->AddComponent<TextComponent>();
		textComponent->SetText("Programming 4 Assignment");
		textComponent->SetFont(font);
		textComponent->SetPosition(80, 20);
	}


	// text comp 2
	{
		go = pScene->MakeGameObject();

		auto font = dae::ResourceManager::GetInstance().LoadFont("fonts/lowres.ttf", 36);
		auto textComponent = go->AddComponent<TextComponent>();
		textComponent->SetText("Press [PAGE UP|DOWN] to change scene");
		textComponent->SetFont(font);
		textComponent->SetPosition(60, 420);
		textComponent->SetColor(255, 160, 100, 255);

	}

	// fps comp 1
	{
		go = pScene->MakeGameObject();
		auto font = dae::ResourceManager::GetInstance().LoadFont("fonts/raju-bold.otf", 42);
		//auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
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
	//	textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::positionScale);

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
	//	textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::positionScale);

	//	go->SetParent(parentGo.get(), true);
	//	auto orbitComp = go->AddComponent<OrbitComponent>();
	//	orbitComp->SetSpeed(6.4f);
	//	orbitComp->SetRadius(75.f);
	//}

	{
		go = pScene->MakeGameObject("Player1");
		auto textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("cheff.png");
		textureComponent->SetPosition(300, 300);
		textureComponent->SetResolution(50, 50);
		textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::positionScale);


		// ASSIGN INPUT
		dae::Input::InputSystem& inputSystem = dae::Input::InputSystem::GetInstance();

		using namespace dae::Input;

		auto* pInputCommandComponent = go->AddComponent<BindInputCommandComponent>();

		// GAMEPAD
		auto gamepadDevice = inputSystem.GetGamepadDevice(0);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ -1,0 }), gamepadDevice, ControllerButton::DPAD_LEFT, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 1,0 }), gamepadDevice, ControllerButton::DPAD_RIGHT, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0,1 }), gamepadDevice, ControllerButton::DPAD_UP, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0,-1 }), gamepadDevice, ControllerButton::DPAD_DOWN, ButtonPressType::Hold);

		// KEYBOARD
		auto keyboardDevice = inputSystem.GetKeyboardDevice();
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ -1,0 }), keyboardDevice, KeyboardButton::KEY_A, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 1,0 }), keyboardDevice, KeyboardButton::KEY_D, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0,1 }), keyboardDevice, KeyboardButton::KEY_W, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0,-1 }), keyboardDevice, KeyboardButton::KEY_S, ButtonPressType::Hold);

		// GAMEPAD ANALOG STICK
		auto stick = gamepadDevice->GetGamepadState().GetAnalogStickFilteredPtr(false);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, stick), gamepadDevice, ControllerButton::THUMBSTICK_LEFT_MOVE, ButtonPressType::Hold);
	}

	{
		go = pScene->MakeGameObject("Player2");

		auto textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("bean.png");
		textureComponent->SetPosition(350, 350);
		textureComponent->SetResolution(50, 50);
		textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::positionScale);

		auto moveComponent = go->AddComponent<MoveComponent>();
		moveComponent->SetMoveSpeed(300.f);

		// ASSIGN INPUT

		dae::Input::InputSystem& inputSystem = dae::Input::InputSystem::GetInstance();

		auto* pInputCommandComponent = go->AddComponent<BindInputCommandComponent>();

		using namespace dae::Input;

		// GAMEPAD
		auto gamepadDevice = inputSystem.GetGamepadDevice(1);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ -1,0 }), gamepadDevice, ControllerButton::DPAD_LEFT, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 1,0 }), gamepadDevice, ControllerButton::DPAD_RIGHT, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0,1 }), gamepadDevice, ControllerButton::DPAD_UP, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0,-1 }), gamepadDevice, ControllerButton::DPAD_DOWN, ButtonPressType::Hold);

		// KEYBOARD
		auto keyboardDevice = inputSystem.GetKeyboardDevice();
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ -1,0 }), keyboardDevice, KeyboardButton::KEY_LEFT, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 1,0 }), keyboardDevice, KeyboardButton::KEY_RIGHT, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0,1 }), keyboardDevice, KeyboardButton::KEY_UP, ButtonPressType::Hold);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, glm::vec2{ 0,-1 }), keyboardDevice, KeyboardButton::KEY_DOWN, ButtonPressType::Hold);



		// GAMEPAD ANALOG STICK
		auto stick = gamepadDevice->GetGamepadState().GetAnalogStickFilteredPtr(false);
		pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, stick), gamepadDevice, ControllerButton::THUMBSTICK_LEFT_MOVE, ButtonPressType::Hold);

		// KEYBOARD DAMAGE PLAYER
		pInputCommandComponent->BindCommand(std::make_unique<DamageCommand>(go, 33), keyboardDevice, KeyboardButton::KEY_F, ButtonPressType::Press);
	}

	{
		go = pScene->MakeGameObject();

		go->AddComponent<dae::Input::InputDebugImguiComponent>();
		go->AddComponent<dae::ScoreBoardComponent>();
	}
}

void dae::SceneFactory::InitFpsDemoScene()
{

	Scene* pScene = dae::SceneManager::GetInstance().AddGameScene("Demo2");

	auto font = dae::ResourceManager::GetInstance().LoadFont("fonts/raju-bold.otf", 72);

	Prefab fpsPrefab{ "FPS display prefab" };
	GameObject* fpsPrefabObject = fpsPrefab.GetPrefabObject();
	FpsComponent* fpsPrefabComponent = fpsPrefabObject->AddComponent<FpsComponent>();
	fpsPrefabComponent->SetFont(font);
	fpsPrefabComponent->SetPosition(200, 180);
	fpsPrefabComponent->SetColor(200, 255, 255, 170);

	fpsPrefab.Instantiate(pScene);

	//GameObject* fpsGO = pScene->MakeGameObject();
	//FpsComponent* fpsComponent = fpsGO->AddComponent<FpsComponent>();
	//fpsComponent->SetFont(font);
	//fpsComponent->SetPosition(200, 180);
	//fpsComponent->SetColor(200, 255, 255, 170);

	GameObject* go = pScene->MakeGameObject();
	Render2DComponent* textureComponent = go->AddComponent<Render2DComponent>();
	textureComponent->SetTexture("demo/colorbars.png");
	textureComponent->SetDrawStyle(dae::Render2DComponent::DrawStyle::background);
}

void dae::SceneFactory::InitBonusScene()
{
	Scene* pScene = dae::SceneManager::GetInstance().AddGameScene("Bonus");

	GameObject* go;
	{
		go = pScene->MakeGameObject();
		Render2DComponent* textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("demo/backdrop_trees.png");
		textureComponent->SetPosition(-80, 0);
		textureComponent->SetResolution(800, 600);
		textureComponent->SetDrawStyle(dae::Render2DComponent::DrawStyle::positionScale);
		Renderer::GetInstance().SetBackgroundColor(SDL_Color{ 100,140,230,255 });
	}
	// orbiting texture 1
	{
		go = pScene->MakeGameObject();
		auto textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("demo/fatalerror.png");
		textureComponent->SetPosition(300, 200);
		textureComponent->SetResolution(100, 100);
		textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::positionScale);

		auto orbitComp = go->AddComponent<OrbitComponent>();
		orbitComp->SetSpeed(2.6f);
		orbitComp->SetRadius(100.f);
	}

	// orbiting texture 2
	{
		auto* parentGo = go; // previous go from RAII block above
		go = pScene->MakeGameObject();

		auto textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("demo/colorbars.png");
		textureComponent->SetPosition(0, 0);
		textureComponent->SetResolution(100, 100);
		textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::positionScale);

		go->SetParent(parentGo, true);
		auto orbitComp = go->AddComponent<OrbitComponent>();
		orbitComp->SetSpeed(8.4f);
		orbitComp->SetRadius(150.f);
	}
}
