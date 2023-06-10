#pragma once
#include "BaseComponent.h"
#include "UpdateContext.h"

namespace engine
{
	class PlayerComponent final : public BaseComponent
	{
	public:
		PlayerComponent() :BaseComponent() {};
		void Initialize();

		void FixedUpdate (const UpdateContext& /*context*/) override {};
		void Update(const UpdateContext& /*context*/) override {};
		void LateUpdate(const UpdateContext& /*context*/) override {};
		void OnSceneActivate() override;
		void Draw() override {};
		void DrawUI(UpdateContext& /*context*/) override {};


		PlayerComponent(const PlayerComponent& other) = delete;
		PlayerComponent(PlayerComponent&& other) = delete;
		PlayerComponent& operator=(const PlayerComponent& other) = delete;
		PlayerComponent& operator=(PlayerComponent&& other) = delete;
	private:

		void InitializeInputCommands();
	};
}

