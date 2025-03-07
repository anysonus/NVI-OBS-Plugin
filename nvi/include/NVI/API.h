/**
 * @file NVI/API.h
 * @brief The NVI library public header file.
 *
 * User can include this header for all API or include required header.
 *
 * @version 1.0.1
 * @date 2023-04-13
 * @copyright Copyright (c) 2023 Sobey Digital Inc.
 *
 */

#ifndef NVI_API_H
#define NVI_API_H

#include <NVI/Send.h>
#include <NVI/Recv.h>
#include <NVI/Network.h>
#include <NVI/Device.h>

/*
 * Sender Quick Start.
 *
 * 1. Create Context After Start:
 *    context = NVIContextCreate(0);
 *
 * 2. Alloc Send Object:
 *    sender = NVISendAlloc(context, 0);
 *
 * 3. Loop To Send AV-Frame:
 *    while(condition)
 *    {
 *      wait_for_av(image, wave); // the user api
 *      NVISendVideo(sender, image, 0);
 *      NVISendAudio(sender, wave, 0);
 *    }
 *
 * 4. Stop It And Clean:
 *    NVISendFree(sender);
 *
 * 5. Destroy Context Before Exit:
 *    NVIContextDestory(context);
 */
#endif  // NVI_API_H
