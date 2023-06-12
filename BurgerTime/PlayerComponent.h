#pragma once
#include "BaseComponent.h"
#include "glm\glm.hpp"
#include "EventListener.h"
#include "PlayerStateEnum.h"

class PlayerStateManagerComponent;
namespace engine
{
	class AnimatedSpriteComponent;
	class GameObject;
}
class PlayerComponent final : public engine::BaseComponent, public engine::EventListener
{
public:
	struct Settings 
	{
		const static inline float Speed = 40.f;
	};
	virtual ~PlayerComponent() = default;
	void Initialize();
	virtual void OnEvent(const engine::Event& sent) override;

	void OnSceneActivate() override;
	void Update(const engine::UpdateContext& context) override;
	void LateUpdate(const engine::UpdateContext& context) override;

	void SetMoveDirection(glm::vec2 direction);

	glm::vec2 GetMoveDelta() { return m_MoveDelta; }
	engine::AnimatedSpriteComponent* GetAnimatedSpriteComponent();
	void SetSpawnPosition(glm::vec2 spawnPos) { m_SpawnPosition = spawnPos; }
	PlayerStateEnum GetPlaterState() const;

private:
	PlayerStateManagerComponent* m_pStateManager;
	engine::AnimatedSpriteComponent* m_pAnimatedSprite;

	glm::vec2 m_MoveDirection;


	glm::vec2 m_MoveDelta;
	glm::vec2 m_LastPosition;

	glm::vec2 m_SpawnPosition;

	engine::GameObject* m_Enemy;


	void InitializeInputCommands();
};

