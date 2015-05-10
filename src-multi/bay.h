/****************************************************************************
 * Copyright (C) 2014 by Lukas Elsner                                       *
 *                                                                          *
 * This file is part of airport-sim.                                        *
 *                                                                          *
 ****************************************************************************/

/**
 * @file bay.h
 * @author Lukas Elsner
 * @date 01-09-2014
 * @brief Header containing the public accessible bay methods.
 *
 * The airport uses bay for the parking areas, where airplane can be parked.
 *
 */
#ifndef BAY_H
#define BAY_H

#include "plane.h"

/**
 * @brief Forward declaration for bay
 *
 * */
typedef struct bay bay;

/**
 * @brief constructor for bay
 * @return A pointer to the bay structure, representing the created object
 *
 * After using this structure, it must be freed with bay_destroy(bay *)
 *
 * */
bay *bay_init();

/**
 * @brief Method for getting the time this bay has been occupied.
 * @param bay* Pointer to structure to work on
 * @return The time since the current parking plane has landed if there is a parking plane currently,
 *         or the overall parking time, if plane has already taken off (bay->plane == NULL)
 *
 * */
time_t bay_get_occupation_time(bay *b);

/**
* @brief Parks a plane in the given bay
* @param bay* Pointer to structure to work on
* @param plane* The plane to park
*
* If the plane is parked, the current time is preserved, to request the overall parking time with bay_get_occupation_time().
*
* */
void bay_park_plane(bay *b, plane *p, int);

/**
* @brief Unparks a plane from the given bay
* @param bay* Pointer to structure to work on
* @return The plane which was unparked
*
* */
plane *bay_unpark_plane(bay *b);

/**
* @brief Gets the current parked plane
* @param bay* Pointer to structure to work on
* @return The plane which is parked in the bay, or NULL if there is none.
*
* */
plane *bay_get_plane(bay *b);
int bay_get_runway(bay *b);

/**
 * @brief Destructor for bay
 * @param bay* Pointer to structure to be freed
 *
 * */
void bay_destroy(bay *);

#endif /* BAY_H  */
