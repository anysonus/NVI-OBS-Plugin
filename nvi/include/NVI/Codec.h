/**
 * @file Codec.h
 * @brief Codec plugin API defined.
 * @version 1.0.1
 * @date 2023-05-05
 * @copyright Copyright (c) 2023 Sobey Digital Inc.
 *
 */

#ifndef NVI_CODEC_H
#define NVI_CODEC_H

#include <NVI/Accelerate.h>
#include <NVI/Stream.h>

/**
 * @brief The video param of encoder/decoder.
 */
typedef struct _NVIVideoCodecParam
{
    uint32_t codec;                   ///< Codec id, see NVICodecID.
    uint32_t profile;                 ///< Profile value of specific codec.
    uint32_t level;                   ///< Level value of specific codec.
    uint32_t width;                   ///< Video pixel width.
    uint32_t height;                  ///< Video pixel height.
    uint32_t frame_rate_num;          ///< Video frame rate numerator.
    uint32_t frame_rate_den;          ///< Video frame rate denominator. fps=fixed_0.2f(frame_rate_num/frame_rate_den).
    uint32_t gop;                     ///< Group of pictures.
    uint32_t rotation;                ///< Video rotation.
    uint32_t avg_bitrate;             ///< Average bitrate, kbps.
    uint32_t max_bitrate;             ///< Maximum bitrate, kbps, >= `avg_bitrate`.
    uint32_t format;                  ///< See NVIPixelFormat
    uint16_t slice_mode;              ///< see NVISliceModeFlag
    uint16_t slice_count;             ///< Slice count of one image.
    NVIColorSpace colorspace;         ///< Colorspace of pixel.
    const NVIVideoAccelerate* accel;  ///< Video accelerator object pointer, maybe nullptr.
    /* extend control */
    uint16_t vbv;       ///< Specifies the VBV(HRD) buffer size, in kbits.
    uint8_t quality;    ///< For h264/h265, 0 will replace by default value of encoder. Usually range is 18~28.
    uint8_t quality_a;  ///< For alpha only, 0 is near lossless
    /* reserved for future use */
    uint32_t _reserved[7];  ///< Must be 0.
} NVIVideoCodecParam;

/**
 * @brief Video encoder interface.
 */
typedef struct _NVIVideoEncode
{
    /**
     * @brief Encoding callback interface.
     * @param packet Bitstream packet.
     * @param user User context.
     */
    typedef int32_t (*OnPacket)(const NVIVideoEncodedPacket* packet, void* user);

    void* encoder;                                                                               ///< Encoder object pointer.
    int32_t (*Config)(void* encoder, const NVIVideoCodecParam* param);                           ///< Configure encoder interface.
    int32_t (*Encoding)(void* encoder, const NVIVideoImageFrame* in, OnPacket out, void* user);  ///< Encoding image interface.
    int32_t (*Release)(void* encoder);                                                           ///< Release encoder object interface.
} NVIVideoEncode;

/**
 * @brief Video decoder interface.
 */
typedef struct _NVIVideoDecode
{
    /**
     * @brief Decoding callback interface.
     * @param frame Image frame.
     * @param user User context.
     */
    typedef int32_t (*OnFrame)(const NVIVideoImageFrame* frame, void* user);

    void* decoder;                                                                                 ///< Decoder object pointer.
    int32_t (*Config)(void* decoder, const NVIVideoCodecParam* param);                             ///< Configure decoder interface.
    int32_t (*Decoding)(void* decoder, const NVIVideoEncodedPacket* in, OnFrame out, void* user);  ///< Decoding bitstream interface.
    int32_t (*Release)(void* decoder);                                                             ///< Release decoder object interface.
} NVIVideoDecode;

/**
 * @brief The audio param of encoder/decoder.
 */
typedef struct _NVIAudioCodecParam
{
    uint32_t codec;        ///< Codec id, see NVICodecID.
    uint32_t profile;      ///< Profile value of specific codec.
    uint32_t bitrate;      ///< Bitrate, in kbps.
    uint32_t sample_rate;  ///< Sampling frequency
    uint16_t depth;        ///< Bit depth per sample, see NVIWaveBitDepth.
    uint16_t channels;     ///< Channel number. see NVIWaveChannels.
} NVIAudioCodecParam;

/**
 * @brief Audio encoder interface.
 */
typedef struct _NVIAudioEncode
{
    /**
     * @brief Encoding callback interface.
     * @param packet Bitstream packet.
     * @param user User context.
     */
    typedef int32_t (*OnPacket)(const NVIAudioEncodedPacket* packet, void* user);

    void* encoder;                                                                              ///< Encoder object pointer.
    int32_t (*Config)(void* encoder, const NVIAudioCodecParam* param);                          ///< Configure encoder interface.
    int32_t (*Encoding)(void* encoder, const NVIAudioWaveFrame* in, OnPacket out, void* user);  ///< Encoding image interface.
    int32_t (*Release)(void* encoder);                                                          ///< Release encoder object interface.
} NVIAudioEncode;

/**
 * @brief Audio decoder interface.
 */
typedef struct _NVIAudioDecode
{
    /**
     * @brief Decoding callback interface.
     * @param frame Wave frame.
     * @param user User context.
     */
    typedef int32_t (*OnFrame)(const NVIAudioWaveFrame* frame, void* user);

    void* decoder;                                                                                 ///< Decoder object pointer.
    int32_t (*Config)(void* decoder, const NVIAudioCodecParam* param);                             ///< Configure decoder interface.
    int32_t (*Decoding)(void* decoder, const NVIAudioEncodedPacket* in, OnFrame out, void* user);  ///< Decoding bitstream interface.
    int32_t (*Release)(void* decoder);                                                             ///< Release decoder object interface.
} NVIAudioDecode;

/**
 * @brief Alloc video encoder interface.
 */
typedef NVIVideoEncode (*NVICodecVideoEncodeAlloc)(uint32_t codec);

/**
 * @brief Alloc video decoder interface.
 */
typedef NVIVideoDecode (*NVICodecVideoDecodeAlloc)(uint32_t codec);

/**
 * @brief Alloc audio encoder interface.
 */
typedef NVIAudioEncode (*NVICodecAudioEncodeAlloc)(uint32_t codec);

/**
 * @brief Alloc audio decoder interface.
 * @param
 */
typedef NVIAudioDecode (*NVICodecAudioDecodeAlloc)(uint32_t codec);

/**
 * @brief The AV codec plugin interface.
 *
 * Used by `NVIConextParam`.
 * User can defined custom encoder or decoder.
 * Or used `NVIRecvEncoded` get bitstream and decode.
 * Or used `NVISendVideoEncoded` and `NVISendAudioEncoded` send codec bitstream.
 */
typedef struct _NVICodecPlugin
{
    NVICodecVideoEncodeAlloc video_encode_alloc;
    NVICodecVideoDecodeAlloc video_decode_alloc;
    NVICodecAudioEncodeAlloc audio_encode_alloc;
    NVICodecAudioDecodeAlloc audio_decode_alloc;
} NVICodecPlugin;

#endif  //NVI_CODEC_H
