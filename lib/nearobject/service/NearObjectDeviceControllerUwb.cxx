
#include <nearobject/service/NearObjectDeviceControllerUwb.hxx>

#include <nearobject/NearObjectSessionEventCallbacks.hxx>

using namespace nearobject::service;

NearObjectDeviceControllerUwb::NearObjectDeviceControllerUwb(std::unique_ptr<uwb::UwbDevice> uwbDevice) :
    m_uwbDevice(std::move(uwbDevice))
{
}

NearObjectDeviceController::StartSessionResult
NearObjectDeviceControllerUwb::StartSessionImpl(const NearObjectProfile& /* profile */, std::weak_ptr<NearObjectSessionEventCallbacks> /* eventCallbacks */)
{
    return { std::nullopt };
}

bool
NearObjectDeviceControllerUwb::IsEqual(const NearObjectDeviceController& other) const noexcept
{
    // This cast is safe since the operator==() implementation guarantees the
    // type of 'other' to be 'NearObjectDeviceControllerUwb' using a typeid check.
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    const auto& rhs = static_cast<const NearObjectDeviceControllerUwb&>(other);

    // TODO: use m_uwbDevice to compare *this and rhs device ids.
    return false;
}
