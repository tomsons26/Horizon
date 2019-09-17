#include "hooker.h"

// alloc.cpp
#include "alloc.h"

memerrorhandler_t &g_MemoryError = Make_Global<memerrorhandler_t>(0x005A292C); // Memory error handler function pointer.
memexithandler_t &g_MemoryErrorExit = Make_Global<memexithandler_t>(0x005A2930);
