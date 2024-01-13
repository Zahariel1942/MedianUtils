#pragma once

#include "Offset.h"
#include "D2Intercepts.h"
#include "D2Handlers.h"

PatchHook Patches[] = {
    {PatchCall,	GetDllOffset("D2Client.dll", 0x70B75),	(DWORD)GameInput_Intercept,				5},
    {PatchCall,	GetDllOffset("D2Client.dll", 0xACE61),	(DWORD)GamePacketReceived_Intercept,	5},
    {PatchBytes,GetDllOffset("D2Client.dll", 0x161B0),	(DWORD)0xc3,							1},
    {PatchCall,	GetDllOffset("D2CMP.dll", 0x17385),		(DWORD)GameCrashFix_Intercept,			10},
};