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
#include "blitters.h"
#include "crc.h"
#include "gbuffer.h"
#include "hooker.h"
#include "lcw.h"
#include "mouse.h"
#include "mouseshape.h"
#include "ostimer.h"
#include "surfacemonitor.h"
#include "xordelta.h"

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

    // crc.h
    Hook_Function(0x004D03E4, Calculate_CRC);

    // blitters.h
    Hook_Function(0x004CE2E0, Buffer_Draw_Line);
    Hook_Function(0x004CF638, Buffer_Fill_Rect);
    Hook_Function(0x004CFB74, Buffer_Remap);
    Hook_Function(0x004D88CC, Buffer_Get_Pixel);
    Hook_Function(0x004D3844, Buffer_Put_Pixel);
    Hook_Function(0x004CD338, Linear_Blit_To_Linear);
    Hook_Function(0x004C9D38, Buffer_Clear);
    Hook_Function(0x004D8984, Buffer_To_Buffer);
    Hook_Function(0x004D8ADC, Buffer_To_Page);
    Hook_Function(0x004D5254, Linear_Scale_To_Linear);

    // gbuffer.h
    Hook_Function(0x004C989C, *GraphicBufferClass::DD_Init);
    Hook_Function(0x004C9970, *GraphicBufferClass::Init);
    Hook_Function(0x004C99F4, *GraphicBufferClass::Un_Init);
    Hook_Function(0x004C9B40, *GraphicBufferClass::Lock);
    Hook_Function(0x004C9C50, *GraphicBufferClass::Unlock);
    Hook_Function(0x004CFC60, *GraphicViewPortClass::Draw_Rect);

    // ostimer.h
    Hook_Function(0x004D02A4, &PlatformTimerClass::Timer_Callback);

    // lcw.h
    Hook_Function(0x004D7FDC, LCW_Uncomp);

    // mouseshape.h
    Hook_Function(0x004DBB1C, Mouse_Shadow_Buffer);
    Hook_Function(0x004DBC61, Mouse_Draw);
    Hook_Function(0x004DBD8F, Mouse_Set_Cursor);

    // mouse.h
    Hook_Function(0x004C8A48, *MouseClass::Low_Hide_Mouse);
    Hook_Function(0x004C8B14, *MouseClass::Low_Show_Mouse);

    // xordelta.h
    Hook_Function(0x004D81AC, Apply_XOR_Delta_To_Page_Or_Viewport);
    Hook_Function(0x004D813C, Apply_XOR_Delta);

    // surfacemonitor.h
#ifdef BUILD_WITH_DDRAW
    Hook_Function(0x004CC568, *SurfaceMonitorClass::Add_Surface);
    Hook_Function(0x004CC594, *SurfaceMonitorClass::Remove_Surface);
    Hook_Function(0x004CC5B4, *SurfaceMonitorClass::Got_Surface_Already);
#endif
    Hook_Function(0x004CC5DC, *SurfaceMonitorClass::Restore_Surfaces);
    Hook_Function(0x004CC648, *SurfaceMonitorClass::Set_Surface_Focus);
    Hook_Function(0x004CC64C, *SurfaceMonitorClass::Release);
#endif
}

#if defined __WATCOMC__ && defined GAME_DLL
// Watcom has no static_assert so we are forced to use this instead
// Watcom throws "dimension cannot be negative" on missmatches
#define ASSERT_SIZEOF(structname, expectedsize) typedef char __ASSERT_SIZEOF__[(sizeof(structname) == expectedsize) ? 1 : -1]

#pragma message("Checking Type Sizes!")
//
ASSERT_SIZEOF(SurfaceMonitorClass, 0x58);
//
#pragma message("Done Checking Type Sizes!")
#endif
