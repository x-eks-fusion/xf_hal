/**
 * @file xf_hal_gpio.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief xf_hal 的 gpio(通用输入输出) 设备抽象。
 * @version 0.1
 * @date 2024-05-13
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_HAL_GPIO_H__
#define __XF_HAL_GPIO_H__

/* ==================== [Includes] ========================================== */

#include "xf_hal_device_config.h"

/**
 * @ingroup group_xf_hal_user
 * @defgroup group_xf_hal_user_gpio gpio
 * @brief 通用输入输出。
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/**
 * @brief 无效 gpio 序号定义。
 */
#define XF_HAL_GPIO_NUM_NONE   ((xf_gpio_num_t)-1)

/* ==================== [Typedefs] ========================================== */

/**
 * @brief gpio 序号。
 */
typedef uint32_t xf_gpio_num_t;

#if XF_HAL_GPIO_IS_ENABLE

/**
 * @brief gpio 中断回调函数原型。
 *
 * @param gpio_num 产生中断的 gpio 的序号。
 * @param level 当前 gpio 电平。
 * @param user_data 用户数据，见 @ref xf_hal_gpio_set_intr_cb
 *                  或 @ref xf_hal_gpio_set_intr_isr
 *                  的 `user_data` 参数。
 */
typedef void (*xf_hal_gpio_irq_cb_t)(xf_gpio_num_t gpio_num, bool level, void *user_data);

/**
 * @brief gpio 的中断类型。
 */
typedef enum _xf_hal_gpio_intr_type_t {
    _XF_HAL_GPIO_INTR_BASE = 0,     /*!< 基础类型 */

    XF_HAL_GPIO_INTR_TYPE_DISABLE = _XF_HAL_GPIO_INTR_BASE, /*!< 禁用中断 */
    XF_HAL_GPIO_INTR_TYPE_RISING,   /*!< 上升沿中断 */
    XF_HAL_GPIO_INTR_TYPE_FALLING,  /*!< 下降沿中断 */
    XF_HAL_GPIO_INTR_TYPE_ANY,      /*!< 双边沿中断 */

    _XF_HAL_GPIO_INTR_TYPE_MAX,     /*!< 中断类型最大值 */
} xf_hal_gpio_intr_type_t;

/**
 * @brief gpio 的方向。
 */
typedef enum _xf_hal_gpio_dir_t {
    _XF_HAL_GPIO_DIR_BASE = 0,  /*!< gpio方向的最小值 */

    XF_HAL_GPIO_DIR_IN = _XF_HAL_GPIO_DIR_BASE, /*!< gpio 输入 */
    XF_HAL_GPIO_DIR_OUT,        /*!< gpio 输出 */
    XF_HAL_GPIO_DIR_OUT_OD,     /*!< gpio 开漏输出 */

    _XF_HAL_GPIO_DIR_MAX        /*!< gpio 方向的最大值 */
} xf_hal_gpio_dir_t;

/**
 * @brief gpio 的上下拉。
 */
typedef enum _xf_hal_gpio_pull_t {
    _XF_HAL_GPIO_PULL_BASE = 0, /*!< gpio 上下拉的最小值 */

    XF_HAL_GPIO_PULL_NONE = _XF_HAL_GPIO_PULL_BASE, /*!< gpio 浮空模式 */
    XF_HAL_GPIO_PULL_UP,        /*!< gpio 上拉模式 */
    XF_HAL_GPIO_PULL_DOWN,      /*!< gpio 下拉模式 */

    _XF_HAL_GPIO_PULL_MAX       /*!< gpio 上下拉的最大值 */
} xf_hal_gpio_pull_t;

/**
 * @brief 中断回调结构体。
 */
typedef struct _xf_hal_gpio_irq_t {
    xf_hal_gpio_irq_cb_t callback;  /*!< 回调函数 */
    void *user_data;                /*!< 回调函数中的用户数据 */
} xf_hal_gpio_callback_t;

/**
 * @brief gpio 命令枚举。用于对接 gpio 设置的命令。
 *
 * @note 普通用户可忽略，移植者需注意。
 */
