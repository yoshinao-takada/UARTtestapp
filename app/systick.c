#include    "base/BLsystick.h"
#include    <stdlib.h>
#include    <unistd.h>

int main()
{
    int err = EXIT_SUCCESS;
    do {
        BLsystick_samplecontext_t context = { 0, stdout };
        BLsystick_t st;
        BLsystick_acttime_setbyfloat(&st.new, 2.3f);
        st.systick_args = (void*)&context;
        st.systick_handler = BLsystick_samplecallback;
        if (EXIT_SUCCESS != (err = BLsystick_set(&st)))
        {
            fprintf(stderr, "%s,%d,err = %d(0x%04x)\n", __FILE__, __LINE__, err, err);
            break;
        }
        for (int i = 0; i < 4; i ++)
        {
            int remaining_time = sleep(10);
            fprintf(stdout, "%s,i = %d, context->count = %d, remaining_time = %d\n",
                __FUNCTION__, i, context.count, remaining_time);
        }
    } while (0);
    return err;
}