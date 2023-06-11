#pragma once
#include "BaseComponent.h"
#include "IPhysicsSystem.hpp"

namespace engine 
{
class AnimatedSpriteComponent final : public engine::BaseComponent
{
public:
	virtual ~AnimatedSpriteComponent() = default;
	void Initialize() override;

	void Update(const engine::UpdateContext& context) override;
	void Draw() override;

private:
	Box m_SrcRect;

	int m_Frames;
};
}