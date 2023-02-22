
#include <random>

#include <catch2/catch_test_macros.hpp>
#include <magic_enum.hpp>

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>

namespace UwbCxDdi = windows::devices::uwb::ddi::lrp;

namespace windows::devices::uwb::ddi::lrp::test
{
template <typename NeutralT>
NeutralT
ConvertRoundtrip(const NeutralT& instance)
{
    return UwbCxDdi::To(UwbCxDdi::From(instance));
}

template <typename NeutralT>
void
ValidateRoundtrip(const NeutralT& instance)
{
    auto instanceCopy = ConvertRoundtrip(instance);
    REQUIRE(instanceCopy == instance);
}
} // namespace windows::devices::uwb::ddi::lrp::test

TEST_CASE("ddi <-> neutral type conversions are stable", "[basic][conversion][windows][ddi]")
{
    using namespace windows::devices::uwb::ddi::lrp;
    using namespace uwb::protocol::fira;

    std::mt19937 RandomEngine{ std::random_device{}() };
    std::uniform_int_distribution<uint32_t> RandomDistribution{};

    SECTION("UwbStatus is stable")
    {
        for (const auto& uwbStatusGeneric : magic_enum::enum_values<UwbStatusGeneric>()) {
            const UwbStatus status{ uwbStatusGeneric };
            test::ValidateRoundtrip(status);
        }
    }

    SECTION("UwbDeviceState is stable")
    {
        for (const auto& uwbDeviceState : magic_enum::enum_values<UwbDeviceState>()) {
            // Avoid roundtrip test for neutral enum value which has no corresponding DDI value.
            if (uwbDeviceState != UwbDeviceState::Uninitialized) {
                test::ValidateRoundtrip(uwbDeviceState);
            }
        }
    }

    SECTION("UwbMulticastAction is stable")
    {
        for (const auto& uwbMulticastAction : magic_enum::enum_values<UwbMulticastAction>()) {
            test::ValidateRoundtrip(uwbMulticastAction);
        }
    }

    SECTION("UwbSessionState is stable")
    {
        for (const auto& uwbSessionState : magic_enum::enum_values<UwbSessionState>()) {
            test::ValidateRoundtrip(uwbSessionState);
        }
    }

    SECTION("UwbSessionReasonCode is stable")
    {
        for (const auto& uwbReasonCode : magic_enum::enum_values<UwbSessionReasonCode>()) {
            test::ValidateRoundtrip(uwbReasonCode);
        }
    }

    SECTION("UwbSessionStatus is stable")
    {
        // Generate all possible session states.
        for (const auto& uwbSessionState : magic_enum::enum_values<UwbSessionState>()) {
            // Generate all possible session reason codes.
            for (const auto& uwbSessionReasonCode : magic_enum::enum_values<UwbSessionReasonCode>()) {
                UwbSessionStatus uwbSessionStatus{
                    .SessionId = RandomDistribution(RandomEngine),
                    .State = uwbSessionState,
                    .ReasonCode = uwbSessionReasonCode
                };
                test::ValidateRoundtrip(uwbSessionStatus);
            }
        }
    }
}
