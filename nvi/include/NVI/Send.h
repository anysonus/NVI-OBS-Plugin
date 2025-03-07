/**
 * @file NVI/Send.h
 * @brief AV stream send API defined.
 * @version 1.0.1
 * @date 2023-05-05
 * @copyright Copyright (c) 2023 Sobey Digital Inc.
 *
 */

#ifndef NVI_SEND_H
#define NVI_SEND_H

#include <NVI/Import.h>
#include <NVI/Base.h>
#include <NVI/Network.h>
#include <NVI/Stream.h>
#include <NVI/Codec.h>
#include <NVI/Meta.h>

/**
 * @brief The NVI AV stream sender object define.
 *
 * Alloc by `NVISendAlloc` .
 * Free by `NVISendFree`.
 *
 */
typedef void* NVI_SENDER;

/**
 * @brief Param of AV stream.
 */
typedef struct _NVISendAllocParam
{
    const char* alias;                  ///< Human friendly name, UTF-8 encoding within 128 bytes.
    const char* local;                  ///< Specific local network interface, like IP, adapter name ...
    const char* remote;                 ///< Specific remote interface.
    const char* tags;                   ///< Tags for stream.
    uint32_t caps_ptz : 1;              ///< =1, support PTZ handler.
    uint32_t caps_proxy_video : 1;      ///< =1, support proxy video stream.
    uint32_t caps_encrypt_payload : 1;  ///< =1, support encrypt payload.
    uint32_t caps_reserved : 29;        ///< Reserved for future used, must be 0.
    uint32_t reserved[4];               ///< Reserved for future used, must be 0.
} NVISendAllocParam;

/**
 * @brief Alloc sender object.
 *
 * @param [in] context From `NVIContextCreate`, not be nullptr.
 * @param [in] param `NVISendAllocParam` pointer, maybe nullptr when default.
 * @return NVI_SENDER nullptr, `NVILastError` get error code.
 */
NVI_API NVI_SENDER NVISendAlloc(NVI_CONTEXT context, const NVISendAllocParam* param);

/**
 * @brief Close stream and free the object.
 *
 * @param [in] sender From `NVISendAlloc`, not be nullptr.
 */
NVI_API void NVISendFree(NVI_SENDER sender);

/**
 * @brief Send video.
 *
 * @param [in] sender From `NVISendAlloc`, not be nullptr.
 * @param [in] image `NVIVideoImageFrame` pointer, not be nullptr.
 * @return int32_t <0, error code; >=0 success.
 */
NVI_API int32_t NVISendVideo(NVI_SENDER sender, const NVIVideoImageFrame* image);

/**
 * @brief Send audio.
 *
 * @param [in] sender From `NVISendAlloc`, not be nullptr.
 * @param [in] wave `NVIAudioWaveFrame` pointer, not be nullptr.
 * @return int32_t <0, error code; >=0 success.
 */
NVI_API int32_t NVISendAudio(NVI_SENDER sender, const NVIAudioWaveFrame* wave);

/**
 * @brief Send meta
 *
 * @param [in] sender From `NVISendAlloc`, not be nullptr.
 * @param [in] meta `NVIMetaData` pointer, not be nullptr.
 * @return int32_t <0, error code; >=0 success.
 */
NVI_API int32_t NVISendMeta(NVI_SENDER sender, const NVIMetaData* meta);

/**
 * @brief Send video bitstream.
 *
 * @param [in] sender From `NVISendAlloc`, not be nullptr.
 * @param [in] encoded `NVIVideoEncodedPacket` pointer, not be nullptr.
 * @return int32_t <0, error code; >=0 success.
 */
NVI_API int32_t NVISendVideoEncoded(NVI_SENDER sender, const NVIVideoEncodedPacket* packet);

/**
 * @brief Send video proxy bitstream.
 *
 * @param [in] sender From `NVISendAlloc`, not be nullptr.
 * @param [in] encoded `NVIVideoEncodedPacket` pointer, not be nullptr.
 * @return int32_t <0, error code; >=0 success.
 */
NVI_API int32_t NVISendVideoProxyEncoded(NVI_SENDER sender, const NVIVideoEncodedPacket* packet);

/**
 * @brief Send audio proxy bitstream.
 *
 * @param [in] sender From `NVISendAlloc`, not be nullptr.
 * @param [in] encoded `NVIAudioEncodedPacket` pointer, not be nullptr.
 * @return int32_t <0, error code; >=0 success.
 */
NVI_API int32_t NVISendAudioEncoded(NVI_SENDER sender, const NVIAudioEncodedPacket* packet);

/**
 * @brief Description of sender status.
 *
 */
typedef struct _NVISendStatus
{
    uint32_t number;        ///[out] Id of stream
    uint32_t pulls;         ///[out] Number of receiving
    uint32_t reserved[30];  ///Reserved for future use, must be 0.
} NVISendStatus;

/**
 * @brief Get sender status.
 *
 * @param [in] sender From `NVISendAlloc`, not be nullptr.
 * @param [in][out] status
 */
NVI_API void NVISendStatusGet(NVI_SENDER sender, NVISendStatus* status);

/**
 * @brief Encoder preset.
 */
typedef struct _NVISendPresetParam
{
    const NVIVideoCodecParam* video;  ///< The video encode param, maybe nullptr.
    const NVIAudioCodecParam* audio;  ///< The audio encode param, maybe nullptr.
} NVISendPresetParam;

/**
 * @brief Set encoder preset.
 *
 * @param [in] sender From `NVISendAlloc`, not be nullptr.
 * @param [in] param `NVISendPresetParam` pointer, not be nullptr.
 * @return <0, error code; >=0 success.
 */
NVI_API int32_t NVISendPreset(NVI_SENDER sender, const NVISendPresetParam* param);

/**
 * @brief Param of receive meta data.
 *
 */
typedef struct _NVISendGetMetaParam
{
    NVIMetaData* meta_out;  ///< [out] Meta data object pointer, set by `NVISendPeekMeta`.
    int32_t timeout_ms;     ///< [in] Max wait time in once call, in millisecond.
    int32_t flags;          ///< [in] Some flag , must be 0.
    uint32_t reserveds[4];  ///< Reserved for future use, must be 0.
} NVISendGetMetaParam;

/**
 * @brief Get next meta data.
 *
 * @param [in] sender Form `NVISendAlloc`, not be nullptr.
 * @param [in] param `NVISendGetMetaParam`
 * @return int32_t <0, error code; >=0 success
 */
NVI_API int32_t NVISendPeekMeta(NVI_SENDER sender, NVISendGetMetaParam* param);

#endif  //NVI_SEND_H
