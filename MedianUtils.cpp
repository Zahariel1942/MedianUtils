// Diablo II Botting System Core

#include <shlwapi.h>
#include <io.h>
#include <fcntl.h>

#include "Offset.h"
#include "Helpers.h"
#include "D2Handlers.h"
#include "MedianUtils.h"
#include "Config.h"
#include "D2Ptrs.h"

static HANDLE hD2Thread = INVALID_HANDLE_VALUE;
Variables Vars;
Config Configs;

BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hDll);
        if (lpReserved != NULL)
        {
            Vars.pModule = (Module*)lpReserved;

            if (!Vars.pModule)
                return FALSE;

            strcpy_s(Vars.szPath, MAX_PATH, Vars.pModule->szPath);
        }
        else
        {
            Vars.hModule = hDll;
            GetModuleFileName(hDll, Vars.szPath, MAX_PATH);
            PathRemoveFileSpec(Vars.szPath);
            strcat_s(Vars.szPath, MAX_PATH, "\\");
        }

        if (!Startup())
            return FALSE;
    }
    break;
    case DLL_PROCESS_DETACH:
        Shutdown();
        break;
    }

    return TRUE;
}

BOOL Startup(void)
{

    InitializeCriticalSection(&Vars.cPrintSection);

    DefineOffsets();
    InstallPatches();

    if ((hD2Thread = CreateThread(NULL, NULL, D2Thread, NULL, NULL, NULL)) == NULL)
        return FALSE;
    return TRUE;
}

void Shutdown(void)
{

    Vars.bActive = FALSE;

    RemovePatches();

    UnhookWindowsHookEx(Vars.hKeybHook);

    DeleteCriticalSection(&Vars.cPrintSection);
}
