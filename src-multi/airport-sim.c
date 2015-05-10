/****************************************************************************
* Copyright (C) 2014 by Lukas Elsner                                       *
*                                                                          *
* This file is part of airport-sim.                                        *
*                                                                          *
****************************************************************************/

/**
* @file airport-sim.c
* @author Lukas Elsner
* @date 01-09-2014
* @brief Main program file with main() entry point.
*
* The airport-sim application.
*
*/

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "airport.h"
#include "tools.h"


#define NUM_LANDING_THREADS 15
#define NUM_TAKEOFF_THREADS 5

/**
* @brief This is set to exit when the application should exit gracefully.
*
* */
bool airport_exit = false;

/**
* @brief Representation of the airport for the simulation.
*
* */
airport *ap;

/**
* @brief Prints the help for airport-sim to the console.
* @param char* Program name
*
* */
void usage(char *pname) {
    fprintf(stderr, "usage: %s <landing probability> <takeoff probability>\n", pname);
}

/**
* @brief Prints the startup banner of the airport to the console.
*
* */
void print_banner() {
    printf("%s", "Welcome to the airport simulator.\n");
    printf("%s", "Press p or P followed by return to display the state of the airport.\n");
    printf("%s", "Press q or Q followed by return to terminate the simulation.\n\n");
    printf("%s", "Press return to start the simulation.\n");
    getchar();
}

/**
* @brief Monitor thread
*
* The monitor thread interacts with the user while the airport-simulation is running.
* To print the state of the airport, the user can press 'p' or 'P'.
* To exit the application, the user can press 'q' or 'Q'.
*
* */
void monitor_thread_func() {
    while (!airport_exit) {
        char c = getchar();
        if (c == 'p' || c == 'P') {
            char *c = airport_to_string(ap);
            printf("%s", c);
            free(c);
        }
        if (c == 'q' || c == 'Q') {
            airport_exit = true;
        }
    }
}

/**
* @brief Landing thread
* @param int Landing probability
*
* The landing thread lands a plane on the airport with the given probability.
* */
void landing_thread_func(int prob) {
    while (!airport_exit) {
        if (prob_bool(prob)) {
            airport_land_plane(ap);
        }
        msleep(500);
    }
}

/**
* @brief Takeoff thread
* @param int Take-off probability
*
* The take-off thread takes off a plane of the airport with the given probability.
* */
void takeoff_thread_func(int prob) {
    while (!airport_exit) {
        if (prob_bool(prob)) {
            airport_takeoff_plane(ap);
        }
        msleep(500);
    }
}

/**
* @brief Main entry point of airport-sim
* @param int Number of arguments
* @param char** Pointer to array of arguments
* @return Exit code of airport-sim
*
* */
int main(int argc, char **argv) {
    /* initialize random seed */
    srand(time(NULL));

    /* set default value for probabilities */
    int landprob = 50;
    int takeoffprob = 50;
    /* program started with one argument */
    if (argc > 1) {
        /* user wants to see help */
        if (!strcmp(argv[1], "-h")) {
            usage(argv[0]);
            return 0;
        }
        landprob = atoi(argv[1]);
    }

    /* program started with two argument */
    if (argc > 2) {
        takeoffprob = atoi(argv[2]);
    }

    /* arguments are out of allowed range */
    if (landprob < 1 || landprob > 90 || takeoffprob < 1 || takeoffprob > 90) {
        usage(argv[0]);
        return -1;
    }

    print_banner();

    /* initialize the airport */
    ap = airport_init("lumans airport");

    pthread_t landing_thread[NUM_LANDING_THREADS];
    pthread_t takeoff_thread[NUM_TAKEOFF_THREADS];
    pthread_t monitor_thread;

    /* set of attributes for the thread */
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    /* create three threads */
    pthread_create(&monitor_thread, &attr, (void *(*)(void *)) monitor_thread_func, NULL);

    for (int i = 0; i < NUM_LANDING_THREADS; ++i) {
        pthread_create(&landing_thread[i], &attr, (void *(*)(void *)) landing_thread_func, (void *) ((uintptr_t) landprob));
    }

    for (int i = 0; i < NUM_TAKEOFF_THREADS; ++i) {
        pthread_create(&takeoff_thread[i], &attr, (void *(*)(void *)) takeoff_thread_func, (void *) ((uintptr_t) takeoffprob));
    }

    /* wait for all three threads to finish their work */
    pthread_join(monitor_thread, NULL);
    for (int i = 0; i < NUM_LANDING_THREADS; ++i) {
        pthread_join(landing_thread[i], NULL);
    }
    for (int i = 0; i < NUM_TAKEOFF_THREADS; ++i) {
        pthread_join(takeoff_thread[i], NULL);
    }

    /* print the airport before exiting */
    char *c = airport_to_string(ap);
    printf("%s", c);
    free(c);

    /* free the airport object */
    airport_destroy(ap);

    return 0;
}


