/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Functions for getting random values
 *
 * @copyright Horizon is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */

#include "always.h"

extern int SimRandIndex;
extern char RandNumb[4];

void randomize();
unsigned char Random();
int Sim_IRandom(int minval, int maxval);
int IRandom(int minval, int maxval);
