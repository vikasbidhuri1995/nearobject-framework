
#include <UwbCxLrpDeviceGlue.h>
#include <wil/result.h>

#include <algorithm>
#include <memory>
#include <magic_enum.hpp>

#include <windows/uwb/UwbSession.hxx>

using namespace windows::devices;

UwbSession::UwbSession(std::weak_ptr<uwb::UwbSessionEventCallbacks> callbacks, wil::unique_hfile handleDriver) :
    uwb::UwbSession(std::move(callbacks)),
    m_handleDriver(std::move(handleDriver))
{}

template <class T>
std::unique_ptr<UWB_APP_CONFIG_PARAM>
GetUWB_APP_CONFIG_PARAM(const T& value){
    auto pdata = std::make_unique<uint8_t>(FIELD_OFFSET(UWB_APP_CONFIG_PARAM,paramValue[sizeof T]));

}

template <class T>
UWB_SET_APP_CONFIG_PARAMS
GetUWB_SET_APP_CONFIG_PARAMS(){

}

/**
 * @brief Maps the param to the length of the value. If the value is 0, means a variable length and special code to handle
 * Taken from the FiRa UCI spec Table 29:APP Configuration IDs
 * 
 */
std::unordered_map<UWB_APP_CONFIG_PARAM_TYPE,unsigned int> appConfigParamLengths {
    {UWB_APP_CONFIG_PARAM_TYPE_DEVICE_TYPE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_RANGING_ROUND_USAGE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_STS_CONFIG,1},
    {UWB_APP_CONFIG_PARAM_TYPE_MULTI_NODE_MODE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_CHANNEL_NUMBER,1},
    {UWB_APP_CONFIG_PARAM_TYPE_NUMBER_OF_CONTROLEES,1},
    {UWB_APP_CONFIG_PARAM_TYPE_DEVICE_MAC_ADDRESS,0},
    {UWB_APP_CONFIG_PARAM_TYPE_DST_MAC_ADDRESS,0},
    {UWB_APP_CONFIG_PARAM_TYPE_SLOT_DURATION,2},
    {UWB_APP_CONFIG_PARAM_TYPE_RANGING_INTERVAL,4},
    {UWB_APP_CONFIG_PARAM_TYPE_STS_INDEX,4},
    {UWB_APP_CONFIG_PARAM_TYPE_MAC_FCS_TYPE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_RANGING_ROUND_CONTROL,1},
    {UWB_APP_CONFIG_PARAM_TYPE_AOA_RESULT_REQ,1},
    {UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_CONFIG,1},
    {UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_PROXIMITY_NEAR,2},
    {UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_PROXIMITY_FAR,2},
    {UWB_APP_CONFIG_PARAM_TYPE_DEVICE_ROLE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_RFRAME_CONFIG,1},
    {UWB_APP_CONFIG_PARAM_TYPE_PREAMBLE_CODE_INDEX,1},
    {UWB_APP_CONFIG_PARAM_TYPE_SFD_ID,1},
    {UWB_APP_CONFIG_PARAM_TYPE_PSDU_DATA_RATE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_PREAMBLE_DURATION,1},
    {UWB_APP_CONFIG_PARAM_TYPE_RANGING_TIME_STRUCT,1},
    {UWB_APP_CONFIG_PARAM_TYPE_SLOTS_PER_RR,1},
    {UWB_APP_CONFIG_PARAM_TYPE_TX_ADAPTIVE_PAYLOAD_POWER,1},
    {UWB_APP_CONFIG_PARAM_TYPE_RESPONDER_SLOT_INDEX,1},
    {UWB_APP_CONFIG_PARAM_TYPE_PRF_MODE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_SCHEDULED_MODE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_KEY_ROTATION,1},
    {UWB_APP_CONFIG_PARAM_TYPE_KEY_ROTATION_RATE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_SESSION_PRIORITY,1},
    {UWB_APP_CONFIG_PARAM_TYPE_MAC_ADDRESS_MODE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_VENDOR_ID,2},
    {UWB_APP_CONFIG_PARAM_TYPE_STATIC_STS_IV,6},
    {UWB_APP_CONFIG_PARAM_TYPE_NUMBER_OF_STS_SEGMENTS,1},
    {UWB_APP_CONFIG_PARAM_TYPE_MAX_RR_RETRY,2},
    {UWB_APP_CONFIG_PARAM_TYPE_UWB_INITIATION_TIME,4},
    {UWB_APP_CONFIG_PARAM_TYPE_HOPPING_MODE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_BLOCK_STRIDE_LENGTH,1},
    {UWB_APP_CONFIG_PARAM_TYPE_RESULT_REPORT_CONFIG,1},
    {UWB_APP_CONFIG_PARAM_TYPE_IN_BAND_TERMINATION_ATTEMPT_COUNT,1},
    {UWB_APP_CONFIG_PARAM_TYPE_SUB_SESSION_ID,4},
    {UWB_APP_CONFIG_PARAM_TYPE_BPRF_PHR_DATA_RATE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_MAX_NUMBER_OF_MEASUREMENTS,2},
    {UWB_APP_CONFIG_PARAM_TYPE_STS_LENGTH,1}
};

