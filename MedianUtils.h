#ifndef __MedianUtils_H__
#define __MedianUtils_H__

#pragma once

#define XP_WIN

#include <windows.h>
#include <vector>
#include <queue>
#include "D2Helpers.h"

#define ArraySize(x) (sizeof((x)) / sizeof((x)[0]))

struct Module
{
    union {
        HMODULE hModule;
        DWORD dwBaseAddress;
    };
    DWORD _1;
    char szPath[MAX_PATH];
};

struct Variables
{
    BOOL    bActive;
    Module* pModule;
    HMODULE hModule;
    HWND    hHandle;

    char szPath[_MAX_PATH];
    
    HHOOK hKeybHook;

    CRITICAL_SECTION cPrintSection;
};

extern Variables Vars;

BOOL Startup(void);
void Shutdown(void);

#endif
