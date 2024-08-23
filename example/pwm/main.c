#include "xf_hal.h"
#include "port.h"
#include "port_xf_lock.h"

int main()
{
    port_xf_lock();
    port_init();

    xf_hal_pwm_init(1, 1000 * 10, 1000 * 5);
    xf_hal_pwm_enable(1);
    return 0;
}
