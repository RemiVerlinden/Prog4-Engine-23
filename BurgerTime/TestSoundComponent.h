#pragma once
#include "BaseComponent.h"
#include "UpdateContext.h"

namespace dae
{
	class TestSoundComponent final : public BaseComponent
	{
	public:
		TestSoundComponent() = default;
		~TestSoundComponent();
		void Initialize();

		void FixedUpdate(const UpdateContext& /*context*/) override {};
		void Update(const UpdateContext& /*context*/) override;
		void LateUpdate(const UpdateContext& /*context*/) override {};

		void Draw() override {};
		void DrawUI(UpdateContext& /*context*/) override {};


		TestSoundComponent(const TestSoundComponent& other) = delete;
		TestSoundComponent(TestSoundComponent&& other) = delete;
		TestSoundComponent& operator=(const TestSoundComponent& other) = delete;
		TestSoundComponent& operator=(TestSoundComponent&& other) = delete;
	private:


		void Clone(GameObject*) override {};
	};
}

