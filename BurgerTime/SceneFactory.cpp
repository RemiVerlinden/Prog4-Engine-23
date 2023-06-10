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

using namespace engine;

void engine::SceneFactory::CreateScenes()
{
	CONSOLE_NEWLINE();
	CONSOLE_NEWLINE();
	APP_TRACE("INPUT PLAYER 1: [WASD]       -	[GAMEPAD 1: DPAD] - [GAMEPAD 1: LEFT STICK]");
	APP_TRACE("INPUT PLAYER 2: [ARROW KEYS] -	[GAMEPAD 2: DPAD] - [GAMEPAD 2: LEFT STICK]");

	Scene* activeScene;

	InitSceneKeybinds();
	activeScene = InitMainMenu1();
	InitMainMenu2();
	InitMainMenu3();
	InitMainMenu();
	activeScene = InitBurgerTimeScene();

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

	GameObject* pWorldObject = pScene->MakeGameObject();
	pWorldObject->AddComponent<WorldDataComponent>();
	//Render2DComponent* textureComponent = pWorldObject->AddComponent<Render2DComponent>();
	//textureComponent->SetTexture("stage1.tga");
	//textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::customResolution);
	//textureComponent->SetResolution(WindowSettings::width,WindowSettings::height);

	GameObject* pPlayer = pScene->MakeGameObject();
	pPlayer->AddComponent<PlayerComponent>();
	CirclePhysicsComponent* physicsComp = pPlayer->AddComponent<CirclePhysicsComponent>();
	physicsComp->GetCircleCollider().radius = WorldData::defaultTileSize;
	pPlayer->m_Transform->SetLocalPosition(110, 168, 0);


	return pScene;
}

static void IntroRedTextPrefab(Scene* pScene)
{

	// text comp 1
	{
		GameObject* go;

		auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/burger-time.otf", 9);
		go = pScene->MakeGameObject();

		auto textComponent = go->AddComponent<TextComponent>();
		textComponent->SetText("BURGERTIME");
		textComponent->SetFont(font);
		textComponent->SetColor(255, 0, 0, 255);
		textComponent->SetPosition(66, 36);
	}

	{
		GameObject* go;

		auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/burger-time.otf", 5);
		go = pScene->MakeGameObject();

		auto textComponent = go->AddComponent<TextComponent>();
		textComponent->SetText("TM");
		textComponent->SetFont(font);
		textComponent->SetColor(255, 0, 0, 255);
		textComponent->SetPosition(156, 40);
	}

	{
		GameObject* go;

		auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/burger-time.otf", 7);
		go = pScene->MakeGameObject();

		auto textComponent = go->AddComponent<TextComponent>();
		textComponent->SetText("© COPR. 1982 DATA EAST INC");
		textComponent->SetFont(font);
		textComponent->SetColor(255, 0, 0, 255);
		textComponent->SetPosition(8, 50);
	}

	{
		GameObject* go;

		auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/burger-time.otf", 7);
		go = pScene->MakeGameObject();

		auto textComponent = go->AddComponent<TextComponent>();
		textComponent->SetText("MFGD BY REMI VERLINDEN MFG. CO.");
		textComponent->SetFont(font);
		textComponent->SetColor(255, 0, 0, 255);
		textComponent->SetPosition(5, 58);
	}
}

static std::pair<std::string, int> GetHighscoreFromFile()
{
	std::pair<std::string, int> score;
	std::ifstream inputFile("../Data/highscore.txt");
	if (inputFile)
	{ // Make sure the file was opened successfully 
		int highscore = 0;
		while (inputFile >> highscore)
		{ // Read in the scores from the file
			score = std::make_pair(std::string("You"), highscore);
		}
	}
	else
	{
		std::cout << "could not find highscore.txt" << std::endl;
	}
	return score;
}

