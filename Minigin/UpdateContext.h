#pragma once
#include "Time.h"
#include "glm/glm.hpp"
//-------------------------------------------------------------------------
// The base update context for anything in the engine that needs to be updated
//-------------------------------------------------------------------------

namespace dae
{
    class UpdateContext
    {

    public:

        // Update info
        //-------------------------------------------------------------------------

        inline Seconds GetDeltaTime() const { return m_DeltaTime; }
        inline uint64_t GetFrameID() const { return m_FrameID; }


        // Frame rate limiter
        //-------------------------------------------------------------------------

        inline bool HasFrameRateLimit() const { return m_FrameRateLimitFPS > 0; }
        inline void SetFrameRateLimit( float FPS ) { m_FrameRateLimitFPS = glm::max( 0.0f, FPS ); }
        inline float GetFrameRateLimit() const { assert( HasFrameRateLimit() ); return m_FrameRateLimitFPS; }
        inline Milliseconds GetLimitedFrameTime() const { assert( HasFrameRateLimit() ); return Milliseconds( 1000 ) / m_FrameRateLimitFPS; }
        inline const Seconds GetFixedTimeStep() const { return m_FixedTimeStep; };
        // Set the time delta for this update
        inline void UpdateDeltaTime( Milliseconds deltaTime )
        {
            assert( deltaTime >= 0 );

            // Update internal time
            m_DeltaTime = deltaTime.ToSeconds();
            ++m_FrameID;
        }
    protected:


    protected:

        Seconds                                     m_DeltaTime = 1.0f / 60.0f;
        uint64_t                                    m_FrameID = 0;
        float                                       m_FrameRateLimitFPS = 666.0f;
        Seconds							            m_FixedTimeStep = 0.02f;

    };
}
