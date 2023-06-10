#pragma once
#include "InputDevice.h"
#include "InputStateMouse.h"
#include "SDLEventQueue.h"
namespace engine
{
	namespace Input
	{
		class InputDeviceMouse final : public InputDevice // ALL SDL GETS HANDLED IN THIS CLASS
		{
		public:

			InputDeviceMouse(SDLEventQueue& eventQueue) :m_EventQueue(eventQueue){};

			inline const InputStateMouse& GetMouseState() const { return m_MouseState; }
			virtual const InputState& GetDeviceInputState() const override { return m_MouseState; }

		private:

			virtual DeviceCategory GetDeviceCategory() const override { return DeviceCategory::Mouse; }

			virtual void Initialize() override {};
			virtual void Shutdown() override {};

			virtual void ProcessInput(Seconds deltaTime) override
			{
				for (const SDL_Event& event : m_EventQueue.GetEvents())
				{
					switch (event.type)
					{

						case SDL_MOUSEWHEEL:
						{
							m_MouseState.ProcesMouseWheel(event.wheel);
						}
						break;
					}
				}
				m_MouseState.ProcessInput(deltaTime);
			};

		private:

			SDLEventQueue&											m_EventQueue;
			InputStateMouse											m_MouseState;
		};
	}
}// Mutable,