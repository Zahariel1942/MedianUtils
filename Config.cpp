#include "Config.h"
#include "DefaultConfig.h"

char* GetDefaultConfigName(void)
{
    static char fname[_MAX_FNAME + MAX_PATH];
    sprintf_s(fname, sizeof(fname), "%sMUConfig.txt", Vars.szPath);
    return fname;
}

void WriteConfigFile()
{
    FILE* log = _fsopen(GetDefaultConfigName(), "w", _SH_DENYNO);
    if (!log)
        return;
    fprintf(log, DefaultConfig);
    fflush(log);
    fclose(log);
}

char* LoadSetting(const char* szKey, const char* szDefault, int size)
{
    char* szValue = new char[size];
    GetPrivateProfileString("settings", szKey, szDefault, szValue, size, GetDefaultConfigName());
    return szValue;
}

void SaveSetting(const char* szKey, const char* szValue)
{
    WritePrivateProfileString("settings", szKey, szValue, GetDefaultConfigName());
}

void LoadSettings(void)
{
    if (GetFileAttributes(GetDefaultConfigName()) == INVALID_FILE_ATTRIBUTES)
    {
        WriteConfigFile();
    }

    Configs.bDebug = StringToBool(LoadSetting("Debug", "false", 6));
}