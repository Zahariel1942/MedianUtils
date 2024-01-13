#pragma once

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <windows.h>
#include <iostream>
#include "MedianUtils.h"
#include "Helpers.h"


struct Config
{
    BOOL    bDebug;
    BOOL    bLogChat;
};

extern Config Configs;

void LoadSettings(void);
#endif
