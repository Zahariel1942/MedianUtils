#include <io.h>
#include <errno.h>
#include <ctime>
#include <cmath>
#include <sstream>
#include <string>

#include "D2Helpers.h"
#include "Constants.h"
#include "Helpers.h"
#include "D2Intercepts.h"
#include "MedianUtils.h"

// NOTE TO CALLERS: szTmp must be a PRE-INITIALIZED string.
const char* GetUnitName(UnitAny* pUnit, char* szTmp, size_t bufSize)
{
    if(!pUnit)
    {
        strcpy_s(szTmp, bufSize, "Unknown");
        return szTmp;
    }
    if(pUnit->dwType == UNIT_MONSTER) {
        wchar_t* wName = D2CLIENT_GetUnitName(pUnit);
        WideCharToMultiByte(CP_ACP, 0, wName, -1, szTmp, bufSize, 0, 0);
        return szTmp;
    }
    if(pUnit->dwType == UNIT_PLAYER && pUnit->pPlayerData)
    {
        // return pUnit->pPlayerData->szName;
        strcpy_s(szTmp, bufSize, pUnit->pPlayerData->szName);
        return szTmp;
    }
    if(pUnit->dwType == UNIT_ITEM)
    {
        wchar_t wBuffer[256] = L"";
        D2CLIENT_GetItemName(pUnit, wBuffer, sizeof(wBuffer));
        char* szBuffer = UnicodeToAnsi(wBuffer);
        if(strchr(szBuffer, '\n'))
            *strchr(szBuffer,'\n') = 0x00;

        strcpy_s(szTmp, bufSize, szBuffer);
        delete[] szBuffer;
        return szTmp;
    }
    if(pUnit->dwType == UNIT_OBJECT || pUnit->dwType == UNIT_TILE)
    {
        if(pUnit->pObjectData && pUnit->pObjectData->pTxt)
        {
            strcpy_s(szTmp, bufSize, pUnit->pObjectData->pTxt->szName);
            return szTmp;
        }
    }
    strcpy_s(szTmp, bufSize, "Unknown");
    return szTmp;
}

// Do not edit without the express consent of bsdunx or lord2800
ClientGameState ClientState(void)
{
    ClientGameState state = ClientStateNull;
    UnitAny* player = D2CLIENT_GetPlayerUnit();
    Control *firstControl = *p_D2WIN_FirstControl;

    if(player && !firstControl)
    {
        if(player && player->pUpdateUnit)
        {
            state = ClientStateBusy;
            return state;
        }
        if(player->pInventory &&
            player->pPath &&
            //player->pPath->xPos &&
            player->pPath->pRoom1 &&
            player->pPath->pRoom1->pRoom2 &&
            player->pPath->pRoom1->pRoom2->pLevel &&
            player->pPath->pRoom1->pRoom2->pLevel->dwLevelNo)
            state = ClientStateInGame;
        else
            state = ClientStateBusy;
    }
    else if(!player && firstControl)
        state = ClientStateMenu;
    else if(!player && !firstControl)
        state = ClientStateNull;

    return state;
}

DWORD __declspec(naked) __fastcall D2CLIENT_GetUnitName_STUB(DWORD UnitAny)
{
    __asm
    {
        mov eax, ecx
        jmp D2CLIENT_GetUnitName_I
    }
}

void __declspec(naked) __fastcall D2CLIENT_GetItemDesc_ASM(DWORD pUnit, wchar_t* pBuffer)
{
    __asm 
    {
        PUSH EDI
        MOV EDI, EDX
        PUSH NULL
        PUSH 1  // TRUE = New lines, FALSE = Comma between each stat value
        PUSH ECX
        CALL D2CLIENT_GetItemDesc_I
        POP EDI
        RETN
    }
}

void __declspec(naked) __fastcall D2CLIENT_PlaySound(DWORD dwSoundId)
{
    __asm
    {
        MOV EBX, ECX
        PUSH NULL
        PUSH NULL
        PUSH NULL
        MOV EAX, p_D2CLIENT_PlayerUnit
        MOV EAX, [EAX]
        MOV ECX, EAX
        MOV EDX, EBX
        CALL D2CLIENT_PlaySound_I
        RETN
    }
}
