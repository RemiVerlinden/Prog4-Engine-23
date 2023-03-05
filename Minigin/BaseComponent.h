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
		virtual void Update(const UpdateContext& context) = 0;
		virtual void Draw() = 0;
		virtual void LateUpdate(const UpdateContext& context) = 0;
		virtual void FixedUpdate(const UpdateContext& context) = 0;


		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

	protected:
		GameObject* m_GameObject;
		
	private:
		friend GameObject;

		virtual void RootInitialize(GameObject* go);
		virtual void RootUpdate(const UpdateContext& context);
		virtual void RootDraw();
		virtual void RootFixedUpdate(const UpdateContext& context);
		virtual void RootLateUpdate(const UpdateContext& context);
	};
}