
#ifndef UWB_SIMULATOR_DDI_CALLBACKS_LRP
#define UWB_SIMULATOR_DDI_CALLBACKS_LRP

#include <cstdint>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "UwbCore.hxx"

namespace windows::devices::uwb::simulator
{
/**
 * @brief 
 */
struct UwbSimulatorDdiCallbacksLrp
{
    /**
     * @brief 
     * 
     * @return UwbStatus 
     */
    virtual UwbStatus
    DeviceReset() = 0;

    /**
     * @brief 
     * 
     * @param deviceInfo 
     * @return UwbStatus 
     */
    virtual UwbStatus
    DeviceGetInfo(UwbDeviceInfo& deviceInfo) = 0;

    /**
     * @brief 
     * 
     * @param deviceCapabilities 
     * @return UwbStatus 
     */
    virtual UwbStatus
    DeviceGetCapabilities(UwbDeviceCapabilities &deviceCapabilities) = 0;

    /**
     * @brief 
     * 
     * @param deviceConfigurationParameterTypes 
     * @param deviceConfigurationParameterResults 
     * @return UwbStatus 
     */
    virtual UwbStatus
    DeviceGetConfigurationParameters(std::vector<UwbDeviceConfigurationParameterType> &deviceConfigurationParameterTypes, std::vector<std::tuple<UwbDeviceConfigurationParameterType, UwbStatus, std::optional<UwbDeviceConfigurationParameter>>> &deviceConfigurationParameterResults) = 0;

    /**
     * @brief 
     * 
     * @param deviceConfigurationParameters 
     * @param deviceConfigurationParameterResults 
     * @return UwbStatus 
     * 
     * TODO: possibly separate the results into a vector with ones that had UwbStatus::Ok, and a separate vector with the ones that weren't retrieved.
     */
    virtual UwbStatus
    DeviceSetConfigurationParameters(const std::vector<UwbDeviceConfigurationParameter> &deviceConfigurationParameters, std::vector<std::tuple<UwbDeviceConfigurationParameterType, UwbStatus>>& deviceConfigurationParameterResults) = 0;

    /**
     * @brief Set the Application Configuration Parameters object
     * 
     * @param applicationConfigurationParameters 
     * @param applicationConfigurationParameterResults 
     * @return UwbStatus 
     */
    virtual UwbStatus
    SetApplicationConfigurationParameters(const std::vector<UwbApplicationConfigurationParameter> &applicationConfigurationParameters, std::vector<std::tuple<UwbApplicationConfigurationParameterType, UwbStatus, std::optional<UwbApplicationConfigurationParameter>>> &applicationConfigurationParameterResults) = 0;

    /**
     * @brief Get the Application Configuration Parameters object
     * 
     * @param applicationConfigurationParameters 
     * @return UwbStatus 
     */
    virtual UwbStatus
    GetApplicationConfigurationParameters(std::vector<UwbApplicationConfigurationParameter> &applicationConfigurationParameters) = 0;

    /**
     * @brief Get the Session Count object
     * 
     * @param sessionCount 
     * @return UwbStatus 
     */
    virtual UwbStatus
    GetSessionCount(uint32_t *sessionCount) = 0;

    /**
     * @brief 
     * 
     * @param sessionId 
     * @param sessionType 
     * @return UwbStatus 
     */
    virtual UwbStatus
    SessionInitialize(uint32_t sessionId, UwbSessionType sessionType) = 0;

    /**
     * @brief 
     * 
     * @param sessionId 
     * @return UwbStatus 
     */
    virtual UwbStatus
    SessionDeninitialize(uint32_t sessionId) = 0;

    /**
     * @brief 
     * 
     * @param sessionId 
     * @param sessionState 
     * @return UwbStatus 
     */
    virtual UwbStatus
    SessionGetState(uint32_t sessionId, UwbSessionState *sessionState) = 0;

    /**
     * @brief 
     * 
     * @param controlees 
     * @return UwbStatus 
     */
    virtual UwbStatus
    SessionUpdateControllerMulticastList(const std::vector<UwbMacAddress>& controlees) = 0;

    /**
     * @brief 
     * 
     * @param sessionId 
     * @return UwbStatus 
     */
    virtual UwbStatus
    SessionStartRanging(uint32_t sessionId) = 0;

    /**
     * @brief 
     * 
     * @param sessionId 
     * @return UwbStatus 
     */
    virtual UwbStatus
    SessionStopRanging(uint32_t sessionId) = 0;
    
    /**
     * @brief 
     * 
     * @param sessionId 
     * @param rangingCount 
     * @return UwbStatus 
     */
    virtual UwbStatus
    SessionGetRangingCount(uint32_t sessionId, uint32_t *rangingCount) = 0;
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_DDI_CALLBACKS_LRP
