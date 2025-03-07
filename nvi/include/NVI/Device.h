/**
 * @file NVI/Device.h
 * @brief Device control/handle API defined.
 * @version 1.0.1
 * @date 2023-07-24
 * @copyright Copyright (c) 2023 Sobey Digital Inc.
 *
 */

#ifndef NVI_DEVICE_H
#define NVI_DEVICE_H

#include <NVI/Import.h>
#include <NVI/Meta.h>
#include <NVI/Send.h>
#include <NVI/Recv.h>

/************************************************************************/
/* NVI Device Handler Section                                           */
/************************************************************************/

/**
 * @brief The device event handler object define.
 *
 * Alloc by `NVIDeviceHandlerAlloc`.
 * Free by `NVIDeviceHandlerFree`.
 *
 */
typedef void* NVI_DEVICE_HANDLER;

/**
 * @brief Param of device handler.
 */
typedef struct _NVIDeviceHandlerAllocParam
{
    NVI_SENDER sender;            ///< From `NVISendAlloc`, maybe nullptr for device, or not nullptr for stream.
    uint32_t caps_ptz0 : 1;       ///< Catch ptz0 message.
    uint32_t caps_ptz1 : 1;       ///< Catch ptz1 message.
    uint32_t caps_reserved : 30;  ///< Reserved for future used.
    uint32_t reserved;            ///< Reserved for future used.
} NVIDeviceHandlerAllocParam;

/**
 * @brief Alloc device handler.
 *
 * @param [in] context From `NVIContextCreate`, not be nullptr.
 * @param [in] param Param of device handler, maybe nullptr when all event catch.
 * @return NVI_DEVICE_HANDLER nullptr, `NVILastError` get error code.
 */
NVI_API NVI_DEVICE_HANDLER NVIDeviceHandlerAlloc(NVI_CONTEXT context, NVIDeviceHandlerAllocParam* param);

/**
 * @brief Close handler and free object.
 * @param [in] handler From `NVIDeviceHandlerAlloc`, not be nullptr.
 */
NVI_API void NVIDeviceHandlerFree(NVI_DEVICE_HANDLER handler);

/**
 * @brief The device event defined
 */
typedef enum _NVIDeviceEventId
{
    NVIDeviceEvent_None = 0,           ///< Event NONE, not used.
    NVIDeviceEvent_PTZ = 1,            ///< The PTZ control event.
    NVIDeviceEvent_Tally = 2,          ///< The tally control event.
    NVIDeviceEvent_UserDefined = 128,  ///< The user defined meta data event.
} NVIDeviceEventId;

/**
 * @brief Param of watch handler event.
 */
typedef struct _NVIDeviceHandlerEventParam
{
    int32_t timeout_ms;    ///< Max wait event time in once call, in millisecond.
    int32_t flags;         ///< Some flag in once call handler.
    uint32_t source;       ///< The event source instance id, set by 'NVIDeviceHandlerEvent'.
    uint32_t reserved[1];  ///< Reserved for future use, must be 0.
} NVIDeviceHandlerEventParam;

/**
 * @brief Wait next device control event.
 *
 * @attention Only one thread call of each NVI_DEVICE_HANDLER object.
 * @param [in] handler From `NVIDeviceHandlerAlloc`, not be nullptr.
 * @param [in][out] param The `NVIDeviceHandlerEventParam` pointer, maybe nullptr for check next event and return immediately.
 * @return int32_t <0, error code, =0, no events, > 1, the event id.
 */
NVI_API int32_t NVIDeviceHandlerEvent(NVI_DEVICE_HANDLER handler, NVIDeviceHandlerEventParam* param);

/**
 * @brief The PTZ version 0 data structure.
 *
 * The `bytes` field is xml string with UTF-8.
 * The `size` field is string length in bytes.
 */
typedef NVIDataBuffer NVIDataPTZ0;

/**
 * @brief The PTZ version 1 data structure.
 *
 * The `bytes` field is json string with UTF-8.
 * The `size` field is string length in bytes.
 */
typedef NVIDataBuffer NVIDataPTZ1;

/**
 * @brief Get next PTZ version 0 message.
 *
 * The PTZ0 data buffer free in next call `NVIDeviceHandlerEvent` or `NVIDeviceHandlerPTZ`.
 *
 * @attention Call with the same thread of NVIDeviceHandlerEvent.
 * @param [in] handler From `NVIDeviceHandlerAlloc`, not be nullptr.
 * @param [in][out] ptz0 `NVIDataPTZ0` pointer, not be nullptr.
 * @return <0, error code.
 */
