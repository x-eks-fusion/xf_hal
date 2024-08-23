#include "xf_hal.h"
#include "port.h"
#include "port_xf_lock.h"

int main()
{
    uint32_t adc_value = 123;
    port_xf_lock();
    port_init();

    xf_hal_dac_init(1);

    xf_hal_dac_write(1, adc_value);
    return 0;
}