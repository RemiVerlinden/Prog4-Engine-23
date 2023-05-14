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

		void FixedUpdate([[maybe_unused]] const UpdateContext& context) override {};
		void Update([[maybe_unused]] const UpdateContext& context) override;
		void LateUpdate([[maybe_unused]] const UpdateContext& context) override {};

		void Draw() override {};
		void DrawUI([[maybe_unused]] UpdateContext& context) override {};


		TestSoundComponent(const TestSoundComponent& other) = delete;
		TestSoundComponent(TestSoundComponent&& other) = delete;
		TestSoundComponent& operator=(const TestSoundComponent& other) = delete;
		TestSoundComponent& operator=(TestSoundComponent&& other) = delete;
	private:


		void Clone(GameObject*) override {};
	};
}

