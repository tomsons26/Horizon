/**
 * @file
 *
 * @author tomsons26
 *
 * @brief
 *
 * @copyright Horizon is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "abstract.h"

AbstractClass::AbstractClass():
    m_Coord(0),
{
}

int AbstractClass::Distance_To_Target(target_t target) const
{
    DEFINE_CALL(func, 0x00412000, int, const AbstractClass *, target_t);
    return func(this, target);
}
