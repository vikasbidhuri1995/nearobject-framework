
#include <stdexcept>

#include <magic_enum.hpp>
#include <nearobject/cli/NearObjectCli.hxx>
#include <optional>

using namespace nearobject::cli;

NearObjectCli::NearObjectCli(std::shared_ptr<NearObjectCliData> cliData) :
    m_cliData(cliData)
{}

CLI::App&
NearObjectCli::GetParser()
{
    if (!m_cliApp) {
        m_cliApp = CreateParser();
        if (!m_cliApp) {
            throw std::runtime_error("failed to create command line parser");
        }
    }

    return *m_cliApp;
}

std::shared_ptr<NearObjectCliData>
NearObjectCli::GetData() const noexcept
{
    return m_cliData;
}

int
NearObjectCli::Parse(int argc, char* argv[]) noexcept
{
    try {
        m_cliApp->parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        return m_cliApp->exit(e);
    }

    return 0;
}

CLI::App&
NearObjectCli::GetUwbApp() noexcept
{
    return *m_uwbApp;
}

CLI::App&
NearObjectCli::GetRangeApp() noexcept
{
    return *m_rangeApp;
}

CLI::App&
NearObjectCli::GetRangeStartApp() noexcept
{
    return *m_rangeStartApp;
}

CLI::App&
NearObjectCli::GetRangeStopApp() noexcept
{
    return *m_rangeStopApp;
}

namespace detail
{
/**
 * @brief Helper function to populate the string-enum mapping
 *
 * @tparam EnumType
 * @return std::map<std::string, EnumType>
 */
template <typename EnumType>
std::unordered_map<std::string, EnumType>
populate_map()
{
    const auto reverseMap = magic_enum::enum_entries<EnumType>();
    std::vector<std::pair<std::string, EnumType>> destVector{ reverseMap.size() };
    std::transform(std::cbegin(reverseMap),
        std::cend(reverseMap),
        std::begin(destVector),
        [](const auto& input) {
            return std::pair{ std::string{ input.second }, input.first };
        });
    return { std::cbegin(destVector), std::cend(destVector) };
}
} // namespace detail

