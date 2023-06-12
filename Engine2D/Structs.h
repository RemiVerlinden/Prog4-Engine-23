#pragma once
#include "glm\glm.hpp"
#include <variant>

namespace engine
{
	struct WindowSettings
	{
		static inline const int scale = 4;
		static inline const int defaultwidth = 224;
		static inline const int defaultheight = 216;
		static inline const int width = defaultwidth * scale;
		static inline const int height = defaultheight * scale;
	};

	namespace physics
	{
		struct Box
		{
			glm::vec2 pos;
			glm::vec2 size;
		};

		struct Circle
		{
			glm::vec2 pos;
			float radius;
		};

		using Shape = std::variant<Box, Circle>;
	}

	namespace render
	{
		enum DrawStyle : int
		{
			normal, // render texture at position
			customResolution, // render texture at position with custom resolution
			spritesheet, // render only part of texture
			background // stretch texture to fit background
		};

		enum FlipTexture
		{
			SDL_FLIP_NONE = 0x00000000,     /**< Do not flip */
			SDL_FLIP_HORIZONTAL = 0x00000001,    /**< flip horizontally */
			SDL_FLIP_VERTICAL = 0x00000002     /**< flip vertically */
		};
	}
}

