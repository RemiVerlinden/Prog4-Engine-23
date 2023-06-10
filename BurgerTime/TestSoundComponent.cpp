#include "TestSoundComponent.h"
#include "InputSystem.h"
#include "Locator.h"
#include "SDLAudioWrapper.h"

//#include "SDL.h"

using namespace engine;
using namespace engine::Input;

//SDL_AudioSpec wavSpec;
//Uint32 wavLength;
//Uint8* wavBuffer;
//SDL_AudioDeviceID deviceId;

engine::TestSoundComponent::~TestSoundComponent()
{
	//SDL_CloseAudioDevice(deviceId);
	//SDL_FreeWAV(wavBuffer);
	//SDL_Quit();
}

void engine::TestSoundComponent::Initialize()
{

    //if (SDL_LoadWAV("../Data/Sounds/burgermenu.wav", &wavSpec, &wavBuffer, &wavLength) == NULL)
    //{
    //    std::cout << "Failed to load wave file: " << SDL_GetError() << std::endl;
    //    return;
    //}

    //deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    //if (deviceId == 0)
    //{
    //    std::cout << "Failed to open audio device: " << SDL_GetError() << std::endl;
    //}
}

void engine::TestSoundComponent::Update(const UpdateContext&)
{
    bool playMusic = InputSystem::GetInstance().GetKeyboardDevice()->GetKeyboardState().WasPressed(KeyboardButton::KEY_K);
    bool stopMusic = InputSystem::GetInstance().GetKeyboardDevice()->GetKeyboardState().WasReleased(KeyboardButton::KEY_L);
    bool playSFX = InputSystem::GetInstance().GetKeyboardDevice()->GetKeyboardState().WasReleased(KeyboardButton::KEY_I);

    static bool pauseAll = false;
    static bool wasPaused = false;  // New variable to track previous pause state

    if (InputSystem::GetInstance().GetKeyboardDevice()->GetKeyboardState().WasReleased(KeyboardButton::KEY_J))
    {
        pauseAll = !pauseAll;
    }

    if (playMusic)
    {
        Locator::GetSoundSystem().Play("burgermenu.wav", 1, false);
    }

    if (stopMusic)
    {
        Locator::GetSoundSystem().StopMusic();
    }

    if (playSFX)
    {
        Locator::GetSoundSystem().Play("coin_inserted.wav", 1, true);
    }

    // Only call PauseAll() or UnpauseAll() when the pause state changes
    if (pauseAll && !wasPaused)
    {
        Locator::GetSoundSystem().PauseAll();
    }
    else if (!pauseAll && wasPaused)
    {
        Locator::GetSoundSystem().UnpauseAll();
    }

    wasPaused = pauseAll;  // Update the previous pause state
}