NVI_DEPRECATED_API int32_t NVIDeviceHandlerPTZ0(NVI_DEVICE_HANDLER handler, NVIDataPTZ0* ptz0);

/**
 * @brief Get next PTZ version 1 message.
 *
 * The PTZ1 data buffer free in next call `NVIDeviceHandlerEvent` or `NVIDeviceHandlerPTZ`.
 *
 * @attention Call with the same thread of NVIDeviceHandlerEvent.
 * @param [in] handler From `NVIDeviceHandlerAlloc`, not be nullptr.
 * @param [in][out] ptz1 `NVIDataPTZ1` pointer, not be nullptr.
 * @return <0, error code.
 */
NVI_API int32_t NVIDeviceHandlerPTZ1(NVI_DEVICE_HANDLER handler, NVIDataPTZ1* ptz1);

/**
 * @brief Get current tally status of device.
 *
 * @param [in] handler From `NVIDeviceHandlerAlloc`, not be nullptr.
 * @param [in][out] tally `NVITally` pointer, not be nullptr.
 * @return <0, error code.
 */
NVI_API int32_t NVIDeviceHandlerTally(NVI_DEVICE_HANDLER handler, NVITally* tally);

/**
 * @brief Get current tally display of device.
 *
 * @param [in] handler From `NVIDeviceHandlerAlloc`, not be nullptr.
 * @param [in][out] display `NVITallyDisplay` pointer, not be nullptr.
 * @return <0, error code.
 */
NVI_API int32_t NVIDeviceHandlerTallyDisplay(NVI_DEVICE_HANDLER handler, NVITallyDisplay* display);

/**
 * @brief Get next meta data message.
 *
 * When return `NVIDeviceEvent_UserDefined` with `NVIDeviceHandlerEvent` you can call this
 * function to get next meta data. And parse it with `NVIMetaParseNode`.
 *
 * @attention Call with the same thread of NVIDeviceHandlerEvent.
 * @param [in] handler From `NVIDeviceHandlerAlloc`, not be nullptr.
 * @param [in][out] data `NVIMetaData` pointer, not be nullptr.
 * @return <0, error code.
 */
NVI_API int32_t NVIDeviceHandlerMetaData(NVI_DEVICE_HANDLER handler, NVIMetaData* data);

/************************************************************************/
/* NVI Device Controller Section                                        */
/************************************************************************/
/**
 * @brief The device controller object define.
 *
 * Alloc by `NVIDeviceControllerAlloc`.
 * Free by `NVIDeviceControllerFree`.
 *
 */
typedef void* NVI_DEVICE_CONTROLLER;

/**
 * @brief Param of device controller.
 */
typedef struct _NVIDeviceControllerAllocParam
{
    NVI_RECVER recver;     ///< From NVIRecvAlloc, maybe nullptr when `instance` field is used.
    uint32_t instance;     ///< From NVINetworkStream::instance or NVINetworkSite::instance.
    uint32_t number;       ///< From NVINetworkStream::number, maybe 0.
    uint32_t reserved[4];  ///< Reserved for future use, must be 0.
} NVIDeviceControllerAllocParam;

/**
 * @brief Alloc device controller.
 *
 * @param [in] context From `NVIContextCreate`, not be nullptr.
 * @param [in] param Param of device handler, maybe nullptr when all event catch.
 * @return NVI_DEVICE_CONTROLLER nullptr, `NVILastError` get error code.
 */
NVI_API NVI_DEVICE_CONTROLLER NVIDeviceControllerAlloc(NVI_CONTEXT context, NVIDeviceControllerAllocParam* param);

/**
 * @brief Close controller and free object.
 * @param [in] controller From `NVIDeviceControllerAlloc`, not be nullptr.
 */
NVI_API void NVIDeviceControllerFree(NVI_DEVICE_CONTROLLER controller);

/**
 * @brief Set tally.
 * @param [in] controller Form `NVIDeviceControllerAlloc`.
 * @param [in] tally Tally status, not be nullptr.
 * @return int32_t <0, error code.
 */
