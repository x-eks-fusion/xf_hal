/**
 * @file xf_hal_pwm.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief xf_hal 的 pwm(脉冲宽度调制) 设备抽象。
 * @version 0.1
 * @date 2024-05-10
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_HAL_PWM_H__
#define __XF_HAL_PWM_H__

/* ==================== [Includes] ========================================== */

#include "xf_hal_device_config.h"

/**
 * @ingroup group_xf_hal_user
 * @defgroup group_xf_hal_user_pwm pwm
 * @brief 脉冲宽度调制。
 * @{
 */

#if XF_HAL_PWM_IS_ENABLE

#include "xf_hal_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief pwm 序号。
 */
typedef uint32_t xf_pwm_num_t;

/**
 * @brief 用于对接 pwm 设置的命令。
 *
 * @note 普通用户可忽略，移植者需注意。
 */
typedef enum _xf_hal_pwm_cmd_t {
    XF_HAL_PWM_CMD_DEFAULT          = 0x0,          /*!< 默认参数命令，在创建设备时优先执行 */

    XF_HAL_PWM_CMD_ENABLE           = 0x1 << 0,     /*!< 启用命令，见 @ref xf_hal_pwm_config_t.enable */
    XF_HAL_PWM_CMD_FREQ             = 0x1 << 1,     /*!< 频率命令，见 @ref xf_hal_pwm_config_t.freq */
    XF_HAL_PWM_CMD_DUTY             = 0x1 << 2,     /*!< 占空比命令，见 @ref xf_hal_pwm_config_t.duty */
    XF_HAL_PWM_CMD_DUTY_RESOLUTION  = 0x1 << 3,     /*!< 占空比分辨率命令，见 @ref xf_hal_pwm_config_t.duty_resolution */
    XF_HAL_PWM_CMD_IO_NUM           = 0x1 << 4,     /*!< 输出 IO 命令，见 @ref xf_hal_pwm_config_t.io_num */

    XF_HAL_PWM_CMD_ALL              = 0x7FFFFFFF,  /*!< 默认设置命令，在创建设备时其次执行 */
} xf_hal_pwm_cmd_t;

/**
 * @brief 用于对接 pwm 设置的参数。
 *
 * @note 普通用户可忽略，移植者需注意。
 */
typedef struct _xf_hal_pwm_config_t {
    bool enable;                /*!< 启用参数，0 为禁用，1 为启用 */
    uint32_t freq;              /*!< 频率参数，设置 pwm 频率单位为 hz */
    uint32_t duty;              /*!< 占空比参数，设置 duty ，最大值为 `2^duty_resolution - 1` */
    uint32_t duty_resolution;   /*!< 占空比分辨率参数，直接影响 duty 的最大值 */
    xf_gpio_num_t io_num;       /*!< 输出 IO 参数，指定 pwm 的输出 io */
} xf_hal_pwm_config_t;

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief pwm 初始化函数。使用 pwm 之前必须要进行初始化。
 *
 * @param pwm_num pwm 的序号。
 * @param freq 设置 pwm 输出的频率。单位为 hz。
 * @param duty 设置 pwm 的占空比。
 * @return xf_err_t
 *      - XF_OK 成功初始化
 *      - other 初始化失败
 */
xf_err_t xf_hal_pwm_init(xf_pwm_num_t pwm_num, uint32_t freq, uint32_t duty);

/**
 * @brief pwm 反初始化函数。恢复到未初始化状态且回收相关资源。
 *
 * @param pwm_num pwm 的序号。
 * @return xf_err_t
 *      - XF_OK 成功反初始化
 *      - XF_ERR_UNINIT 该 pwm 未初始化
 *      - other 反初始化失败
 */
xf_err_t xf_hal_pwm_deinit(xf_pwm_num_t pwm_num);

/**
 * @brief pwm 启用函数。启用后，pwm 信号开始输出。
 *
 * @param pwm_num pwm 的序号。
 * @return xf_err_t
 *      - XF_OK 成功启用
 *      - XF_ERR_UNINIT 该 pwm 未初始化
 *      - other 启用失败
 */
xf_err_t xf_hal_pwm_enable(xf_pwm_num_t pwm_num);

/**
 * @brief pwm 禁用函数。禁用后，pwm 将不会输出。
 *
 * @param pwm_num pwm 的序号。
 * @return xf_err_t
 *      - XF_OK 成功禁用
 *      - XF_ERR_UNINIT 该 pwm 未初始化
 *      - other 禁用失败
 */
xf_err_t xf_hal_pwm_disable(xf_pwm_num_t pwm_num);

/**
 * @brief pwm 设置输出频率。
 *
 * @param pwm_num pwm 的序号。
 * @param freq pwm 的频率，单位为 hz，其最大值受到 duty_resolution 影响。
 * @return xf_err_t
 *      - XF_OK 成功设置
 *      - XF_ERR_UNINIT 该 pwm 未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_pwm_set_freq(xf_pwm_num_t pwm_num, uint32_t freq);

/**
 * @brief pwm 设置占空比。
 *
 * @param pwm_num pwm 的序号。
 * @param duty pwm 占空比的值，最大值为 `2^duty_resolution-1`。
 * @return xf_err_t
 *      - XF_OK 成功设置
 *      - XF_ERR_UNINIT 该 pwm 未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_pwm_set_duty(xf_pwm_num_t pwm_num, uint32_t duty);

/**
 * @brief pwm 设置占空比分辨率。
 *
 * @param pwm_num pwm 的序号。
 * @param duty_resolution pwm 分辨率。分辨率越低，freq 可以设置的越高，占空比设置的就更低。
 * @return xf_err_t
 *      - XF_OK 成功设置
 *      - XF_ERR_UNINIT 该 pwm 未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_pwm_set_duty_resolution(xf_pwm_num_t pwm_num, uint32_t duty_resolution);

/**
 * @brief pwm 设置 gpio 占空比。
 *
 * @param pwm_num pwm 的序号。
 * @param io_num 设置输出的 pwm。
 * @return xf_err_t
 *      - XF_OK 成功设置
 *      - XF_ERR_UNINIT 该 pwm 未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_pwm_set_gpio(xf_pwm_num_t pwm_num, xf_gpio_num_t io_num);

/**
 * @brief pwm 获取输出频率。
 *
 * @param pwm_num pwm 的序号。
 * @return uint32_t 频率的数值
 */
uint32_t xf_hal_pwm_get_freq(xf_pwm_num_t pwm_num);

/**
 * @brief pwm 获取占空比。
 *
 * @param pwm_num pwm 的序号。
 * @return uint32_t 占空比的数值
 */
uint32_t xf_hal_pwm_get_duty(xf_pwm_num_t pwm_num);

/**
 * @brief pwm 是否启用。
 *
 * @param pwm_num pwm 的序号。
 * @return true pwm 已启用
 * @return false pwm 未启用
 */
bool xf_hal_pwm_is_enable(xf_pwm_num_t pwm_num);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // XF_HAL_PWM_IS_ENABLE

/**
 * End of group_xf_hal_user_pwm
 * @}
 */

#endif // __XF_HAL_PWM_H__
