/**
 * @file xf_hal_posix.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-10
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_kernel_config.h"

#if XF_HAL_POSIX_IS_ENABLE

#include "xf_hal_posix.h"
#include "xf_hal_dev.h"
#include <stdarg.h>

/* ==================== [Defines] =========================================== */

#define DEV_STR_NUM (sizeof(dev_str) / sizeof(const char *))
#define TAG "hal_posix"

/* ==================== [Typedefs] ========================================== */

typedef struct dev_name_t {
    uint16_t type;
    uint16_t id;
} dev_name_t;

/* ==================== [Static Prototypes] ================================= */

static const char *is_prefix(const char *substr, const char *str);
static bool _atoi(const char *src, uint16_t *num);
static bool name_to_type_and_id(const char *pathname, uint16_t *type, uint16_t *id);

/* ==================== [Static Variables] ================================== */

static const char *dev_str[] = {
#define XF_HAL_TABLE_STR
#include "../device/xf_hal_reg_table.inc"
};

/* ==================== [Macros] ============================================ */

#define FD_TO_ID(fd) ((fd) & 0xffff)
#define FD_TO_TYPE(fd) (((fd) >> 16) & 0xffff)
#define TYPE_ID_TO_FD(type, id) (((type) << 16) | (id))

/* ==================== [Global Functions] ================================== */

int open(const char *pathname, int flags)
{
    uint16_t type;
    uint16_t id;
    xf_hal_flag_t hal_flag;
    int fp;

    if (!name_to_type_and_id(pathname, &type, &id)) {
        XF_LOGE(TAG, "pathname not paser to type and id");
        XF_LOGE(TAG, "pathname: %s", pathname);
        XF_LOGE(TAG, "type: %d", type);
        XF_LOGE(TAG, "type: %d", id);
        return -1;
    }

    xf_hal_dev_t *dev = xf_hal_driver_create(type, id);
    if (dev == NULL) {
        XF_LOGE(TAG, "open failed!");
        XF_LOGE(TAG, "type: %d", type);
        XF_LOGE(TAG, "type: %d", id);
        return -1;
    }

    fp = TYPE_ID_TO_FD(type, id);

    hal_flag = xf_hal_driver_get_flag(type);

    if (BITS_CHECK(hal_flag, XF_HAL_FLAG_ONLY_READ) &&
            !BITS_CHECK(hal_flag, XF_HAL_FLAG_ONLY_WRITE) &&
            flags == O_RDONLY) {
        return fp;
    }

    if (!BITS_CHECK(hal_flag, XF_HAL_FLAG_ONLY_READ) &&
            BITS_CHECK(hal_flag, XF_HAL_FLAG_ONLY_WRITE) &&
            flags == O_WRONLY) {
        return fp;
    }

    if (BITS_CHECK(hal_flag, XF_HAL_FLAG_ONLY_READ) &&
            BITS_CHECK(hal_flag, XF_HAL_FLAG_ONLY_WRITE) &&
            flags == O_RDWR) {

        return fp;
    }

    XF_LOGE(TAG, "flags error!");
    XF_LOGE(TAG, "flags:%d", flags);
    return -1;
}

int ioctl(int fd, unsigned long request, ...)
{
    int ret = 0;
    va_list args;
    void *arg_in = NULL;
    uint16_t type = FD_TO_ID(fd);
    uint16_t id = FD_TO_TYPE(fd);
    xf_hal_dev_t *dev = xf_hal_device_find(type, id);

    if (dev == NULL) {
        XF_LOGE(TAG, "not find this device");
        XF_LOGE(TAG, "type;%d", type);
        XF_LOGE(TAG, "id;%d", id);
    }

    va_start(args, request);
    arg_in = va_arg(args, void *);
    va_end(args);

    ret = xf_hal_driver_ioctl(dev, request, arg_in);
    if (ret) {
        XF_LOGE(TAG, "ioctl failed!");
    }

    return ret;
}

size_t write(int fd, const void *buf, size_t count)
{
    int ret = 0;
    uint16_t type = FD_TO_ID(fd);
    uint16_t id = FD_TO_TYPE(fd);
    xf_hal_dev_t *dev = xf_hal_device_find(type, id);

    if (dev == NULL) {
        XF_LOGE(TAG, "not find this device");
        XF_LOGE(TAG, "type;%d", type);
        XF_LOGE(TAG, "id;%d", id);
    }

    ret = xf_hal_driver_write(dev, buf, count);
    if (ret < 0) {
        XF_LOGE(TAG, "write failed!");
    }

    return ret;
}

size_t read(int fd, void *buf, size_t count)
{
    int ret = 0;
    uint16_t type = FD_TO_ID(fd);
    uint16_t id = FD_TO_TYPE(fd);
    xf_hal_dev_t *dev = xf_hal_device_find(type, id);

    if (dev == NULL) {
        XF_LOGE(TAG, "not find this device");
        XF_LOGE(TAG, "type;%d", type);
        XF_LOGE(TAG, "id;%d", id);
    }

    ret = xf_hal_driver_read(dev, buf, count);
    if (ret < 0) {
        XF_LOGE(TAG, "read failed!");
    }

    return ret;
}

int close(int fd)
{
    int ret = 0;
    uint16_t type = FD_TO_ID(fd);
    uint16_t id = FD_TO_TYPE(fd);
    xf_hal_dev_t *dev = xf_hal_device_find(type, id);

    if (dev == NULL) {
        XF_LOGE(TAG, "not find this device");
        XF_LOGE(TAG, "type;%d", type);
        XF_LOGE(TAG, "id;%d", id);
    }

    ret = xf_hal_driver_close(dev);
    if (ret) {
        XF_LOGE(TAG, "read failed!");
    }

    return ret;
}

/* ==================== [Static Functions] ================================== */

static const char *is_prefix(const char *substr, const char *str)
{
    while (*substr != '\0') {
        if (*substr != *str) {
            return NULL;
        }
        substr++;
        str++;
    }
    return str;
}

static bool _atoi(const char *src, uint16_t *num)
{
    *num = 0;

    while (*src >= '0' && *src <= '9') {
        *num = *num * 10 + (*src++ - '0');
    }

    if (*src != '\0') {
        return false;
    }

    return true;
}

static bool name_to_type_and_id(const char *pathname, uint16_t *type, uint16_t *id)
{
    for (size_t i = 0; i < DEV_STR_NUM; i++) {
        const char *other = is_prefix(dev_str[i], pathname);
        if (other == NULL) {
            continue;
        }
        _atoi(other, id);
        *type = i;
        return true;
    }

    return false;
}

#endif
