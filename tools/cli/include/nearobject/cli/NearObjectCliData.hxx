
#ifndef NEAR_OBJECT_CLI_DATA_HXX
#define NEAR_OBJECT_CLI_DATA_HXX

#include <cstdint>
#include <any>
#include <optional>
#include <string>

#include <uwb/UwbMacAddress.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/UwbConfiguration.hxx>
#include <uwb/protocols/fira/UwbSessionData.hxx>

using namespace uwb::protocol::fira;

namespace nearobject::cli
{
/**
 * @brief Helper structure to hold parameters for a ranging session.
 */
struct UwbRangingParameters
{
    uint32_t SessionId;
    std::vector<uwb::protocol::fira::UwbApplicationConfigurationParameter> ApplicationConfigurationParameters;
};

/**
 * @brief Helper sturcture for staging UwbConfiguration data fields from CLI
 * input. This is an intermediary structure that allows native use with the
 * CLI11 parsing library.
 */
struct UwbConfigurationData
{
    // Mirrored properties from UwbConfiguration data. Any newly added fields
    // that should be supported from the command-line must also be added here,
    // along with parsing support in a NearObjectCli instance.
    std::optional<uwb::protocol::fira::DeviceRole> deviceRole;
    std::optional<uwb::protocol::fira::RangingDirection> rangingDirection;
    std::optional<uwb::protocol::fira::MeasurementReportMode> rangingMeasurementReportMode;
    std::optional<uwb::protocol::fira::StsConfiguration> stsConfiguration;
    std::optional<uwb::protocol::fira::MultiNodeMode> multiNodeMode;
    std::optional<uwb::protocol::fira::RangingMode> rangingTimeStruct;
    std::optional<uwb::protocol::fira::SchedulingMode> schedulingMode;
    std::optional<bool> hoppingMode;
    std::optional<bool> blockStriding;
    std::optional<uint32_t> uwbInitiationTime;
    std::optional<uwb::protocol::fira::Channel> channel;
    std::optional<uwb::protocol::fira::StsPacketConfiguration> rframeConfig;
    std::optional<uwb::protocol::fira::ConvolutionalCodeConstraintLength> convolutionalCodeConstraintLength;
    std::optional<uwb::protocol::fira::PrfMode> prfMode;
    std::optional<uint8_t> sp0PhySetNumber;
    std::optional<uint8_t> sp1PhySetNumber;
    std::optional<uint8_t> sp3PhySetNumber;
    std::optional<uint8_t> preambleCodeIndex;
    std::optional<uwb::UwbMacAddressType> macAddressMode;
    std::optional<uwb::UwbMacAddress> controleeShortMacAddress;
    std::optional<uwb::UwbMacAddress> controllerMacAddress;
    std::optional<uint8_t> slotsPerRangingRound;
    std::optional<uint8_t> maxContentionPhaseLength;
    std::optional<uint8_t> slotDuration;
    std::optional<uint16_t> rangingInterval;
    std::optional<uint8_t> keyRotationRate;
    std::optional<uwb::UwbMacAddressFcsType> macAddressFcsType;
    std::optional<uint16_t> maxRangingRoundRetry;
    std::optional<uint8_t> numberOfControlees;
    std::optional<uwb::protocol::fira::DeviceType> deviceType;

    // Free-form string arugments that will be manually parsed to their target format(s).
    std::string firaPhyVersionString;
    std::string firaMacVersionString;
    std::string resultReportConfigurationString;

    /**
     * @brief Convert the staging data to a complete UwbConfiguration object.
     *
     * @return uwb::protocol::fira::UwbConfiguration
     */
    operator uwb::protocol::fira::UwbConfiguration() const noexcept;
};

/**
 * @brief Helper structure for staging UwbApplicationConfigurationParameter data fields from CLI
 * input. This is an intermediary structure that allows native use with the
 * CLI11 parsing library.
 */
struct UwbApplicationConfigurationParameterData
{
    // Mirrored properties from UwbApplicationConfigurationParameter data. Any newly added fields
    // that should be supported from the command-line must also be added here,
    // along with parsing support in a NearObjectCli instance.
    std::optional<uwb::protocol::fira::DeviceRole> deviceRole;
    std::optional<uwb::protocol::fira::MultiNodeMode> multiNodeMode;
    std::optional<uint8_t> numberOfControlees;
    std::optional<uwb::UwbMacAddress> deviceMacAddress;
    std::optional<std::unordered_set<uwb::UwbMacAddress>> destinationMacAddresses;
    std::optional<uwb::protocol::fira::DeviceType> deviceType;
    std::optional<uwb::UwbMacAddressType> macAddressMode;

    /**
     * @brief Variant for all possible property types.
     */
    using ParameterTypesVariant = std::variant<
        uint8_t,
        ::uwb::protocol::fira::DeviceRole,
        ::uwb::protocol::fira::DeviceType,
        ::uwb::protocol::fira::MultiNodeMode,
        ::uwb::UwbMacAddress,
        ::uwb::UwbMacAddressType,
        std::unordered_set<::uwb::UwbMacAddress>>;

    std::unordered_map<UwbApplicationConfigurationParameterType, ParameterTypesVariant>
    GetValueMap() const
    {
        std::unordered_map<UwbApplicationConfigurationParameterType, ParameterTypesVariant> valuesMap;
        if (deviceRole.has_value()) {
            valuesMap[UwbApplicationConfigurationParameterType::DeviceRole] = deviceRole.value();
        }
        if (multiNodeMode.has_value()) {
            valuesMap[UwbApplicationConfigurationParameterType::MultiNodeMode] = multiNodeMode.value();
        }
        if (numberOfControlees.has_value()) {
            valuesMap[UwbApplicationConfigurationParameterType::NumberOfControlees] = numberOfControlees.value();
        }
        if (deviceMacAddress.has_value()) {
            valuesMap[UwbApplicationConfigurationParameterType::DeviceMacAddress] = deviceMacAddress.value();
        }
        if (destinationMacAddresses.has_value()) {
            valuesMap[UwbApplicationConfigurationParameterType::DestinationMacAddresses] = destinationMacAddresses.value();
        }
        if (deviceType.has_value()) {
            valuesMap[UwbApplicationConfigurationParameterType::DeviceType] = deviceType.value();
        }
        return valuesMap;
    }
};

/**
 * @brief Base class for data parsed by NearObjectCli.
 */
struct NearObjectCliData
{
    virtual ~NearObjectCliData() = default;

    bool HostIsController{ false };
    std::string deviceMacAddressString;
    std::string destinationMacAddressString; // TODO: List of strings (or large string of mac address substrings) to support multiple controlees
    UwbConfigurationData uwbConfiguration{};
    UwbApplicationConfigurationParameterData appConfigParamsData{};
    uwb::protocol::fira::StaticRangingInfo StaticRanging{};
    uwb::protocol::fira::UwbSessionData SessionData{};
    UwbRangingParameters RangingParameters{};
    uint32_t SessionId{ 0 };
};

} // namespace nearobject::cli

#endif // NEAR_OBJECT_CLI_HXX
