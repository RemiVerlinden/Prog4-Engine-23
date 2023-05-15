#include "TrashTheCacheComponent.h"
#include "glm\glm.hpp"
#include "ImguiWrapper.h"
#include "GameObject.h"

void dae::TrashTheCacheComponent::Initialize()
{
}

void dae::TrashTheCacheComponent::DrawUI(UpdateContext& context)
{
	DrawImguiExercise1Window();
	DrawImguiExercise2Window();
}


void dae::TrashTheCacheComponent::DrawImguiExercise1Window()
{
	// Set window size
	static ImVec2 windowSize = ImVec2(300, 300);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);

	// Create window
	ImGui::Begin("Exercise 1");

	// Add a float slider
	static int numSamples = 10;
	const int minNumSamples = 3; // we will remove the best and worst sample so we need at least 3 to start off with 
	if (ImGui::InputInt("# samples", &numSamples))
	{
		
		numSamples = (((numSamples) > (minNumSamples)) ? (numSamples) : (minNumSamples));
	}

	// Add a button
	static bool displayGraph = false;
	if (!displayGraph)
	{
		if (ImGui::Button("Trash the cache"))
		{
			displayGraph = true;
		}
	}

	if (displayGraph)
	{
		static std::vector<float> timePerStep;
		static std::vector<float> sizeOfEachStep;
		if (timePerStep.size() > 0)
		{
			ImGui::PlotConfig config;
			config.values.count = static_cast<int>(timePerStep.size());
			config.values.xs = sizeOfEachStep.data();
			config.values.ys = timePerStep.data(); // use ys_list to draw several lines simultaneously
			config.values.ys_count = 1;
			config.values.color = ImColor(255, 127, 0);
			config.scale.min = 0;
			config.scale.max = timePerStep.front();
			config.tooltip.show = true;
			config.tooltip.format = "[%.0f] : %0.0f us";
			config.grid_x.show = true;
			config.grid_x.size = 0;
			config.grid_x.subticks = 0;
			config.grid_y.size = 0;
			config.grid_y.subticks = 0;
			config.grid_y.show = true;
			config.frame_size = ImVec2(200, 100);
			ImGui::Plot("TrashTheCache 1", config);
		}
		else
		{
			static std::vector<TimeData> timeMeasurements;

			InputTimingParameters inout;
			inout.arraySize = 10'000'000;
			inout.sampleSize = numSamples;
			inout.timings = &timeMeasurements;
			TimeInt(inout);

			for (TimeData& data : timeMeasurements)
			{
				using namespace std::chrono;

				timePerStep.push_back(static_cast<float>(data.measuredtime.count()));
				sizeOfEachStep.push_back(data.step);
			}
		}

	}


	// End the window
	ImGui::End();
}

