#include "hooker.h"

// alloc.cpp
#include "alloc.h"

memerrorhandler_t &g_MemoryError = Make_Global<memerrorhandler_t>(0x005A292C); // Memory error handler function pointer.
memexithandler_t &g_MemoryErrorExit = Make_Global<memexithandler_t>(0x005A2930);

// pal.cpp
uint8_t *g_currentPalette = reinterpret_cast<uint8_t *>(0x0050B7FC);

// surfacemonitor.cpp
class SurfaceMonitorClass;
void (*&MiscFocusLoss)() = Make_Global<void (*)()>(0x00567CBC);
void (*&MiscFocusRestore)() = Make_Global<void (*)()>(0x00567CC0);
SurfaceMonitorClass &g_allSurfaces = Make_Global<SurfaceMonitorClass>(0x00567C58);
