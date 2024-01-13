#pragma once

#include <windows.h>
#include "D2Structs.h"

DWORD WINAPI D2Thread(LPVOID lpParam);
DWORD __fastcall GameInput(wchar_t* wMsg);
LRESULT CALLBACK KeyPress(int code, WPARAM wParam, LPARAM lParam);
DWORD __fastcall GamePacketReceived(BYTE* pPacket, DWORD dwSize);