std::unique_ptr<CLI::App>
NearObjectCli::CreateParser() noexcept
{
    // generate the maps
    {
        m_cliData->DeviceRoleMap = detail::populate_map<uwb::protocol::fira::DeviceRole>();
        m_cliData->RangingMethodMap = detail::populate_map<uwb::protocol::fira::RangingMethod>();
        m_cliData->MeasurementReportModeMap = detail::populate_map<uwb::protocol::fira::MeasurementReportMode>();
        m_cliData->StsConfigurationMap = detail::populate_map<uwb::protocol::fira::StsConfiguration>();
        m_cliData->MultiNodeModeMap = detail::populate_map<uwb::protocol::fira::MultiNodeMode>();
        m_cliData->RangingModeMap = detail::populate_map<uwb::protocol::fira::RangingMode>();
        m_cliData->SchedulingModeMap = detail::populate_map<uwb::protocol::fira::SchedulingMode>();
        m_cliData->ChannelMap = detail::populate_map<uwb::protocol::fira::Channel>();
        m_cliData->StsPacketConfigurationMap = detail::populate_map<uwb::protocol::fira::StsPacketConfiguration>();
        m_cliData->ConvolutionalCodeConstraintLengthMap = detail::populate_map<uwb::protocol::fira::ConvolutionalCodeConstraintLength>();
        m_cliData->PrfModeMap = detail::populate_map<uwb::protocol::fira::PrfMode>();
        m_cliData->UwbMacAddressFcsTypeMap = detail::populate_map<uwb::UwbMacAddressFcsType>();
        m_cliData->ResultReportConfigurationMap = detail::populate_map<uwb::protocol::fira::ResultReportConfiguration>();
    }

    auto app = std::make_unique<CLI::App>();
    app->name("nocli");
    app->description("A command line tool to assist with all things nearobject");

    app->require_subcommand();
    m_uwbApp = app->add_subcommand("uwb", "commands related to uwb")->require_subcommand()->fallthrough();
    m_rangeApp = m_uwbApp->add_subcommand("range", "commands related to ranging")->require_subcommand()->fallthrough();
    m_rangeStartApp = m_rangeApp->add_subcommand("start", "start ranging. Please refer to Table 53 of the FiRa CSML spec for more info on the options")->fallthrough();

    // TODO is there a way to put all the enums into a list of [optionName, optionDestination, optionMap] so we don't have to create the initializer list each time
    // TODO get rid of these strings, instead use a macro to extract the enum name
    m_rangeStartApp->add_option("--DeviceRole", m_cliData->SessionData.UwbConfiguration.DeviceRole)->transform(CLI::CheckedTransformer(m_cliData->DeviceRoleMap))->capture_default_str();
    m_rangeStartApp->add_option("--RangingMethod", m_cliData->SessionData.UwbConfiguration.RangingConfiguration.Method)->transform(CLI::CheckedTransformer(m_cliData->RangingMethodMap))->capture_default_str();
    m_rangeStartApp->add_option("--MeasurementReportMode", m_cliData->SessionData.UwbConfiguration.RangingConfiguration.ReportMode)->transform(CLI::CheckedTransformer(m_cliData->MeasurementReportModeMap))->capture_default_str();
    m_rangeStartApp->add_option("--StsConfiguration", m_cliData->SessionData.UwbConfiguration.StsConfiguration)->transform(CLI::CheckedTransformer(m_cliData->StsConfigurationMap))->capture_default_str();
    m_rangeStartApp->add_option("--MultiNodeMode", m_cliData->SessionData.UwbConfiguration.MultiNodeMode)->transform(CLI::CheckedTransformer(m_cliData->MultiNodeModeMap))->capture_default_str();
    m_rangeStartApp->add_option("--RangingMode", m_cliData->SessionData.UwbConfiguration.RangingTimeStruct)->transform(CLI::CheckedTransformer(m_cliData->RangingModeMap))->capture_default_str();
    m_rangeStartApp->add_option("--SchedulingMode", m_cliData->SessionData.UwbConfiguration.SchedulingMode)->transform(CLI::CheckedTransformer(m_cliData->SchedulingModeMap))->capture_default_str();
    m_rangeStartApp->add_option("--Channel", m_cliData->SessionData.UwbConfiguration.Channel)->transform(CLI::CheckedTransformer(m_cliData->ChannelMap))->capture_default_str();
    m_rangeStartApp->add_option("--StsPacketConfiguration", m_cliData->SessionData.UwbConfiguration.RFrameConfig)->transform(CLI::CheckedTransformer(m_cliData->StsPacketConfigurationMap))->capture_default_str();
    m_rangeStartApp->add_option("--ConvolutionalCodeConstraintLength", m_cliData->SessionData.UwbConfiguration.ConvolutionalCodeConstraintLength)->transform(CLI::CheckedTransformer(m_cliData->ConvolutionalCodeConstraintLengthMap))->capture_default_str();
    m_rangeStartApp->add_option("--PrfMode", m_cliData->SessionData.UwbConfiguration.PrfMode)->transform(CLI::CheckedTransformer(m_cliData->PrfModeMap))->capture_default_str();
    m_rangeStartApp->add_option("--UwbMacAddressFcsType", m_cliData->SessionData.UwbConfiguration.MacAddressFcsType)->transform(CLI::CheckedTransformer(m_cliData->UwbMacAddressFcsTypeMap))->capture_default_str();

    // booleans
    m_rangeStartApp->add_flag("--controller,!--controlee", m_cliData->HostIsController, "default is controlee")->capture_default_str();
    m_rangeStartApp->add_flag("--HoppingMode", m_cliData->SessionData.UwbConfiguration.HoppingMode)->capture_default_str();
    m_rangeStartApp->add_flag("--BlockStriding", m_cliData->SessionData.UwbConfiguration.BlockStriding)->capture_default_str();

    // TODO check for int sizes when parsing input
    m_rangeStartApp->add_option("--UwbInitiationTime", m_cliData->SessionData.UwbConfiguration.UwbInitiationTime, "uint32_t")->capture_default_str();
    m_rangeStartApp->add_option("--Sp0PhySetNumber", m_cliData->SessionData.UwbConfiguration.Sp0PhySetNumber, "uint8_t")->capture_default_str();
    m_rangeStartApp->add_option("--Sp1PhySetNumber", m_cliData->SessionData.UwbConfiguration.Sp1PhySetNumber, "uint8_t")->capture_default_str();
    m_rangeStartApp->add_option("--Sp3PhySetNumber", m_cliData->SessionData.UwbConfiguration.Sp3PhySetNumber, "uint8_t")->capture_default_str();
    m_rangeStartApp->add_option("--PreableCodeIndex", m_cliData->SessionData.UwbConfiguration.PreableCodeIndex, "uint8_t")->capture_default_str();
    m_rangeStartApp->add_option("--SlotsPerRangingRound", m_cliData->SessionData.UwbConfiguration.SlotsPerRangingRound, "uint8_t")->capture_default_str();
    m_rangeStartApp->add_option("--MaxContentionPhaseLength", m_cliData->SessionData.UwbConfiguration.MaxContentionPhaseLength, "uint8_t")->capture_default_str();
    m_rangeStartApp->add_option("--SlotDuration", m_cliData->SessionData.UwbConfiguration.SlotDuration, "uint8_t")->capture_default_str();
    m_rangeStartApp->add_option("--RangingInterval", m_cliData->SessionData.UwbConfiguration.RangingInterval, "uint16_t")->capture_default_str();
    m_rangeStartApp->add_option("--KeyRotationRate", m_cliData->SessionData.UwbConfiguration.KeyRotationRate, "uint8_t")->capture_default_str();
    m_rangeStartApp->add_option("--MaxRangingRoundRetry", m_cliData->SessionData.UwbConfiguration.MaxRangingRoundRetry, "uint16_t")->capture_default_str();

    m_rangeApp->add_option("--SessionDataVersion", m_cliData->SessionData.SessionDataVersion)->capture_default_str();
    m_rangeApp->add_option("--SessionId", m_cliData->SessionData.SessionId)->capture_default_str();
    m_rangeApp->add_option("--SubSessionId", m_cliData->SessionData.SubSessionId)->capture_default_str();

    // strings
    m_rangeStartApp->add_option("--FiraPhyVersion", m_cliData->PhyVersionString)->capture_default_str();
    m_rangeStartApp->add_option("--FiraMacVersion", m_cliData->MacVersionString)->capture_default_str();
    m_rangeStartApp->add_option("--ResultReportConfiguration", m_cliData->ResultReportConfigurationString)->capture_default_str();

    m_rangeStartApp->parse_complete_callback([&] {
        std::cout << "Selected parameters:" << std::endl;

        for (const auto& [optionName, optionSelected] :
            std::initializer_list<std::tuple<std::string_view, std::string_view>>{
                { magic_enum::enum_type_name<uwb::protocol::fira::DeviceRole>(), magic_enum::enum_name(m_cliData->SessionData.UwbConfiguration.DeviceRole) },
                { magic_enum::enum_type_name<uwb::protocol::fira::RangingMethod>(), magic_enum::enum_name(m_cliData->SessionData.UwbConfiguration.RangingConfiguration.Method) },
                { magic_enum::enum_type_name<uwb::protocol::fira::MeasurementReportMode>(), magic_enum::enum_name(m_cliData->SessionData.UwbConfiguration.RangingConfiguration.ReportMode) },
                { magic_enum::enum_type_name<uwb::protocol::fira::StsConfiguration>(), magic_enum::enum_name(m_cliData->SessionData.UwbConfiguration.StsConfiguration) },
                { magic_enum::enum_type_name<uwb::protocol::fira::MultiNodeMode>(), magic_enum::enum_name(m_cliData->SessionData.UwbConfiguration.MultiNodeMode) },
                { magic_enum::enum_type_name<uwb::protocol::fira::RangingMode>(), magic_enum::enum_name(m_cliData->SessionData.UwbConfiguration.RangingTimeStruct) },
                { magic_enum::enum_type_name<uwb::protocol::fira::Channel>(), magic_enum::enum_name(m_cliData->SessionData.UwbConfiguration.Channel) },
                { magic_enum::enum_type_name<uwb::protocol::fira::StsPacketConfiguration>(), magic_enum::enum_name(m_cliData->SessionData.UwbConfiguration.RFrameConfig) },
                { magic_enum::enum_type_name<uwb::protocol::fira::ConvolutionalCodeConstraintLength>(), magic_enum::enum_name(m_cliData->SessionData.UwbConfiguration.ConvolutionalCodeConstraintLength) },
                { magic_enum::enum_type_name<uwb::protocol::fira::PrfMode>(), magic_enum::enum_name(m_cliData->SessionData.UwbConfiguration.PrfMode) },
                { magic_enum::enum_type_name<uwb::UwbMacAddressFcsType>(), magic_enum::enum_name(m_cliData->SessionData.UwbConfiguration.MacAddressFcsType) } }) {
            std::cout << optionName << "::" << optionSelected << std::endl;
        }
        if (m_cliData->MacVersionString.size()) {
            auto result = uwb::protocol::fira::StringToVersion(m_cliData->MacVersionString);
            if (not result) {
                std::cout << "could not parse MacVersionString" << std::endl;
            } else {
                m_cliData->SessionData.UwbConfiguration.FiraMacVersion = result.value();
            }
        }
        if (m_cliData->PhyVersionString.size()) {
            auto result = uwb::protocol::fira::StringToVersion(m_cliData->PhyVersionString);
            if (not result) {
                std::cout << "could not parse PhyVersionString" << std::endl;
            } else {
                m_cliData->SessionData.UwbConfiguration.FiraPhyVersion = result.value();
            }
        }

        if (m_cliData->ResultReportConfigurationString.size()) {
            auto result = uwb::protocol::fira::StringToResultReportConfiguration(m_cliData->ResultReportConfigurationString, m_cliData->ResultReportConfigurationMap);
            if (not result) {
                std::cout << "could not parse ResultReportConfiguration" << std::endl;
            } else {
                m_cliData->SessionData.UwbConfiguration.ResultReportConfigurations = result.value();
            }
        }

        std::cout << "FiRa MAC Version: " << std::setfill('0') << std::showbase << std::setw(8) << std::left << std::hex << m_cliData->SessionData.UwbConfiguration.FiraMacVersion << std::endl;
        std::cout << "FiRa PHY Version: " << std::setfill('0') << std::showbase << std::setw(8) << std::left << std::hex << m_cliData->SessionData.UwbConfiguration.FiraPhyVersion << std::endl;
        std::cout << "ResultReportConfigurations: " << uwb::protocol::fira::ResultReportConfigurationToString(m_cliData->SessionData.UwbConfiguration.ResultReportConfigurations) << std::endl;
    });

    m_rangeStopApp = m_rangeApp->add_subcommand("stop", "stop ranging")->fallthrough();
    m_rangeStopApp->parse_complete_callback([&] {
        std::cout << "stop ranging" << std::endl;
    });

    return app;
}
