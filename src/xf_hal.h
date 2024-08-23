/**
 * @file xf_hal.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief xf_hal 模块用户总头文件。
 * @version 0.1
 * @date 2024-05-11
 * @note 此文件不需要防止重复包含。
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/**
 * @defgroup group_xf_hal xf_hal
 * @brief xf_hal (硬件抽象层) 允许在不同硬件平台上使用同样的硬件功能。
 *
 * 如 gpio, adc, dac, uart, i2c, spi, timer 等等。
 *
 */

/**
 * @ingroup group_xf_hal
 * @defgroup group_xf_hal_user 用户接口
 * @brief 调用具体功能的接口。如初始化、配置、输入输出等。
 *
 * xfusion 用户使用 xf_hal 时只需 `#include "xf_hal.h"` 即可。
 *
 */

/**
 * @ingroup group_xf_hal
 * @defgroup group_xf_hal_port 移植接口
 * @brief 用于对接具体硬件驱动到 xf_hal 中的接口。
 *
 * 对接 xf_hal 时只需 `#include "xf_hal_port.h"` 即可。
 *
 */

/**
 * @ingroup group_xf_hal
 * @defgroup group_xf_hal_internal 内部接口
 * @brief 组件内部实现某些功能时定义的接口。用户不一定需要。
 */

/* ==================== [Includes] ========================================== */

#include "kernel/xf_hal_posix.h"
#include "device/xf_hal_device.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif
