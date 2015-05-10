/****************************************************************************
 * Copyright (C) 2014 by Lukas Elsner                                       *
 *                                                                          *
 * This file is part of airport-sim.                                        *
 *                                                                          *
 ****************************************************************************/

/**
 * @file bay.c
 * @author Lukas Elsner
 * @date 01-09-2014
 * @brief File containing public methods for bay class.
 *
 * The airport .....
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "tools.h"
#include "bay.h"

/**
* @brief Bay structure for representing an instance of a bay
*
*/
struct bay {
    plane *plane; /**< Plane parked in bay, null if there is none. */
    time_t parking_time; /**< Time, the plane was parked or unparked. */
    int from_runway; /**< Time, the plane was parked or unparked. */
};

bay *bay_init()
{
    bay *b = (bay *)malloc(sizeof(bay));
    b->plane = NULL;
    b->parking_time = 0;
    return b;
}

time_t bay_get_occupation_time(bay *b)
{
    /* when no plane is parked, we can read the overall parking time of the last parked plane,
    otherwise, we read the time since the plane was parked */
    if(b->plane == NULL) {
        return b->parking_time;
    } else {
        return current_timestamp() - b->parking_time;
    }
}

void bay_park_plane(bay *b, plane *p, int runway)
{
    b->plane = p;
    b->parking_time = current_timestamp();
    b->from_runway = runway;
}

plane *bay_unpark_plane(bay *b)
{
    plane *p = b->plane;
    b->parking_time = bay_get_occupation_time(b);
    b->plane = NULL;
    return p;
}

int bay_get_runway(bay *b)
{
    return b->from_runway;
}

plane *bay_get_plane(bay *b)
{
    return b->plane;
}

void bay_destroy(bay *b)
{
    /* if the bay is being freed with a parking plane, it should be freed, too */
    if(b->plane)
        plane_destroy(b->plane);
    free(b);
}
