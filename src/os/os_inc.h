// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

#ifndef OS_INC_H
#define OS_INC_H

#if !defined(OS_FEATURE_GRAPHICAL)
# define OS_FEATURE_GRAPHICAL 0
#endif

#include "core/os_core.h"

#if OS_WINDOWS
# include "core/win32/os_core_win32.h"
#elif OS_LINUX
# include "core/linux/os_core_linux.h"
#else
# error OS core layer not implemented for this operating system.
#endif

#endif // OS_INC_H
