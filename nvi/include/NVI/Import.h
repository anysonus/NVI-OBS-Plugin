/**
 * @file NVI/Import.h
 * @brief Library symbol defined.
 * @version 1.0.1
 * @date 2023-05-05
 * @copyright Copyright (c) 2023 Sobey Digital Inc.
 *
 */

#ifndef NVI_IMPORT_H
#define NVI_IMPORT_H

#ifdef __cplusplus
#include <cstdint>
#include <cstddef>
#define NVI_EXTERN_C extern "C"
#else
#include <stdint.h>
#include <stddef.h>
#endif  // __cplusplus

#ifdef NVI_STATIC_LIB

#define NVI_API

#else

#ifdef _WIN32

#ifdef NVI_EXPORTS
#define NVI_API NVI_EXTERN_C __declspec(dllexport)
#else
#define NVI_API NVI_EXTERN_C __declspec(dllimport)
#endif  // NVI_EXPORTS

#else

#if defined(__GNUC__) && (__GNUC__ >= 4)
#define NVI_API NVI_EXTERN_C __attribute((visibility("default")))
#endif  // defined(__GNUC__) && (__GNUC__ >= 4)

#endif

#endif  //NVI_STATIC_LIB

#ifndef NVI_API
#define NVI_API
#endif  // NVI_API

/*
 * Why is __stdcall not declared on all API?
 *
 * There are no plans to support x86 yet, __stdcall is a convention specific to the Microsoft x86 architecture.
 *
 */
#define STDCALL

#ifdef _MSC_VER
#define NVI_DEPRECATED __declspec(deprecated)
#else
#define NVI_DEPRECATED __attribute((deprecated))
#endif
#define NVI_DEPRECATED_API NVI_API NVI_DEPRECATED

#endif  // NVI_IMPORT_H
