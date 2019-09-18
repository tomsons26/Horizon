/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Global variables that are used amongst many classes.
 *
 * @copyright Horizon is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include "always.h"

// Hook the original binary's globals until standalone or have implemented and hooked all references.
#ifdef GAME_DLL
#include "hooker.h"

extern BOOL &g_gameInFocus;
#else
extern BOOL g_gameInFocus;
#endif

#endif // GLOBALS_H
