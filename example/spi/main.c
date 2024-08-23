#include "xf_hal.h"
#include "port.h"
#include "port_xf_lock.h"

int main()
{
    uint8_t data[10];

    port_xf_lock();
    port_init();

    xf_hal_spi_init(1, XF_HAL_SPI_HOSTS_MASTER, 1000 * 5);
    xf_hal_spi_read(1, data, 7, 1000);
    xf_hal_spi_write(1, data, 7, 1000);
    return 0;
}
