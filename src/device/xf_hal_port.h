/**
 * @file xf_hal_port.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief xf_hal 对接接口总头文件。
 * @version 0.1
 * @date 2024-05-11
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_HAL_PORT_H__
#define __XF_HAL_PORT_H__

/* ==================== [Includes] ========================================== */

#include "../kernel/xf_hal_dev.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/**
 * @addtogroup group_xf_hal_port
 * @{
 */

#if XF_HAL_GPIO_IS_ENABLE
/**
 * @brief gpio 设备注册。
 * 
 * @param driver_ops 驱动操作集。
 * @return xf_err_t 
 *      - XF_OK 成功
 *      - XF_ERR_INVALID_ARG 无效参数
 *      - XF_FAIL 失败
 */
xf_err_t xf_hal_gpio_register(const xf_driver_ops_t *driver_ops);
#endif

#if XF_HAL_TIM_IS_ENABLE
/**
 * @brief tim 设备注册。
 * 
 * @param driver_ops 驱动操作集。
 * @return xf_err_t 
 *      - XF_OK 成功
 *      - XF_ERR_INVALID_ARG 无效参数
 *      - XF_FAIL 失败
 */
xf_err_t xf_hal_tim_register(const xf_driver_ops_t *driver_ops);
#endif

#if XF_HAL_PWM_IS_ENABLE
/**
 * @brief pwm 设备注册。
 * 
 * @param driver_ops 驱动操作集。
 * @return xf_err_t 
 *      - XF_OK 成功
 *      - XF_ERR_INVALID_ARG 无效参数
 *      - XF_FAIL 失败
 */
xf_err_t xf_hal_pwm_register(const xf_driver_ops_t *driver_ops);
#endif

#if XF_HAL_ADC_IS_ENABLE
/**
 * @brief adc 设备注册。
 * 
 * @param driver_ops 驱动操作集。
 * @return xf_err_t 
 *      - XF_OK 成功
 *      - XF_ERR_INVALID_ARG 无效参数
 *      - XF_FAIL 失败
 */
xf_err_t xf_hal_adc_register(const xf_driver_ops_t *driver_ops);
#endif

#if XF_HAL_DAC_IS_ENABLE
/**
 * @brief dac 设备注册。
 * 
 * @param driver_ops 驱动操作集。
 * @return xf_err_t 
 *      - XF_OK 成功
 *      - XF_ERR_INVALID_ARG 无效参数
 *      - XF_FAIL 失败
 */
xf_err_t xf_hal_dac_register(const xf_driver_ops_t *driver_ops);
#endif

#if XF_HAL_UART_IS_ENABLE
/**
 * @brief uart 设备注册。
 * 
 * @param driver_ops 驱动操作集。
 * @return xf_err_t 
 *      - XF_OK 成功
 *      - XF_ERR_INVALID_ARG 无效参数
 *      - XF_FAIL 失败
 */
xf_err_t xf_hal_uart_register(const xf_driver_ops_t *driver_ops);
#endif

#if XF_HAL_I2C_IS_ENABLE
/**
 * @brief i2c 设备注册。
 * 
 * @param driver_ops 驱动操作集。
 * @return xf_err_t 
 *      - XF_OK 成功
 *      - XF_ERR_INVALID_ARG 无效参数
 *      - XF_FAIL 失败
 */
xf_err_t xf_hal_i2c_register(const xf_driver_ops_t *driver_ops);
#endif

#if XF_HAL_SPI_IS_ENABLE
/**
 * @brief spi 设备注册。
 * 
 * @param driver_ops 驱动操作集。
 * @return xf_err_t 
 *      - XF_OK 成功
 *      - XF_ERR_INVALID_ARG 无效参数
 *      - XF_FAIL 失败
 */
xf_err_t xf_hal_spi_register(const xf_driver_ops_t *driver_ops);
#endif

/** 
 * End of group_xf_hal_port
 * @}
 */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_HAL_PORT_H__
