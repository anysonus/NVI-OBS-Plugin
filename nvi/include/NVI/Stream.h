/**
 * @file NVI/Stream.h
 * @brief AV format defined.
 * @version 1.0.1
 * @date 2023-05-05
 * @copyright Copyright (c) 2023 Sobey Digital Inc.
 *
 */

#ifndef NVI_STREAM_H
#define NVI_STREAM_H

#include <NVI/Import.h>

#ifndef NVI_FOURCC
#define NVI_FOURCC(c0, c1, c2, c3) \
    (((uint32_t)(uint8_t)(c0) << 24) | ((uint32_t)(uint8_t)(c1) << 16) | ((uint32_t)(uint8_t)(c2) << 8) | ((uint32_t)(uint8_t)(c3)))
#endif

/**
 * @brief Chromaticity coordinates of the source primaries.
 *
 * ColourPrimaries indicates the chromaticity coordinates of the source colour primaries
 *  as specified in Table 2 in terms of the CIE 1931 definition of x and y as specified by
 *  ISO 11664-1.
 *
 * The indices are equal to the values specified in T-REC H.273 Table 2.
 * These values match the ones defined by ISO/IEC 23001-8_2013 § 7.1.
 *
 */
typedef enum _NVIPrimaryID
{
    NVIPrimary_BT709 = 1,        ///< See T-REC H.273 Table 2.
    NVIPrimary_Unspecified = 2,  ///< Determined by the application.
    NVIPrimary_Reserved = 3,     ///< For future use by ITU-T | ISO/IEC
    NVIPrimary_BT470M = 4,       ///< See T-REC H.273 Table 2.
    NVIPrimary_BT470BG = 5,      ///< See T-REC H.273 Table 2.
    NVIPrimary_SMPTE170M = 6,    ///< See T-REC H.273 Table 2.
    NVIPrimary_SMPTE240M = 7,    ///< See T-REC H.273 Table 2.
    NVIPrimary_FILM = 8,         ///< See T-REC H.273 Table 2.
    NVIPrimary_BT2020 = 9,       ///< See T-REC H.273 Table 2.
    NVIPrimary_SMPTEST428 = 10,  ///< See T-REC H.273 Table 2.
    NVIPrimary_SMPTEST431 = 11,  ///< See T-REC H.273 Table 2.
    NVIPrimary_SMPTEST432 = 12,  ///< See T-REC H.273 Table 2.
    NVIPrimary_JEDECP22 = 22,    ///< See T-REC H.273 Table 2.
} NVIPrimaryID;

/**
 * @brief Color Transfer Characteristic.
 *
 * The indices are equal to the values specified in T-REC H.273 Table 3.
 * These values match the ones defined by ISO/IEC 23001-8_2013 § 7.2.
 *
 */
typedef enum _NVITransferID
{
    NVITransfer_BT709 = 1,          ///< See T-REC H.273 Table 3.
    NVITransfer_Unspecified = 2,    ///< Determined by the application.
    NVITransfer_Reserved = 3,       ///< For future use by ITU-T | ISO/IEC
    NVITransfer_GAMMA22 = 4,        ///< See T-REC H.273 Table 3.
    NVITransfer_GAMMA28 = 5,        ///< See T-REC H.273 Table 3.
    NVITransfer_SMPTE170M = 6,      ///< See T-REC H.273 Table 3.
    NVITransfer_SMPTE240M = 7,      ///< See T-REC H.273 Table 3.
    NVITransfer_LINEAR = 8,         ///< See T-REC H.273 Table 3.
    NVITransfer_LOG = 9,            ///< See T-REC H.273 Table 3.
    NVITransfer_LOG_SQRT = 10,      ///< See T-REC H.273 Table 3.
    NVITransfer_IEC61966_2_4 = 11,  ///< See T-REC H.273 Table 3.
    NVITransfer_BT1361_ECG = 12,    ///< See T-REC H.273 Table 3.
    NVITransfer_IEC61966_2_1 = 13,  ///< See T-REC H.273 Table 3.
    NVITransfer_BT2020_10 = 14,     ///< See T-REC H.273 Table 3.
    NVITransfer_BT2020_12 = 15,     ///< See T-REC H.273 Table 3.
    NVITransfer_SMPTEST2084 = 16,   ///< See T-REC H.273 Table 3.
    NVITransfer_SMPTEST428 = 17,    ///< See T-REC H.273 Table 3.
    NVITransfer_ARIB_STD_B67 = 18,  ///< See T-REC H.273 Table 3.
} NVITransferID;

