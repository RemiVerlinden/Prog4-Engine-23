#pragma once
#include "BaseComponent.h"
#include "UpdateContext.h"
#include "EventListener.h"

namespace engine
{
	class RenderComponent;
	class GameObject;
}
class LivesComponent final : public engine::BaseComponent, public engine::EventListener
{
public:
	virtual ~LivesComponent() = default;
	void Initialize();

	virtual void OnEvent(const engine::Event& sent) override;

	int GetLives();
	void Linkplayer(engine::GameObject* player);
private:
	void RemoveLife();
	void InitPlayerComponent(engine::RenderComponent* component, size_t currentLife, bool isPlayer2);
	void RemoveLastLifeRenderComponent();
	struct Lives
	{
		static const inline int max = 3;
		int current = max;
	};
	Lives m_Lives;
	std::vector<engine::RenderComponent*> m_pRenderComponents;
	engine::GameObject* m_pPlayer = nullptr;
};


