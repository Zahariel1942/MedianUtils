#include <vector>
#include <algorithm>
#include <string>

#include "D2Handlers.h"
#include "D2NetHandlers.h"
#include "Helpers.h"
#include "Constants.h"
#include "MedianUtils.h"
#include "Offset.h"
#include "Command.h"
#include "Config.h"

using namespace std;

DWORD WINAPI D2Thread(LPVOID lpParam)
{
    bool bInGame = false;
    LoadSettings();
    if(!InitHooks())
    {
        Log("MedianUtils Engine startup failed.");
        return FALSE;
    }

    while(Vars.bActive)
    {
        switch(ClientState())
        {
            case ClientStateInGame:
            {
                if(!bInGame)
                {
                    Sleep(500);
                    GameJoined();
                    bInGame = true;
                }
                break;
            }
            case ClientStateMenu:
            {
                if(bInGame)
                {
                    Sleep(500);
                    MenuEntered();
                    bInGame = false;
                }
                break;
            }
            case ClientStateBusy:
            case ClientStateNull:
                break;
        }
        Sleep(50);
    }
    return NULL;
}

DWORD __fastcall GameInput(wchar_t* wMsg)
{
    bool result = false;

    if(wMsg[0] == L'.')
    {
        char* szBuffer = UnicodeToAnsi(wMsg);
        result = ProcessCommand(szBuffer+1, false);
        delete[] szBuffer;
    }

    return result == true ? -1 : 0;
}

DWORD __fastcall GamePacketReceived(BYTE* pPacket, DWORD dwSize)
{
    switch(pPacket[0])
    {
        case 0x26: return ChatEventHandler(pPacket, dwSize);
        case 0x9C: 
        case 0x9D: return ItemActionHandler(pPacket, dwSize);
    }

    return true;
}

LRESULT CALLBACK KeyPress(int code, WPARAM wParam, LPARAM lParam)
{
    if(code >= HC_ACTION)
    {
        WORD repeatCount = LOWORD(lParam);
        bool altState = !!(HIWORD(lParam) & KF_ALTDOWN);
        bool previousState = !!(HIWORD(lParam) & KF_REPEAT);
        bool transitionState = !!(HIWORD(lParam) & KF_UP);
        bool isRepeat = !transitionState && repeatCount != 1;
        bool isDown = !(previousState && transitionState);
        bool isUp = previousState && transitionState;

        bool gameState = ClientState() == ClientStateInGame;

        if (altState && wParam == VK_F4)
            return CallNextHookEx(NULL, code, wParam, lParam);

        if (isDown && !isRepeat && code == HC_ACTION) {

            if (wParam == VK_F9) {
                Print("ÿc2MedianUtilsÿc0 :: F9");
            }

            if (wParam == VK_F10) {
            }

            if (wParam == VK_F12) {
            }
        }
    }
    return CallNextHookEx(NULL, code, wParam, lParam);
}
