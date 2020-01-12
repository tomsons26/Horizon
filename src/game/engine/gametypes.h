/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Enumerations and other constant defintions that don't have a better home.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef GAMETYPES_H
#define GAMETYPES_H

#include "always.h"

#define DEF_TICKS_PER_SECOND 15
#define DEF_TICKS_PER_MINUTE (DEF_TICKS_PER_SECOND * 60)
#define TICKS_HALF_MINUTE (DEF_TICKS_PER_MINUTE / 2)

typedef uint32_t coord_t;
typedef int16_t cell_t;
typedef int32_t target_t;
typedef int16_t lepton_t;
enum SpeedType
{
    SPEED_NONE = -1,
    SPEED_FIRST = 0,
    SPEED_COUNT
};

DEFINE_ENUMERATION_OPERATORS(SpeedType);

enum FireErrorType
{
    FIRE_NONE = -1,     // no valid value
    FIRE_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(FireErrorType);

enum ZoneType
{
    ZONE_NONE = -1,
    ZONE_CORE,
    ZONE_FIRST = 0,
};

DEFINE_ENUMERATION_OPERATORS(ZoneType);

enum MarkType
{
    MARK_NONE = -1,
    MARK_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(MarkType);

enum MoveType
{
    MOVE_NONE = -1,
    MOVE_OK,
    MOVE_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(MoveType);

enum SpecialDialogType
{
    SPECIAL_DLG_NONE = 0,
    SPECIAL_DLG_OPTIONS = 1,
    SPECIAL_DLG_SURRENDER = 2,
    SPECIAL_DLG_SPECIAL = 3,
};

//TODO: To be moved to mission.h when class is implimented
enum MissionType
{
    MISSION_NONE = -1,
    MISSION_FIRST = 0,
    MISSION_SLEEP = 0,
    MISSION_ATTACK,
    MISSION_MOVE,
    MISSION_RETREAT,
    MISSION_GUARD,
    MISSION_STICKY,
    MISSION_ENTER,
    MISSION_CAPTURE,
    MISSION_HARVEST,
    MISSION_AREA_GUARD,
    MISSION_RETURN,
    MISSION_STOP,
    MISSION_AMBUSH,
    MISSION_HUNT,
    MISSION_TIMED_HUNT,
    MISSION_UNLOAD,
    MISSION_SABOTAGE,
    MISSION_CONSTRUCTION,
    MISSION_DECONSTRUCTION,
    MISSION_REPAIR,
    MISSION_RESCUE,
    MISSION_MISSILE,
    MISSION_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(MissionStatusType);

enum WeaponSlotType
{
    WEAPON_SLOT_NONE = -1,
    WEAPON_SLOT_PRIMARY,
    WEAPON_SLOT_SECONDARY,
    WEAPON_SLOT_COUNT,
};

enum HousesType
{
    HOUSES_NONE = -1,

    HOUSES_GOODGUY = 0,
    HOUSES_BADGUY = 1,
    HOUSES_NEUTRAL = 2,
    HOUSES_SPECIAL = 3,
    HOUSES_LAST = 3,

    HOUSES_MULTI_FIRST = 4,
    HOUSES_MULTI_1 = 4,
    HOUSES_MULTI_2 = 5,
    HOUSES_MULTI_3 = 6,
    HOUSES_MULTI_4 = 7,
    HOUSES_MULTI_5 = 8,
    HOUSES_MULTI_6 = 9,
    HOUSES_MULTI_LAST = 9,
    HOUSES_MULTI_COUNT = 6,

