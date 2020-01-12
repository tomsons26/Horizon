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
#pragma once

#ifndef ABSTRACT_H
#define ABSTRACT_H
#include "always.h"
#include "gametypes.h"

class NoInitClass;

class AbstractClass
{
public:
    AbstractClass();
    virtual ~AbstractClass() {}

    virtual HousesType Owner() const { return HOUSES_NONE; }
    virtual coord_t Center_Coord() const { return m_Coord; }
    virtual coord_t Target_Coord() const { return m_Coord; }
    virtual MoveType Can_Enter_Cell(cell_t cellnum, FacingType facing = FACING_NONE) const { return MOVE_OK; }
    virtual void AI() {}

    int Distance_To_Target(target_t target) const;

protected:
    coord_t m_Coord;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_IsActive : 1; // 1
#else
    bool m_IsActive;
#endif
};

#endif //ABSTRACT_H
