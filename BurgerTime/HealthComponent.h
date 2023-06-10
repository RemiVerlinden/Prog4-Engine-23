#pragma once
#include "BaseComponent.h"
#include "UpdateContext.h"

namespace engine
{
	class HealthComponent final : public BaseComponent
	{
	public:
		void Initialize() {};

		void SetMaxHealth(int maxHealth);
		void SetCurrentHealth(int health);
		void Damage(int damageAmount);
		void Heal(int healAmount);
	private:
		struct Health
		{
			int max = 100;
			int current = max;
		};

		Health m_Health;
	};
}

