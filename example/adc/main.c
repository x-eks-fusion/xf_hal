#include "xf_hal.h"
#include "port.h"
#include "port_xf_lock.h"

int main()
{
    uint32_t adc_value = 0;

    port_xf_lock();
    port_init();

    xf_hal_adc_init(1);

    adc_value = xf_hal_adc_read_raw(1);

    printf("read adc value: %d\n", adc_value);
    return 0;
}