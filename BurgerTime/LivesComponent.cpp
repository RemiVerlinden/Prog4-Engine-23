#include "LivesComponent.h"
#include "GameObject.h"
#include "TagComponent.h"
#include "LivesComponent.h"
#include "EventManager.h"
#include "AppComponents.h"
#include <format>
#include "Common.h"
#include "Gamemode.h"

using namespace engine;

void LivesComponent::Initialize()
{
    EventSubscribe(OnDeath);

    const auto& gameMode = Gamemode::GetInstance().GetGamemode();
    const std::string texturePath = "../Data/Sprites/burgertime-sprites.png";

    for (size_t life = 0; life < Lives::max; ++life)
    {
        auto* component = m_GameObject->AddComponent<RenderComponent>(std::format("RenderComponent{}", life));
        m_pRenderComponents.push_back(component);

        if (fileExists(texturePath))
        {
            component->SetTexture(texturePath);
        }

        switch (gameMode)
        {
            case Gamemode::GameType::SinglePlayer:
                InitPlayerComponent(component, life, false);
                break;
            case Gamemode::GameType::Coop:
            case Gamemode::GameType::Versus:
                bool isPlayer2 = m_GameObject->GetTag() == "player2_lives";
                InitPlayerComponent(component, life, isPlayer2);
                break;
        }
    }
}

void LivesComponent::OnEvent(const Event& event)
{
    if (event.GetEventType() == EventType::OnDeath)
    {
        const auto& deathEvent = static_cast<const OnDeathEvent&>(event);
        const auto livesTag = std::format("{}{}", deathEvent.GetObjectName(), "_lives");

        if (m_GameObject->GetTag() == livesTag)
        {
            RemoveLife();
        }
    }
}

void LivesComponent::Linkplayer(engine::GameObject* player)
{
    m_pPlayer = player;
}

void LivesComponent::RemoveLife()
{
    if (--m_Lives.current < -1)
    {
        APP_ERROR("LivesComponent::RemoveLife() - lives can't be lower than -1");
        return;
    }

    if (m_Lives.current < 0)
    {
        EventManager::GetInstance().SendEvent(engine::OnGameOverEvent{m_pPlayer->GetTag()});
    }
    else
    {
        RemoveLastLifeRenderComponent();
        EventManager::GetInstance().SendEvent(engine::OnRespawnEvent{m_pPlayer});
    }
}

void LivesComponent::RemoveLastLifeRenderComponent()
{
    auto* component = m_pRenderComponents.back();
    m_pRenderComponents.pop_back();
    m_GameObject->RemoveComponentWithTag<RenderComponent>(component->GetComponentTag());
}

void LivesComponent::InitPlayerComponent(RenderComponent* component, size_t currentLife, bool isPlayer2)
{
    const float imageWidth = 7.f;
    const float imageHeight = 8.f;
    const float padding = 1.f + currentLife;
    const float drawY = 8.f * currentLife + padding;

    const float drawX = isPlayer2 ? WindowSettings::defaultwidth - imageWidth - 1.f : 1.f;

    component->SetSourceRect(201, 0, imageWidth, imageHeight);
    component->SetDestinationRect(drawX, drawY, imageWidth, imageHeight);
}
