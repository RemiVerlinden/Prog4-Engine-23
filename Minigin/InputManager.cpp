#include <SDL.h>
#include "InputManager.h"
#include "SceneManager.h"
#include <iostream>

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
		}
		if (e.type == SDL_KEYUP) {
			switch (e.key.keysym.sym)
			{
				case SDLK_PAGEUP:
					SceneManager::GetInstance().NextScene();
					break;
				case SDLK_PAGEDOWN:
					SceneManager::GetInstance().PreviousScene();
					break;
			}
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {

		}
		// etc...
	}

	return true;
}
