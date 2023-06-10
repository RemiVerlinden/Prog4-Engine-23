#pragma once
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
}