/**
 * @file xf_hal_tim.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief xf_hal 的 timer(定时器) 设备抽象。
 * @version 0.1
 * @date 2024-05-14
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_HAL_TIM_H__
#define __XF_HAL_TIM_H__

/* ==================== [Includes] ========================================== */

#include "xf_hal_device_config.h"

/**
 * @ingroup group_xf_hal_user
 * @defgroup group_xf_hal_user_tim tim
 * @brief 定时器。
 * @{
 */

#if XF_HAL_TIM_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief tim 序号。
 */
typedef uint32_t xf_tim_num_t;

/**
 * @brief tim 回调函数原型。
 *
 * @param tim_num 请求回调的 tim 序号。
 * @param ticks 当前的计数值。
 * @param user_data 用户数据，见 @ref xf_hal_tim_set_isr
 *                  的 `user_data` 参数。
 */
typedef void (*xf_hal_tim_cb_t)(xf_tim_num_t tim_num, uint32_t ticks, void *user_data);

/**
 * @brief 定时器回调结构体。
 */
typedef struct _xf_hal_tim_callback_t {
    xf_hal_tim_cb_t callback;
    void *user_data;
} xf_hal_tim_callback_t;

/**
 * @brief 定时器是否开始定时。
 */
typedef enum _xf_hal_tim_active_t {
    _XF_HAL_TIM_ACTIVE_BASE = 0,

    XF_HAL_TIM_ACTIVE_STOP = _XF_HAL_TIM_ACTIVE_BASE, /*!< 定时器停止工作 */
    XF_HAL_TIM_ACTIVE_START,    /*!< 定时器开始工作 */

    _XF_HAL_TIM_ACTIVE_MAX
} xf_hal_tim_active_t;

/**
 * @brief 定时器计数方向。
 */
typedef enum _xf_hal_tim_count_dir_t {
    _XF_HAL_TIM_COUNT_DIR_BASE = 0,

    XF_HAL_TIM_COUNT_DIR_UP = _XF_HAL_TIM_COUNT_DIR_BASE, /*!< 向上计数 */
    XF_HAL_TIM_COUNT_DIR_DOWN,      /*!< 向下计数 */
    XF_HAL_TIM_COUNT_DIR_CENTER,    /*!< 中心对齐计数 */

    _XF_HAL_TIM_COUNT_DIR_MAX
} xf_hal_tim_count_dir_t;

/**
 * @brief 用于对接定时器设置的命令。
 *
 * @note 普通用户可忽略，移植者需注意。
 */
typedef enum _xf_hal_tim_cmd_t {
    XF_HAL_TIM_CMD_DEFAULT          = 0,            /*!< 默认参数命令，在创建设备时优先执行 */

    XF_HAL_TIM_CMD_ACTIVE           = 0X1 << 0,     /*!< 激活命令，见 @ref xf_hal_tim_config_t.active */
    XF_HAL_TIM_CMD_AUTO_RELOAD      = 0X1 << 1,     /*!< 自动重装载命令，见 @ref xf_hal_tim_config_t.auto_reload */
    XF_HAL_TIM_CMD_COUNT_DIR        = 0X1 << 2,     /*!< 计数方向命令，见 @ref xf_hal_tim_config_t.count_dir */
    XF_HAL_TIM_CMD_TICK_FREQ_HZ     = 0X1 << 3,     /*!< 计数频率命令，见 @ref xf_hal_tim_config_t.tick_freq_hz */
    XF_HAL_TIM_CMD_TARGET_TICKS     = 0X1 << 4,     /*!< 目标计数值命令，见 @ref xf_hal_tim_config_t.target_ticks */
    XF_HAL_TIM_CMD_CB               = 0X1 << 5,     /*!< 定时器中断回调命令，见 @ref xf_hal_tim_config_t.cb */
    XF_HAL_TIM_CMD_ISR              = 0X1 << 6,     /*!< 定时器中断服务命令，见 @ref xf_hal_tim_config_t.isr */

    XF_HAL_TIM_CMD_ALL             = 0x7FFFFFFF,  /*!< 默认设置命令，在创建设备时其次执行 */
} xf_hal_tim_cmd_t;

/**
 * @brief 用于对接 ADC 设置的参数。
 *
 * @note 普通用户可忽略，移植者需注意。
 */
