
//-------------------------------------------------------------------------




// BIG DISCLAIMER

// TIME.H - TIME.CPP - TIMERS.H

// THESE FILES ARE NOT MINE, I DID NOT WRITE THEM

// I LIKE HOW THEY WORK AND JUST WANT TO EXPERIMENT WITH THEM

// CREDITS https://github.com/BobbyAnguelov/Esoterica



//-------------------------------------------------------------------------

#include "Time.h"
#include <chrono>

//-------------------------------------------------------------------------

namespace dae
{
    Nanoseconds EngineClock::CurrentTime = 0;

    //-------------------------------------------------------------------------

    Nanoseconds::operator Microseconds() const
    {
        auto const duration = std::chrono::duration<uint64_t, std::chrono::steady_clock::period>( m_value );
        uint64_t const numMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>( duration ).count();
        return float( numMicroseconds );
    }

    //-------------------------------------------------------------------------

    Nanoseconds PlatformClock::GetTime()
    {
        auto const time = std::chrono::high_resolution_clock::now();
        uint64_t const numNanosecondsSinceEpoch = time.time_since_epoch().count();
        return Nanoseconds( numNanosecondsSinceEpoch );
    }

    //-------------------------------------------------------------------------

    void EngineClock::Update( Milliseconds deltaTime )
    {
        assert( deltaTime >= 0 );
        CurrentTime += deltaTime.ToNanoseconds();
    }
}