/**
 * @brief YUV colorspace type.
 *
 * The indices are equal to the values specified in T-REC H.273 Table 4.
 * These values match the ones defined by ISO/IEC 23001-8_2013 § 7.3.
 *
 */
typedef enum _NVIMatrixID
{
    NVIMatrix_RGB = 0,            ///< See T-REC H.273 Table 4.
    NVIMatrix_BT709 = 1,          ///< See T-REC H.273 Table 4.
    NVIMatrix_Unspecified = 2,    ///< Determined by the application.
    NVIMatrix_Reserved = 3,       ///< For future use by ITU-T | ISO/IEC.
    NVIMatrix_FCC = 4,            ///< See T-REC H.273 Table 4.
    NVIMatrix_BT470BG = 5,        ///< See T-REC H.273 Table 4.
    NVIMatrix_SMPTE170M = 6,      ///< See T-REC H.273 Table 4.
    NVIMatrix_SMPTE240M = 7,      ///< See T-REC H.273 Table 4.
    NVIMatrix_YCOCG = 8,          ///< See T-REC H.273 Table 4.
    NVIMatrix_BT2020_NCL = 9,     ///< See T-REC H.273 Table 4.
    NVIMatrix_BT2020_CL = 10,     ///< See T-REC H.273 Table 4.
    NVIMatrix_SMPTE2085 = 11,     ///< See T-REC H.273 Table 4.
    NVIMatrix_CDNCLS = 12,        ///< See T-REC H.273 Table 4.
    NVIMatrix_CDCLS = 13,         ///< See T-REC H.273 Table 4.
    NVIMatrix_BT2100_ICTCP = 14,  ///< See T-REC H.273 Table 4.
} NVIMatrixID;

/**
 * @brief Visual content value range.
 */
typedef enum _NVIRangeID
{
    NVIRange_Invalid = 0,  ///< https://www.webmproject.org/docs/container/#colour for the element Range.
    NVIRange_Limited = 1,  ///< Typically, ranges: 16-235 for luma and 16-240 for chroma.
    NVIRange_Full = 2,     ///< Full ranges from 0 to 255.
    NVIRange_Derived = 3,  ///< Range is defined by NVIMatrixID/NVITransferID.
} NVIRangeID;

/**
 * @brief Format of image pixel.
 */
typedef enum _NVIPixelFormat
{
    NVIPixel_Unspecific = 0,                            /**< Not used.*/
    NVIPixel_I420 = NVI_FOURCC('i', '4', '2', '0'),     /**< YUV420-8bit, plane[0]=yyyy.. plane[1]=uu.. plane[2]=vv.. */
    NVIPixel_420A = NVI_FOURCC('4', '2', '0', 'a'),     /**< YUV420-8bit and alpha, plane[0]=yyyy.. plane[1]=uu.. plane[2]=vv.. plane[3]=aaaa.. */
    NVIPixel_NV12 = NVI_FOURCC('n', 'v', '1', '2'),     /**< YUV420-8bit, plane[0]=yyyy.. plane[1]=uvuv.. */
    NVIPixel_NV12A = NVI_FOURCC('y', '1', '2', 'a'),    /**< YUV420-8bit and alpha, plane[0]=yyyy.. plane[1]=uvuv.. plane[2]=aaaa.. */
    NVIPixel_NV21 = NVI_FOURCC('n', 'v', '2', '1'),     /**< YUV420-8bit, plane[0]=yyyy.. plane[1]=vuvu.. */
    NVIPixel_NV21A = NVI_FOURCC('y', '2', '1', 'a'),    /**< YUV420-8bit and alpha, plane[0]=yyyy.. plane[1]=vuvu.. plane[2]=aaaa.. */
    NVIPixel_422P = NVI_FOURCC('4', '2', '2', 'p'),     /**< YUV422-8bit, plane[0]=yyyy.. plane[1]=uu.. plane[2]=vv.. */
    NVIPixel_422A = NVI_FOURCC('4', '2', '2', 'a'),     /**< YUV422-8bit, plane[0]=yyyy.. plane[1]=uu.. plane[2]=vv.. plane[3]=aaaa.. */
    NVIPixel_P010LE = NVI_FOURCC('0', '1', '0', 'l'),   /**< YUV420-10bit stacked in 16bit little-endian, MSB, plane[0]=yyyy.. plane[1]=uvuv.. */
    NVIPixel_P010BE = NVI_FOURCC('0', '1', '0', 'b'),   /**< YUV420-10bit stacked in 16bit big-endian, MSB, plane[0]=yyyy.. plane[1]=uvuv.. */
    NVIPixel_420P10LE = NVI_FOURCC('i', '2', '0', 'l'), /**< YUV420-10bit stacked in 16bit little-endian, plane[0]=yyyy.. plane[1]=uu.. plane[2]=vv.. */
    NVIPixel_420P10BE = NVI_FOURCC('i', '2', '0', 'b'), /**< YUV420-10bit stacked in 16bit big-endian, plane[0]=yyyy.. plane[1]=uu.. plane[2]=vv.. */
    NVIPixel_422P10LE = NVI_FOURCC('p', '1', '0', 'l'), /**< YUV422-10bit stacked in 16bit little-endian, plane[0]=yyyy.. plane[1]=uu.. plane[2]=vv..*/
    NVIPixel_422P10BE = NVI_FOURCC('p', '1', '0', 'b'), /**< YUV422-10bit stacked in 16bit big-endian, plane[0]=yyyy.. plane[1]=uu.. plane[2]=vv..*/
    NVIPixel_V210 = NVI_FOURCC('v', '2', '1', '0'),     /**< YUV422-10bit stacked in 16bit host-order, plane[0]=uyvy.. */
    NVIPixel_Mono = NVI_FOURCC('m', 'o', 'n', 'o'),     /**< Monotone-8bit, plane[0]=.. */
} NVIPixelFormat;

