/**
 * @file xf_hal_reg_table.inc
 * @author cangyu (sky.kirto@qq.com)
 * @brief xf_hal 的外设注册表。
 * @version 0.1
 * @date 2024-05-15
 * @note 此文件不需要防止重复包含。
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 * @details 用法：
 * 在包含本文件前定义 `XF_HAL_REG` 可以实现宏的复用。
 */

#include "../kernel/xf_hal_table_rule.h"
#include "xf_hal_device_config.h"

#if XF_HAL_GPIO_IS_ENABLE
XF_HAL_REG(GPIO)
#endif

#if XF_HAL_PWM_IS_ENABLE
XF_HAL_REG(PWM)
#endif

#if XF_HAL_TIM_IS_ENABLE
XF_HAL_REG(TIM)
#endif

#if XF_HAL_UART_IS_ENABLE
XF_HAL_REG(UART)
#endif

#if XF_HAL_I2C_IS_ENABLE
XF_HAL_REG(I2C)
#endif

#if XF_HAL_SPI_IS_ENABLE
XF_HAL_REG(SPI)
#endif

#if XF_HAL_ADC_IS_ENABLE
XF_HAL_REG(ADC)
#endif

#if XF_HAL_DAC_IS_ENABLE
XF_HAL_REG(DAC)
#endif
