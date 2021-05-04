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
#pragma region signal_handlers
static void sigint_handler(int signum)
{
    if (signum != SIGINT) return;
    fprintf(stderr, "%s() entered\n", __FUNCTION__);
}

static void sigcont_handler(int signum)
{
    if (signum != SIGCONT) return;
    fprintf(stderr, "%s() entered\n", __FUNCTION__);
}


static void sighup_handler(int signum)
{
    if (signum != SIGHUP) return;
    fprintf(stderr, "%s() entered\n", __FUNCTION__);
}
#pragma endregion signal_handlers
static void set_sigactions(int signum, __sighandler_t handler)
{
    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, signum);
    if (sigaction(signum, &sa, NULL))
    {
        abort();
    }
}
#pragma region thread_procedures
static void* blocking_sleep_thread(void* param)
{
    set_sigactions(SIGINT, sigint_handler);
    int sleep_result = sleep(100);
    fprintf(stderr, "%s(), sleep_result = %d\n", __FUNCTION__, sleep_result);
    return param;
}

static void* blocking_nanosleep_thread(void* param)
{
    set_sigactions(SIGCONT, sigcont_handler);
    struct timespec t = { 100, 0 };
    int nanosleep_result = nanosleep(&t, NULL);
    fprintf(stderr, "%s, nanosleep_result = %d\n", __FUNCTION__, nanosleep_result);
    return param;
}

static void* blocking_read_thread(void* param)
{
    set_sigactions(SIGHUP, sighup_handler);
    int *fd = (int*)param;
    char buf[256];
    ssize_t read_result = read(*fd, (void*)buf, sizeof(buf));
    fprintf(stderr, "%s, read_result = %d\n", __FUNCTION__, read_result);
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
            err = errno;
            fprintf(stderr, "%s,%d,error in open(), errno=%d(0x%04x)\n",
                __FILE__, __LINE__, err, err);
            break;
        }
        if (-1 == tcgetattr(fd, &iosetup))
        {
            err = errno;
            fprintf(stderr, "%s,%d,error in tcgetattr(), errno=%d(0x%04x)\n",
                __FILE__, __LINE__, err, err);
            break;
        }
        cfsetispeed(&iosetup, B9600);
        cfsetospeed(&iosetup, B9600);
        if (-1 == tcsetattr(fd, TCSAFLUSH, &iosetup))
        {
            err = errno;
            fprintf(stderr, "%s,%d,error in tcsetattr(), errno=%d(0x%04x)\n",
                __FILE__, __LINE__, err, err);
            break;
        }
        if ((pthread_create(&thread_sleep, NULL, blocking_sleep_thread, NULL)) ||
            (pthread_create(&thread_nanosleep, NULL, blocking_nanosleep_thread, NULL)) ||
            (pthread_create(&thread_read, NULL, blocking_read_thread, &fd)))
        {
            err = errno;
            fprintf(stderr, "%s,%d,error in pthread_create(), errno=%d(0x%04x)\n",
                __FILE__, __LINE__, err, err);
            break;
        }
        sleep(1); // wait awhile for the threads have started.
        // send signals to the threads.
        if (pthread_kill(thread_sleep, SIGINT) ||
            pthread_kill(thread_nanosleep, SIGCONT) ||
            pthread_kill(thread_read, SIGHUP))
        {
            err = errno;
            fprintf(stderr, "%s,%d,error in pthread_kill(), errno=%d(0x%04x)\n",
                __FILE__, __LINE__, err, err);
            break;
        }
        // synchronize with the thread terminations.
        fprintf(stderr, "pthread_join(thread_sleep) = %d\n", pthread_join(thread_sleep, &thread_sleep_return));
        fprintf(stderr, "pthread_join(thread_nanosleep) = %d\n", pthread_join(thread_nanosleep, &thread_nanosleep_return));
        fprintf(stderr, "pthread_join(thread_read) = %d\n", pthread_join(thread_read, &thread_read_return));
    } while (0);
    return err;
}