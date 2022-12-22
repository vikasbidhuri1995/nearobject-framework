/**
 * @file Device.cxx
 * @brief This file contains the device entry points and callbacks.
 *
 * @copyright Copyright (c) 2022
 */

#include "device.tmh"
#include "UwbSimulatorDevice.hxx"
#include "UwbSimulatorDeviceFileObject.hxx"
#include "driver.hxx"

NTSTATUS
UwbSimulatorCreateDevice(WDFDEVICE_INIT *deviceInit)
/*++

Routine Description:

    Worker routine called to create a device and its software resources.

Arguments:

    DeviceInit - Pointer to an opaque init structure. Memory for this
                    structure will be freed by the framework when the WdfDeviceCreate
                    succeeds. So don't access the structure after that point.

Return Value:

    NTSTATUS

--*/
{
    WDF_FILEOBJECT_CONFIG fileConfiguration;
    WDF_FILEOBJECT_CONFIG_INIT(&fileConfiguration, &UwbSimulatorDevice::OnFileCreate, &UwbSimulatorDevice::OnFileClose, WDF_NO_EVENT_CALLBACK);

    WDF_OBJECT_ATTRIBUTES fileAttributes;
    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&fileAttributes, UwbSimulatorDeviceFileObject);

    WDF_OBJECT_ATTRIBUTES deviceAttributes;
    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, DEVICE_CONTEXT);

    WDFDEVICE device;
    NTSTATUS status = WdfDeviceCreate(&deviceInit, &deviceAttributes, &device);
    if (!NT_SUCCESS(status)) {
        // TODO: log
        return status;
    }

    //
    // Get a pointer to the device context structure that we just associated
    // with the device object. We define this structure in the device.h
    // header file. DeviceGetContext is an inline function generated by
    // using the WDF_DECLARE_CONTEXT_TYPE_WITH_NAME macro in device.h.
    // This function will do the type checking and return the device context.
    // If you pass a wrong object handle it will return NULL and assert if
    // run under framework verifier mode.
    //
    DEVICE_CONTEXT *deviceContext = DeviceGetContext(device);

    //
    // Initialize the context.
    //
    deviceContext->PrivateDeviceData = 0;

    //
    // Create a device interface so that applications can find and talk
    // to us.
    //
    status = WdfDeviceCreateDeviceInterface(device, &GUID_DEVINTERFACE_UwbSimulator, nullptr);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    //
    // Initialize the I/O Package and any Queues
    //
    status = UwbSimulatorQueueInitialize(device);

    return status;
}
