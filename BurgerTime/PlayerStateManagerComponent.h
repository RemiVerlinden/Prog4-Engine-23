#pragma once
#include "BaseComponent.h"
#include "EventListener.h"
#include <memory>
#include "PlayerStateEnum.h"

class PlayerState;
class PlayerComponent;
class PlayerStateManagerComponent : public engine::BaseComponent, public engine::EventListener
{
public:

	PlayerStateManagerComponent();
	virtual ~PlayerStateManagerComponent() = default;

	virtual void Initialize() override;
	virtual void LateUpdate(const engine::UpdateContext& context) override;
	virtual void OnEvent(const engine::Event & sent) override;

	PlayerComponent* GetPlayerComponent() const;
	void SwitchState(std::unique_ptr<PlayerState>&& state);
	PlayerStateEnum GetState() const;
private:
	std::unique_ptr<PlayerState> m_pCurrentState;

	PlayerComponent* m_pPlayerComponent;
};