/**
 * @brief The buffer type define.
 */
typedef enum _NVIBufferType
{
    NVIBuffer_HOST = 0,        ///< CPU buffer.
    NVIBuffer_CUDA,            ///< CUDA buffer.
    NVIBuffer_D3DSurface9,     ///< Direct3D Surfaces.
    NVIBuffer_D3D11Texture2D,  ///< ID3D11Texture2D
    NVIBuffer_CVPixelBufferRef,
    NVIBuffer_MediaCodecBuffer,
    NVIBuffer_VASurfaceID,
} NVIBufferType;

/**
 * @brief The rotation angle of the image relative to the captured object.
 */
typedef enum _NVIRotation
{
    NVIRotation_0 = 0,      ///< Coincide.
    NVIRotation_90 = 90,    ///< Rotate 90 degrees clockwise.
    NVIRotation_180 = 180,  ///< Rotate 180 degrees clockwise.
    NVIRotation_270 = 270,  ///< Rotate 270 degrees clockwise.
} NVIRotation;

/**
 * @brief The video encoded frame kind define.
 *
 * @attention The NVI system not official support for bidirectional predicted picture.
 */
typedef enum _NVIFrameKindID
{
    NVIFrameKind_Unknown = 0,  ///< Unused.
    NVIFrameKind_Single,       ///< Single frame, no reference.
    NVIFrameKind_Intra,        ///< Intra-coded picture, like H264/5 I frame.
    NVIFrameKind_Delta,        ///< Predicted picture, like H264/5 P frame.
} NVIFrameKindID;

/**
 * @brief The image data storage format.
 */
typedef enum _NVIFrameFormat
{
    NVIFrame_Progressive = 0,  ///< Line-by-line, like 1 2 3 4...
    NVIFrame_Field1 = 1,       ///< Only contain odd rows, like 1 3 5...
    NVIFrame_Field2 = 2,       ///< Only contain even rows, like 2 4 6...
    NVIFrame_Interleaved = 3,  ///< Split odd and even rows, used for encoding.
} NVIFrameFormat;

/**
 * @brief Defined split slice mode for encoding.
 */
typedef enum _NVISliceModeFlag
{
    NVISliceMode_CodecCase = 0,          ///< Determined by encoder.
    NVISliceMode_MultiEncoder = 0x0001,  ///< Each slice need to an encoder and decoder.
    NVISliceMode_MultiSlice = 0x0002,    ///< Force split multi slice.
    NVISliceMode_InOrder = 0x0100,       ///< Arrange slices in order and decoding.
} NVISliceModeFlag;

/**
 * @brief Define AV codec id to FourCC.
 */
typedef enum _NVICodecID
{
    NVICodec_HF = NVI_FOURCC('h', 'f', 'v', 'c'),      ///< Sobey High-Fidelity Video Codec.
    NVICodec_SUVC = NVI_FOURCC('s', 'u', 'v', 'c'),    ///< Sobey Scalable Codec.
    NVICodec_AVC = NVI_FOURCC('h', '2', '6', '4'),     ///< AVC, H264.
    NVICodec_HEVC = NVI_FOURCC('h', '2', '6', '5'),    ///< HEVC, H265.
    NVICodec_LPCM = NVI_FOURCC('l', 'p', 'c', 'm'),    ///< Linear pulse-code modulation (LPCM).
    NVICodec_AAC = NVI_FOURCC('m', 'p', '4', 'a'),     ///< MP4A, AAC.
    NVICodec_OPUS = NVI_FOURCC('o', 'p', 'u', 's'),    ///< OPUS.
    NVICodec_JPEGLS = NVI_FOURCC('j', 'p', 'l', 's'),  ///< JPEG-LS.
    NVICodec_JPEGXS = NVI_FOURCC('j', 'x', 's', 'v'),  ///< JPEG-XS.
    NVICodec_HFM = NVI_FOURCC('a', 'v', 's', 'f'),     ///< AVS-HFM.
} NVICodecID;

