
#ifndef I_UWB_SIMULATOR_SESSION_HXX
#define I_UWB_SIMULATOR_SESSION_HXX

#include <memory>
#include <unordered_set>
#include <vector>

#include <uwb/UwbMacAddress.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>
#include <windows/devices/uwb/UwbAppConfiguration.hxx>

namespace windows::devices::uwb::simulator
{
using ::uwb::protocol::fira::UwbSessionState;
using ::uwb::protocol::fira::UwbSessionType;

struct IUwbSimulatorSession
{
    uint32_t Id;
    UwbSessionType Type{ UwbSessionType::RangingSession };
    UwbSessionState State{ UwbSessionState::Deinitialized };
    uint32_t Sequence{ 0 };
    uint32_t RangingCount{ 0 };
    std::unordered_set<::uwb::UwbMacAddress> Controlees;
    std::vector<std::shared_ptr<IUwbAppConfigurationParameter>> ApplicationConfigurationParameters;
};
} // namespace windows::devices::uwb::simulator

#endif // I_UWB_SIMULATOR_SESSION_HXX
