/****************************************************************************
 * Copyright (C) 2014 by Lukas Elsner                                       *
 *                                                                          *
 * This file is part of airport-sim.                                        *
 *                                                                          *
 ****************************************************************************/

/**
 * @file plane.c
 * @author Lukas Elsner
 * @date 01-09-2014
 * @brief File containing the plane structure and its member methods.
 *
 * A plane is generated by the landing thread and stored in a bay of the airport.
 *
 */
#include <time.h>
#include <stdlib.h>
#include "tools.h"
#include "plane.h"

/**
* @brief Plane structure for representing an instance of a plane
*
*/
struct plane {
  char *name; /**< Name of the plane. */
};

plane *plane_init() {
  plane *p = (plane *)malloc(sizeof(plane));
  p->name = generate_rand(2, 4);
  return p;
}

char *plane_get_name(plane *p) {
  return p->name;
}

void plane_destroy(plane *p) {
  free(p->name);
  free(p);
}
