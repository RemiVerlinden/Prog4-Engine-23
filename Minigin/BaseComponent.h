#pragma once
#include <string>
namespace dae {
	class BaseComponent
	{
	public:
		BaseComponent() = default;
		~BaseComponent();

		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void LateUpdate() = 0;
		virtual void FixedUpdate() = 0;


		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

	private:
		virtual void RootInitialize();
		virtual void RootUpdate();
		virtual void RootDraw();
		virtual void RootFixedUpdate();
		virtual void RootLateUpdate();

		std::string m_Name;

	};
}