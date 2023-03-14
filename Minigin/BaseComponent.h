#pragma once
#include <string>

namespace dae {
	class GameObject;
	class UpdateContext;
	class BaseComponent
	{
	public:
		BaseComponent();
		virtual ~BaseComponent() = default;

		virtual void Initialize() = 0;
		virtual void Update([[maybe_unused]] const UpdateContext& context) {};
		virtual void Draw() {};
		virtual void LateUpdate([[maybe_unused]] const UpdateContext& context) {};
		virtual void FixedUpdate([[maybe_unused]] const UpdateContext& context) {};

		static uint64_t GetComponentCount() { return m_ComponentCount; }

		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

	protected:
		GameObject*						m_GameObject;
		static uint64_t					m_ComponentCount;

	private:
		friend GameObject;

		void RootInitialize(GameObject* go);
		void RootUpdate(const UpdateContext& context);
		void RootDraw();
		void RootFixedUpdate(const UpdateContext& context);
		void RootLateUpdate(const UpdateContext& context);
	};
}