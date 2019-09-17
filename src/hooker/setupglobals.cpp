#include "hooker.h"

// alloc.cpp
#include "alloc.h"

memerrorhandler_t &g_MemoryError = Make_Global<memerrorhandler_t>(0x005A292C); // Memory error handler function pointer.
memexithandler_t &g_MemoryErrorExit = Make_Global<memexithandler_t>(0x005A2930);

// ostimer.cpp
#include "ostimer.h"
PlatformTimerClass *&PlatformTimer = Make_Global<PlatformTimerClass *>(0x00541E48);
BOOL &PlatformTimerClass::s_timerSystemOn = Make_Global<BOOL>(0x005A2860);
BOOL &PlatformTimerClass::s_inCallback = Make_Global<BOOL>(0x005A2884);
void *&PlatformTimerClass::s_threadHandle = Make_Global<void *>(0x005A2880);

// initvideo.cpp
#ifdef BUILD_WITH_DDRAW
#define DIRECTDRAW_VERSION 0x300
#include <ddraw.h>
LPDIRECTDRAW &g_directDrawObject = Make_Global<LPDIRECTDRAW>(0x00567840);
LPDIRECTDRAWSURFACE &g_paletteSurface = Make_Global<LPDIRECTDRAWSURFACE>(0x00567C54);
// ARRAY_DEF(0x0056784C, tagPALETTEENTRY, g_paletteEntries, 256);
LPDIRECTDRAWPALETTE &g_palettePtr = Make_Global<LPDIRECTDRAWPALETTE>(0x00567C4C);
#endif

// eventhandler.cpp
typedef void (*focusfunc_t)();
unsigned &CCFocusMessage = Make_Global<unsigned>(0x00500450);
focusfunc_t &AudioFocusLoss = Make_Global<focusfunc_t>(0x0056261C);
focusfunc_t &GBufferFocusLoss = Make_Global<focusfunc_t>(0x005A2730);
// mouse.cpp
class MouseClass;
MouseClass *&g_mouse = Make_Global<MouseClass *>(0x00562628);
MouseClass *&g_wwmouse = Make_Global<MouseClass *>(0x00542ABC);

// gbuffer.cpp
#include "gbuffer.h"
int &GraphicViewPortClass::ScreenWidth = Make_Global<int>(0x0050556C);
int &GraphicViewPortClass::ScreenHeight = Make_Global<int>(0x00505570);
GraphicViewPortClass *&g_logicPage = Make_Global<GraphicViewPortClass *>(0x005A272C);
GraphicViewPortClass &g_seenBuff = Make_Global<GraphicViewPortClass>(0x00541C00);
GraphicViewPortClass &g_hidPage = Make_Global<GraphicViewPortClass>(0x00541CCC);
GraphicBufferClass &g_visiblePage = Make_Global<GraphicBufferClass>(0x00541AB8);
GraphicBufferClass &g_hiddenPage = Make_Global<GraphicBufferClass>(0x00541B5C);
GraphicBufferClass &g_sysMemPage = Make_Global<GraphicBufferClass>(0x00541CF4);
GraphicBufferClass &g_modeXBuff = Make_Global<GraphicBufferClass>(0x00541C28);

// pal.cpp
uint8_t *g_currentPalette = reinterpret_cast<uint8_t *>(0x0050B7FC);

// surfacemonitor.cpp
class SurfaceMonitorClass;
void (*&MiscFocusLoss)() = Make_Global<void (*)()>(0x00567CBC);
void (*&MiscFocusRestore)() = Make_Global<void (*)()>(0x00567CC0);
SurfaceMonitorClass &g_allSurfaces = Make_Global<SurfaceMonitorClass>(0x00567C58);

// globals.cpp
#include "globals.h"
BOOL &g_gameInFocus = Make_Global<BOOL>(0x0053DD44);

// timer.cpp
TimerClass &TickCount = Make_Global<TimerClass>(0x005A2864);
CountDownTimerClass &CountDown = Make_Global<CountDownTimerClass>(0x005A2870);
