/* Timer */

#ifndef _H_TIMER
#define _H_TIMER

#include <stdio.h>
#include <sys/time.h>

/* OSが管理する時刻を取得 */
inline
unsigned long long gettimeval(void) {
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return ((unsigned long long)tv.tv_sec)*1000000+tv.tv_usec;
}

/* 時間計測の開始 */
inline
void StartTimer(unsigned int *startt) {
    *startt = (unsigned int)gettimeval();
    return;
}

/* 時間計測の終了 */
inline
unsigned int StopTimer(unsigned int *startt) {
    unsigned int stopt = (unsigned int)gettimeval();
    return (stopt>=*startt)?(stopt-*startt):(stopt);
}

/* 時間計測結果の表示 */
#define print_timer(te) {printf("time of %s:%f[msec]\n", #te, te*1.0e-3);}

#endif /*_H_TIMER*/

