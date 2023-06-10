#pragma once
#include "BaseComponent.h"

//==============================================================================
//
// THIS IS NOT A REAL COMPONENT : THIS IS JUST SO I CAN COPY PASTE IT WHEN I MAKE ACTUAL NEW COMPONENTS
//
//==============================================================================

namespace engine
{
	class TagComponent final : public BaseComponent
	{
	public:
		void Initialize() {};

		std::string m_Tag = "Undefined";
	private:
	};
}

