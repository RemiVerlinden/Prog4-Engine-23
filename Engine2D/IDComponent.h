#pragma once
#include "BaseComponent.h"
#include "UpdateContext.h"

//==============================================================================
//
// THIS IS NOT A REAL COMPONENT : THIS IS JUST SO I CAN COPY PASTE IT WHEN I MAKE ACTUAL NEW COMPONENTS
//
//==============================================================================

namespace engine
{
	class IDComponent final : public BaseComponent
	{
	public:
		void Initialize() { m_ID = m_UniqueID++; };

		uint64_t GetID() { return m_ID; }
	private:
		void Clone(GameObject* clone) override;
		uint64_t m_ID;
		inline static uint64_t m_UniqueID = 0;

	};
}

