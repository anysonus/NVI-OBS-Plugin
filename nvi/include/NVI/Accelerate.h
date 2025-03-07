/**
 * @file NVI/Accelerate.h
 * @brief Some video accelerator defined.
 *
 * The NVI built-in support some common hardware codecs, like NVCodec, Video Toolbox.
 * Users can achieve zero copy through some contextual configurations here to achieve maximum system utilization.
 *
 * @version 1.0.1
 * @date 2023-06-08
 * @copyright Copyright (c) 2023 Sobey Digital Inc.
 *
 */

#ifndef NVI_ACCELERATE_H
#define NVI_ACCELERATE_H

#include <NVI/Import.h>

/**
 * @brief Defined for video accelerator.
 */
typedef enum _NVISupportAccelType
{
    NVIAccel_CPU = -1,      ///< Forced software.
    NVIAccel_Auto = 0,      ///< Choice by auto detect.
    NVIAccel_NVCodec,       ///< Nvidia Video Codec, on Windows & Linux.
    NVIAccel_DXVA2,         ///< DirectX Video Acceleration, on Windows, only for decode.
    NVIAccel_D3D11VA,       ///< Direct3D 11 Video Acceleration, on Windows, only for decode.
    NVIAccel_VideoToolbox,  ///< On Apple.
    NVIAccel_MediaCodec,    ///< On Android.
    NVIAccel_VAAPI,         ///< Video Acceleration API
} NVIAccelType;

/**
 * @brief Some flag of accelerator.
 */
typedef enum _NVIAccelFlags
{
    NVIAccelFlag_None = 0,                      ///< None
    NVIAccelFlag_UseDeviceBuffer = 0x00000001,  ///< The video pixel device buffer, like CUDA, D3D11Texture2D.
} NVIAccelFlags;

/**
 * @brief Structure of accelerator context
 */
typedef struct _NVIVideoAccelerate
{
    int32_t type;          ///< See NVIAccelType
    int32_t flags;         ///< See NVIAccelFlags
    int32_t _reserved[2];  ///< Must be set to 0
    union
    {
        /**
         * @brief The NVCodec context.
         */
        struct
        {
            void* context;             ///< CUcontext
            int32_t device;            ///< CUDevice
            bool use_primary_context;  ///< Use primary CUDA context, see <https://docs.nvidia.com/cuda/cuda-driver-api/group__CUDA__PRIMARY__CTX.html>.
        } cuda;
        /**
         * @brief The DXVA2 context.
         */
        struct
        {
            void* manager;  ///< IDirect3DDeviceManager9*
        } dxva2;
        /**
         * @brief The D3D11VA context.
         */
        struct
        {
            void* d3d11_device;           ///< ID3D11Device*
            void* d3d11_context;          ///< ID3D11DeviceContext*
            void* video_device;           ///< ID3D11VideoDevice*
            void* video_context;          ///< ID3D11VideoContext*
            void* mutex;                  ///< User defined mutex
            void (*lock)(void* mutex);    ///< User defined callback.
            void (*unlock)(void* mutex);  ///< User defined callback.
        } d3d11va;
        /**
         * @brief The Video Toolbox context.
         */
        struct
        {
            void* _reserved;  ///< must be set NULL
        } video_toolbox;
        /**
         * @brief The MediaCodec context.
         */
        struct
        {
            void* surface;
        } media_codec;
        /**
         * @brief The VAAPI context.
         */
        struct
        {
            void* display;    ///< VADisplay
            const char* x11;  ///< Or Linux x11
            const char* drm;  ///< Or Linux drm
            int32_t adapter;  ///< Or Windows DXGI Adapter
        } vaapi;
        uint32_t _space[16];  ///< For structure align.
    } context;
} NVIVideoAccelerate;

#endif  //NVI_ACCELERATE_H