Scene* engine::SceneFactory::InitMainMenu1()
{
	Scene* pScene = engine::SceneManager::GetInstance().AddGameScene("MainMenu1");

	IntroRedTextPrefab(pScene);

	{
		GameObject* go;

		auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/burger-time.otf", 8);
		go = pScene->MakeGameObject();

		auto textComponent = go->AddComponent<TextComponent>();
		textComponent->SetText("BEST FIVE PLAYERS");
		textComponent->SetFont(font);
		textComponent->SetPosition(42, 85);
	}

	{
		std::vector<std::pair<std::string, int>> scores;
		scores.emplace_back(std::make_pair("KEN", 28000));
		scores.emplace_back(std::make_pair("H,I", 10100));
		scores.emplace_back(std::make_pair("GON", 9400));
		scores.emplace_back(std::make_pair("H,K", 6550));
		scores.emplace_back(std::make_pair("K.H", 4850));

		scores.emplace_back(GetHighscoreFromFile());

		std::sort(scores.begin(), scores.end(), [](auto const& a, auto const& b)
			{
				return a.second > b.second; // Sort by score in descending order
			});

		int rank = 1;
		float drawY = 100;
		for (auto& [name, score] : scores)
		{
			std::string displayText = std::format("{} {}{:>7} PTS", rank, name, score);

			GameObject* go;

			auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/burger-time.otf", 8);
			go = pScene->MakeGameObject();

			auto textComponent = go->AddComponent<TextComponent>();
			textComponent->SetText(displayText);
			textComponent->SetFont(font);
			textComponent->SetPosition(41, drawY);

			drawY += 15;
			++rank;
		}
	}

	return pScene;
}




Scene* engine::SceneFactory::InitMainMenu3()
{
	Scene* pScene = engine::SceneManager::GetInstance().AddGameScene("MainMenu3");


	IntroRedTextPrefab(pScene);

	{
		GameObject* go;

		auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/burger-time.otf", 8);
		go = pScene->MakeGameObject();

		auto textComponent = go->AddComponent<TextComponent>();
		textComponent->SetText("-SCORE-");
		textComponent->SetFont(font);
		textComponent->SetPosition(80, 85);
	}

	{
		GameObject* go;
		float drawY = 175;

		auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/burger-time.otf", 8);
		go = pScene->MakeGameObject();

		auto textComponent = go->AddComponent<TextComponent>();
		textComponent->SetText("BONUS  FOR EVERY 10000 PTS");
		textComponent->SetFont(font);
		textComponent->SetPosition(8, drawY);


		auto textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("../data/sprites/burgertime-sprites.png");
		textureComponent->SetDrawStyle(Render2DComponent::spritesheet);
		textureComponent->SetSourceRect(16.f, 0.f, 16, 16);
		textureComponent->SetPosition(50, drawY - 2);
		textureComponent->SetResolution(11, 11);
	}



	return pScene;
}

Scene* engine::SceneFactory::InitMainMenu()
{
	Scene* pScene = engine::SceneManager::GetInstance().AddGameScene("MainMenu");

	IntroRedTextPrefab(pScene);

	{
		GameObject* go;

		go = pScene->MakeGameObject();

		auto mainMenu = go->AddComponent<MainMenuComponent>();
		mainMenu;
	}

	{
		engine::Input::InputSystem& inputSystem = engine::Input::InputSystem::GetInstance();
		engine::Input::CommandHandler& commandHandler = inputSystem.GetCommandHandler();

		using namespace engine::Input;

		auto bindSelectGamemode = [&commandHandler](InputDevice* pDevice, deviceButton button, ButtonPressType pressType, bool menuSelectDown)
		{

			engine::Input::InputAction inputAction;
			//inputAction.command = menuSelectDown ? std::make_unique<MainMenuSelectDown>() : std::make_unique<MainMenuSelectUp>(); this is not working for some reason
			if (menuSelectDown)
				inputAction.command = std::make_unique<MainMenuSelectDown>();
			else
				inputAction.command = std::make_unique<MainMenuSelectUp>();

			inputAction.device = pDevice;
			inputAction.pressType = pressType;

			commandHandler.BindNewAction(button, inputAction);
		};

		// KEYBOARD
		auto inputDeviceKeyboard = inputSystem.GetKeyboardDevice();
		bindSelectGamemode(inputDeviceKeyboard, KeyboardButton::KEY_T, ButtonPressType::Release, false);
		bindSelectGamemode(inputDeviceKeyboard, KeyboardButton::KEY_G, ButtonPressType::Release, true);

		auto inputDeviceGamepad1 = inputSystem.GetGamepadDevice(0);
		bindSelectGamemode(inputDeviceGamepad1, ControllerButton::DPAD_UP, ButtonPressType::Release, false);
		bindSelectGamemode(inputDeviceGamepad1, ControllerButton::DPAD_DOWN, ButtonPressType::Release, true);


		auto bindStartGame = [&commandHandler](InputDevice* pDevice, deviceButton button, ButtonPressType pressType)
		{

			engine::Input::InputAction inputAction;
			inputAction.command = std::make_unique<MainMenuStartGame>();
			inputAction.device = pDevice;
			inputAction.pressType = pressType;

			commandHandler.BindNewAction(button, inputAction);
		};

		bindStartGame(inputDeviceKeyboard, KeyboardButton::KEY_L, ButtonPressType::Release);

		bindStartGame(inputDeviceGamepad1, ControllerButton::BUTTON_A, ButtonPressType::Release);
	}


	return pScene;
}