/**
 * @brief Audio PCM sample bit depth.
 */
typedef enum _NVIWaveBitDepth
{
    NVIWaveBit_8 = 0x0008,        ///< 8-bit samples
    NVIWaveBit_16 = 0x0010,       ///< 16-bit samples, host endianness
    NVIWaveBit_24 = 0x0018,       ///< 24-bit samples, host endianness
    NVIWaveBit_F32 = 0x0020,      ///< IEEE Float 32-bit samples, host endianness
    NVIWaveBit_Mask_LE = 0x0100,  ///< Bit mask of little-endian
    NVIWaveBit_16_LE = 0x0110,    ///< 16-bit samples, little-endian
    NVIWaveBit_24_LE = 0x0118,    ///< 24-bit samples, little-endian
    NVIWaveBit_F32_LE = 0x0120,   ///< IEEE Float 32-bit samples, little-endian
    NVIWaveBit_Mask_BE = 0x0200,  ///< Bit mask of big-endian
    NVIWaveBit_16_BE = 0x0210,    ///< 16-bit samples, big-endian
    NVIWaveBit_24_BE = 0x0218,    ///< 24-bit samples, big-endian
    NVIWaveBit_F32_BE = 0x0220,   ///< IEEE Float 32-bit samples, big-endian
} NVIWaveBitDepth;

/**
 * @brief The audio channels and layout configuration defined.
 *
 * Position of channels as given by the table below:
 *
 * ------|--------------
 * 'FL'  | front left
 * 'FR'  | front right
 * 'FC'  | front center
 * 'LFE' | low frequency
 * 'BL'  | back left
 * 'BR'  | back right
 * 'BC'  | back center
 * 'SL'  | side left
 * 'SR'  | side right
 */
typedef enum _NVIWaveChannels
{
    NVIWaveChannel_1 = 0x0001,    ///< Mono, FC
    NVIWaveChannel_2 = 0x0002,    ///< Stereo, FL+FR
    NVIWaveChannel_5D1 = 0x0006,  ///< 5.1, FL+FR+FC+LFE+BL+BR
    NVIWaveChannel_6D1 = 0x0007,  ///< 6.1, FL+FR+FC+LFE+BL+BR+BC
    NVIWaveChannel_7D1 = 0x0008,  ///< 7.1, FL+FR+FC+LFE+BL+BR+SL+SR
} NVIWaveChannels;

/**
 * @brief Param of colour space.
 */
typedef struct _NVIColorSpace
{
    uint8_t primary;   ///< see NVIPrimaryID
    uint8_t transfer;  ///< see NVITransferID
    uint8_t matrix;    ///< see NVIMatrixID
    uint8_t range;     ///< see NVIRangeID
} NVIColorSpace;

/**
 * @brief The NVI data buffer structure.
 */
typedef struct _NVIDataBuffer
{
    size_t size;
    const uint8_t* bytes;
} NVIDataBuffer;

/**
 * @brief The ancillary data buffer define.
 *
 * @attention Require size less 1024 for now.
 */
typedef NVIDataBuffer NVISideData;

/**
 * @brief The AV frame start tick define.
 *
 * In audio, typical `freq_num`=1 `freq_den`=sample_rate.
 * In video, typical `freq_num`=1 `freq_den`=90000.
 */
typedef struct _NVITimeTick
{
    uint64_t value;     ///< Tick value.
    uint32_t freq_num;  ///< Tick frequency numerator
    uint32_t freq_den;  ///< Tick frequency denominator
} NVITimeTick;

/**
 * @brief The NVI system time format define.
 *
 */
typedef int64_t NVIDateTime;

/**
 * @brief Aspect ratio.
 *
 * The aspect ratio of an image is the ratio of its width to its height.
 */
typedef struct _NVIAspectRatio
{
    uint16_t width;
    uint16_t height;
} NVIAspectRatio;

/**
 * @brief Description of video image.
 */
