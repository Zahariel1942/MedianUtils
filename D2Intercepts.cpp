#include "D2Handlers.h"
#include "D2Ptrs.h"
#include "MedianUtils.h"
#include "Helpers.h"
#include <shlwapi.h>

void __declspec(naked) GamePacketReceived_Intercept()
{
    __asm
    {
        pop ebp;
        pushad;

        call GamePacketReceived;
        test eax, eax;

        popad;
        jnz OldCode;

        mov edx, 0;

    OldCode:
        call D2NET_ReceivePacket_I;

        push ebp;
        ret;
    }
}


void __declspec(naked) GameInput_Intercept()
{
    __asm 
    {
        pushad
        mov ecx, ebx
        call GameInput
        cmp eax, -1
        popad
        je BlockIt
        call D2CLIENT_InputCall_I
        ret

BlockIt:
        xor eax,eax
        ret
    }
}

void __declspec(naked) GameCrashFix_Intercept()
{
    __asm
    {
        CMP ECX, 0
        JE Skip
        MOV DWORD PTR DS:[ECX+0x10],EDX
Skip:
        MOV DWORD PTR DS:[EAX+0xC],0
        RETN
    }
}