Scene* engine::SceneFactory::InitMainMenu2()
{


	Scene* pScene = engine::SceneManager::GetInstance().AddGameScene("MainMenu2");

	IntroRedTextPrefab(pScene);
	{
		std::vector<std::string> characters;
		characters.emplace_back("PETER PEPPER");
		characters.emplace_back("MR HOT DOG");
		characters.emplace_back("MR PICKLE");
		characters.emplace_back("MR EGG");

		float drawY = 90;

		for (std::string& name : characters)
		{
			static int sourceRectPos = -2; sourceRectPos += 2; // start at pos 0 and go up to 8 for each character in burgertime-sprites.png

			GameObject* go;

			auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/burger-time.otf", 8);
			go = pScene->MakeGameObject();

			//first spawn character texture

			auto textureComponent = go->AddComponent<Render2DComponent>();
			textureComponent->SetTexture("../data/sprites/burgertime-sprites.png");
			textureComponent->SetDrawStyle(Render2DComponent::spritesheet);
			textureComponent->SetSourceRect(0.f, sourceRectPos * 16.f, 16, 16);
			textureComponent->SetPosition(47, drawY - 5);
			textureComponent->SetResolution(16, 16);

			auto textComponent = go->AddComponent<TextComponent>();
			textComponent->SetText(name);
			textComponent->SetFont(font);
			textComponent->SetPosition(75, drawY);

			auto TM_font = engine::ResourceManager::GetInstance().LoadFont("fonts/burger-time.otf", 5);

			// try to offset TM text to the bottom right of each name
			static int currentCharacter = 0; ++currentCharacter; // I dont care what the number is, just that it's unique
			auto TM_textComponent = go->AddComponent<TextComponent>(std::format("TM-Text {}", currentCharacter));
			TM_textComponent->SetText("TM");
			TM_textComponent->SetFont(TM_font);
			TM_textComponent->SetPosition(75 + name.size() * 8.0f, drawY + 10);

			static bool doOnce = true; // first name needs to be up a bit higher
			drawY += (doOnce) ? 30 : 20;
			doOnce = false;
		}
	}



	return pScene;
}


Scene* engine::SceneFactory::InitSteamTestScene()
{
	Scene* pScene = engine::SceneManager::GetInstance().AddGameScene("SteamTest");
	//engine::SceneManager::GetInstance().SetActiveGameScene("SteamTest");

	// Background
	{
		GameObject* go = pScene->MakeGameObject();
		Render2DComponent* textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("demo/background.tga");
		textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::background);

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
		auto textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("cheff.png");
		textureComponent->SetPosition(300, 300);
		textureComponent->SetResolution(50, 50);
		textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::customResolution);


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

		auto textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("bean.png");
		textureComponent->SetPosition(350, 350);
		textureComponent->SetResolution(50, 50);
		textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::customResolution);

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
	Render2DComponent* textureComponent = go->AddComponent<Render2DComponent>();
	textureComponent->SetTexture("demo/colorbars.png");
	textureComponent->SetDrawStyle(engine::Render2DComponent::DrawStyle::background);


	return pScene;

}

Scene* engine::SceneFactory::InitBonusScene()
{
	Scene* pScene = engine::SceneManager::GetInstance().AddGameScene("Bonus");

	GameObject* go;
	{
		go = pScene->MakeGameObject();
		Render2DComponent* textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("demo/backdrop_trees.png");
		textureComponent->SetPosition(-80, 0);
		textureComponent->SetResolution(800, 600);
		textureComponent->SetDrawStyle(engine::Render2DComponent::DrawStyle::customResolution);
		Renderer::GetInstance().SetBackgroundColor(SDL_Color{ 100,140,230,255 });
	}
	// orbiting texture 1
	{
		go = pScene->MakeGameObject();
		auto textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("demo/fatalerror.png");
		textureComponent->SetPosition(300, 200);
		textureComponent->SetResolution(100, 100);
		textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::customResolution);

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
		textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::customResolution);

		go->SetParent(parentGo, true);
		auto orbitComp = go->AddComponent<OrbitComponent>();
		orbitComp->SetSpeed(8.4f);
		orbitComp->SetRadius(150.f);
	}

	return pScene;

}
