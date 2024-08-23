/**
 * @file port.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-03
 * 
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 * 
 */

/* ==================== [Includes] ========================================== */
#include "port.h"

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

extern void xf_hal_ADC_reg();
extern void xf_hal_DAC_reg();
extern void xf_hal_GPIO_reg();
extern void xf_hal_I2C_reg();
extern void xf_hal_PWM_reg();
extern void xf_hal_SPI_reg();
extern void xf_hal_TIM_reg();
extern void xf_hal_UART_reg();

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void port_init(void)
{
    xf_hal_ADC_reg();
    xf_hal_DAC_reg();
    xf_hal_GPIO_reg();
    xf_hal_I2C_reg();
    xf_hal_PWM_reg();
    xf_hal_SPI_reg();
    xf_hal_TIM_reg();
    xf_hal_UART_reg();
}

/* ==================== [Static Functions] ================================== */
