/**
 * @file xf_hal_dev.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-09
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_dev.h"

/* ==================== [Defines] =========================================== */

#define TAG "hal_dev"
#define DEV_TABLE_SIZE XF_HAL_TYPE_MAX

/* ==================== [Typedefs] ========================================== */

typedef struct _xf_hal_driver_t {
    xf_list_t dev_list;
    uint32_t flag : 16;
    uint32_t dev_count : 16;
    xf_driver_ops_t driver_ops;
    xf_hal_dev_create_t constructor;
#if XF_HAL_LOCK_IS_ENABLE
    void *mutex;
#endif
} xf_hal_driver_t;

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

static xf_hal_driver_t dev_table[DEV_TABLE_SIZE] = {0};

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

xf_err_t xf_hal_driver_register(xf_hal_type_t type, xf_hal_flag_t flag, xf_hal_dev_create_t constructor,
                                const xf_driver_ops_t *driver_ops)
{
    XF_ASSERT(driver_ops, XF_ERR_INVALID_ARG, TAG, "driver_ops must not be NULL");
    XF_ASSERT(type < DEV_TABLE_SIZE && type >= 0, XF_ERR_INVALID_ARG, TAG, "type must between 0 and %d", DEV_TABLE_SIZE);

    xf_list_init(&dev_table[type].dev_list);
    dev_table[type].driver_ops.open = driver_ops->open;
    dev_table[type].driver_ops.ioctl = driver_ops->ioctl;
    dev_table[type].driver_ops.write = driver_ops->write;
    dev_table[type].driver_ops.read = driver_ops->read;
    dev_table[type].driver_ops.close = driver_ops->close;
    dev_table[type].dev_count = 0;
    dev_table[type].flag = flag;
    dev_table[type].constructor = constructor;
#if XF_HAL_LOCK_IS_ENABLE
    xf_err_t err = xf_lock_init(&dev_table[type].mutex);
    XF_ASSERT(!err, err, TAG, "lock init failed!");
#endif

    return XF_OK;
}

uint32_t xf_hal_driver_get_flag(xf_hal_type_t type)
{
    XF_ASSERT(type < DEV_TABLE_SIZE && type >= 0, _XF_HAL_FLAG_NOT_USE, TAG, "type must between 0 and %d", DEV_TABLE_SIZE);


    uint32_t flag = dev_table[type].flag;

    return flag;
}

uint32_t xf_hal_driver_get_enable_count(xf_hal_type_t type)
{
    XF_ASSERT(type < DEV_TABLE_SIZE && type >= 0, 0, TAG, "type must between 0 and %d", DEV_TABLE_SIZE);

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_table[type].mutex);
#endif

    uint32_t dev_count = dev_table[type].dev_count;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_table[type].mutex);
#endif

    return dev_count;
}

xf_hal_dev_t *xf_hal_driver_create(xf_hal_type_t type, uint32_t id)
{
    XF_ASSERT(type < DEV_TABLE_SIZE && type >= 0, NULL, TAG, "type must between 0 and %d", DEV_TABLE_SIZE);

    xf_err_t err = XF_OK;

    xf_hal_dev_t *dev = dev_table[type].constructor(id);

    if (dev == NULL) {
        return NULL;
    }

    err = xf_hal_driver_ioctl(dev, XF_HAL_DEV_CMD_DEFAULT, (uint8_t *)dev + sizeof(xf_hal_dev_t));
    if (err != XF_OK) {
        XF_LOGE(TAG, "set default failed, %d", (int)err);
    }

    err = xf_hal_driver_ioctl(dev, XF_HAL_DEV_CMD_ALL, (uint8_t *)dev + sizeof(xf_hal_dev_t));
    if (err != XF_OK) {
        XF_LOGE(TAG, "set all failed, %d", (int)err);
    }

    return dev;
}

xf_err_t xf_hal_driver_open(xf_hal_dev_t *dev, xf_hal_type_t type, uint32_t id)
{
    XF_ASSERT(dev, XF_ERR_INVALID_ARG, TAG, "dev must not be NULL");
    XF_ASSERT(type < DEV_TABLE_SIZE && type >= 0, XF_ERR_INVALID_ARG, TAG, "type must between 0 and %d", DEV_TABLE_SIZE);
    XF_ASSERT(!xf_hal_device_find(type, id), XF_ERR_INVALID_ARG, TAG, "Duplicate device registration!");

    dev->type = type;
    dev->id = id;
    dev->platform_data = NULL;
    xf_list_init(&dev->node);
    xf_err_t err = xf_hal_device_add(dev);
    UNUSED(err);
    XF_ASSERT(!err, err, TAG, "device add failed!");

    err = dev_table[type].driver_ops.open(dev);
    XF_ASSERT(!err, err, TAG, "open failed:%d!", (int)err);

#if XF_HAL_LOCK_IS_ENABLE
    err = xf_lock_init(&dev->mutex);
    XF_ASSERT(!err, err, TAG, "lock init failed!");
#endif

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_table[type].mutex);
#endif

    dev_table[type].dev_count++;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_table[type].mutex);
