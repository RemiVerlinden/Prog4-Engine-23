#pragma once
#include "BaseComponent.h"

namespace dae::Input
{
	class InputDebugImguiComponent final : public BaseComponent
	{
	public:
		void Initialize() override;
		void DrawUI() override;
	private:
		void DrawIsDeviceConnectedWindow();
		void DrawDebugGamepadInputVisualisationWindow();
	};

}
