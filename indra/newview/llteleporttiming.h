/*
Littlefield Grid FTRLV modified version, 2026-11-22.
Changes relative to Kokua 7.2.0.61057 are documented in doc/CHANGES-FTRLV.md.
Existing upstream copyright and license notices are retained.
*/
// Viewer-thread diagnostics for a locally initiated teleport.
#ifndef LL_LLTELEPORTTIMING_H
#define LL_LLTELEPORTTIMING_H

#include <chrono>

namespace LLTeleportTiming
{
    using Clock = std::chrono::steady_clock;

    struct FinishEventTimes
    {
        Clock::time_point response_available{};
        Clock::time_point identified{};
        Clock::time_point queue_post{};
        Clock::time_point queue_dequeued{};
    };

    void markFinishEventDispatch(const FinishEventTimes& times);
    void begin();
    void mark(const char* stage);
    void arrive();
    void finish();
    void cancel();
}

#endif
