#include    "base/BLdispatcher.h"
#include    "base/BLsystick.h"
#include    <stdlib.h>
#include    <stdio.h>
#include    <unistd.h>

static int counters[] = { 0, 0 };

/*!
\brierf increment counters[0] and print counters[].
\param param [in] void* casted counters
*/
static void* handler0(void* param)
{
    int* counters = (int*)param;
    counters[0]++;
    printf("%s(): counters[] = {%d, %d}\n", __FUNCTION__, counters[0], counters[1]);
    return param;
}


/*!
\brierf increment counters[1] and print counters[].
\param param [in] void* casted counters
*/
static void* handler1(void* param)
{
    int* counters = (int*)param;
    counters[1]++;
    printf("%s(): counters[] = {%d, %d}\n", __FUNCTION__, counters[0], counters[1]);
    return param;
}

static const BLdispatcher_core_t init_cores[] = 
{
    { 30, 30, handler0, (void*)counters },
    { 20, 20, handler1, (void*)counters }
};

static const struct timespec tick = BL10msTIMESPEC;

int main()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = BLsystickdispatcher_init(2, init_cores, &tick)))
        {
            break;
        }
        while ((counters[0] < 5) && (counters[1] < 5))
        {
            sleep(1);
        }
        BLsystickdispatcher_destroy();
    } while (0);
    return err;
}