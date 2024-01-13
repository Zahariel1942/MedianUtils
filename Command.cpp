#include "Command.h"

bool ProcessCommand(const char* command, bool unprocessedIsCommand)
{
    bool result = false;
    char* buf = _strdup(command);
    char* next_token1 = NULL;
    char* argv = strtok_s(buf, " ", &next_token1);

    // no command?
    if (argv == NULL)
        return false;

    if (_strcmpi(argv, "help") == 0)
    {
        Print("ÿc2MedianUtilsÿc0 :: Help");
        result = true;
    }
    else if (_strcmpi(argv, "reload") == 0)
    {
        LoadSettings();
        Print("Reload()");
        result = true;
    }
    free(buf);
    return result;
}

void GameJoined(void)
{

    Print("Game joined.");
}

void MenuEntered()
{
    Print("Menu entered.");
}
