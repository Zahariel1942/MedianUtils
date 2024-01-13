#include "D2NetHandlers.h"
#include "Helpers.h"
#include "Constants.h"
#include "MedianUtils.h"

using namespace std;

DWORD ChatEventHandler(BYTE* pPacket, DWORD dwSize)
{
    char* pName = (char*)pPacket+10;
    char* pMessage = (char*)pPacket + strlen(pName) + 11;

    Print("Chat event: %s, %s", pName, pMessage);

    return true;
}

DWORD ItemActionHandler(BYTE* pPacket, DWORD dwSize)
{
    // TODO: fix this code later by changing the way it's parsed
    INT64 icode   = 0;
    char code[5]  = "";
    BYTE mode     = pPacket[1];
    DWORD gid     = *(DWORD*)&pPacket[4];
    BYTE dest     = ((pPacket[13] & 0x1C) >> 2);

    switch(dest)
    {
        case 0: 
        case 2:
            icode = *(INT64 *)(pPacket+15)>>0x04;
            break;
        case 3:
        case 4:
        case 6:
            if(!((mode == 0 || mode == 2) && dest == 3))
            {
                if(mode != 0xF && mode != 1 && mode != 12)
                    icode = *(INT64 *)(pPacket+17) >> 0x1C;
                else
                    icode = *(INT64 *)(pPacket+15) >> 0x04;
            } 
            else  
                icode = *(INT64 *)(pPacket+17) >> 0x05;
            break;
        default:
            break;
    }

    // Converting and Cleaning
    memcpy(code, &icode, 4);
    if(code[3] == ' ') code[3] = '\0';

    /*if(strcmp(code, "gld") == 0)
        GoldDropEvent(gid, mode);
    else*/
        //ItemActionEvent(gid, code, mode, (pPacket[0] == 0x9d));

    Print("Drop event: %s", code);

    return TRUE;
}
