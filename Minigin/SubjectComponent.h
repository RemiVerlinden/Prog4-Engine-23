#pragma once
#include "BaseComponent.h"
#include "UpdateContext.h"

namespace dae
{
	class SubjectComponent final : public BaseComponent
	{
	public:
		void Initialize() {};

		void FixedUpdate([[maybe_unused]] const UpdateContext& context) override {};
		void Update([[maybe_unused]] const UpdateContext& context) override {};
		void LateUpdate([[maybe_unused]] const UpdateContext& context) override {};

		void Draw() override {};
		void DrawUI([[maybe_unused]] UpdateContext& context) override {};


		SubjectComponent(const SubjectComponent& other) = delete;
		SubjectComponent(SubjectComponent&& other) = delete;
		SubjectComponent& operator=(const SubjectComponent& other) = delete;
		SubjectComponent& operator=(SubjectComponent&& other) = delete;
	private:

	};
}

