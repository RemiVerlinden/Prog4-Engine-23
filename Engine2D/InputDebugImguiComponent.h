#pragma once
#include "BaseComponent.h"
#include "EventListener.h"
namespace engine::Input
{
	class InputDebugImguiComponent final : public BaseComponent, public EventListener
	{
	public:
		void Initialize() override;
		void DrawUI(UpdateContext& context) override;
		virtual void OnEvent(const Event& sent);
	private:
		void DrawIsDeviceConnectedWindow();
		void DrawDebugGamepadInputVisualisationWindow();

		int DeathCounter{0};


	private:
		void Clone(GameObject* clone) override;
	};

}
