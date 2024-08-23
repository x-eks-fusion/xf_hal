#include "xf_hal.h"
#include "port.h"
#include "port_xf_lock.h"

int main()
{
    uint8_t str[15] = {0};
    port_xf_lock();
    port_init();

    xf_hal_uart_init(1, 115200);
    xf_hal_uart_read(1, str, 15);
    xf_hal_uart_write(1, str, 15);
    return 0;
}
