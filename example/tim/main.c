#include "xf_hal.h"
#include "port.h"
#include "port_xf_lock.h"

int main()
{
    uint32_t ms = 0;
    port_xf_lock();
    port_init();

    xf_hal_tim_init(1, 1, 1000 * 5, false);
    ms = xf_hal_tim_get_raw_ticks(1);
    printf("ms:%d\n", ms);
    return 0;
}