typedef struct _NVIImageInfo
{
    uint32_t codec;            ///< See NVICodecID.
    uint32_t width;            ///< Pixel width.
    uint32_t height;           ///< Pixel height.
    uint32_t frame_rate_num;   ///< Video frame rate numerator.
    uint32_t frame_rate_den;   ///< Video frame rate denominator.
    uint32_t frame_kind;       ///< See NVIFrameKindID.
    uint32_t frame_format;     ///< See NVIFrameFormat.
    NVIAspectRatio sar;        ///< Sample aspect ratio.
    uint32_t rotation;         ///< See NVIRotation.
    NVIColorSpace colorspace;  ///< Pixel color space.
    NVITimeTick tick;          ///< Time tick of image.
    NVIDateTime time;          ///< System time of microseconds.
} NVIImageInfo;

/**
 * @brief The image rectangular object define.
 */
typedef struct _NVIUpdateRect
{
    uint32_t x;       ///< Position of horizontal, left is 0.
    uint32_t y;       ///< Position of vertical, top is 0.
    uint32_t width;   ///< Pixel width of rect, right is x + width.
    uint32_t height;  ///< Pixel height of rect, bottom is y + height.
} NVIUpdateRect;

/**
 * @brief Maximum number of video pixel plane buffers.
 *
 */
static const size_t MaxPixelPlanes = 4;

/**
 * @brief The pixel buffer define.
 */
typedef struct _NVIPixelBuffer
{
    const uint8_t* planes[MaxPixelPlanes];  ///< Plane buffers.
    uint32_t strides[MaxPixelPlanes];       ///< Buffer horizontal strides.
    uint32_t format;                        ///< See NVIPixelFormat.
    uint32_t type;                          ///< See NVIBufferType.
    uint32_t reserved[2];                   ///< Must be 0.
} NVIPixelBuffer;

/**
 * @brief The video frame object define.
 */
typedef struct _NVIVideoImageFrame
{
    NVIImageInfo info;       ///< Description of video.
    NVIPixelBuffer buffer;   ///< Buffer of pixel.
    NVISideData side;        ///< Ancillary data buffer, see `NVIMetaNode`.
    NVIUpdateRect* updated;  ///< Update rectangular.
    uint32_t reserved[2];    ///< Reserved for future use.
} NVIVideoImageFrame;

/**
 * @brief The video encoded bit stream object define.
 */
typedef struct _NVIVideoEncodedPacket
{
    NVIImageInfo info;      ///< Description of video.
    NVIDataBuffer buffer;   ///< Bit stream buffer.
    NVISideData side;       ///< Ancillary data buffer, see `NVIMetaNode`.
    uint16_t slice_mode;    ///< See NVISliceModeFlag.
    uint16_t slice_count;   ///< Slice count of frame.
    uint16_t slice_offset;  ///< Slice offset of frame.
    uint16_t slice_number;  ///< Slice number in buffer.
    uint32_t pixel_format;  ///< Input pixel format of image.
    uint32_t reserved[1];   ///< Reserved for future use.
} NVIVideoEncodedPacket;

/**
 * @brief Description of audio.
 */
typedef struct _NVIWaveInfo
{
    uint32_t codec;        ///< See NVICodecID.
    uint32_t sample_rate;  ///< Sample rate.
    uint16_t depth;        ///< Bit depth per sample, see NVIWaveBitDepth.
    uint16_t channels;     ///< Channel number, see NVIWaveChannels.
    NVITimeTick tick;      ///< Time tick of first sample.
    NVIDateTime time;      ///< System time of microseconds.
    uint32_t reserved[6];  ///< Reserved for future use.
} NVIWaveInfo;

/**
 * @brief Audio PCM buffer.
 *
 * @attention Only support interleaved storage.
 */
typedef struct _NVIWaveBuffer
{
    const uint8_t* data;   ///< PCM data.
    size_t size;           ///< Data length in bytes.
    uint16_t align;        ///< Bytes per sample.
    uint16_t samples;      ///< Sample number per channel.
    uint32_t reserved[2];  ///< Reserved for future use.
} NVIWaveBuffer;

/**
 * @brief The audio frame object define.
 */
typedef struct _NVIAudioWaveFrame
{
    NVIWaveInfo info;      ///< Description of audio.
    NVIWaveBuffer buffer;  ///< PCM buffer.
} NVIAudioWaveFrame;

/**
 * @brief The audio encoded bit stream object define.
 */
typedef struct _NVIAudioEncodedPacket
{
    NVIWaveInfo info;      ///< Description of audio.
    NVIDataBuffer buffer;  ///< Bit stream buffer.
} NVIAudioEncodedPacket;

/**
 * @brief The NVI meta data buffer define.
 *
 */
typedef NVIDataBuffer NVIMetaData;

#endif  // NVI_STREAM_H
