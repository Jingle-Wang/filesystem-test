//
// Created by jingle on 16-9-7.
//



#include <math.h>
#include <time.h>
#include <sys/time.h>

double getUTtime()
{
    struct timeval t;
    gettimeofday(&t, 0);
    return t.tv_sec + t.tv_usec * 1e-6;
}

int getTime()
{
    struct timeval t;
    gettimeofday(&t, 0);

    return t.tv_sec * 1e6 + t.tv_usec;

}
