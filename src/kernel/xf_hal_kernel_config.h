/**
 * @file xf_hal_kernel_config.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief xf_hal 内核配置(仅 xf_hal kernel 内部使用)。
 * @version 0.1
 * @date 2024-07-10
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_HAL_KERNEL_CONFIG_H__
#define __XF_HAL_KERNEL_CONFIG_H__

/* ==================== [Includes] ========================================== */

#include "../xf_hal_config_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#if (!defined(XF_HAL_LOCK_DISABLE))||(XF_HAL_LOCK_DISABLE)
#   define XF_HAL_LOCK_IS_ENABLE  (0)
#else
#   define XF_HAL_LOCK_IS_ENABLE  (1)
#endif

#if (!defined(XF_HAL_POSIX_DISABLE))||(XF_HAL_POSIX_DISABLE)
#   define XF_HAL_POSIX_IS_ENABLE  (0)
#else
#   define XF_HAL_POSIX_IS_ENABLE  (1)
#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_HAL_KERNEL_CONFIG_H__
