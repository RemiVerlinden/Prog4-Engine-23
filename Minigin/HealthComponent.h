#pragma once
#include "BaseComponent.h"
#include "UpdateContext.h"

namespace dae
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
		void Clone(GameObject* clone) override;
		struct Health
		{
			int max = 100;
			int current = max;
		};

		Health m_Health;
	};
}

