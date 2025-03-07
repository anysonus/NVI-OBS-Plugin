/**
 * @file NVI/Base.h
 * @brief Base API defined.
 * @version 1.0.1
 * @date 2023-05-05
 * @copyright Copyright (c) 2023 Sobey Digital Inc.
 *
 */

#ifndef NVI_BASE_H
#define NVI_BASE_H

#include <NVI/Import.h>
#include <NVI/Codec.h>

/**
 * @brief The NVI context define.
 *
 * Alloc by `NVIContextCreate`.
 * Free by `NVIContextDestory`.
 *
 */
typedef void* NVI_CONTEXT;

typedef struct _NVISiteConfig
{
    const char* name;  ///< The site name.
    const char* ip;    ///< Local IP address.
    uint16_t port;     ///< Specify local port.
    uint16_t kind;     ///< Not yet open, must be set 0.
    uint32_t reserved;
    void* reserved1[2];
} NVISiteConfig;

/**
 * @brief The NVI site config param.
 *
 * Used by `NVIContextCreate`.
 *
 */
typedef struct _NVIConextParam
{
    uint32_t version;              ///< set to NVI_CONTEXT_VER
    uint32_t flags_recv_only : 1;  ///< =1, If only support enum, receive and control enum NVI stream.
    uint32_t flags_send_only : 1;  ///< =1, If only support send NVI stream.
    uint32_t flags : 30;           ///< Reserved flags for future use.
    const NVICodecPlugin* plugin;  ///< The user defined codec.
    const NVISiteConfig* site;     ///< The site info.
    void* reserved[19];
} NVIConextParam;

/**
 * @brief The NVI context param version.
 *
 * Used by `NVIConextParam::version`.
 *
 */
#define NVI_CONTEXT_VER (0x00000002u)

/**
 * @brief Get current NVI library version number.
 * @return uint32_t library version
 */
NVI_API uint32_t NVIVersion();

/**
 * @brief Get current NVI library version description.
 * @return const char* readable library version
 */
NVI_API const char* NVIVersionReadable();

/**
 * @brief The NVI log level define.
 *
 * Like syslog level.
 * See <https://en.wikipedia.org/wiki/Syslog>
 *
 */
typedef enum _NVILogLevel
{
    NVILog_FATAL = 0,   ///< Emergency
    NVILog_ALERT = 1,   ///< Alert
    NVILog_CRIT = 2,    ///< Critical
    NVILog_FAULT = 3,   ///< ERROR
    NVILog_WARN = 4,    ///< Warning
    NVILog_NOTICE = 5,  ///< Notice, Normal but significant.
    NVILog_INFO = 6,    ///< Informational, Informational messages.
    NVILog_DEBUG = 7,   ///< Debug, Debug-level messages.
    NVILog_MAX = 0x7FFF,
} NVILogLevel;

/**
 * @brief The NVI log output configuration.
 */
typedef struct _NVILogConfigParam
{
    void (*Message)(uint32_t level, const char* message, size_t length);  ///< Callback of log message.
    uint32_t max_level;                                                   ///< Max output log level. See NVILogLevel.
    bool enable_stdout;                                                   ///< Write log message to stdout.
    bool enable_prefix;                                                   ///< Contain prefix(timestamp, thread-number, log-level).
} NVILogConfigParam;

/**
 * @brief Set log output configuration.
 * @param [in] param The `NVILogConfigParam` pointer, not be nullptr.
 */
NVI_API void NVILogConfig(const NVILogConfigParam* param);

/**
 * @brief Create NVI site and get context.
 *
 * @param [in] param The `NVIConextParam` pointer, maybe nullptr.
 * @return NVI_CONTEXT nullptr, `NVILastError` get error code.
 */
NVI_API NVI_CONTEXT NVIContextCreate(const NVIConextParam* param);

/**
 * @brief Clean NVI resource and free context.
 *
 * @param [in] context From `NVIContextCreate`, not be nullptr
 */
NVI_API void NVIContextDestory(NVI_CONTEXT context);

/**
 * @brief Get last error on current thread call.
 *
 * @return int32_t error code.
 */
NVI_API int32_t NVILastError();

#endif  //NVI_BASE_H