    HOUSES_FIRST = HOUSES_GOODGUY,
};

DEFINE_ENUMERATION_OPERATORS(HousesType);

//TODO: To be moved to house.h when class is implimented.
enum OwnerType
{
    OWNER_NONE = 0,                             // No owner(s)
    OWNER_GOODGUY = 1 << HOUSES_GOODGUY,        //GDI
    OWNER_BADGUY = 1 << HOUSES_BADGUY,          //Nod
    OWNER_NEUTRAL = 1 << HOUSES_NEUTRAL,
    OWNER_SPECIAL = 1 << HOUSES_SPECIAL,
    OWNER_MULTI_1 = 1 << HOUSES_MULTI_1,
    OWNER_MULTI_2 = 1 << HOUSES_MULTI_2,
    OWNER_MULTI_3 = 1 << HOUSES_MULTI_3,
    OWNER_MULTI_4 = 1 << HOUSES_MULTI_4,
    OWNER_MULTI_5 = 1 << HOUSES_MULTI_5,
    OWNER_MULTI_6 = 1 << HOUSES_MULTI_6,
    OWNER_ALL = OWNER_GOODGUY | OWNER_BADGUY | OWNER_NEUTRAL | OWNER_SPECIAL
              | OWNER_MULTI_1 | OWNER_MULTI_2 | OWNER_MULTI_3 | OWNER_MULTI_4 | OWNER_MULTI_5 | OWNER_MULTI_6
};

DEFINE_ENUMERATION_OPERATORS(OwnerType);

enum SpecialWeaponType {
    SPECIAL_COUNT,
    SPECIAL_NONE = -1,
    SPECIAL_FIRST = 0,
};

enum RadioMessageType
{
    RADIO_NONE = -1,
    RADIO_FIRST = 0,
    RADIO_STATIC = 0,              // "hisssss"
    RADIO_ROGER,                   // "Roger."
    RADIO_HELLO,                   // "Come in."
    RADIO_OVER_AND_OUT,            // "Over and out."
    RADIO_NEED_TRANSPORT,          // "Requesting transport."
    RADIO_ATTACH_TO_TRANSPORT,     // "Attach to transport."
    RADIO_GOT_DELIVERY,            // "I've got a delivery for you."
    RADIO_PERFORMING_MANEUVER,     // "I'm performing load/unload maneuver. Be careful."
    RADIO_IM_CLEAR,                // "I'm clear."
    RADIO_CLEAR_TO_UNLOAD,         // "You are clear to unload. Driving away now.
    RADIO_UNABLE_TO_COMPLY,        // "Am unable to comply."
    RADIO_STARTING_CONSTRUCTION,   // "I'm starting construction now... act busy."
    RADIO_FINISHED_CONSTRUCTION,   // "I've finished construction. You are free."
    RADIO_WE_BUMPED,               // "We bumped, redraw yourself please."
    RADIO_TRYING_TO_LOAD,          // "I'm trying to load up now."
    RADIO_BECOME_PASSENGER,        // "May I become a passenger?"
    RADIO_READY_TO_RECEIVE,        // "Are you ready to receive shipment?"
    RADIO_NEED_A_LIFT,             // "Are you trying to become a passenger?"
    RADIO_MOVE_TO_LOCATION,        // "Move to location X."
    RADIO_NEED_TO_MOVE,            // "Do you need to move?"
    RADIO_NOW_WHAT,                // "All right already. Now what?"
    RADIO_IM_IN,                   // "I'm a passenger now."
    RADIO_BACK_UP,                 // "Backup into refinery now."
    RADIO_RUN_AWAY,                // "Run away!"
    RADIO_TETHER_ESTABLISHED,      // "Tether established."
    RADIO_TETHER_BROKEN,           // "Tether broken."
    RADIO_REPAIR_ONE_STEP,         // "Repair one step."
    RADIO_FIGHTING_WORDS,          // "Are you prepared to fight?"
    RADIO_ATTACK_THIS,             // "Attack this target please."
    RADIO_RELOAD,                  // "Reload one step."
    RADIO_KICK,                    // "Take this kick! You... You..."
    RADIO_PUNCH,                   // "Take this punch! You... You..."
    RADIO_FIGHT,                   // "Fancy a little fisticuffs, eh?"
    RADIO_COUNT
};

#endif // GAMETYPES_H
