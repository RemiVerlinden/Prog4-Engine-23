#pragma once
#include <string>

namespace engine {
	class GameObject;
	class UpdateContext;
	class BaseComponent
	{
	public:
		BaseComponent(const std::string& name = "");
		virtual ~BaseComponent() = default;

		virtual void Initialize() = 0;
		virtual void Update(const UpdateContext& /*context*/) {};
		virtual void Draw() {};
		virtual void DrawUI(UpdateContext& /*context*/) {};
		virtual void LateUpdate( const UpdateContext& /*context*/) {};
		virtual void FixedUpdate(const UpdateContext& /*context*/) {};

		static uint64_t GetComponentCount() { return m_ComponentCount; }
		
		std::string GetComponentTag() const { return m_ComponentTag; }
		void SetComponentTag(const std::string& tag) { m_ComponentTag = tag; }

		GameObject* GetGameObject() const;

		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

		bool CanBeCloned() const { return m_CanBeCloned; }
		void SetCanBeCloned(bool canBeCloned) { m_CanBeCloned = canBeCloned; }
	protected:
		virtual void Clone(GameObject* clone) = 0;

		GameObject*						m_GameObject;
	private:
		bool m_CanBeCloned = true; // this should be set to false if the component is created by another component and not by the user (look at FpsComponent constructor for example)
		static uint64_t					m_ComponentCount;
		std::string						m_ComponentTag;
		friend GameObject;

		void RootInitialize(GameObject* go);
		void RootUpdate(const UpdateContext& context);
		void RootDraw();
		void RootDrawUI(UpdateContext& context);
		void RootFixedUpdate(const UpdateContext& context);
		void RootLateUpdate(const UpdateContext& context);
	};
}