#endif
    XF_LOGD(TAG, "open success");

    return XF_OK;
}

xf_err_t xf_hal_driver_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    XF_ASSERT(dev, XF_ERR_INVALID_ARG, TAG, "dev must not be NULL");
    XF_ASSERT(config, XF_ERR_INVALID_ARG, TAG, "config must not be NULL");

    xf_err_t err = dev_table[dev->type].driver_ops.ioctl(dev, cmd, config);
    UNUSED(err);
    XF_ASSERT(!err, err, TAG, "ioctl failed:%d!", (int)err);

    XF_LOGD(TAG, "ioctl success");

    return XF_OK;
}

int xf_hal_driver_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    XF_ASSERT(dev, XF_ERR_INVALID_ARG, TAG, "dev must not be NULL");
    XF_ASSERT(BITS_CHECK(dev_table[dev->type].flag, XF_HAL_FLAG_ONLY_READ), XF_ERR_NOT_SUPPORTED,  TAG,
              "device not support read:%d!", dev_table[dev->type].flag);

    xf_err_t err = dev_table[dev->type].driver_ops.read(dev, buf, count);
    XF_ASSERT(err >= 0, err, TAG, "driver read failed:%d!", (int) - err);

    return err;
}

int xf_hal_driver_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    XF_ASSERT(dev, XF_ERR_INVALID_ARG, TAG, "dev must not be NULL");
    XF_ASSERT(BITS_CHECK(dev_table[dev->type].flag, XF_HAL_FLAG_ONLY_WRITE), XF_ERR_NOT_SUPPORTED,  TAG,
              "device not support write:%d!", dev_table[dev->type].flag);

    xf_err_t err = dev_table[dev->type].driver_ops.write(dev, buf, count);
    XF_ASSERT(err >= 0, err, TAG, "driver write failed:%d!", (int) - err);

    return err;
}

xf_err_t xf_hal_driver_close(xf_hal_dev_t *dev)
{
    XF_ASSERT(dev, XF_ERR_INVALID_ARG, TAG, "dev must not be NULL");

    xf_err_t err = dev_table[dev->type].driver_ops.close(dev);
    UNUSED(err);
    XF_ASSERT(!err, err, TAG, "driver close failed");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_table[dev->type].mutex);
#endif

    xf_list_del_init(&dev->node);
    xf_free(dev);
    dev_table[dev->type].dev_count--;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_table[dev->type].mutex);
#endif
    XF_LOGD(TAG, "close success");

    return XF_OK;
}

xf_err_t xf_hal_device_add(xf_hal_dev_t *dev)
{
    XF_ASSERT(dev, XF_ERR_INVALID_ARG, TAG, "dev must not be NULL");
    XF_ASSERT(!xf_hal_device_find((xf_hal_type_t)dev->type, dev->id), XF_ERR_INVALID_ARG, TAG,
              "Duplicate device registration!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_table[dev->type].mutex);
#endif

    xf_list_init(&dev->node);
    xf_list_add_tail(&dev->node, &dev_table[dev->type].dev_list);

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_table[dev->type].mutex);
#endif

    XF_LOGD(TAG, "Device registration successful");

    return XF_OK;
}

xf_hal_dev_t *xf_hal_device_find(xf_hal_type_t type, uint32_t id)
{
    XF_ASSERT(type < DEV_TABLE_SIZE && type >= 0, NULL, TAG, "type must between 0 and %d", DEV_TABLE_SIZE);

    xf_hal_dev_t *dev = NULL;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_table[type].mutex);
#endif

    xf_list_t *dev_list = &dev_table[type].dev_list;
    xf_list_for_each_entry(dev, dev_list, xf_hal_dev_t, node) {
        if (dev->id == id) {
            break;
        }
    }

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_table[type].mutex);
#endif

    if (&dev->node == dev_list) {
        dev = NULL;
    }

    return dev;
}

/* ==================== [Static Functions] ================================== */
