/**
 * @file xf_hal_device_config.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief xf_hal 设备配置(仅 xf_hal device 内部使用)。
 * @version 0.1
 * @date 2024-05-12
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_HAL_DEVICE_CONFIG_H__
#define __XF_HAL_DEVICE_CONFIG_H__

/* ==================== [Includes] ========================================== */

#include "../xf_hal_config_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#if (!defined(XF_HAL_GPIO_ENABLE)) || (XF_HAL_GPIO_ENABLE)
#   define XF_HAL_GPIO_IS_ENABLE    (1)
#else
#   define XF_HAL_GPIO_IS_ENABLE    (0)
#endif

#if (!defined(XF_HAL_PWM_ENABLE)) || (XF_HAL_PWM_ENABLE)
#   define XF_HAL_PWM_IS_ENABLE     (1)
#else
#   define XF_HAL_PWM_IS_ENABLE     (0)
#endif

#if (!defined(XF_HAL_TIM_ENABLE)) || (XF_HAL_TIM_ENABLE)
#   define XF_HAL_TIM_IS_ENABLE     (1)
#else
#   define XF_HAL_TIM_IS_ENABLE     (0)
#endif

#if (!defined(XF_HAL_UART_ENABLE)) || (XF_HAL_UART_ENABLE)
#   define XF_HAL_UART_IS_ENABLE    (1)
#else
#   define XF_HAL_UART_IS_ENABLE    (0)
#endif

#if (!defined(XF_HAL_I2C_ENABLE)) || (XF_HAL_I2C_ENABLE)
#   define XF_HAL_I2C_IS_ENABLE     (1)
#else
#   define XF_HAL_I2C_IS_ENABLE     (0)
#endif

#if (!defined(XF_HAL_SPI_ENABLE)) || (XF_HAL_SPI_ENABLE)
#   define XF_HAL_SPI_IS_ENABLE     (1)
#else
#   define XF_HAL_SPI_IS_ENABLE     (0)
#endif

#if (!defined(XF_HAL_ADC_ENABLE)) || (XF_HAL_ADC_ENABLE)
#   define XF_HAL_ADC_IS_ENABLE     (1)
#else
#   define XF_HAL_ADC_IS_ENABLE     (0)
#endif

#if (!defined(XF_HAL_DAC_ENABLE)) || (XF_HAL_DAC_ENABLE)
#   define XF_HAL_DAC_IS_ENABLE     (1)
#else
#   define XF_HAL_DAC_IS_ENABLE     (0)
#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_HAL_DEVICE_CONFIG_H__
