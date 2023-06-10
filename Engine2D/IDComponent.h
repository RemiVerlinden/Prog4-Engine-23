#pragma once
#include "BaseComponent.h"
#include "UpdateContext.h"

namespace engine
{
	class IDComponent final : public BaseComponent
	{
	public:
		void Initialize() { m_ID = m_UniqueID++; };

		uint64_t GetID() { return m_ID; }
	private:
		uint64_t m_ID;
		inline static uint64_t m_UniqueID = 0;

	};
}
