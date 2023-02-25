#pragma once
#include <string>

namespace dae {
	class GameObject;
	class GameTime;
	class BaseComponent
	{
	public:
		BaseComponent();
		~BaseComponent() = default;

		virtual void Initialize(GameTime* time) = 0;
		virtual void Update(float ts) = 0;
		virtual void Draw() = 0;
		virtual void LateUpdate(float ts) = 0;
		virtual void FixedUpdate(float ts) = 0;


		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

	private:
		friend GameObject;

		virtual void RootInitialize(GameObject* go, GameTime* time);
		virtual void RootUpdate(float ts);
		virtual void RootDraw();
		virtual void RootFixedUpdate(float ts);
		virtual void RootLateUpdate(float ts);

		GameObject* m_GameObject;
	};
}