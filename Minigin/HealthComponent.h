#pragma once
#include "BaseComponent.h"
#include "UpdateContext.h"

namespace dae
{
	class HealthComponent final : public BaseComponent
	{
	public:
		void Initialize() {};

		void FixedUpdate([[maybe_unused]] const UpdateContext& context) override {};
		void Update([[maybe_unused]] const UpdateContext& context) override {};
		void LateUpdate([[maybe_unused]] const UpdateContext& context) override {};

		void Draw() override {};
		void DrawUI([[maybe_unused]] UpdateContext& context) override {};


		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;
	private:

	};
}