void dae::TrashTheCacheComponent::DrawImguiExercise2Window()
{
	// Set window size
	static ImVec2 windowSize = ImVec2(300, 300);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);



	// Create window
	ImGui::Begin("Exercise 2");

	// Add a float slider
	static int numSamples = 100;
	const int minNumSamples = 3; // we will remove the best and worst sample so we need at least 3 to start off with 
	if (ImGui::InputInt("# samples", &numSamples))
	{
		numSamples = (((numSamples) > (minNumSamples)) ? (numSamples) : (minNumSamples));
	}

	// Add a button
	static bool showGraphGameObject = false;
	if (!showGraphGameObject)
	{
		if (ImGui::Button("Trash the cache with GameObject3D"))
		{
			showGraphGameObject = true;
		}
	}

	static std::vector<float> timePerStepGameObject;
	static std::vector<float> sizeOfEachStepGameObject;
	if (showGraphGameObject)
	{
		if (timePerStepGameObject.size() > 0)
		{
			ImGui::Text("GameObject3D:");

			ImGui::PlotConfig config;
			config.values.count = static_cast<int>(timePerStepGameObject.size());
			config.values.xs = sizeOfEachStepGameObject.data();
			config.values.ys = timePerStepGameObject.data(); // use ys_list to draw several lines simultaneously
			config.values.ys_count = 1;
			config.values.color = ImColor(127, 127, 255);
			config.scale.min = 0;
			config.scale.max = timePerStepGameObject.front();
			config.tooltip.show = true;
			config.tooltip.format = "[%.0f] : %0.0f us";
			config.grid_x.show = true;
			config.grid_x.size = 0;
			config.grid_x.subticks = 0;
			config.grid_y.size = 0;
			config.grid_y.subticks = 0;
			config.grid_y.show = true;
			config.frame_size = ImVec2(200, 100);
			ImGui::Plot("TrashTheCache 2", config);
		}
		else
		{
			static std::vector<TimeData> timeMeasurements;
			InputTimingParameters inout;
			inout.arraySize = 10'000'000;
			inout.sampleSize = numSamples;
			inout.timings = &timeMeasurements;
			TimeGameObject<GameObject3D>(inout);

			for (TimeData& data : timeMeasurements)
			{
				using namespace std::chrono;
				timePerStepGameObject.push_back(static_cast<float>(data.measuredtime.count()));
				sizeOfEachStepGameObject.push_back(data.step);
			}
		}

	}

	static bool showGraphGameObjectAlt = false;
	if (!showGraphGameObjectAlt)
	{
		if (ImGui::Button("Trash the cache with GameObject3DAlt"))
		{
			showGraphGameObjectAlt = true;
		}
	}

	static std::vector<float> timePerStepGameObjectAlt;
	static std::vector<float> sizeOfEachStepGameObjectAlt;
	if (showGraphGameObjectAlt)
	{
		static float highestTime;
		if (timePerStepGameObjectAlt.size() > 0)
		{
			ImGui::Text("GameObject3DAlt:");

			ImGui::PlotConfig config;
			config.values.count = static_cast<int>(timePerStepGameObjectAlt.size());
			config.values.xs = sizeOfEachStepGameObjectAlt.data();
			config.values.ys = timePerStepGameObjectAlt.data(); // use ys_list to draw several lines simultaneously
			config.values.ys_count = 1;
			config.values.color = ImColor(127, 255, 0);
			config.scale.min = 0;
			config.scale.max = timePerStepGameObjectAlt.front();
			config.tooltip.show = true;
			config.tooltip.format = "[%.0f] : %0.0f us";
			config.grid_x.show = true;
			config.grid_x.size = 0;
			config.grid_x.subticks = 0;
			config.grid_y.size = 0;
			config.grid_y.subticks = 0;
			config.grid_y.show = true;
			config.frame_size = ImVec2(200, 100);
			ImGui::Plot("TrashTheCache 3", config);
		}
		else
		{
			static std::vector<TimeData> timeMeasurements;
			InputTimingParameters inout;
			inout.arraySize = 10'000'000;
			inout.sampleSize = 100;
			inout.timings = &timeMeasurements;
			TimeGameObject<GameObject3DAlt>(inout);


			for (TimeData& data : timeMeasurements)
			{
				using namespace std::chrono;

				timePerStepGameObjectAlt.push_back(static_cast<float>(data.measuredtime.count()));
				sizeOfEachStepGameObjectAlt.push_back(data.step);
			}
		}
	}

	if (showGraphGameObjectAlt && showGraphGameObject)
	{
		ImGui::Text("Combined:");

		static const float* y_data[] = { timePerStepGameObject.data(), timePerStepGameObjectAlt.data() };
		static float highestTimeMeasure1 = *std::max_element(timePerStepGameObject.begin(), timePerStepGameObject.end());
		static float highestTimeMeasure2 = *std::max_element(timePerStepGameObjectAlt.begin(), timePerStepGameObjectAlt.end());
		
		static float highestTime = (((highestTimeMeasure1) > (highestTimeMeasure2)) ? (highestTimeMeasure1) : (highestTimeMeasure2));

		static ImU32 colors[2] = { ImColor(127, 127, 255), ImColor(127, 255, 0) };
		ImGui::PlotConfig config;
		config.values.count = static_cast<int>(timePerStepGameObject.size());
		config.values.ys_list = y_data; // use ys_list to draw several lines simultaneously
		config.values.ys_count = 2;
		config.values.colors = colors;
		config.scale.min = 0;
		config.scale.max = highestTime;
		config.tooltip.show = false;
		config.grid_x.show = true;
		config.grid_x.size = 0;
		config.grid_x.subticks = 0;
		config.grid_y.size = 0;
		config.grid_y.subticks = 0;
		config.grid_y.show = true;
		config.frame_size = ImVec2(200, 100);
		ImGui::Plot("TrashTheCache 3", config);
	}


	// End the window
	ImGui::End();
}

