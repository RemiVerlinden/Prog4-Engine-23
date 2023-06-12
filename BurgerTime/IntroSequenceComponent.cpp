#include "IntroSequenceComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Common.h"
#include <fstream>
#include <string>
#include "Components.h"

using namespace	engine;

IntroSequenceComponent::~IntroSequenceComponent()
{
	m_IsComponentAlive = false;
}

void IntroSequenceComponent::Initialize()
{
	if (m_IsComponentAlive) return; // dont want to have this component twice
	m_IsComponentAlive = true;

	InitSequence1();
	InitSequence2();
	InitSequence3();
	// disable all sequences
	for (GameObject* go : m_IntroObjects)
	{
		go->SetActive(false);
	}
	m_IntroObjects[0]->SetActive(true);

	InitMainMenuFinal();

	m_Timer.Start(m_SequenceDuration);
}

void IntroSequenceComponent::Update(const engine::UpdateContext& context)
{
	
	if (m_Timer.Update(context.GetDeltaTime())) // every 3 seconds, this if loop will execute
	{
		++m_ActiveSequence;
		SetIntroSequenceActive(m_ActiveSequence);

		m_Timer.Start(m_SequenceDuration);
	}
}

void IntroSequenceComponent::OnSceneActivate()
{
	m_ActiveSequence = 0;
	SetIntroSequenceActive(m_ActiveSequence);
	m_Timer.Start(3);
	Locator::GetSoundSystem().StopMusic();
}

void IntroSequenceComponent::SetIntroSequenceActive(int sequenceID)
{
	if (sequenceID > m_TotalSequences) 
	{
		SceneManager::GetInstance().NextScene();
		sequenceID = m_TotalSequences;
	}

	for (GameObject* go : m_IntroObjects)
	{
		go->SetActive(false);
	}
	m_IntroObjects[sequenceID]->SetActive(true);
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

void IntroSequenceComponent::InitSequence1()
{
	Scene* pScene = m_GameObject->GetScene();
	IntroRedTextPrefab(pScene);

	GameObject* go = m_IntroObjects[0] = pScene->MakeGameObject();

	{

		auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/burger-time.otf", 8);

		auto textComponent = go->AddComponent<TextComponent>(UniqueName());
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


			auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/burger-time.otf", 8);

			auto textComponent = go->AddComponent<TextComponent>(UniqueName());
			textComponent->SetText(displayText);
			textComponent->SetFont(font);
			textComponent->SetPosition(41, drawY);

			drawY += 15;
			++rank;
		}
	}
}

void IntroSequenceComponent::InitSequence2()
{
	Scene* pScene = m_GameObject->GetScene();

	GameObject* go = m_IntroObjects[1] = pScene->MakeGameObject();

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



			auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/burger-time.otf", 8);

			//first spawn character texture

			auto textureComponent = go->AddComponent<RenderComponent>(UniqueName());
			textureComponent->SetTexture("../data/sprites/burgertime-sprites.png");
			textureComponent->SetDrawStyle(engine::render::spritesheet);
			textureComponent->SetSourceRect(0.f, sourceRectPos * 16.f, 16, 16);
			textureComponent->SetPosition(47, drawY - 5);
			textureComponent->SetResolution(16, 16);

			auto textComponent = go->AddComponent<TextComponent>(UniqueName());
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
}


void IntroSequenceComponent::InitSequence3()
{
	Scene* pScene = m_GameObject->GetScene();

	GameObject* go = m_IntroObjects[2] = pScene->MakeGameObject();

	IntroRedTextPrefab(pScene);

	{

		auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/burger-time.otf", 8);

		auto textComponent = go->AddComponent<TextComponent>(UniqueName());
		textComponent->SetText("-SCORE-");
		textComponent->SetFont(font);
		textComponent->SetPosition(80, 85);
	}

	{
		float drawY = 175;

		auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/burger-time.otf", 8);

		auto textComponent = go->AddComponent<TextComponent>(UniqueName());
		textComponent->SetText("BONUS  FOR EVERY 10000 PTS");
		textComponent->SetFont(font);
		textComponent->SetPosition(8, drawY);


		auto textureComponent = go->AddComponent<RenderComponent>();
		textureComponent->SetTexture("../data/sprites/burgertime-sprites.png");
		textureComponent->SetDrawStyle(engine::render::spritesheet);
		textureComponent->SetSourceRect(16.f, 0.f, 16, 16);
		textureComponent->SetPosition(50, drawY - 2);
		textureComponent->SetResolution(11, 11);
	}


}

void IntroSequenceComponent::InitMainMenuFinal()
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

		auto bindSelectGamemode = [&commandHandler, &pScene](InputDevice* pDevice, deviceButton button, ButtonPressType pressType, bool menuSelectDown)
		{

			engine::Input::InputAction inputAction;
			//inputAction.command = menuSelectDown ? std::make_unique<MainMenuSelectDown>() : std::make_unique<MainMenuSelectUp>(); this is not working for some reason
			if (menuSelectDown)
				inputAction.command = std::make_unique<MainMenuSelectDown>(pScene);
			else
				inputAction.command = std::make_unique<MainMenuSelectUp>(pScene);

			inputAction.device = pDevice;
			inputAction.pressType = pressType;

			commandHandler.BindNewAction(button, inputAction);
		};

		// KEYBOARD
		auto inputDeviceKeyboard = inputSystem.GetKeyboardDevice();
		bindSelectGamemode(inputDeviceKeyboard, KeyboardButton::KEY_W, ButtonPressType::Release, false);
		bindSelectGamemode(inputDeviceKeyboard, KeyboardButton::KEY_S, ButtonPressType::Release, true);
		bindSelectGamemode(inputDeviceKeyboard, KeyboardButton::KEY_UP, ButtonPressType::Release, false);
		bindSelectGamemode(inputDeviceKeyboard, KeyboardButton::KEY_DOWN, ButtonPressType::Release, true);

		auto inputDeviceGamepad1 = inputSystem.GetGamepadDevice(0);
		bindSelectGamemode(inputDeviceGamepad1, ControllerButton::DPAD_UP, ButtonPressType::Release, false);
		bindSelectGamemode(inputDeviceGamepad1, ControllerButton::DPAD_DOWN, ButtonPressType::Release, true);

		auto inputDeviceGamepad2 = inputSystem.GetGamepadDevice(1);
		bindSelectGamemode(inputDeviceGamepad2, ControllerButton::DPAD_UP, ButtonPressType::Release, false);
		bindSelectGamemode(inputDeviceGamepad2, ControllerButton::DPAD_DOWN, ButtonPressType::Release, true);


		auto bindStartGame = [&commandHandler, &pScene](InputDevice* pDevice, deviceButton button, ButtonPressType pressType)
		{

			engine::Input::InputAction inputAction;
			inputAction.command = std::make_unique<MainMenuStartGame>(pScene);
			inputAction.device = pDevice;
			inputAction.pressType = pressType;

			commandHandler.BindNewAction(button, inputAction);
		};


		bindStartGame(inputDeviceGamepad1, ControllerButton::BUTTON_A, ButtonPressType::Release);
		bindStartGame(inputDeviceGamepad2, ControllerButton::BUTTON_A, ButtonPressType::Release);
	}
}


