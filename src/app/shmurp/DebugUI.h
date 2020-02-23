#pragma once

#include <aunteater/UpdateTiming.h>

#include <websocket/WebSocket.h>

namespace ad {

class DebugUI
{
public:
    DebugUI();

    // Non-movable, non-copyable (captures this)
    DebugUI(const DebugUI &) = delete;

    void broadcast(const aunteater::UpdateTiming & aTimings);

private:
    std::mutex mSessionsMutex;
    std::vector<WebSocket> mSessions;
    NetworkContext mNetworkContext;
};

} // namespace ad
