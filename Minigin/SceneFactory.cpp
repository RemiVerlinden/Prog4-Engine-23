#include "SceneFactory.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Scene.h"
#include "TextComponent.h"
#include "FpsComponent.h"
#include "Render2DComponent.h"
#include "GameObject.h"
#include "OrbitComponent.h"
#include "InputSystem.h"
#include "InputDebugImguiComponent.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Xinput.h"
#include <iostream>

using namespace dae;

void dae::SceneFactory::CreateScenes()
{
	std::cout << std::endl << std::endl;
	std::cout << "INPUT: [WASD] - [DPAD] - [LEFT STICK GAMEPAD]\n";
	std::cout << "SUPPORT FOR 4 GAMEPADS TOTAL\n";
	InitDefaultScene();
	InitFpsDemoScene();
	InitBonusScene();
}

void dae::SceneFactory::InitDefaultScene()
{
	Scene* pScene = dae::SceneManager::GetInstance().AddGameScene("Demo").get();
	dae::SceneManager::GetInstance().SetActiveGameScene("Demo");

	std::shared_ptr<GameObject> go = pScene->MakeGameObject();

	// Background
	{
		Render2DComponent* textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("demo/background.tga");
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

		auto textComponent = go->AddComponent<TextComponent>("Programming 4 Assignment", font);
		textComponent->SetPosition(80, 20);
	}


	// text comp 2
	{
		go = pScene->MakeGameObject();

		auto font = dae::ResourceManager::GetInstance().LoadFont("fonts/lowres.ttf", 36);
		auto textComponent = go->AddComponent<TextComponent>("Press [PAGE UP|DOWN] to change scene", font);
		textComponent->SetPosition(60, 420);
		textComponent->SetColor(255, 160, 100, 255);

	}

	// fps comp 1
	{
		go = pScene->MakeGameObject();
		auto font = dae::ResourceManager::GetInstance().LoadFont("fonts/raju-bold.otf", 42);
		//auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
		FpsComponent* fpsComponent = go->AddComponent<FpsComponent>(font);
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
		go = pScene->MakeGameObject();
		auto textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("cheff.png");
		textureComponent->SetPosition(300, 300);
		textureComponent->SetResolution(50, 50);
		textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::positionScale);

		dae::Input::InputSystem& inputSystem = dae::Input::InputSystem::GetInstance();
		dae::Input::CommandHandler* commandHandler = inputSystem.GetCommandHandler();

		using namespace dae::Input;

		auto bindMove = [&go, &commandHandler](InputDevice* pDevice, deviceButton button, ButtonPressType pressType, glm::vec2 dir)
		{

			dae::Input::InputAction inputAction;
			inputAction.command = std::make_unique<MoveCommand>(go.get(), dir);
			inputAction.device = pDevice;
			inputAction.pressType = pressType;

			commandHandler->BindNewAction(button, inputAction);
		};

		auto gamepadDevice = inputSystem.GetGamepadDevice(0);
		bindMove(gamepadDevice, XINPUT_GAMEPAD_DPAD_RIGHT, ButtonPressType::Hold, glm::vec2{ 1,0 });
		bindMove(gamepadDevice, XINPUT_GAMEPAD_DPAD_LEFT, ButtonPressType::Hold, glm::vec2{ -1,0 });
		bindMove(gamepadDevice, XINPUT_GAMEPAD_DPAD_UP, ButtonPressType::Hold, glm::vec2{ 0,1 });
		bindMove(gamepadDevice, XINPUT_GAMEPAD_DPAD_DOWN, ButtonPressType::Hold, glm::vec2{ 0,-1 });


		bindMove(inputSystem.GetKeyboardMouseDevice(), SDL_SCANCODE_A, ButtonPressType::Hold, glm::vec2{ -1,0 });
		bindMove(inputSystem.GetKeyboardMouseDevice(), SDL_SCANCODE_D, ButtonPressType::Hold, glm::vec2{ 1,0 });
		bindMove(inputSystem.GetKeyboardMouseDevice(), SDL_SCANCODE_W, ButtonPressType::Hold, glm::vec2{ 0,1 });
		bindMove(inputSystem.GetKeyboardMouseDevice(), SDL_SCANCODE_S, ButtonPressType::Hold, glm::vec2{ 0,-1 });

		auto bindAnalogStickMove = [&go, &commandHandler](InputDevice* pDevice, deviceButton button, ButtonPressType pressType, const glm::vec2* dir)
		{

			dae::Input::InputAction inputAction;
			inputAction.command = std::make_unique<MoveCommand>(go.get(), dir);
			inputAction.device = pDevice;
			inputAction.pressType = pressType;

			commandHandler->BindNewAction(button, inputAction);
		};

		bindAnalogStickMove(gamepadDevice, XINPUT_GAMEPAD_LEFT_STICK, ButtonPressType::Hold, gamepadDevice->GetGamepadState().GetAnalogStickFilteredPtr(false));
	}

	{
		auto parentGo = go;
		go = pScene->MakeGameObject();

		auto textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("bean.png");
		textureComponent->SetPosition(350, 350);
		textureComponent->SetResolution(50, 50);
		textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::positionScale);


		dae::Input::InputSystem& inputSystem = dae::Input::InputSystem::GetInstance();
		dae::Input::CommandHandler* commandHandler = inputSystem.GetCommandHandler();

		using namespace dae::Input;

		auto bindMove = [&go, &commandHandler](InputDevice* pDevice, deviceButton button, ButtonPressType pressType, glm::vec2 dir)
		{

			dae::Input::InputAction inputAction;
			inputAction.command = std::make_unique<MoveCommand>(go.get(), dir);
			inputAction.device = pDevice;
			inputAction.pressType = pressType;

			commandHandler->BindNewAction(button, inputAction);
		};

		auto gamepadDevice = inputSystem.GetGamepadDevice(1);
		bindMove(gamepadDevice, XINPUT_GAMEPAD_DPAD_RIGHT, ButtonPressType::Hold, glm::vec2{ 1,0 });
		bindMove(gamepadDevice, XINPUT_GAMEPAD_DPAD_LEFT, ButtonPressType::Hold, glm::vec2{ -1,0 });
		bindMove(gamepadDevice, XINPUT_GAMEPAD_DPAD_UP, ButtonPressType::Hold, glm::vec2{ 0,1 });
		bindMove(gamepadDevice, XINPUT_GAMEPAD_DPAD_DOWN, ButtonPressType::Hold, glm::vec2{ 0,-1 });


		bindMove(inputSystem.GetKeyboardMouseDevice(), SDL_Scancode(SDL_SCANCODE_LEFT), ButtonPressType::Hold, glm::vec2{ -1,0 });
		bindMove(inputSystem.GetKeyboardMouseDevice(), SDL_Scancode(SDL_SCANCODE_RIGHT), ButtonPressType::Hold, glm::vec2{ 1,0 });
		bindMove(inputSystem.GetKeyboardMouseDevice(), SDL_Scancode(SDL_SCANCODE_UP), ButtonPressType::Hold, glm::vec2{ 0,1 });
		bindMove(inputSystem.GetKeyboardMouseDevice(), SDL_Scancode(SDL_SCANCODE_DOWN), ButtonPressType::Hold, glm::vec2{ 0,-1 });

		auto bindAnalogStickMove = [&go, &commandHandler](InputDevice* pDevice, deviceButton button, ButtonPressType pressType, const glm::vec2* dir)
		{

			dae::Input::InputAction inputAction;
			inputAction.command = std::make_unique<MoveCommand>(go.get(), dir);
			inputAction.device = pDevice;
			inputAction.pressType = pressType;

			commandHandler->BindNewAction(button, inputAction);
		};

		bindAnalogStickMove(gamepadDevice, XINPUT_GAMEPAD_LEFT_STICK, ButtonPressType::Hold, gamepadDevice->GetGamepadState().GetAnalogStickFilteredPtr(false));
	}

	{
		go = pScene->MakeGameObject();

		go->AddComponent<dae::Input::InputDebugImguiComponent>();
	}
}

