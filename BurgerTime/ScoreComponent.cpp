#include "ScoreComponent.h"
#include "GameObject.h"
#include "AppComponents.h"
#include "Structs.h"
#include "Gamemode.h"
#include "ResourceManager.h"
#include "Common.h"
#include <fstream>

using namespace engine;

void ScoreComponent::Initialize()
{
    EventSubscribe(OnPickup);
    auto gamemode = Gamemode::GetInstance().GetGamemode();

    if (gamemode == Gamemode::GameType::SinglePlayer)
    {
        InitSinglePlayer();
    }
    else if (gamemode == Gamemode::GameType::Coop)
    {
        InitCoop();
    }
    else if (gamemode == Gamemode::GameType::Versus)
    {
        InitVersus();
    }
    m_Timer.SetCycleTime(2.f);
}

void ScoreComponent::Update(const engine::UpdateContext& context) 
{
    if (m_Timer.Update(context.GetDeltaTime()))
    {
        m_ScorePlayer1 += 500;
        m_pPlayer1ScoreText->SetText(std::format("{}", m_ScorePlayer1));
    }
}

void ScoreComponent::OnEvent(const Event& sent)
{
    if (sent.GetEventType() != EventType::OnPickup)
        return;

    const OnPickupEvent& pickupEvent = static_cast<const OnPickupEvent&>(sent);
    std::string pickupName = pickupEvent.GetPickupName();
    std::string player = pickupEvent.GetObjectName();

    std::vector<std::string> pickupTypes = { "icecream", "fries", "coffee" };
    if (std::find(pickupTypes.begin(), pickupTypes.end(), pickupName) == pickupTypes.end())
        return;

    if (player == "player1")
    {
        m_ScorePlayer1 += 100;
        m_pPlayer1ScoreText->SetText(std::format("{}", m_ScorePlayer1));
    }
    else if (player == "player2")
    {
        m_ScorePlayer2 += 100;
        m_pPlayer2ScoreText->SetText(std::format("{}", m_ScorePlayer2));
    }
    Locator::GetSoundSystem().Play("pickup.wav", 1.f, true);
}

TextComponent* ScoreComponent::InitTextComponent(glm::ivec4 color)
{
    auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/burger-time.otf", 8);
    auto pText = m_GameObject->AddComponent<TextComponent>(UniqueName());
    pText->SetFont(font);
    pText->SetColor(color.r, color.g, color.b, color.a);
    pText->SetText("0");

    return pText;
}

void ScoreComponent::InitSinglePlayer()
{
    glm::vec2 position = { WindowSettings::defaultwidth / 4, 1 };
    auto permanentText = InitTextComponent({ 255,0,0,255 });
    permanentText->SetPosition(position);
    permanentText->SetText("Score");

    position.y += 10;

    m_pPlayer1ScoreText = InitTextComponent();
    m_pPlayer1ScoreText->SetPosition(position);
}

void ScoreComponent::InitCoop()
{
    glm::vec2 position = { WindowSettings::defaultwidth / 4, 1 };
    auto permanentText = InitTextComponent({ 255,0,0,255 });
    permanentText->SetPosition(position);
    permanentText->SetText("Score");

    position.y += 10;

    m_pPlayer1ScoreText = InitTextComponent();
    m_pPlayer1ScoreText->SetPosition(position);
}

void ScoreComponent::InitVersus()
{
    glm::vec2 position = { WindowSettings::defaultwidth / 4, 1 };
    auto permanentText = InitTextComponent({ 255,0,0,255 });
    permanentText->SetPosition(position);
    permanentText->SetText("Score");

    position.y += 10;

    m_pPlayer1ScoreText = InitTextComponent();
    m_pPlayer1ScoreText->SetPosition(position);
}

void ScoreComponent::OnSceneDeactivate()
{
    std::ifstream inFile;
    const std::string& filePath = "../Data/highscore.txt";
    inFile.open(filePath);

    if (!inFile)
    {
        APP_ERROR("cannot open file {}", filePath);
    }

    int highScoreFromFile;
    inFile >> highScoreFromFile;

    inFile.close();

    int maxScore = std::max(highScoreFromFile, m_ScorePlayer1);

    std::ofstream outFile;
    outFile.open("../Data/highscore.txt");

    if (!outFile)
    {
        APP_ERROR("cannot open file {}", filePath);
    }

    outFile << maxScore;
    outFile.close();

    APP_INFO("Stored high score: {}", maxScore);
}

