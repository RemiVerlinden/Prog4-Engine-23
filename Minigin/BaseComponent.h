#pragma once
#include <string>

namespace dae {
	class GameObject;
	class GameTime;
	class BaseComponent
	{
	public:
		BaseComponent();
		virtual ~BaseComponent() = default;

		virtual void Initialize(GameTime* time) = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void LateUpdate() = 0;
		virtual void FixedUpdate() = 0;


		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

	protected:
		GameObject* m_GameObject;

	private:
		friend GameObject;

		virtual void RootInitialize(GameObject* go, GameTime* time);
		virtual void RootUpdate();
		virtual void RootDraw();
		virtual void RootFixedUpdate();
		virtual void RootLateUpdate();
	};
}