typedef enum _xf_hal_gpio_cmd_t {
    XF_HAL_GPIO_CMD_DEFAULT     = 0x0,          /*!< 默认参数命令，在创建设备时优先执行 */

    XF_HAL_GPIO_CMD_DIRECTION   = 0x1 << 0,     /*!< 方向命令，见 @ref xf_hal_gpio_config_t.direction */
    XF_HAL_GPIO_CMD_PULL        = 0x1 << 1,     /*!< 上下拉命令，见 @ref xf_hal_gpio_config_t.pull */
    XF_HAL_GPIO_CMD_SPEED       = 0x1 << 2,     /*!< 速度命令，见 @ref xf_hal_gpio_config_t.speed */
    XF_HAL_GPIO_CMD_INTR_ENABLE = 0x1 << 3,     /*!< 中断启用命令，见 @ref xf_hal_gpio_config_t.intr_enable */
    XF_HAL_GPIO_CMD_INTR_TYPE   = 0x1 << 4,     /*!< 中断类型命令，见 @ref xf_hal_gpio_config_t.intr_type */
    XF_HAL_GPIO_CMD_INTR_CB     = 0x1 << 5,     /*!< 中断回调命令，见 @ref xf_hal_gpio_config_t.cb */
    XF_HAL_GPIO_CMD_INTR_ISR    = 0x1 << 6,     /*!< 中断服务命令，见 @ref xf_hal_gpio_config_t.isr */

    XF_HAL_GPIO_CMD_ALL         = 0x7FFFFFFF,  /*!< 默认设置命令，在创建设备时其次执行 */
} xf_hal_gpio_cmd_t;

/**
 * @brief gpio 配置结构体。用于对接 gpio 设置的参数。
 *
 * @note 普通用户可忽略，移植者需注意。
 */
typedef struct _xf_hal_gpio_config_t {
    uint32_t direction      : 2;    /*!< gpio 方向参数，见 @ref xf_hal_gpio_dir_t */
    uint32_t pull           : 2;    /*!< gpio 上下拉参数，见 @ref xf_hal_gpio_pull_t */
    uint32_t speed          : 25;   /*!< gpio 速度参数，单位 Hz */
    uint32_t intr_enable    : 1;    /*!< gpio 中断启用参数，0 为禁用，1 为启用 */
    uint32_t intr_type      : 2;    /*!< gpio 中断类型参数，见 @ref xf_hal_gpio_intr_type_t */
    xf_hal_gpio_callback_t cb;      /*!< gpio 中断回调参数，该回调函数运行于异步任务 */
    xf_hal_gpio_callback_t isr;     /*!< gpio 中断服务参数，该服务函数运行于中断服务 */
} xf_hal_gpio_config_t;

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief gpio 初始化函数。使用 gpio 之前必须要进行初始化。
 *
 * @param gpio_num gpio 的序号。
 * @param direction gpio方向，可以是输入，输出，开漏输出。
 * @return xf_err_t
 *      - XF_OK 成功初始化
 *      - other 初始化失败
 */
xf_err_t xf_hal_gpio_init(xf_gpio_num_t gpio_num, xf_hal_gpio_dir_t direction);

/**
 * @brief gpio 反初始化。恢复到未初始化状态且回收相关资源。
 *
 * @param gpio_num gpio 的序号。
 * @return xf_err_t
 *      - XF_OK 成功反初始化
 *      - XF_ERR_UNINIT 该 gpio 未初始化
 *      - other 反初始化失败
 */
xf_err_t xf_hal_gpio_deinit(xf_gpio_num_t gpio_num);

/**
 * @brief gpio 设置方向。
 *
 * @param gpio_num gpio 的序号。
 * @param direction gpio 的方向，可以是输入，输出，开漏输出。
 * @return xf_err_t
 *      - XF_OK 成功设置方向
 *      - XF_ERR_UNINIT 该 gpio 未初始化
 *      - other 设置方向失败
 */
xf_err_t xf_hal_gpio_set_direction(xf_gpio_num_t gpio_num, xf_hal_gpio_dir_t direction);

/**
 * @brief gpio 设置上下拉模式。
 *
 * @param gpio_num gpio 的序号。
 * @param pull 上下拉模式。可以是浮空，上拉，下拉。
 * @return xf_err_t
 *      - XF_OK 成功设置上下拉模式
 *      - XF_ERR_UNINIT 该 gpio 未初始化
 *      - other 上下拉模式设置失败
 */
xf_err_t xf_hal_gpio_set_pull(xf_gpio_num_t gpio_num, xf_hal_gpio_pull_t pull);