void dae::SceneFactory::InitFpsDemoScene()
{
	Scene* pScene = dae::SceneManager::GetInstance().AddGameScene("Demo2").get();

	auto font = dae::ResourceManager::GetInstance().LoadFont("fonts/raju-bold.otf", 72);
	std::shared_ptr<dae::GameObject> fpsGO = pScene->MakeGameObject();


	FpsComponent* fpsComponent = fpsGO->AddComponent<FpsComponent>(font);
	fpsComponent->SetPosition(200, 180);
	fpsComponent->SetColor(200, 255, 255, 170);

	std::shared_ptr<GameObject> go = pScene->MakeGameObject();
	Render2DComponent* textureComponent = go->AddComponent<Render2DComponent>();
	textureComponent->SetTexture("demo/colorbars.png");
	textureComponent->SetDrawStyle(dae::Render2DComponent::DrawStyle::background);
}

void dae::SceneFactory::InitBonusScene()
{
	Scene* pScene = dae::SceneManager::GetInstance().AddGameScene("Bonus").get();

	std::shared_ptr<GameObject> go;
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
		auto parentGo = go; // previous go from RAII block above
		go = pScene->MakeGameObject();

		auto textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("demo/colorbars.png");
		textureComponent->SetPosition(0, 0);
		textureComponent->SetResolution(100, 100);
		textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::positionScale);

		go->SetParent(parentGo.get(), true);
		auto orbitComp = go->AddComponent<OrbitComponent>();
		orbitComp->SetSpeed(8.4f);
		orbitComp->SetRadius(150.f);
	}
}
