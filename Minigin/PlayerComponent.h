#pragma once
#include "BaseComponent.h"
#include "UpdateContext.h"

namespace dae
{
	class PlayerComponent final : public BaseComponent
	{
	public:
		PlayerComponent() :BaseComponent() {};
		void Initialize();

		void FixedUpdate ([[maybe_unused]] const UpdateContext& context) override {};
		void Update([[maybe_unused]] const UpdateContext& context) override {};
		void LateUpdate([[maybe_unused]] const UpdateContext& context) override {};

		void Draw() override {};
		void DrawUI([[maybe_unused]] UpdateContext& context) override {};


		PlayerComponent(const PlayerComponent& other) = delete;
		PlayerComponent(PlayerComponent&& other) = delete;
		PlayerComponent& operator=(const PlayerComponent& other) = delete;
		PlayerComponent& operator=(PlayerComponent&& other) = delete;
	private:


		void Clone(GameObject*) override;
	};
}

