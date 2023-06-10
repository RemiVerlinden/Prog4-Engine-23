#pragma once
#include "BaseComponent.h"
#include "UpdateContext.h"

//==============================================================================
//
// THIS IS NOT A REAL COMPONENT : THIS IS JUST SO I CAN COPY PASTE IT WHEN I MAKE ACTUAL NEW COMPONENTS
//
//==============================================================================

namespace engine
{
	class TemplateComponent final : public BaseComponent
	{
	public:
		void Initialize() {};

		void FixedUpdate (const UpdateContext& /*context*/) override {};
		void Update(const UpdateContext& /*context*/) override {};
		void LateUpdate(const UpdateContext& /*context*/) override {};

		void Draw() override {};
		void DrawUI(UpdateContext& /*context*/) override {};


		TemplateComponent(const TemplateComponent& other) = delete;
		TemplateComponent(TemplateComponent&& other) = delete;
		TemplateComponent& operator=(const TemplateComponent& other) = delete;
		TemplateComponent& operator=(TemplateComponent&& other) = delete;
	private:


		void Clone(GameObject*) override {};
	};
}