/**
 * @brief gpio 速度设置。
 *
 * @note 不是所有的芯片都支持速度的设置。
 *
 * @param gpio_num gpio 的序号。
 * @param speed gpio 的速度(只有 25 位有效, 范围 `0 ~ 33554431`)，单位为 hz
 * @return xf_err_t
 *      - XF_OK 成功设置速度
 *      - XF_ERR_UNINIT 该 gpio 未初始化
 *      - other 设置速度失败
 */
xf_err_t xf_hal_gpio_set_speed(xf_gpio_num_t gpio_num, uint32_t speed);

/**
 * @brief gpio 设置中断回调。
 * 
 * @attention cb 和 isr 相比，中断回调(cb)是使用任务进行异步的回调，
 *            此回调可以进行打印等操作，但是实时性不会很好。
 *
 * @param gpio_num gpio 的序号。
 * @param callback 回调函数。
 * @param user_data 自定义的参数。
 * @return xf_err_t
 *      - XF_OK 成功设置中断回调
 *      - XF_ERR_UNINIT 该 gpio 未初始化
 *      - other 设置中断回调失败
 */
xf_err_t xf_hal_gpio_set_intr_cb(xf_gpio_num_t gpio_num, xf_hal_gpio_irq_cb_t callback, void *user_data);

/**
 * @brief gpio 设置中断服务。
 * 
 * @attention isr 和 cb 相比，中断服务(isr)运行在中断函数内，
 *            此回调不要打印，但是实时性会更高。
 *
 * @param gpio_num gpio 的序号。
 * @param callback 回调服务函数。
 * @param user_data 自定义的参数。
 * @return xf_err_t
 *      - XF_OK 成功设置中断服务
 *      - XF_ERR_UNINIT 该 gpio 未初始化
 *      - other 设置中断服务失败
 */
xf_err_t xf_hal_gpio_set_intr_isr(xf_gpio_num_t gpio_num, xf_hal_gpio_irq_cb_t callback, void *user_data);

/**
 * @brief 设置 gpio 中断类型。
 *
 * @param gpio_num gpio 的序号。
 * @param intr_type 可以是上升沿中断，下降沿中断，双边沿中断或者禁用中断。
 * @return xf_err_t
 *      - XF_OK 成功设置中断类型
 *      - XF_ERR_UNINIT 该 gpio 未初始化
 *      - other 设置中断类型失败
 */
xf_err_t xf_hal_gpio_set_intr_type(xf_gpio_num_t gpio_num, xf_hal_gpio_intr_type_t intr_type);

/**
 * @brief 设置 gpio 启用中断。
 *
 * @param gpio_num gpio 的序号。
 * @return xf_err_t
 *      - XF_OK 成功设置中断启动
 *      - XF_ERR_UNINIT 该 gpio 未初始化
 *      - other 设置中断启动失败
 */
xf_err_t xf_hal_gpio_set_intr_enable(xf_gpio_num_t gpio_num);

/**
 * @brief 设置 gpio 禁用中断。
 *
 * @param gpio_num gpio 的序号。
 * @return xf_err_t
 *      - XF_OK 成功设置中断禁用
 *      - XF_ERR_UNINIT 该 gpio 未初始化
 *      - other 设置中断禁用失败
 */
xf_err_t xf_hal_gpio_set_intr_disable(xf_gpio_num_t gpio_num);

/**
 * @brief 设置 gpio 设置电平高低。
 *
 * @param gpio_num gpio 的序号。
 * @param level 电平高低，1 为高电平，0 为低电平。
 * @return xf_err_t
 *      - XF_OK 成功设置
 *      - XF_ERR_UNINIT 该 gpio 未初始化
 *      - other 设置电平失败
 */
xf_err_t xf_hal_gpio_set_level(xf_gpio_num_t gpio_num, bool level);

/**
 * @brief 获取 gpio 电平高低。
 *
 * @param gpio_num gpio 的序号。
 * @return true 高电平
 * @return false 低电平
 */
bool xf_hal_gpio_get_level(xf_gpio_num_t gpio_num);

/* ==================== [Macros] ============================================ */

#endif // XF_HAL_GPIO_IS_ENABLE

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * End of group_xf_hal_user_gpio
 * @}
 */

#endif // __XF_HAL_GPIO_H__
