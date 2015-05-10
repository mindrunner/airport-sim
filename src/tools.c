/****************************************************************************
 * Copyright (C) 2014 by Lukas Elsner                                       *
 *                                                                          *
 * This file is part of airport-sim.                                        *
 *                                                                          *
 ****************************************************************************/

/**
 * @file tools.c
 * @author Lukas Elsner
 * @date 02-09-2014
 * @brief File containing functions methods with general purpose
 *
 */

#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include "tools.h"

char *generate_rand(int num_l, int num_n)
{
    char *name = (char *)malloc(sizeof(char) * (num_l + num_n + 1));
    char *c = name;
    /* generate num_l letters */
    for (int i = 0; i < num_l; ++i)
        *(c++) = 'A' + (rand() % 26);
    /* generate num_n numbers */
    for (int i = 0; i < num_n; ++i)
        *(c++) = '0' + (rand() % 10);
    *(c++) = 0;
    return name;
}

time_t current_timestamp()
{
    struct timeval te;
    gettimeofday(&te, NULL);
    /* calculate milliseconds */
    return te.tv_sec * 1000LL + te.tv_usec / 1000;
}

bool prob_bool(int prob)
{
    /* check if probability is within allowed range */
    if(prob > 100) prob = 100;
    if(prob < 0) prob = 0;
    return rand() <  prob * 0.01 * ((double)RAND_MAX + 1.0);
}

void msleep(long long m)
{
    struct timespec req = { .tv_sec = m / 1000, .tv_nsec = 1000000 * (m % 1000) };
    nanosleep(&req, NULL);
}