typedef struct _xf_hal_tim_config_t {
    uint32_t active     : 1;    /*!< 激活参数，0 为停止，1 为激活 */
    uint32_t auto_reload: 1;    /*!< 自动重装载参数，0 为不使用重装载，1 为使用重装载 */
    uint32_t count_dir  : 2;    /*!< 计数方向参数，0 为向上计数，1 为向下计数，2 为中心对齐计数 */
    uint32_t reverse    : 28;
    uint32_t tick_freq_hz;      /*!< 定时器频率参数，定义了定时器的定时频率，单位为 hz */
    uint32_t target_ticks;      /*!< 目标计数参数，定义了计数的目标 */
    xf_hal_tim_callback_t cb;   /*!< 定时器中断回调参数 */
    xf_hal_tim_callback_t isr;  /*!< 定时器中断服务参数 */
} xf_hal_tim_config_t;

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief 定时器初始化函数。
 *
 * 使用定时器之前必须要进行初始化。
 *
 * @param tim_num 定时器的序号。
 * @param tick_freq_hz 定时器的频率。
 * @param count_dir 定时器方向。
 * @param auto_reload 是否自动重装载。
 * @return xf_err_t
 *      - XF_OK 成功初始化
 *      - other 初始化失败
 */
xf_err_t xf_hal_tim_init(xf_tim_num_t tim_num, uint32_t tick_freq_hz, xf_hal_tim_count_dir_t count_dir,
                         bool auto_reload);

/**
 * @brief 定时器反初始化函数。
 *
 * 恢复到未初始化状态且回收相关资源。
 *
 * @param tim_num 定时器的序号。
 * @return xf_err_t
 *      - XF_OK 成功反初始化
 *      - XF_ERR_UNINIT 该定时器未初始化
 *      - other 反初始化失败
 */
xf_err_t xf_hal_tim_deinit(xf_tim_num_t tim_num);

/**
 * @brief 定时器设置中断回调函数。
 *
 * 该回调函数会在异步任务中执行，虽然实时性较差。但是能够在回调中打印等操作。
 *
 * @param tim_num 定时器的序号。
 * @param callback 中断回调函数。
 * @param user_data 用户自定义参数
 * @return xf_err_t
 *      - XF_OK 成功设置
 *      - XF_ERR_UNINIT 该定时器未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_tim_set_cb(xf_tim_num_t tim_num, xf_hal_tim_cb_t callback, void *user_data);

/**
 * @brief 定时器设置中断服务函数。
 *
 * 该函数运行于中断中，实时性比较高，但是不能运行打印等操作。
 *
 * @param tim_num 定时器的序号。
 * @param callback 定时器中断服务函数。
 * @param user_data 用户自定义参数。
 * @return xf_err_t
 *      - XF_OK 成功设置
 *      - XF_ERR_UNINIT 该定时器未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_tim_set_isr(xf_tim_num_t tim_num, xf_hal_tim_cb_t callback, void *user_data);

/**
 * @brief 定时器开始执行。
 *
 * @param tim_num 定时器的序号。
 * @param target_ticks 定时器目标计数值。
 * @return xf_err_t
 *      - XF_OK 成功执行
 *      - XF_ERR_UNINIT 该定时器未初始化
 *      - other 执行失败
 */
xf_err_t xf_hal_tim_start(xf_tim_num_t tim_num, uint32_t target_ticks);

/**
 * @brief 定时器停止执行。
 *
 * @param tim_num 定时器的序号。
 * @return xf_err_t
 *      - XF_OK 成功停止执行
 *      - XF_ERR_UNINIT 该定时器未初始化
 *      - other 停止执行失败
 */
xf_err_t xf_hal_tim_stop(xf_tim_num_t tim_num);

/**
 * @brief 设置当前的计数值。
 *
 * @param tim_num 定时器的序号。
 * @param ticks 设置的计数值。
 * @return xf_err_t
 *      - XF_OK 成功设置计数值
 *      - XF_ERR_UNINIT 该定时器未初始化
 *      - other 设置计数值失败
 */
xf_err_t xf_hal_tim_set_raw_ticks(xf_tim_num_t tim_num, uint32_t ticks);

/**
 * @brief 获取当前计数值。
 *
 * @param tim_num 定时器的序号。
 * @return uint32_t 获取到当前的计数值，如果是 0 则为失败。
 */
uint32_t xf_hal_tim_get_raw_ticks(xf_tim_num_t tim_num);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // XF_HAL_TIM_IS_ENABLE

/**
 * End of group_xf_hal_user_tim
 * @}
 */

#endif // __XF_HAL_TIM_H__
