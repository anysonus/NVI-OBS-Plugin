/**
 * @file NVI/Network.h
 * @brief Network host discover API defined.
 * @version 1.0.1
 * @date 2023-05-06
 * @copyright Copyright (c) 2023 Sobey Digital Inc.
 *
 */

#ifndef NVI_NETWORK_H
#define NVI_NETWORK_H

#include <NVI/Import.h>
#include <NVI/Base.h>

/**
 * @brief Description of NVI resource.
 */
typedef struct _NVINetworkStream
{
    const char* alias;              ///< [out]: Stream alias, set by `NVINetworkEnumStream`.
    const char* sites;              ///< [out]: Site name, set by `NVINetworkEnumStream`.
    const char* domain;             ///< [out]: Realm name, set by `NVINetworkEnumStream`.
    const char* uri;                ///< [out]: Stream access uri, set by `NVINetworkEnumStream`.
    const char* tags;               ///< [out]: Stream tags, set by `NVINetworkEnumStream`.
    uint32_t number;                ///< [out]: unique number of stream in site, set by `NVINetworkEnumStream`.
    uint32_t instance;              ///< [out]: unique number of NVI instance, set by `NVINetworkEnumStream`.
    uint32_t caps_ptz : 1;          ///< [out]: =1, support PTZ handler, set by `NVINetworkEnumStream`.
    uint32_t caps_proxy_video : 1;  ///< [out]: =1, support proxy video stream, set by `NVINetworkEnumStream`.
    uint32_t caps_reserved : 30;    ///< Reserved for future use.
    uint32_t reserved[3];           ///< Reserved for future use.
} NVINetworkStream;

/**
 * @brief Param of enum resource.
 */
typedef struct _NVINetworkEnumParam
{
    NVINetworkStream* streams;  ///< [in][out] The streams info buffer, alloc by user, fill by `NVINetworkEnumStream`.
    uint32_t streams_size;      ///< [in] Size of `streams` param, ex. streams = NVINetworkStream[2] streams_size = 2.
    int32_t timeout_ms;         ///< [in] Wait for stream info out of milliseconds
    const char* filter;         ///< [in] Stream tags filter, split by comma.
    uint32_t loopback : 1;      ///< [in] 0: Exclude loopback streams; 1: Include loopback streams.
    uint32_t rescan : 1;        ///< [in] 0: Passive detection; 1: Active rescan NVI network.
    uint32_t flags : 30;        ///< Reserved for future use.
    uint32_t reserved[1];       ///< Reserved for padding.
} NVINetworkEnumParam;

/**
 * @brief Get resource info.
 *
 * @param [in] context From `NVIContextCreate`, not be nullptr.
 * @param [in][out] param `NVINetworkEnumParam` pointer, not be nullptr.
 * @return int32_t <0, error code; >=0 size of streams.
 */
NVI_API int32_t NVINetworkEnumStream(NVI_CONTEXT context, NVINetworkEnumParam* param);

/**
 * @brief The site flags defined.
 */
typedef enum _NVISiteFlags
{
    NVISiteFlag_None = 0,  ///< Not used.
} NVISiteFlags;

/**
 * @brief Description of NVI site.
 */
typedef struct _NVINetworkSite
{
    const char* name;    ///< [out] Site name, set by `NVINetworkEnumSite`.
    const char* domain;  ///< [out] Domain name, set by `NVINetworkEnumSite`.
    uint32_t instance;   ///< [out] Unique number of NVI instance, set by `NVINetworkEnumSite`.
    uint32_t flags;      ///< [out] Device flags of site, see `NVISiteFlags`, set by `NVINetworkEnumSite`.
    const char* id;      ///< [out] Device id, set by `NVINetworkEnumSite`.
} NVINetworkSite;

/**
 * @brief Param of enum site.
 */
typedef struct _NVINetworkEnumSiteParam
{
    NVINetworkSite* sites;  ///< [in][out] The sites info buffer, alloc by user, fill by `NVINetworkEnumSite`.
    uint32_t sites_size;    ///< [in] Size of `sites` param, ex. sites = NVINetworkSite[2] sites_size = 2.
    uint32_t reserved;      ///< Reserved for future use.
} NVINetworkEnumSiteParam;

