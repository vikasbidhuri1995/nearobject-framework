
#include <nearobject/service/NearObjectDeviceDiscoveryAgent.hxx>

using namespace nearobject::service;

NearObjectDeviceDiscoveryAgent::NearObjectDeviceDiscoveryAgent(std::function<void(NearObjectDevicePresence presence, const std::shared_ptr<NearObjectDevice> deviceChanged)> onDevicePresenceChanged) :
    m_onDevicePresenceChanged(std::move(onDevicePresenceChanged))
{}

void
NearObjectDeviceDiscoveryAgent::RegisterDiscoveryEventCallback(std::function<void(NearObjectDevicePresence presence, const std::shared_ptr<NearObjectDevice> deviceChanged)> onDevicePresenceChanged)
{
    std::unique_lock<std::shared_mutex> onDevicePresenceChangedLock{ m_onDevicePresenceChangedGate };
    m_onDevicePresenceChanged = std::move(onDevicePresenceChanged);
}

void
NearObjectDeviceDiscoveryAgent::DevicePresenceChanged(NearObjectDevicePresence presence, std::shared_ptr<NearObjectDevice> deviceChanged) const noexcept
{
    std::shared_lock<std::shared_mutex> onDevicePresenceChangedLock{ m_onDevicePresenceChangedGate };
    if (m_onDevicePresenceChanged) {
        m_onDevicePresenceChanged(presence, std::move(deviceChanged));
    }
}

bool
NearObjectDeviceDiscoveryAgent::IsStarted() const noexcept
{
    return m_started;
}

void
NearObjectDeviceDiscoveryAgent::Start()
{
    bool expected = false;
    if (m_started.compare_exchange_weak(expected, true)) {
        StartImpl();
    }
}

void
NearObjectDeviceDiscoveryAgent::Stop()
{
    bool expected = true;
    if (m_started.compare_exchange_weak(expected, false)) {
        StopImpl();
    }
}

std::future<std::vector<std::shared_ptr<NearObjectDevice>>>
NearObjectDeviceDiscoveryAgent::ProbeAsync()
{
    return ProbeAsyncImpl();
}

void
NearObjectDeviceDiscoveryAgent::StartImpl()
{}

void
NearObjectDeviceDiscoveryAgent::StopImpl()
{}

std::future<std::vector<std::shared_ptr<NearObjectDevice>>>
NearObjectDeviceDiscoveryAgent::ProbeAsyncImpl()
{
    std::promise<std::vector<std::shared_ptr<NearObjectDevice>>> probePromise{};
    probePromise.set_value({});
    return probePromise.get_future();
}
