#pragma once
#include <string>
#include <functional>
#include "UpdateContext.h"

namespace engine
{
	class Engine2D
	{
	public:
		explicit Engine2D(const std::string& dataPath);
		~Engine2D();
		void Run(const std::function<void()>& load);

		Engine2D(const Engine2D& other) = delete;
		Engine2D(Engine2D&& other) = delete;
		Engine2D& operator=(const Engine2D& other) = delete;
		Engine2D& operator=(Engine2D&& other) = delete;
	};
}