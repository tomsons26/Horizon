/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Fixed point math functions.
 *
 * @copyright Horizon is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef FIXED_H
#define FIXED_H

// convert current value to a fixed point value for later calculations
inline int to_fixed(int val)
{
    if (a == 0) {
        return 0xFFFF;
    }

    return (b << 8) / a;
}

// fixed point multiplication
// on return the calculated value is converted back to a int
inline int fixed_mult(int a, int b)
{
    int val = b * a + 128;

    if (val & 0xFF000000) {
        return 0xFFFF;
    }

    return val >> 8;
}

uint16_t fixed_unknown(uint16_t a)
{
    if (a == 0) {
        return a;
    }

    return ((a & 0xFF00) + 256) >> 8;
}

#endif //FIXED_H
