#pragma once
#include "BaseComponent.h"
#include "UpdateContext.h"

namespace dae
{
	class HealthComponent final : public BaseComponent
	{
	public:
		void Initialize() {};

		void SetHealth(int maxHealth);
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

