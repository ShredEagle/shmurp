#include "DebugUI.h"
#include <sstream>


namespace ad {

DebugUI::DebugUI() :
    mNetworkContext("0.0.0.0", 4321, [this](WebSocket aWebsocket)
    {
        //aWebsocket.onmessage([&aWebsocket](const std::string & aData)
        //{
        //    std::cerr << "Received: " << aData << "\n";
        //});
        std::lock_guard<std::mutex> guard(mSessionsMutex);
        mSessions.push_back(std::move(aWebsocket));
    })
{
    mNetworkContext.run();
}

void DebugUI::broadcast(const aunteater::UpdateTiming & aTimings)
{
    std::ostringstream oss;
    aTimings.outputTimings(oss);

    {
        std::lock_guard<std::mutex> guard(mSessionsMutex);
        for (auto & socket : mSessions)
        {
            socket.send(oss.str());
        }
    }
}
} // namespace ad
