/****************************************************************************
 * Copyright (C) 2014 by Lukas Elsner                                       *
 *                                                                          *
 * This file is part of airport-sim.                                        *
 *                                                                          *
 ****************************************************************************/

/**
 * @file airport.h
 * @author Lukas Elsner
 * @date 01-09-2014
 * @brief Header containing the public accessible airport methods.
 *
 * The airport is the main data structure of this application. It provides the parking bays and thread safe
 * take-off and landing functionality.
 *
 */

#ifndef AIRPORT_H
#define AIRPORT_H

/**
* @brief Forward declaration for airport
*
* */
typedef struct airport airport;

/**
 * @brief constructor for airport
 * @param char* The name of the airport
 * @return A pointer to the airport structure, representing the created object
 *
 * After using this structure, it must be freed with airport_destroy(airport *)
 *
 * */
 airport *airport_init(char *);

/**
* @brief Lets a plane land on the airport
* @param airport* Pointer to structure to work on
*
* This method creates a plane and parks it in a randomly chosen empty parking bay.
* It is thread safe.
* When the airport is full, it blocks for maximum of 500ms to get a free slot. After that it returns without any side effects.
*
* */
void airport_land_plane(airport *);

/**
 * @brief Lets a plane take off from the airport
 * @param airport* Pointer to structure to work on
 *
 * This method chooses a random plane from the parking bay to take off.
 * It is thread safe.
 * When the airport is empty, it blocks for maximum of 500ms to get a plane. After that it returns without any side effects.
 *
 * */
void airport_takeoff_plane(airport *);

/**
 * @brief Method for getting a string representation of the current airport state.
 * @param airport* Pointer to structure to work on
 * @return A pointer to a string representation of passed structure. Must be freed by caller.
 *
 * This is a thread safe call to return the current airport state in a human readable form.
 *
 * */
char *airport_to_string(airport *);

/**
 * @brief Destructor for airport
 * @param airport* Pointer to structure to be freed
 *
 * */
void airport_destroy(airport *);

#endif /* AIRPORT_H */
