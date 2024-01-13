#ifndef D2HELPERS_H
#define D2HELPERS_H

#include "D2Structs.h"
#include "D2Ptrs.h"

enum ClientGameState
{
    ClientStateNull, ClientStateMenu, ClientStateInGame, ClientStateBusy
};

ClientGameState ClientState(void);

const char* GetUnitName(UnitAny* pUnit, char* szBuf, size_t bufSize);


DWORD __fastcall D2CLIENT_GetUnitName_STUB(DWORD UnitAny);
void  __fastcall D2CLIENT_GetItemDesc_ASM(DWORD pUnit, wchar_t* pBuffer);
void __fastcall D2CLIENT_PlaySound(DWORD dwSoundId);
#endif