NVI_API int32_t NVIDeviceControllerTally(NVI_DEVICE_CONTROLLER controller, const NVITally* tally);

/**
 * @brief Set tally display.
 * @param [in] controller Form `NVIDeviceControllerAlloc`.
 * @param [in] display Tally display info, not be nullptr.
 * @return int32_t <0, error code.
 */
NVI_API int32_t NVIDeviceControllerTallyDisplay(NVI_DEVICE_CONTROLLER controller, const NVITallyDisplay* display);

/**
 * @brief NVI PTZ-Zoom for camera device.
 *
 * @param [in] controller From `NVIDeviceControllerAlloc`, not be nullptr.
 * @param [in] zoom Value is 0.0f to 1.0f(min ~ max).
 * @return int32_t <0, error code.
 */
NVI_API int32_t NVIDeviceControllerPtzZoom(NVI_DEVICE_CONTROLLER controller, float zoom);

/**
 * @brief NVI PTZ-ZoomSpeed for camera device.
 *
 * Zoom at a particular speed.
 *
 * @param [in] controller From `NVIDeviceControllerAlloc`, not be nullptr.
 * @param [in] speed Value is -1.0 (zoom outwards) ... +1.0 (zoom inwards).
 * @return int32_t <0, error code.
 */
NVI_API int32_t NVIDeviceControllerPtzZoomSpeed(NVI_DEVICE_CONTROLLER controller, float speed);

/**
 * @brief NVI PTZ-PanTilt for camera device.
 *
 * Set the pan and tilt to an absolute value.
 *
 * @param [in] controller From `NVIDeviceControllerAlloc`, not be nullptr.
 * @param [in] pan  = -1.0 (left) ... 0.0 (centered) ... +1.0 (right).
 * @param [in] tilt = -1.0 (bottom) ... 0.0 (centered) ... +1.0 (top).
 * @return int32_t <0, error code.
 */
NVI_API int32_t NVIDeviceControllerPtzPanTilt(NVI_DEVICE_CONTROLLER controller, float pan, float tilt);

/**
 * @brief NVI PTZ-PanTiltSpeed for camera device.
 *
 * Set the pan and tilt direction and speed.
 *
 * @param [in] controller From `NVIDeviceControllerAlloc`, not be nullptr.
 * @param [in] pan Value is -1.0 (moving left) ... 0.0 (stopped) ... +1.0 (moving right).
 * @param [in] tilt Value is -1.0 (down) ... 0.0 (stopped) ... +1.0 (moving up).
 * @return int32_t <0, error code.
 */
NVI_API int32_t NVIDeviceControllerPtzPanTiltSpeed(NVI_DEVICE_CONTROLLER controller, float pan, float tilt);

/**
 * @brief NVI PTZ-StorePreset for camera device.
 *
 * Store the current position, focus, etc... as a preset.
 *
 * @param [in] controller From `NVIDeviceControllerAlloc`, not be nullptr.
 * @param [in] preset Value is 0 ... 255.
 * @return int32_t <0, error code.
 */
NVI_API int32_t NVIDeviceControllerPtzStorePreset(NVI_DEVICE_CONTROLLER controller, uint32_t preset);

/**
 * @brief NVI PTZ-RecallPreset for camera device.
 *
 * Recall a preset, including position, focus, etc...
 *
 * @param [in] controller From `NVIDeviceControllerAlloc`, not be nullptr.
 * @param [in] preset Value is 0 ... 255.
 * @param [in] speed Value is 0.0(as slow as possible) ... 1.0(as fast as possible) The speed at which to move to the new preset.
 * @return int32_t <0, error code.
 */
NVI_API int32_t NVIDeviceControllerPtzRecallPreset(NVI_DEVICE_CONTROLLER controller, uint32_t preset, float speed);

/**
 * @brief NVI PTZ-AutoFocus for camera device.
 *
 * Put the camera in auto focus.
 *
 * @param [in] controller From `NVIDeviceControllerAlloc`, not be nullptr.
 * @return int32_t <0, error code.
 */
NVI_API int32_t NVIDeviceControllerPtzAutoFocus(NVI_DEVICE_CONTROLLER controller);

/**
 * @brief NVI PTZ-Focus for camera device.
 *
 * Focus to an absolute value.
 *
 * @param [in] controller From `NVIDeviceControllerAlloc`, not be nullptr.
 * @param [in] focus Value is 0.0 (focused as close as possible) ... 1.0 (focused to infinity)
 * @return int32_t <0, error code.
 */
