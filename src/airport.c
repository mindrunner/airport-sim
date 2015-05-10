/****************************************************************************
* Copyright (C) 2014 by Lukas Elsner                                       *
*                                                                          *
* This file is part of airport-sim.                                        *
*                                                                          *
****************************************************************************/

/**
* @file airport.c
* @author Lukas Elsner
* @date 01-09-2014
* @brief File containing public methods for airport class.
*
* The airport is the main data structure of this application. It provides the parking bays and thread safe
* take-off and landing functionality.
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdbool.h>
#include "airport.h"
#include "tools.h"
#include "bay.h"

/**
* @brief Number of parking slots the airport supplies
*
*/
#define NUM_BAYS 10

/**
* @brief Airport structure for representing an instance of an airport
*
*/
struct airport {
    char *name; /**< Name of the airport. */
    bay **bays;/**< Bays in which planes can be parked. Has length NUM_BAYS. */
    pthread_mutex_t runway;/**< A virtual runway, to prevent multiple take-off/landing operations at the same time. */
    sem_t empty;/**< Semaphore to block on empty bay. */
    sem_t full;/**< Semaphore to block on full bay. */
};


/**
* @brief Checks if a airport is empty
* @param airport* Pointer to structure to work on
* @return True, if the airport is empty, false otherwise
*
* This method queries the empty-semaphore to check if the given airport is empty.
*
* */
bool airport_is_empty(airport *ap) {
    int sval;
    sem_getvalue(&ap->empty, &sval);
    return sval == NUM_BAYS;
}

/**
* @brief Checks if a airport is full
* @param airport* Pointer to structure to work on
* @return True, if the airport is full, false otherwise
*
* This method queries the full-semaphore to check if the given airport is full.
*
* */
bool airport_is_full(airport *ap) {
    int sval;
    sem_getvalue(&ap->full, &sval);
    return sval == NUM_BAYS;
}

/**
* @brief Gets a random free bay number
* @param airport* Pointer to structure to work on
* @return A random free bay number
*
* The caller has to make sure, that there is a free bay existing. Otherwise, this function never returns.
*
* */
int get_random_free_bay_nr(airport *ap) {
    while (1) {
        int i = rand() % NUM_BAYS;
        if (ap->bays[i] == NULL)
            return i;
    }
}

/**
* @brief Gets a random occupied bay number
* @param airport* Pointer to structure to work on
* @return A random occupied bay number
*
* The caller has to make sure, that there is a occupied bay existing. Otherwise, this function never returns.
*
* */
int get_random_alloc_bay_nr(airport *ap) {
    while (1) {
        int i = rand() % NUM_BAYS;
        if (ap->bays[i] != NULL)
            return i;
    }
}

airport *airport_init(char *name) {
    airport *ap = (airport *) malloc(sizeof(airport));

    /* initialize semaphores and mutex */
    sem_init(&(ap->empty), 0, NUM_BAYS);
    sem_init(&(ap->full), 0, 0);
    pthread_mutex_init(&(ap->runway), NULL);

    ap->bays = (bay **) malloc(NUM_BAYS * sizeof(bay *));

    /* copy name of airport, so that we are responsible for the memory management */
    ap->name = malloc(strlen(name) + 1);
    sprintf(ap->name, "%s", name);

    /* empty all bays */
    for (int i = 0; i < NUM_BAYS; ++i) {
        ap->bays[i] = NULL;
    }
    return ap;
}

void airport_land_plane(airport *ap) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 5;

    /* If we use sem_wait() here, we can get a deadlock while gracefully shutting down the application.
    * Therefore, this blocks for a maximum of 5 seconds */
    if (sem_timedwait(&ap->empty, &ts) == 0) {
        plane *p = plane_init();
        printf("Plane %s is landing ...\n", plane_get_name(p));

        //Acquire mutex lock to protect buffer
        pthread_mutex_lock(&(ap->runway));

        int bay_nr = get_random_free_bay_nr(ap);
        ap->bays[bay_nr] = bay_init();

        /* landing time is 2 seconds */
        msleep(2000);
        bay_park_plane(ap->bays[bay_nr], p);
        printf("Plane %s parked in landing bay %d.\n", plane_get_name(p), bay_nr);

        //Release mutex lock and full semaphore
        pthread_mutex_unlock(&(ap->runway));
        sem_post(&ap->full);
        if (airport_is_full(ap)) {
            printf("The airport is full\n");
        }
    }
}

void airport_takeoff_plane(airport *ap) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 5;

    /* If we use sem_wait() here, we can get a deadlock while gracefully shutting down the application.
    * Therefore, this blocks for a maximum of 5 seconds */
    if (sem_timedwait(&ap->full, &ts) == 0) {
        /* Acquire mutex lock to protect buffer */
        pthread_mutex_lock(&(ap->runway));

        int bay_nr = get_random_alloc_bay_nr(ap);
        plane *p = bay_unpark_plane(ap->bays[bay_nr]);

        printf("After staying at bay %d for %.2f seconds, plane %s is taking off ...\n", bay_nr, bay_get_occupation_time(ap->bays[bay_nr]) / 1000.0f, plane_get_name(p));

        bay_destroy(ap->bays[bay_nr]);
        ap->bays[bay_nr] = NULL;

        /* take-off time is 2 seconds */
        msleep(2000);
        printf("Plane %s has finished taking off.\n", plane_get_name(p));
        plane_destroy(p);

        /* Release mutex lock and empty semaphore */
        pthread_mutex_unlock(&(ap->runway));
        sem_post(&ap->empty);
        if (airport_is_empty(ap)) {
            printf("The airport is empty\n");
        }
    }
}

char *airport_to_string(airport *ap) {
    /* Allocate memory based on NUM_BAYS, and ap->name */
    char *c = malloc(64 * NUM_BAYS + strlen(ap->name));
    sprintf(c, "Airport \'%s\' state: \n", ap->name);
    /**
    * We need this lock because of a very unlikely race condition, where a plane is taking off while this buffer is filled,
    * causing a SIGSEGV while accessing the plane's name after taking off!
    * */
    pthread_mutex_lock(&(ap->runway));
    for (int i = 0; i < NUM_BAYS; ++i) {
        if (ap->bays[i] == NULL)
            sprintf(c, "%s%d: empty \n", c, i);
        else
            sprintf(c, "%s%d: %s (has parked for %.2f seconds)\n", c, i,
                    plane_get_name(bay_get_plane(ap->bays[i])),
                    bay_get_occupation_time(ap->bays[i]) / 1000.0f);
    }
    pthread_mutex_unlock(&(ap->runway));
    return c;
}

void airport_destroy(airport *ap) {
    /* free the airport's name */
    free(ap->name);
    /* free the landing bays */
    for (int i = 0; i < NUM_BAYS; ++i) {
        if (ap->bays[i] != NULL) {
            bay_destroy(ap->bays[i]);
        }
    }
    free(ap->bays);
    /* destroy the semaphores and mutex */
    pthread_mutex_destroy(&ap->runway);
    sem_destroy(&ap->full);
    sem_destroy(&ap->empty);
    /* free airport structure */
    free(ap);
}




