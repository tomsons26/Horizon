/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Platform/Framework specific event handling code.
 *
 * @copyright Horizon is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "eventhandler.h"

#ifndef GAME_DLL
unsigned CCFocusMessage = 0x432;
focusfunc_t AudioFocusLoss;
focusfunc_t GBufferFocusLoss;
#endif

#if defined PLATFORM_WINDOWS
LRESULT __stdcall WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef GAME_DLL
    LRESULT(__stdcall * func)
    (HWND, UINT, WPARAM, LPARAM) = reinterpret_cast<LRESULT(__stdcall *)(HWND, UINT, WPARAM, LPARAM)>(0x00410E30);

    return func(hwnd, uMsg, wParam, lParam);
#else
    return 0;
#endif
}
#endif
