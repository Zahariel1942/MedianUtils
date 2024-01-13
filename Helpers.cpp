#include "MedianUtils.h"
#include "D2Handlers.h"
#include "Helpers.h"
#include "Config.h"

void Print(const char* szFormat, ...)
{
    using namespace std;

    const char REPLACE_CHAR = (char)(unsigned char)0xFE;

    va_list vaArgs;
    va_start(vaArgs, szFormat);
    int len = _vscprintf(szFormat, vaArgs);
    char* str = new char[len + 1];
    vsprintf_s(str, len + 1, szFormat, vaArgs);
    va_end(vaArgs);

    replace(str, str + len, REPLACE_CHAR, '%');

    const int maxlen = 98;

    // Break into lines through \n.
    list<string> lines;
    string temp;
    stringstream ss(str);
    while (getline(ss, temp))
        SplitLines(temp, maxlen, ' ', lines);

    EnterCriticalSection(&Vars.cPrintSection);

    if (ClientState() == ClientStateInGame)
    {
        // Convert and send every line.
        for (list<string>::iterator it = lines.begin(); it != lines.end(); ++it)
        {
            wchar_t* output = AnsiToUnicode(it->c_str());
            D2CLIENT_PrintGameString(output, 0);
            delete[] output;
        }
    }
    else if (ClientState() == ClientStateMenu)
    {
        // TODO: Double check this function, make sure it is working as intended.
        for (list<string>::iterator it = lines.begin(); it != lines.end(); ++it)
            D2MULTI_PrintChannelText((char*)it->c_str(), 0);
    }


    LeaveCriticalSection(&Vars.cPrintSection);

    delete[] str;
    str = NULL;
}

bool SplitLines(const std::string& str, size_t maxlen, const char delim, std::list<std::string>& lst)
{
    using namespace std;

    if (str.length() < 1 || maxlen < 2)
        return false;

    size_t pos, len;
    string tmp(str);

    while (tmp.length() > maxlen)
    {
        len = tmp.length();
        // maxlen-1 since std::string::npos indexes from 0
        pos = tmp.find_last_of(delim, maxlen - 1);
        if (!pos || pos == string::npos)
        {
            //Target delimiter was not found, breaking at maxlen
            // maxlen-1 since std::string::npos indexes from 0
            lst.push_back(tmp.substr(0, maxlen - 1));
            tmp.erase(0, maxlen - 1);
            continue;
        }
        pos = tmp.find_last_of(delim, maxlen - 1);
        if (pos && pos != string::npos)
        {
            //We found the last delimiter before maxlen
            lst.push_back(tmp.substr(0, pos) + delim);
            tmp.erase(0, pos);
        }
        else
            DebugBreak();
    }
    if (!tmp.length())
        DebugBreak();

    if (tmp.length())
        lst.push_back(tmp);

    return true;
}


void Log(char* szFormat, ...)
{
    if (!Configs.bDebug) return;

    va_list vaArgs;

    va_start(vaArgs, szFormat);
    int len = _vscprintf(szFormat, vaArgs);
    char* szString = new char[len + 1];
    vsprintf_s(szString, len + 1, szFormat, vaArgs);
    va_end(vaArgs);

    LogNoFormat(szString);

    delete[] szString;
}

void LogNoFormat(char* szString)
{
    time_t tTime;
    time(&tTime);
    char szTime[128] = "";
    struct tm time;
    localtime_s(&time, &tTime);
    strftime(szTime, sizeof(szTime), "%x %X", &time);

    char path[_MAX_PATH + _MAX_FNAME] = "";
    sprintf_s(path, sizeof(path), "%sMedianUtils.log", Vars.szPath);

    FILE* log = _fsopen(path, "a+", _SH_DENYNO);
    fprintf(log, "[%s]: %s\n", szTime, szString);
    fflush(log);
    fclose(log);
}

wchar_t* AnsiToUnicode(const char* str)
{
    wchar_t* buf = NULL;
    int len = MultiByteToWideChar(936, 0, str, -1, buf, 0);
    buf = new wchar_t[len];
    MultiByteToWideChar(936, 0, str, -1, buf, len);
    return buf;
}

char* UnicodeToAnsi(const wchar_t* str)
{
    char* buf = NULL;
    int len = WideCharToMultiByte(936, 0, str, -1, buf, 0, "?", NULL);
    buf = new char[len];
    WideCharToMultiByte(936, 0, str, -1, buf, len, "?", NULL);
    return buf;
}

bool StringToBool(const char* str)
{
    switch (tolower(str[0]))
    {
    case 't': case '1':
        return true;
    case 'f': case '0': default:
        return false;
    }
}

bool InitHooks(void)
{
    int i = 0;
    while (!Vars.bActive)
    {
        if (i >= 300)
        {
            MessageBox(0, "Failed to set hooks, exiting!", "MedianUtils", 0);
            return false;
        }

        if (D2GFX_GetHwnd() && (ClientState() == ClientStateMenu || ClientState() == ClientStateInGame))
        {
            DWORD mainThread = GetWindowThreadProcessId(D2GFX_GetHwnd(), 0);
            if (mainThread)
            {
                if (!Vars.hKeybHook)
                    Vars.hKeybHook = SetWindowsHookEx(WH_KEYBOARD, KeyPress, NULL, mainThread);
            }
        }
        else
            continue;
        if (Vars.hKeybHook)
        {
            Vars.bActive = TRUE;
        }
        Sleep(50);
        i++;
    }
    return true;
}