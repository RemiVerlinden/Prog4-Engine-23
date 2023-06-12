#pragma once
#include "BaseComponent.h"
#include "EventListener.h"

class GameOverComponent final : public engine::BaseComponent, public engine::EventListener
{
public:
	virtual ~GameOverComponent() = default;
	virtual void Initialize() override;

	virtual void OnEvent(const engine::Event& sent) override;
	virtual void OnSceneDeactivate() override;
private:

};