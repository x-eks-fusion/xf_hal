/**
 * @file xf_hal_posix.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief (暂未使用)基于 xf_hal_driver 的 posix 实现。
 * @version 0.1
 * @date 2024-05-10
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_HAL_POSIX_H__
#define __XF_HAL_POSIX_H__

/* ==================== [Includes] ========================================== */

#include "xf_hal_kernel_config.h"

/**
 * @ingroup group_xf_hal_internal
 * @defgroup group_xf_hal_internal_posix posix
 * @brief (暂未使用)基于 xf_hal_driver 的 posix 实现。
 * @{
 */

#if XF_HAL_POSIX_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define O_RDONLY         0x00
#define O_WRONLY         0x01
#define O_RDWR           0x02

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

int open(const char *pathname, int flags);
int ioctl(int fd, unsigned long request, ...);
size_t write(int fd, const void *buf, size_t count);
size_t read(int fd, void *buf, size_t count);
int close(int fd);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_HAL_POSIX_IS_ENABLE */

/**
 * End of group_xf_hal_internal_posix
 * @}
 */

#endif // __XF_HAL_POSIX_H__
