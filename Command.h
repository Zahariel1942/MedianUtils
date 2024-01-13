#pragma once

#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <Helpers.h>
#include "Config.h"

bool ProcessCommand(const char* command, bool unprocessedIsCommand);

void GameJoined(void);
void MenuEntered(void);
#endif
