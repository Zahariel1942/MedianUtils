#pragma once

#include <windows.h>

DWORD ChatEventHandler(BYTE* pPacket, DWORD dwSize);
DWORD ItemActionHandler(BYTE* pPacket, DWORD dwSize);
