/**
 * @file NVI/Recv.h
 * @brief AV stream receive API defined.
 * @version 1.0.1
 * @date 2023-05-05
 * @copyright Copyright (c) 2023 Sobey Digital Inc.
 *
 */

#ifndef NVI_RECV_H
#define NVI_RECV_H

#include <NVI/Import.h>
#include <NVI/Base.h>
#include <NVI/Network.h>
#include <NVI/Stream.h>
#include <NVI/Codec.h>

/**
 * @brief The NVI AV stream receiver object define.
 *
 * Alloc by `NVIRecvAlloc`.
 * Free by `NVIRecvFree`.
 *
 */
typedef void* NVI_RECVER;

/**
 * @brief Param of AV stream receiver.
 *
 */
typedef struct _NVIRecvAllocParam
{
    const char* local;             ///< Specific local network interface, like IP, adapter name ...
    const char* remote;            ///< Remote url
    uint32_t flags_rx_proxy : 1;   ///< Only get low-quality proxy video stream and if support with sender.
    uint32_t flags_off_video : 1;  ///< Exclude video data of stream.
    uint32_t flags_off_audio : 1;  ///< Exclude audio data of stream.
    uint32_t flags_off_meta : 1;   ///< Exclude meta data of stream.
    uint32_t flags_reserved : 28;  ///< Reserved for future use.
    uint32_t reserved[5];          ///< Reserved for future use.
} NVIRecvAllocParam;

/**
 * @brief Alloc AV stream receiver object.
 *
 * @param [in] context From `NVIContextCreate`, not be nullptr.
 * @param [in] param `NVIRecvAllocParam` pointer, not be nullptr.
 * @return NVI_RECVER nullptr, `NVILastError` get error code.
 */
NVI_API NVI_RECVER NVIRecvAlloc(NVI_CONTEXT context, const NVIRecvAllocParam* param);

/**
 * @brief Close stream and free the object.
 *
 * @param [in] receiver From `NVIRecvAlloc`, not be nullptr.
 */
NVI_API void NVIRecvFree(NVI_RECVER recver);

/**
 * @brief Param of receive AV frame.
 */
typedef struct _NVIRecvFrameOut
{
    NVIVideoImageFrame* image_out;  ///< [out] Video frame object pointer, set by `NVIRecvFrame`.
    NVIAudioWaveFrame* wave_out;    ///< [out] Audio frame object pointer, set by `NVIRecvFrame`.
    NVIMetaData* meta_out;          ///< [out] Meta data object pointer, set by `NVIRecvFrame`.
    int32_t timeout_ms;             ///< [in] Max wait time in once call, in millisecond.
    int32_t flags;                  ///< [in] Some flag of receive.
    int32_t reserved[8];            ///< Reserved for future use.
} NVIRecvFrameOut;

/**
 * @brief Get next AV frame.
 *
 * example:
 * ```c
 * NVIRecvFrameOut param;
 * while(your_condition)
 * {
 *     memset(&param, 0, sizeof(param));
 *     param.timeout_ms = 40;
 *     int32_t error = NVIRecvFrame(recver, &param); // @recver from NVIRecvAlloc
 *     if (error >= 0)
 *     {
 *         if(param.image_out) your_render_video(param.image_out);
 *         if(param.wave_out) your_render_audio(param.wave_out);
 *         if(param.meta_out) your_process_meta(param.meta);
 *     }
 * }
 * ```
 *
 * @attention You can't free xxx_out field in `NVIRecvFrameOut`.
 * Free in next `NVIRecvFrame` or `NVIRecvFree` or `NVIRecvEncoded`.
 *
 * @param recver From `NVIRecvAlloc`, not be nullptr.
 * @param param `NVIRecvFrameOut` pointer, not be nullptr.
 * @return int32_t <0 fail, need re-open or close stream; >=0 success.
 */
NVI_API int32_t NVIRecvFrame(NVI_RECVER recver, NVIRecvFrameOut* param);

/**
 * @brief Param of AV stream receive.
 */
typedef struct _NVIRecvEncodedOut
{
    NVIVideoEncodedPacket* video_out;  ///< [out] Video bitstream object pointer, set by `NVIRecvEncoded`.
    NVIAudioEncodedPacket* audio_out;  ///< [out] Audio bitstream object pointer, set by `NVIRecvEncoded`.
    NVIMetaData* meta_out;             ///< [out] Meta data object pointer, set by `NVIRecvEncoded`.
    int32_t timeout_ms;                ///< [in] Max wait time in once call, in millisecond.
    int32_t flags;                     ///< [in] Some flag of receive.
} NVIRecvEncodedOut;

/**
 * @brief Get next AV bitstream.
 *
 * @attention You can't free xxx_out field in NVIRecvEncodedOut.
 * Free in next `NVIRecvFrame` or `NVIRecvFree` or `NVIRecvEncoded`.
 *
 * @param recver From `NVIRecvAlloc`, not be nullptr.
 * @param param `NVIRecvEncodedOut` param, not be nullptr.
 * @return int32_t <0 fail, need re-open or close stream; >=0 success.
 */
NVI_API int32_t NVIRecvEncoded(NVI_RECVER recver, NVIRecvEncodedOut* param);

/**
 * @brief Config hardware accelerate for video decoding.
 *
 * @attention Require called before first NVIRecvFrame.
 * @param [in] recver From `NVIRecvAlloc`, not be nullptr.
 * @param [in] accel `NVIVideoAccelerate` object pointer, cannot be nullptr.
 * @return int32_t <0 fail; >=0 success.
 */
NVI_API int32_t NVIRecvVideoAccelPreset(NVI_RECVER recver, const NVIVideoAccelerate* accel);

/**
 * @brief Param of stream information.
 */
typedef struct _NVIRecvStreamInfo
{
    const char* name;
    void* future[3];
    uint32_t reserved[8];
} NVIRecvStreamInfo;

/**
 * @brief Get live stream description.
 *
 * Received the audio and video data, call this interface to obtain the stream information.
 *
 * @attention The parameters of NVIRecvStreamInfo will be released on the next call NVIRecvFrame or NVIRecvEncoded.
 * @param [in] recver From `NVIRecvAlloc`, not be nullptr.
 * @param [in] info `NVIRecvStreamInfo` object pointer, cannot be nullptr.
 * @return int32_t <0 fail; >=0 success.
 */
NVI_API int32_t NVIRecvStreamInfoPeek(NVI_RECVER recver, NVIRecvStreamInfo* info);

#endif  //NVI_RECV_H