void dae::TrashTheCacheComponent::Clone(GameObject* clone)
{
	if (CanBeCloned() == false) return;
	clone->AddComponent<TrashTheCacheComponent>(GetComponentTag());
}



void dae::TrashTheCacheComponent::TimeInt(InputTimingParameters input)
{
	using namespace std::chrono;

	const int arraySize = input.arraySize;
	int* dataArray = new int[arraySize];

	std::fill(dataArray, dataArray + arraySize, 1);

	std::vector<TimeData>& timeVec = *input.timings;

	static int numSamples = input.sampleSize;

	for (int step = 1; step <= 1024; step *= 2)
	{
		std::multiset<nanoseconds> individualSampleTimes;

		for (int sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
		{
			steady_clock::time_point begin = steady_clock::now();
			for (int dataIndex = 0; dataIndex < arraySize; dataIndex += step)
			{
				dataArray[dataIndex] *= 2;
			}
			individualSampleTimes.insert(steady_clock::now() - begin);
		}
		size_t usedSamples = 0;
		nanoseconds averageSampleTime{ 0 };
		// get rid of the best & worst sampleTimes by offsetting the for loop start & end
		for (auto sampleIter = std::next(individualSampleTimes.begin(), numSamples / 10); sampleIter != std::prev(individualSampleTimes.end(), numSamples / 10); ++sampleIter)
		{
			averageSampleTime += *sampleIter;
			++usedSamples;
		}
		averageSampleTime /= usedSamples;

		timeVec.push_back(TimeData{ static_cast<float>(step), averageSampleTime });
	}

	delete[] dataArray;
}



template <typename GameObjectType>
void dae::TrashTheCacheComponent::TimeGameObject(InputTimingParameters input)
{
	using namespace std::chrono;

	static_assert(std::is_same<GameObjectType, GameObject3D>::value || std::is_same<GameObjectType, GameObject3DAlt>::value,
		"TimeGameObject can only be called with GameObject3D or GameObject3DAlt");

	const int arraySize = input.arraySize;
	GameObjectType* dataArray = new GameObjectType[arraySize];

	std::vector<TimeData>& timeVec = *input.timings;

	int numSamples = input.sampleSize;

	for (int step = 1; step <= 1024; step *= 2)
	{
		std::multiset <std::chrono::nanoseconds> individualSampleTimes;

		for (int sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
		{
			steady_clock::time_point begin = steady_clock::now();
			for (int dataIndex = 0; dataIndex < arraySize; dataIndex += step)
			{
				dataArray[dataIndex].ID *= 2;
			}
			individualSampleTimes.insert(steady_clock::now() - begin);
		}
		size_t usedSamples = 0;
		nanoseconds averageSampleTime{ 0 };
		for (auto sampleIter = std::next(individualSampleTimes.begin(), numSamples / 10); sampleIter != std::prev(individualSampleTimes.end(), numSamples / 10); ++sampleIter)
		{
			averageSampleTime += *sampleIter;
			++usedSamples;
		}
		averageSampleTime /= usedSamples;

		timeVec.push_back(TimeData{ static_cast<float>(step), averageSampleTime });
	}

	delete[] dataArray;
}
