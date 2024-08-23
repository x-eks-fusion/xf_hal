/**
 * @file xf_hal_dev.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief xf_hal 设备抽象。
 * @version 0.1
 * @date 2024-05-09
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_HAL_DEV_H__
#define __XF_HAL_DEV_H__

/* ==================== [Includes] ========================================== */

#include "xf_hal_kernel_config.h"

/**
 * @ingroup group_xf_hal_internal
 * @defgroup group_xf_hal_internal_device device
 * @brief 设备抽象。
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define XF_HAL_DEV_CMD_DEFAULT  0x0
#define XF_HAL_DEV_CMD_ALL      0x7FFFFFFF

/* ==================== [Typedefs] ========================================== */

typedef struct _xf_hal_dev_t xf_hal_dev_t;

typedef enum _xf_hal_type_t {
		XF_HAL_TYPE_NONE = -1,
#define XF_HAL_TABLE_TYPE
#include "../device/xf_hal_reg_table.inc"
    XF_HAL_TYPE_MAX
} xf_hal_type_t;

typedef xf_hal_dev_t *(*xf_hal_dev_create_t)(uint32_t id);

typedef enum _xf_hal_flag_t {
    _XF_HAL_FLAG_NOT_USE = 0x00,
    XF_HAL_FLAG_ONLY_READ = 0x01 << 0,
    XF_HAL_FLAG_ONLY_WRITE = 0x01 << 1,
    XF_HAL_FLAG_READ_WRITE = XF_HAL_FLAG_ONLY_READ | XF_HAL_FLAG_ONLY_WRITE,
} xf_hal_flag_t;

typedef struct _xf_driver_ops_t {
    xf_err_t (*open)(xf_hal_dev_t *dev);
    xf_err_t (*ioctl)(xf_hal_dev_t *dev, uint32_t cmd, void *config);
    int (*read)(xf_hal_dev_t *dev, void *buf, size_t count);
    int (*write)(xf_hal_dev_t *dev, const void *buf, size_t count);
    xf_err_t (*close)(xf_hal_dev_t *dev);
} xf_driver_ops_t;

typedef struct _xf_hal_dev_t {
    xf_list_t node;
    uint32_t type;              /*!< 保存外设类型 */
    uint32_t id;                /*!< id 用于保存设备唯一标识符 */
    void *platform_data;        /*!< 用户通过该参数穿越不同的 ops 之间 */
#if XF_HAL_LOCK_IS_ENABLE
    void *mutex;
#endif
} xf_hal_dev_t;

/* ==================== [Global Prototypes] ================================= */

xf_err_t xf_hal_driver_register(xf_hal_type_t type, xf_hal_flag_t flag, xf_hal_dev_create_t constructor,
                                const xf_driver_ops_t *driver_ops);

uint32_t xf_hal_driver_get_flag(xf_hal_type_t type);
xf_hal_dev_t *xf_hal_driver_create(xf_hal_type_t type, uint32_t id);

xf_err_t xf_hal_driver_open(xf_hal_dev_t *dev, xf_hal_type_t type, uint32_t id);
xf_err_t xf_hal_driver_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config);
int xf_hal_driver_read(xf_hal_dev_t *dev, void *buf, size_t count);
int xf_hal_driver_write(xf_hal_dev_t *dev, const void *buf, size_t count);
xf_err_t xf_hal_driver_close(xf_hal_dev_t *dev);

xf_err_t xf_hal_device_add(xf_hal_dev_t *dev);
xf_hal_dev_t *xf_hal_device_find(xf_hal_type_t type, uint32_t id);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * End of group_xf_hal_internal_device
 * @}
 */

#endif // __XF_HAL_DEV_H__