void
UwbSession::ConfigureImpl(const uwb::protocol::fira::UwbSessionData& uwbSessionData)
{
    // TODO: collect options set from uwbConfiguration, translate them to
    // UWB_APP_CONFIG_PARAM, and send them to the driver.
    
    // Populate the session initialization command argument.
    UWB_SESSION_INIT sessionInit;
    sessionInit.sessionId = uwbSessionData.sessionId;
    sessionInit.sessionType = UWB_SESSION_TYPE_RANGING_SESSION;

    // Request a new session from the driver.
    HRESULT hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_SESSION_INIT, &sessionInit, sizeof sessionInit, nullptr, 0, nullptr, nullptr);
    if (FAILED(hr)) {
        // TODO: handle this
    }

    m_sessionId = uwbSessionData.sessionId;

    // Populate the PUWB_SET_APP_CONFIG_PARAMS
    std::vector<uint8_t> dataHolder;
    dataHolder.reserve(FIELD_OFFSET(UWB_SET_APP_CONFIG_PARAMS,appConfigParams[magic_enum::enum_count<UWB_APP_CONFIG_PARAM_TYPE>()])); // TODO minimum number of bytes needed

    


    auto tmpHolder = std::make_shared<uint8_t>(dataHolder.size());
    memcpy(tmpHolder.get(),dataHolder.data(),dataHolder.size());
    auto paramHolder = std::reinterpret_pointer_cast<UWB_SET_APP_CONFIG_PARAMS,uint8_t>(tmpHolder);

    // Allocate memory for the PUWB_SET_APP_CONFIG_PARAMS_STATUS
    auto statusSize = dataHolder.size(); // TODO actually calculate the size
    tmpHolder = std::make_shared<uint8_t>(statusSize); 
    auto statusHolder = std::reinterpret_pointer_cast<UWB_SET_APP_CONFIG_PARAMS_STATUS,uint8_t>(tmpHolder);

    hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_SET_APP_CONFIG_PARAMS, paramHolder.get(), dataHolder.size(), statusHolder.get(), statusSize, nullptr, nullptr);
    if (FAILED(hr)) {
        // TODO: handle this
    }
}

void
UwbSession::StartRangingImpl()
{
    UWB_START_RANGING_SESSION startRangingSession;
    startRangingSession.sessionId = GetId();
    HRESULT hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_STOP_RANGING_SESSION, &startRangingSession, sizeof startRangingSession, nullptr, 0, nullptr, nullptr);
    if (FAILED(hr)) {
        // TODO
    }
}

void
UwbSession::StopRangingImpl()
{
    UWB_STOP_RANGING_SESSION stopRangingSession;
    stopRangingSession.sessionId = GetId();
    HRESULT hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_STOP_RANGING_SESSION, &stopRangingSession, sizeof stopRangingSession, nullptr, 0, nullptr, nullptr);
    if (FAILED(hr)) {
        // TODO
    }
}

void
UwbSession::AddPeerImpl(uwb::UwbMacAddress /* peerMacAddress */)
{
    // TODO: two main options for updating the UWB-CLX peer list:
    //  1) Every time a peer is added (on-demand)
    //  2) Only when StartRanging() is called.
    // Below sample code exemplifies 1) for simplicity but this is not necessarily the way to go.
    //
    // TODO: request UWB-CLX to update controlee list per below pseudo-code,
    // which is *very* rough and some parts probably plain wrong:
    //
    const auto macAddressLength = m_uwbMacAddressSelf.GetLength();
    const auto macAddressessLength = macAddressLength * m_peers.size();

    std::size_t appConfigParamsSize = 0;
    appConfigParamsSize += macAddressessLength;
    // TODO: all other memory required for this structure must be accounted for, the above calculation was left incomplete.
    // Also, proper memory alignment of trailing structures in the allocated buffer has not been taken into account.
    auto appConfigParamsBuffer = std::make_unique<uint8_t[]>(appConfigParamsSize);
    auto *appConfigParams = reinterpret_cast<UWB_SET_APP_CONFIG_PARAMS *>(appConfigParamsBuffer.get());
    appConfigParams->sessionId = GetId();
    appConfigParams->appConfigParamsCount = 2;
    UWB_APP_CONFIG_PARAM *appConfigParamList = reinterpret_cast<UWB_APP_CONFIG_PARAM *>(appConfigParams + 1);
    UWB_APP_CONFIG_PARAM *appConfigParamNumberOfControlees = appConfigParamList + 0;
    UWB_APP_CONFIG_PARAM *appConfigParamDstMacAddress = appConfigParamList + 1;

    // Populate NUMBER_OF_CONTROLEES app configuration parameter.
    auto &numberOfControleesPayload = *reinterpret_cast<uint8_t *>(appConfigParamNumberOfControlees + 1);
    appConfigParamNumberOfControlees->paramType = UWB_APP_CONFIG_PARAM_TYPE_NUMBER_OF_CONTROLEES;
    appConfigParamNumberOfControlees->paramLength = 1;
    numberOfControleesPayload = static_cast<uint8_t>(m_peers.size());

    // Populate DST_MAC_ADDRESS app configuration parameter.
    auto dstMacAddressPayload = reinterpret_cast<uint8_t *>(appConfigParamDstMacAddress + 1);
    appConfigParamDstMacAddress->paramType = UWB_APP_CONFIG_PARAM_TYPE_DST_MAC_ADDRESS;
    appConfigParamDstMacAddress->paramLength = static_cast<uint32_t>(macAddressessLength);
    auto dstMacAddress = dstMacAddressPayload;
    for (const auto &peer : m_peers) {
        const auto value = peer.GetValue();
        std::copy(std::cbegin(value), std::cend(value), dstMacAddress);
        std::advance(dstMacAddress, std::size(value));
    }

    // Attempt to set all new parameters.
    DWORD bytesReturned = 0;
    UWB_SET_APP_CONFIG_PARAMS_STATUS appConfigParamsStatus; // TODO: this needs to be dynamically allocated to fit returned content
    HRESULT hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_SET_APP_CONFIG_PARAMS, &appConfigParams, static_cast<DWORD>(appConfigParamsSize), &appConfigParamsStatus, sizeof appConfigParamsStatus, &bytesReturned, nullptr);
    if (FAILED(hr)) {
        // TODO
    }
}
