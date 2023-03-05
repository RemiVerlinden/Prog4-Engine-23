#pragma once
#include <chrono>
#include "glm/glm.hpp"

namespace dae 
{
	using namespace std::chrono;
	class GameTime
	{
	public:
		GameTime() = default;
		~GameTime() = default;

		GameTime(const GameTime& other) = delete;
		GameTime(GameTime&& other) noexcept = delete;
		GameTime& operator=(const GameTime& other) = delete;
		GameTime& operator=(GameTime&& other) noexcept = delete;


		void Reset();
		void Update();

		float GetElapsed() const { return  m_ElapsedGameTime; }
		float GetFixedTimeStep() const { return  m_FixedTimeStep; }
		float GetTotal() const { return m_TotalGameTime; }
		int GetFPS() const { return m_FPS; }
		void ForceElapsedUpperbound(bool force, float upperBound = 0.03f) { m_ForceElapsedUpperBound = force; m_ElapsedUpperBound = upperBound; }
		bool IsRunning() const { return !m_IsStopped; }

		inline void SetFrameRateLimit(float FPS) { m_frameRateLimitFPS = glm::max(0.0f, FPS); }
		inline float HasFrameRateLimit() const { return m_frameRateLimitFPS > 0; }
		inline float GetFrameRateLimit() const { return m_frameRateLimitFPS; }
		inline float GetLimitedFrameTime() const { return (1000.f / m_frameRateLimitFPS); }

		void Start();
		void Stop();

	private:
		float											m_TotalGameTime{};
		float											m_ElapsedGameTime{};


		bool											m_ForceElapsedUpperBound{};
		float											m_ElapsedUpperBound{ 0.03f };
		float											m_FixedTimeStep{ 0.02f };
		float											m_frameRateLimitFPS = 144.0f;

		std::chrono::high_resolution_clock::time_point	m_BaseTime{};
		std::chrono::high_resolution_clock::time_point	m_StopTime{};
		std::chrono::high_resolution_clock::time_point	m_PrevTime{};
		std::chrono::high_resolution_clock::time_point	m_CurrTime{};
		std::chrono::high_resolution_clock::duration	m_PausedDuration{};

		bool											m_IsStopped{ true };

		int												m_FPS{};
		float											m_FpsTimer{};
		int												m_FpsCount{};
	};
}