#include "xf_hal.h"
#include "port.h"
#include "port_xf_lock.h"

int main()
{
    uint32_t adc_value = 0;
    port_xf_lock();
    port_init();

    xf_hal_gpio_init(1, XF_HAL_GPIO_DIR_OUT);

    adc_value = xf_hal_gpio_set_level(1, 1);

    printf("adc_value:%d\n", adc_value);

    return 0;
}