/**
 * @brief Get sites info.
 *
 * @param [in] context From `NVIContextCreate`, not be nullptr.
 * @param [in][out] param `NVINetworkEnumSiteParam` pointer, not be nullptr.
 * @return int32_t <0, error code; >=0 size of sites.
 */
NVI_API int32_t NVINetworkEnumSite(NVI_CONTEXT context, NVINetworkEnumSiteParam* param);

/**
 * @brief Description of self host.
 */
typedef struct _NVINetworkHostInfoParam
{
    const char* name;      ///< [out] The local host name, set by `NVINetworkHostInfo`.
    uint32_t instance;     ///< [out] NVI network instance, set by `NVINetworkHostInfo`.
    uint16_t port;         ///< [out] NVI network port, set by `NVINetworkHostInfo`.
    uint16_t flags;        ///< [in] Some flag of get info.
    uint32_t reserved[4];  ///< Reserved for future use.
} NVINetworkHostInfoParam;

/**
 * @brief Get self host info of NVI network.
 *
 * @param [in] context From `NVIContextCreate`, not be nullptr.
 * @param [in][out] param `NVINetworkHostInfoParam` pointer, not be nullptr.
 * @return int23_t <0, context object invalid; >=0 success.
 */
NVI_API int32_t NVINetworkHostInfo(NVI_CONTEXT context, NVINetworkHostInfoParam* param);

/**
 * @brief Param of NVI site.
 */
typedef struct _NVINetworkOpenSiteParam
{
    const char* domain;  ///< Domain name, could be empty.
    const char* url;     ///< NVI site url, like nvi://192.168.1.4
} NVINetworkOpenSiteParam;

/**
 * @brief Open site.
 *
 * Call will immediately return and be processed asynchronously by the internal thread.
 *
 * @param [in] context From `NVIContextCreate`, not be nullptr.
 * @param [in] param `NVINetworkOpenSiteParam` pointer, not be nullptr.
 * @return int32_t <0 fail; >=0 success in call.
 */
NVI_API int32_t NVINetworkOpenSite(NVI_CONTEXT context, const NVINetworkOpenSiteParam* param);

/**
 * @brief Closing param of site.
 */
typedef struct _NVINetworkCloseSiteParam
{
    const char* url;    ///< Same with NVINetworkOpenSiteParam::url, can be nullptr.
    uint32_t instance;  ///< From NVINetworkStream or NVINetworkSite.
    uint32_t reserved;  ///< Reserved for future use.
} NVINetworkCloseSiteParam;

/**
 * @brief Close site by url or instance.
 *
 * @param [in] context From `NVIContextCreate`, not be nullptr.
 * @param [in] param `NVINetworkCloseSiteParam` pointer, not be nullptr.
 * @return int32_t <0 fail; >=0 success.
 */
NVI_API int32_t NVINetworkCloseSite(NVI_CONTEXT context, const NVINetworkCloseSiteParam* param);

/**
 * @brief Param of discover server.
 */
typedef struct _NVINetworkJoinGroupParam
{
    const char* url;       ///< The group access server url, ex. nvi-ws://172.16.10.11:7060.
    uint32_t number;       ///< Group number for user defined, value of 0x00000001~0xFFFFFFFF.
    uint32_t reserved[5];  ///< Reserved for future use.
} NVINetworkJoinGroupParam;

/**
 * @brief Connect discover server.
 *
 * Call will immediately return and be processed asynchronously by the internal thread.
 *
 * @param [in] context From `NVIContextCreate`, not be nullptr.
 * @param [in] param `NVINetworkJoinGroupParam` pointer, not be nullptr.
 * @return int32_t <0 fail; >=0 success in call.
 */
NVI_API int32_t NVINetworkJoinGroup(NVI_CONTEXT context, const NVINetworkJoinGroupParam* param);

/**
 * @brief Disconnect discover server.
 *
 * @param [in] context From `NVIContextCreate`, not be nullptr.
 * @param [in] number From `NVINetworkJoinGroupParam::number`.
 * @return int32_t int32_t <0 fail; >=0 success.
 */
NVI_API int32_t NVINetworkLeaveGroup(NVI_CONTEXT context, uint32_t number);

#endif  // NVI_NETWORK_H
