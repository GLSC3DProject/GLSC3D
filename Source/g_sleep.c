#include "glsc3d_private.h"
//#include <unistd.h>
#define __USE_POSIX199309
#include <time.h>

void g_sleep(double wait_time)
{
    struct timespec wait;
    wait.tv_sec  =  (int)wait_time;
    wait.tv_nsec =  (wait_time - wait.tv_sec)*1.0e+9;
    //usleep(1000000 * wait_time);//deprecated
    //nanosleep(&wait,NULL);
}
