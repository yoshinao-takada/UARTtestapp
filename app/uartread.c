#include    "base/BLdispatcher.h"
#include    "base/UASDKiobase.h"
#include    <stdio.h>
#include    <stdlib.h>
#include    <unistd.h>
#include    <errno.h>
#include    <signal.h>
#include    <string.h>
#define DEVNAME0    "/dev/ttyUSB0"
#define DEVNAME1    "/dev/ttyUSB1"
#ifndef ARRAYSIZE
#define ARRAYSIZE(_a_)  (sizeof(_a_)/sizeof(_a_[0]))
#endif
static const UASDKbaudrate_t baudrate = UASDKBAUD(115200);
static const struct timespec basetick = BL100msTIMESPEC;

typedef struct {
    UASDKuartdescriptor_t uart;
    int counter;
} Tx_context_t, *pTx_context_t;
typedef const Tx_context_t *pcTx_context_t;

static Tx_context_t tx_context = { UASDKUARTDEF0, 0 };

static void* tx_message(void* param)
{
    char* buf = NULL;
    pTx_context_t context = (pTx_context_t)param;
    if (context->counter >= 4) return param;
    buf = (char*)malloc(64);
    snprintf(buf, 64, "%s() counter = %d", __FUNCTION__, context->counter);
    int write_status = write(context->uart.fd, buf, strlen(buf));
    free((void*)buf);
    context->counter++;
    return param;
}

typedef struct {
    UASDKuartdescriptor_t uart;
    UASDKunibuf_t buf;
    size_t watchdog_index;
} Rx_context_t, *pRx_context_t;
typedef const Rx_context_t *pcRx_context_t;

static Rx_context_t rx_context = { UASDKUARTDEF0, UASDKunibuf_initdef, 0 };

static void* rx_thread(void* param)
{
    pRx_context_t context = (pRx_context_t)param;
    int continue_more = 1;
    UASDKiobase_enableinterrupt(SIGINT);
    while (continue_more)
    {
        context->buf.bytebuf.filled_bytes = 0;
        int read_status = UASDKiobase_read(&context->uart, &context->buf);
        if (read_status == EINTR)
        { // watchdog signaled the thread to abort blocking read operation by UASDKiobase_read().
            printf("%s,read() was interrupted.\n", __FUNCTION__);
            continue_more = 0;
        }
        else
        { // message from the Tx handler was received.
            context->buf.bytebuf.buf[context->buf.bytebuf.filled_bytes] = '\0';
            printf("%s,Rx message: %s\n", __FUNCTION__, context->buf.bytebuf.buf);
            BLsystickdispatcher_reload(context->watchdog_index); // suspend watchdog expiration
        }
    }
    return param;
}

typedef struct {
    pthread_t watched_thread; // destination to send SIGINT signal.
} Watchdog_context_t, *pWatchdog_context_t;
typedef const Watchdog_context_t *pcWatchdog_context_t;

static void* watchdog_handler(void* param)
{
    pWatchdog_context_t context = (pWatchdog_context_t)param;
    pthread_kill(context->watched_thread, SIGINT);
    return param;
}

static Watchdog_context_t watchdog_context;

static const BLdispatcher_core_t sysdispatcher_ini[] = 
{
    { 10, 10, tx_message, &tx_context },
    { 20, 20, watchdog_handler, &watchdog_context }
};

int init_contexts()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = UASDKiobase_open(&tx_context.uart, DEVNAME0, &baudrate, UASDKbyteformat_N1)))
        {
            break;
        }
        if (EXIT_SUCCESS != (err = UASDKiobase_open(&rx_context.uart, DEVNAME1, &baudrate, UASDKbyteformat_N1)))
        {
            break;
        }
        if (EXIT_SUCCESS != (err = UASDKunibuf_initbyte(&rx_context.buf, 256)))
        {
            break;
        }
        rx_context.watchdog_index = 1;
        if (pthread_create(&watchdog_context.watched_thread, NULL, rx_thread, &rx_context))
        {
            err = errno;
            break;
        }
    } while (0);
    return err;
}

int main()
{
    int err = EXIT_SUCCESS;
    void* rx_thread_return = NULL;
    do {
        if (EXIT_SUCCESS != (err = init_contexts()))
        {
            break;
        }
        if (EXIT_SUCCESS != (err = BLsystickdispatcher_init(ARRAYSIZE(sysdispatcher_ini), sysdispatcher_ini, &basetick)))
        {
            break;
        }
        pthread_join(watchdog_context.watched_thread, &rx_thread_return);
        BLsystickdispatcher_destroy();
        close(tx_context.uart.fd);
        close(rx_context.uart.fd);
    } while (0);
    return err;
}