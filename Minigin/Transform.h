#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);

		const glm::vec3& GetScale() const { return m_Scale; }
		void SetScale(float x, float y, float z);
	private:
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
	};
}
