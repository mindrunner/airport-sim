/****************************************************************************
 * Copyright (C) 2014 by Lukas Elsner                                       *
 *                                                                          *
 * This file is part of airport-sim.                                        *
 *                                                                          *
 ****************************************************************************/

/**
 * @file tools.h
 * @author Lukas Elsner
 * @date 02-09-2014
 * @brief Header file containing functions methods with general purpose
 *
 */

#ifndef TOOLS_H
#define TOOLS_H

#include <stdbool.h>

/**
* @brief Generates a random name based on num_l letters followed by num_n numbers
* @return A pointer to a the generated string. Has to be freed by caller.
*
* */
char *generate_rand(int num_l, int num_n);

/**
* @brief Get the current timestamp
* @return The current timestamp in milliseconds.
*
* */
time_t current_timestamp();

/**
 * @brief Boolean generator based on a given probability
 * @param int A probability value between 0 and 100.
 * @return A randomly generated boolean value
 *
 * Values below 0 will be handled as 0, values above 100 will be handled as 100.
 *
 * */
bool prob_bool(int);

/**
* @brief Sleep function with milliseconds granularity
* @param long-long The time in milliseconds
*
* The current thread will block for the given time in milliseconds.
* This is implemented with help of the POSIX nanosleep function and a timespec struct.
*
*
* */
void msleep(long long);

#endif /* TOOLS_H */