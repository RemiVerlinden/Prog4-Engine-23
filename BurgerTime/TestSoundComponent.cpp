#include "TestSoundComponent.h"
#include "InputSystem.h"
#include "Locator.h"
#include "SDLAudioWrapper.h"

//#include "SDL.h"

using namespace dae;
using namespace dae::Input;

//SDL_AudioSpec wavSpec;
//Uint32 wavLength;
//Uint8* wavBuffer;
//SDL_AudioDeviceID deviceId;

dae::TestSoundComponent::~TestSoundComponent()
{
	//SDL_CloseAudioDevice(deviceId);
	//SDL_FreeWAV(wavBuffer);
	//SDL_Quit();
}

void dae::TestSoundComponent::Initialize()
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

void dae::TestSoundComponent::Update(const UpdateContext&)
{
	bool playSound = InputSystem::GetInstance().GetKeyboardDevice()->GetKeyboardState().WasPressed(KeyboardButton::KEY_K);

	if (playSound)
	{
        Locator::GetSoundSystem().Play("burgermenu.wav",1,true);
        //playMusic("../Data/Sounds/burgermenu.wav", SDL_MIX_MAXVOLUME);
        //SDL_QueueAudio(deviceId, wavBuffer, wavLength);
        //SDL_PauseAudioDevice(deviceId, 0);
	}
}
