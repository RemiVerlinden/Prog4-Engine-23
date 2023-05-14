#include "SDLAudioWrapper.h"

// For error output
#include <iostream>

Sound::Sound(std::string filepath)
{
    if (SDL_LoadWAV("../Data/Sounds/burgermenu.wav", &m_WaveSpec, &m_WaveBuffer, &m_WaveLength) == NULL)
    {
        std::cout << "Failed to load wave file: " << SDL_GetError() << std::endl;
    }
    SetupDevice();
}

Sound::~Sound()
{
    SDL_FreeWAV(m_WaveBuffer);
    SDL_CloseAudioDevice(m_DeviceId);
}

void Sound::PlaySound()
{
    // Queue the audio (so we play when available,
    //                  as oppososed to a callback function)
    //int status = SDL_QueueAudio(m_DeviceId, m_WaveBuffer, m_WaveLength);
    //(status);
    //SDL_PauseAudioDevice(m_DeviceId, 0);

    SDL_QueueAudio(m_DeviceId, m_WaveBuffer, m_WaveLength);
    SDL_PauseAudioDevice(m_DeviceId, 0);

    //SDL_QueueAudio(deviceId, wavBuffer, wavLength);
//SDL_PauseAudioDevice(deviceId, 0);
}

void Sound::StopSound()
{
    SDL_PauseAudioDevice(m_DeviceId, 1);
}

void Sound::SetupDevice()
{
    // Request the most reasonable default device
    // Set the device for playback for 0, or '1' for recording.
    //m_device = SDL_OpenAudioDevice(nullptr, 0, &m_audioSpec, nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE);
    //// Error message if no suitable device is found to play
    //// audio on.
    //if (0 == m_device)
    //{
    //    std::cerr << "sound device error: " << SDL_GetError() << std::endl;
    //}

    m_DeviceId = SDL_OpenAudioDevice(NULL, 0, &m_WaveSpec, NULL, 0);
    if (m_DeviceId == 0)
    {
        std::cout << "Failed to open audio device: " << SDL_GetError() << std::endl;
    }
}

