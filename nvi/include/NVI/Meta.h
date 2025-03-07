/**
 * @file NVI/Meta.h
 * @brief Meta data assist API defined.
 * @version 1.0.1
 * @date 2023-07-24
 * @copyright Copyright (c) 2023 Sobey Digital Inc.
 *
 */

#ifndef NVI_META_H
#define NVI_META_H

#include <NVI/Import.h>

/**
 * @brief Predefined meta data type.
 */
typedef enum _NVIMetaDefined
{
    NVIMeta_BeginWith = 0,           ///< Not used.
    NVIMeta_Binary = 1,              ///< Binary data.
    NVIMeta_Text = 2,                ///< Text data.
    NVIMeta_Json = 3,                ///< JSON format data.
    NVIMeta_Tally0 = 4,              ///< Tally data, see NVITally.
    NVIMeta_PTZ0 = 5,                ///< PTZ version 0 data, xml format.
    NVIMeta_Tally1 = 6,              ///< Tally display data, json format.
    NVIMeta_PTZ1 = 7,                ///< PTZ version 1 data, json format.
    NVIMeta_ROI = 16,                ///< ROI table data.
    NVIMeta_StaticHDR = 17,          ///< Static HDR data.
    NVIMeta_DynamicHDR = 18,         ///< Dynamic HDR data.
    NVIMeta_EndWithPredefine = 255,  ///< User define begin, FourCC format be recommend.
} NVIMetaDefined;

/**
 * @brief Text data view.
 */
typedef struct _NVITextView
{
    const char* data;  ///< Text data pointer, with UTF-8.
    size_t length;     ///< Length of text.
} NVITextView;

/**
 * @brief Text data view.
 */
typedef struct _NVIRegionOfInterest
{
    NVITextView label;  ///< Label of rectangle area, length less 256.
    uint32_t x;         ///< Position of horizontal, left is 0.
    uint32_t y;         ///< Position of vertical, top is 0.
    uint32_t width;     ///< Pixel width of rect, right is x + width.
    uint32_t height;    ///< Pixel height of rect, bottom is y + height.
} NVIRegionOfInterest;

/**
 * @brief Get length of ROI table from metadata.
 *
 * @param [in] data The meta data pointer.
 * @param [in] size The meta data size.
 * @return size_t =0, data invalid; >0, length of ROI table.
 */
NVI_API size_t NVIMetaParseROILength(const uint8_t* data, size_t size);

/**
 * @brief Parse ROI table from NVIMetaData.
 *
 * @param [in] data The meta data pointer, from NVIMetaData object.
 * @param [in] size The meta data size,, from NVIMetaData object.
 * @param [in][out] table `NVIRegionOfInterest` array pointer, not be nullptr.
 * @param [in] length table length, less 256.
 * @return int32_t <0, data invalid; =0, is empty; >0, parsed size.
 */
NVI_API int32_t NVIMetaParseROITable(const uint8_t* data, size_t size, NVIRegionOfInterest* table, size_t length);

/**
 * @brief Load  `NVIRegionOfInterest` table to buffer.
 *
 * @param [in] table `NVIRegionOfInterest` array pointer, not be nullptr.
 * @param [in] length table length, less 256.
 * @param [in] buffer The bytes buffer, not be nullptr.
 * @param [in] size The buffer size, require >= 8bytes header + 1bytes table-length + value-length.
 * @return size_t =0, buffer insufficient space; >0 load size in bytes.
 */
NVI_API size_t NVIMetaSetupROITable(const NVIRegionOfInterest* table, size_t length, uint8_t* buffer, size_t size);

/**
 * @brief Param of tally.
 */
typedef struct _NVITally
{
    uint8_t version;       ///< Must be 0;
    uint8_t program : 1;   ///< 0, is false; 1, is true;
    uint8_t preview : 1;   ///< 0, is false; 1, is true;
    uint8_t reserved : 6;  ///< For future used;
    uint16_t index;        ///< Tally light number, default is 0.
} NVITally;

/**
 * @brief Light color defined.
 */
enum NVITallyColor
{
    None = 0,
    Red,
    Green,
    Amber,
};

/**
 * @brief The display message and lights.
 *
 * The length of `text` range from 0 to 255 in bytes, and encoded in UTF-8.
 */
typedef struct _NVITallyDisplay
{
    const char* text;      ///< Display text.
    uint8_t length;        ///< Length of text.
    uint8_t light;         ///< see NVITallyColor.
    uint16_t index;        ///< Light number, default is 0.
    uint8_t reserveds[4];  ///< For future used;
} NVITallyDisplay;

/**
 * @brief Meta data format.
 */
typedef struct _NVIMetaNode
{
    uint32_t defined;      ///< See NVIMetaDefined.
    uint32_t length;       ///< Length of value in bytes.
    const uint8_t* value;  ///< Meta data.
} NVIMetaNode;

/**
 * @brief Parse meta data to `NVIMetaNode`.
 *
 * @param [in] data The meta data pointer, from NVIMetaData object.
 * @param [in] size The meta data size, from NVIMetaData object.
 * @param [in][out] node `NVIMetaNode` pointer, not be nullptr.
 * @return int32_t <0, data invalid; =0, is empty; >0, parsed size.
 */
NVI_API int32_t NVIMetaParseNode(const uint8_t* data, size_t size, NVIMetaNode* node);

/**
 * @brief Load  `NVIMetaNode` to buffer.
 *
 * @param [in] node `NVIMetaNode` pointer, not be nullptr.
 * @param [in] buffer The bytes buffer, not be nullptr.
 * @param [in] size The buffer size, require >= 8bytes + value-length.
 * @return size_t =0, buffer insufficient space; >0 load size in bytes.
 */
NVI_API size_t NVIMetaSetupNode(const NVIMetaNode* node, uint8_t* buffer, size_t size);

#endif  // NVI_META_H
