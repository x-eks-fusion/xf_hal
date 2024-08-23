#include "xf_hal.h"
#include "port.h"
#include "port_xf_lock.h"

int main()
{
    uint8_t data[10];

    port_xf_lock();
    port_init();

    xf_hal_i2c_init(1, XF_HAL_I2C_HOSTS_MASTER, 1000 * 5);
    xf_hal_i2c_read(1, data, 10, 1000);
    xf_hal_i2c_write(1, data, 10, 1000);
    return 0;
}