NVI_API int32_t NVIDeviceControllerPtzFocus(NVI_DEVICE_CONTROLLER controller, float focus);

/**
 * @brief NVI PTZ-FocusSpeed for camera device.
 *
 * Focus at a particular speed.
 *
 * @param [in] controller From `NVIDeviceControllerAlloc`, not be nullptr.
 * @param [in] speed Value is -1.0 (focus inwards) ... +1.0 (focus outwards).
 * @return int32_t <0, error code.
 */
NVI_API int32_t NVIDeviceControllerPtzFocusSpeed(NVI_DEVICE_CONTROLLER controller, float speed);

/**
 * @brief Defined white balance mode.
 */
typedef enum _NVIDevicePTZWhiteBalanceMode
{
    NVIDevicePTZWhiteBalance_Auto = 0,     ///< Auto mode.
    NVIDevicePTZWhiteBalance_Manual = 1,   ///< Manual mode.
    NVIDevicePTZWhiteBalance_Indoor = 2,   ///< Indoor mode.
    NVIDevicePTZWhiteBalance_Outdoor = 3,  ///< Outdoor mode.
    NVIDevicePTZWhiteBalance_OnePush = 4,  ///< Onepush mode.
} NVIDevicePTZWhiteBalanceMode;

/**
 * @brief Param of White balance.
 */
typedef struct _NVIDevicePTZWhiteBalanceParam
{
    uint32_t mode;         ///< See NVIDevicePTZWhiteBalanceMode
    float red;             ///< Only used with NVIDevicePTZWhiteBalance_Manual
    float blue;            ///< Only used with NVIDevicePTZWhiteBalance_Manual
    uint32_t reserved[5];  ///< Reserved for future used.
} NVIDevicePTZWhiteBalanceParam;

/**
 * @brief NVI PTZ-WhiteBalance for camera device.
 *
 * Put the camera in auto|indoor|outdoor|oneshot white balance mode.
 *
 * @param [in] controller From `NVIDeviceControllerAlloc`, not be nullptr.
 * @param [in] param `NVIDevicePTZWhiteBalanceParam` pointer, not be nullptr.
 * @return int32_t <0, error code.
 */
NVI_API int32_t NVIDeviceControllerPtzWhiteBalance(NVI_DEVICE_CONTROLLER controller, NVIDevicePTZWhiteBalanceParam* param);

/**
 * @brief Defined exposure mode.
 */
typedef enum _NVIDevicePTZExposureMode
{
    NVIDevicePTZExposure_Auto = 0,    ///< Auto mode.
    NVIDevicePTZExposure_Manual = 1,  ///< Manual mode.
} NVIDevicePTZExposureMode;

/**
 * @brief Param of exposure.
 */
typedef struct _NVIDevicePTZExposureParam
{
    uint32_t mode;         ///< See NVIDevicePTZExposureMode.
    float iris;            ///< Only used with NVIDevicePTZWhiteBalance_Manual, 0.0(dark) ... 1.0(light).
    float gain;            ///< Only used with NVIDevicePTZWhiteBalance_Manual, 0.0(dark) ... 1.0(light).
    float shutter;         ///< Shutter speed.
    uint32_t reserved[4];  ///< Reserved for future used.
} NVIDevicePTZExposureParam;

/**
 * @brief NVI PTZ-ExposureAuto for camera device.
 *
 * Put the camera exposure param.
 *
 * @param [in] controller From `NVIDeviceControllerAlloc`, not be nullptr.
 * @param [in] param `NVIDevicePTZExposureParam` pointer, not be nullptr.
 * @return int32_t <0, error code.
 */
NVI_API int32_t NVIDeviceControllerPtzExposure(NVI_DEVICE_CONTROLLER controller, NVIDevicePTZExposureParam* param);

/**
 * @brief Send meta data.
 *
 * @param controller controller From `NVIDeviceControllerAlloc`, not be nullptr.
 * @param data `NVIMetaData` pointer, not be nullptr.
 * @return int32_t <0, error code.
 */
NVI_API int32_t NVIDeviceControllerMetaData(NVI_DEVICE_CONTROLLER controller, NVIMetaData* data);

#endif  // NVI_DEVICE_H
