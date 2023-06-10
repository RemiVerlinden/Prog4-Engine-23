#pragma once
#include "BaseComponent.h"
#include <vector>
#include <set>
#include <chrono>
namespace engine
{
	class TrashTheCacheComponent final : public BaseComponent
	{
	private:


		struct Transform
		{
			float matrix[16] =
			{
				1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1
			};
		};

		class GameObject3D
		{
		public:
			Transform transform;
			int ID = 1;
		};

		class GameObject3DAlt
		{
		public:
			Transform* transform;
			int ID = 1;
			GameObject3DAlt() :transform(new Transform{}) {};
			~GameObject3DAlt() { delete[] transform; };

		};

		struct TimeData
		{
			float step;
			std::chrono::nanoseconds measuredtime;
		};

		struct InputTimingParameters
		{
			int sampleSize = 100;
			int arraySize = 1'000'000;
			std::vector<TimeData>* timings;
		};


	public:
		virtual void Initialize() override;
		virtual void DrawUI(UpdateContext& context) override;

	private:
		void TimeInt(InputTimingParameters input);

		template <typename GameObjectType>
		void TimeGameObject(InputTimingParameters input);
		void DrawImguiExercise1Window();
		void DrawImguiExercise2Window();


	private:
	};
}

