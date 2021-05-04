#include    <stdio.h>
#include    <stdlib.h>
#include    <signal.h>
#include    <unistd.h>
#include    <termios.h>
#include    <time.h>
#include    <pthread.h>
#include    <sys/stat.h>
#include    <fcntl.h>
#include    <errno.h>
#include    <assert.h>
#pragma region signal_handlers
static void sigint_handler(int signum)
{
    if (signum != SIGINT) return;
    printf("%s() entered\n", __FUNCTION__);
}

static void sigcont_handler(int signum)
{
    if (signum != SIGCONT) return;
    printf("%s() entered\n", __FUNCTION__);
}


static void sighup_handler(int signum)
{
    if (signum != SIGHUP) return;
    printf("%s() entered\n", __FUNCTION__);
}
#pragma endregion signal_handlers
static void set_sigactions()
{
    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGINT);
    assert(0 == sigaction(SIGINT, &sa, NULL));
    sa.sa_handler = sigcont_handler;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGCONT);
    assert(0 == sigaction(SIGCONT, &sa, NULL));
    sa.sa_handler = sighup_handler;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGHUP);
    assert(0 == sigaction(SIGHUP, &sa, NULL));
}
#pragma region thread_procedures
static void* blocking_sleep_thread(void* param)
{
    set_sigactions();
    int sleep_result = sleep(100);
    printf("%s(), sleep_result = %d\n", __FUNCTION__, sleep_result);
    return param;
}

static void* blocking_nanosleep_thread(void* param)
{
    set_sigactions();
    struct timespec t = { 100, 0 };
    int nanosleep_result = nanosleep(&t, NULL);
    printf("%s, nanosleep_result = %d\n", __FUNCTION__, nanosleep_result);
    return param;
}

static void* blocking_read_thread(void* param)
{
    set_sigactions();
    int *fd = (int*)param;
    char buf[256];
    ssize_t read_result = read(*fd, (void*)buf, sizeof(buf));
    printf("%s, read_result = %d\n", __FUNCTION__, read_result);
    return param;
}
#pragma endregion thread_procedures

int main()
{
    int err = EXIT_SUCCESS;
    int fd = -1;
    struct termios iosetup;
    pthread_t thread_sleep, thread_nanosleep, thread_read;
    void *thread_sleep_return, *thread_nanosleep_return, *thread_read_return;
    do {
        fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
        if (-1 == fd)
        {
            fprintf(stderr, "%s,%d,error in open(), errno=%d(0x%04x)\n",
                __FILE__, __LINE__, errno, errno);
            break;
        }
        if (-1 == tcgetattr(fd, &iosetup))
        {
            fprintf(stderr, "%s,%d,error in tcgetattr(), errno=%d(0x%04x)\n",
                __FILE__, __LINE__, errno, errno);
            break;
        }
        cfsetispeed(&iosetup, B9600);
        cfsetospeed(&iosetup, B9600);
        if (-1 == tcsetattr(fd, TCSAFLUSH, &iosetup))
        {
            fprintf(stderr, "%s,%d,error in tcsetattr(), errno=%d(0x%04x)\n",
                __FILE__, __LINE__, errno, errno);
            break;
        }
        if ((pthread_create(&thread_sleep, NULL, blocking_sleep_thread, NULL)) ||
            (pthread_create(&thread_nanosleep, NULL, blocking_nanosleep_thread, NULL)) ||
            (pthread_create(&thread_read, NULL, blocking_read_thread, &fd)))
        {
            fprintf(stderr, "%s,%d,error in pthread_create(), errno=%d(0x%04x)\n",
                __FILE__, __LINE__, errno, errno);
            break;
        }
        sleep(1); // wait awhile for the threads have started.
        // send signals to the threads.
        if (pthread_kill(thread_sleep, SIGINT) ||
            pthread_kill(thread_nanosleep, SIGCONT) ||
            pthread_kill(thread_read, SIGHUP))
        {
            fprintf(stderr, "%s,%d,error in pthread_kill(), errno=%d(0x%04x)\n",
                __FILE__, __LINE__, errno, errno);
            break;
        }
        // synchronize with the thread terminations.
        printf("pthread_join(thread_sleep) = %d\n", pthread_join(thread_sleep, &thread_sleep_return));
        printf("pthread_join(thread_nanosleep) = %d\n", pthread_join(thread_nanosleep, &thread_nanosleep_return));
        printf("pthread_join(thread_read) = %d\n", pthread_join(thread_read, &thread_read_return));
    } while (0);
    return err;
}