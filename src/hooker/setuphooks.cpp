/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Function to hook all required functions.
 *
 * @copyright Horizon is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "setuphooks.h"
#include "alloc.h"
#include "crc.h"
#include "hooker.h"

#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void Setup_Hooks()
{
#ifdef __WATCOMC__
    // Disable the DirectDraw based terrain caching system.
    Make_Global<BOOL>(0x0050B760) = false; // IconCacheAllowed

    // Hooking memory allocation functions.
    Hook_Function(0x004D0531, &malloc);
    Hook_Function(0x004C69B5, &free);
    Hook_Function(0x004DDFC4, &realloc);
    Hook_Function(0x004D5F70, &Alloc);
    Hook_Function(0x004D5FC0, &Free);
    Hook_Function(0x004D5FD0, &Resize_Alloc);

    Hook_Function(0x004D03E4, Calculate_CRC);
#endif
}

#if defined __WATCOMC__ && defined GAME_DLL
// Watcom has no static_assert so we are forced to use this instead
// Watcom throws "dimension cannot be negative" on missmatches
#define ASSERT_SIZEOF(structname, expectedsize) typedef char __ASSERT_SIZEOF__[(sizeof(structname) == expectedsize) ? 1 : -1]

#pragma message("Checking Type Sizes!")
//
//
#pragma message("Done Checking Type Sizes!")
#endif